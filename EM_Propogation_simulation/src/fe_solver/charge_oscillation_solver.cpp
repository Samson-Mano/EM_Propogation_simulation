#include "charge_oscillation_solver.h"

charge_oscillation_solver::charge_oscillation_solver()
{
	// Empty constructor
}

charge_oscillation_solver::~charge_oscillation_solver()
{
	// Empty destructor
}

void charge_oscillation_solver::charge_oscillation_analysis_start(const nodes_list_store& grid_nodes,
	const elementtri_list_store& grid_trimesh,
	charge_path_store& charge_path,
	const double charge_oscillation_freq,
	const double total_simulation_time,
	const double time_interval,
	nodevector_list_store& node_vector,
	tricontour_list_store& node_contour,
	bool& is_analysis_complete)
{
	is_analysis_complete = false;

	// Step 1: Find the charge location, velocity and acceleration
	std::vector<glm::vec2> charge_loc_at_t; // Location at time t
	std::vector<glm::vec2> charge_velcoty_at_t; // Velocity at time t
	std::vector<glm::vec2> charge_acceleration_at_t; // Acceleration at time t

	double angular_freq = 2.0 * m_pi * charge_oscillation_freq; // Angular charge oscillation frequency
	int cycle_n = 1; // number of cycle for closed loop
	double t1 = 0.0; // variable to aid in loop cycle time skip
	double time_t = 0.0; // time t

	for (time_t = 0.0; time_t <= total_simulation_time; time_t = time_t + time_interval)
	{
		double displ_mag = 0.0; // Displacement magnitude
		double velocity_mag = 0.0; // Velocity magnitude
		double acceleration_mag = 0.0; // Acceleration magnitude

		// Displacement
		displ_mag = std::sin(angular_freq * time_t);

		// Velocity
		velocity_mag = angular_freq * std::cos(angular_freq * time_t);

		// Acceleration
		acceleration_mag = -1.0 * angular_freq * angular_freq * std::sin(angular_freq * time_t);


		// get the charge location
		glm::vec2 loc_at_t = glm::vec2(0); // charge_path.get_charge_path_location_at_t(param_t).first;
		
		// velocity and acceleration vector
		glm::vec2 velo_at_t = glm::vec2(0);
		glm::vec2 accl_at_t = glm::vec2(0);

		get_charge_location_data(charge_path.curve_type,
			displ_mag,
			velocity_mag,
			acceleration_mag,
			loc_at_t,
			velo_at_t,
			accl_at_t);


		// Add to the list
		charge_loc_at_t.push_back(loc_at_t); // Charge location
		charge_velcoty_at_t.push_back(velo_at_t); // Charge velocity
		charge_acceleration_at_t.push_back(accl_at_t); // Charge acceleration
	}

	// Add the charge path
	this->time_interval = time_interval;
	this->time_step_count = static_cast<int>(charge_loc_at_t.size());

	charge_path.add_charge_oscillation(charge_loc_at_t, charge_velcoty_at_t, charge_acceleration_at_t);

	// Electric field solve
	time_t = 0.0;
	int step_i = 0;
	const double light_speed_c = 300; // Light speed c (KM/s)
	glm::vec2 ref_zero = glm::vec2(-100000, -100000); // Reference zero

	std::unordered_map<int, vector_data> snap_shot_Electric_field; // Electric field
	std::unordered_map<int, tricontour_data> snap_shot_Electric_potential; // Electric potential

	// Create a Frame work for the vectors
	for (const auto& nd_m : grid_trimesh.all_mesh_nodes)
	{
		int nd_id = nd_m.first;
		node_store nd = nd_m.second;

		// Set the vector id & vector point
		snap_shot_Electric_field[nd_id].vector_id = nd_id;
		snap_shot_Electric_field[nd_id].vector_loc.resize(this->time_step_count, nd.node_pt);
	}

	// Create a Frame work for the contours
	for (const auto& tri_m : grid_trimesh.elementtriMap)
	{
		int tri_id = tri_m.first;
		elementtri_store tri = tri_m.second;

		// Set the triangle id & triangle 3 points
		snap_shot_Electric_potential[tri_id].tri_id = tri_id;
		snap_shot_Electric_potential[tri_id].nd1 = tri.nd1;
		snap_shot_Electric_potential[tri_id].nd2 = tri.nd2;
		snap_shot_Electric_potential[tri_id].nd3 = tri.nd3;
	}


	// Variable store the maximum and minimum vector magnitude in this time step
	std::vector<double> fieldmax_at_time_step;
	std::vector<double> fieldmin_at_time_step;

	// Variable store the maximum and minimum potential in this time step
	std::vector<double> potentialmax_at_time_step;
	std::vector<double> potentialmin_at_time_step;


	for (step_i = 0; step_i < this->time_step_count; step_i++)
	{
		time_t = step_i * time_interval; // current time t

		// Field max
		double snapshot_field_max = 0.0;
		double snapshot_field_min = DBL_MAX;
		int delayed_index = 0;

		// All nodes snap shot vector
		// std::unordered_map<int, glm::vec2> snap_shot_vector;

		// Loop through every individual grid nodes
		for (const auto& nd_m : grid_trimesh.all_mesh_nodes)
		{
			int node_id = nd_m.second.node_id; // Get the node id of the grid node
			glm::vec2 grid_node_pt = nd_m.second.node_pt; // Get the node pt of the grid point

			// Find the index of acceleration at delayed time
			delayed_index = get_delayed_index(grid_node_pt,step_i,time_interval,light_speed_c);

			// Charge parameters
			glm::vec2 loc_at_t = charge_path.ch_location_at_t[delayed_index]; // charge location at t
			glm::vec2 v_at_t = charge_path.ch_velocity_at_t[delayed_index]; // charge velocity at t
			glm::vec2 a_at_t = charge_path.ch_acceleration_at_t[delayed_index]; // charge acceleration at t

			double v_mag_at_t = glm::length(v_at_t); // Magnitude of velocity

			// Find the w-vector
			glm::vec2 w_vector = loc_at_t - ref_zero;

			// E Vector
			// glm::vec2 e_vec = lienard_wiechert_field(grid_node_pt, v_at_t, a_at_t, v_mag_at_t, light_speed_c, w_vector, ref_zero);
			glm::vec2 e_vec = larmour_field(grid_node_pt, v_at_t, a_at_t, v_mag_at_t, light_speed_c, w_vector, ref_zero);

			double e_vec_mag = glm::length(e_vec); // Magnitude of e vector

			// Add the vector at time step
			snap_shot_Electric_field[node_id].vector_values.push_back(e_vec);
			//_____________________________________________________________________

			// Find the maximum field magnitude in this time step
			if (snapshot_field_max < e_vec_mag)
			{
				snapshot_field_max = e_vec_mag;
			}

			// Find the minimum field magnitude in this time step
			if (snapshot_field_min > e_vec_mag)
			{
				snapshot_field_min = e_vec_mag;
			}

		}

		// Add the field maximum at time step
		fieldmax_at_time_step.push_back(snapshot_field_max);

		// Add the field minimum at time step
		fieldmin_at_time_step.push_back(snapshot_field_min);

		// Potential max
		double snapshot_potential_max = DBL_MIN;
		double snapshot_potential_min = DBL_MAX;


		// Loop through every individual mesh triangles
		for (const auto& tri_m : grid_trimesh.elementtriMap)
		{
			int tri_id = tri_m.first; // Get the node id of the grid node
			elementtri_store tri = tri_m.second;

			// Get the triangle pt (3 points)
			glm::vec2 tri_pt1 = tri.nd1->node_pt; // Get the triangle pt 1
			glm::vec2 tri_pt2 = tri.nd2->node_pt; // Get the triangle pt 2
			glm::vec2 tri_pt3 = tri.nd3->node_pt; // Get the triangle pt 3

			// Find the r dash-vectors 1, 2, 3 (3 triangle points)
			// Find the index of acceleration at delayed time
			int delayed_index1 = get_delayed_index(tri_pt1, step_i, time_interval, light_speed_c);
			int delayed_index2 = get_delayed_index(tri_pt2, step_i, time_interval, light_speed_c);
			int delayed_index3 = get_delayed_index(tri_pt3, step_i, time_interval, light_speed_c);

			// Charge parameters
			// location
			glm::vec2 loc_at_t1 = charge_path.ch_location_at_t[delayed_index1]; // charge location at t
			glm::vec2 loc_at_t2 = charge_path.ch_location_at_t[delayed_index2]; // charge location at t
			glm::vec2 loc_at_t3 = charge_path.ch_location_at_t[delayed_index3]; // charge location at t

			// velocity
			glm::vec2 v_at_t1 = charge_path.ch_velocity_at_t[delayed_index1]; // charge velocity at t
			glm::vec2 v_at_t2 = charge_path.ch_velocity_at_t[delayed_index2]; // charge velocity at t
			glm::vec2 v_at_t3 = charge_path.ch_velocity_at_t[delayed_index3]; // charge velocity at t

			// acceleration
			glm::vec2 a_at_t1 = charge_path.ch_acceleration_at_t[delayed_index1]; // charge acceleration at t
			glm::vec2 a_at_t2 = charge_path.ch_acceleration_at_t[delayed_index2]; // charge acceleration at t
			glm::vec2 a_at_t3 = charge_path.ch_acceleration_at_t[delayed_index3]; // charge acceleration at t

			double v_mag_at_t1 = glm::length(v_at_t1); // Magnitude of velocity at node 1
			double v_mag_at_t2 = glm::length(v_at_t2); // Magnitude of velocity at node 2
			double v_mag_at_t3 = glm::length(v_at_t3); // Magnitude of velocity at node 3

			// Find the w-vector
			glm::vec2 w_vector1 = loc_at_t1 - ref_zero;
			glm::vec2 w_vector2 = loc_at_t2 - ref_zero;
			glm::vec2 w_vector3 = loc_at_t3 - ref_zero;

			// E Vector
			glm::vec2 e_vec1 = larmour_field(tri_pt1, v_at_t1, a_at_t1, v_mag_at_t1, light_speed_c, w_vector1, ref_zero);
			glm::vec2 e_vec2 = larmour_field(tri_pt2, v_at_t2, a_at_t2, v_mag_at_t2, light_speed_c, w_vector2, ref_zero);
			glm::vec2 e_vec3 = larmour_field(tri_pt3, v_at_t3, a_at_t3, v_mag_at_t3, light_speed_c, w_vector3, ref_zero);

			//__________________________________________________________________
			double scalar_potential_pt1, scalar_potential_pt2, scalar_potential_pt3;
		
			scalar_potential_pt1 = glm::length(e_vec1);
			scalar_potential_pt2 = glm::length(e_vec2);
			scalar_potential_pt3 = glm::length(e_vec3);


			//___________________________________________________________________
			snap_shot_Electric_potential[tri_id].nd1_values.push_back(scalar_potential_pt1); // Add the scalar potential tri pt 1
			snap_shot_Electric_potential[tri_id].nd2_values.push_back(scalar_potential_pt2); // Add the scalar potential tri pt 2
			snap_shot_Electric_potential[tri_id].nd3_values.push_back(scalar_potential_pt3); // Add the scalar potential tri pt 3

			//_____________________________________________________________________

			// Find the maximum potential magnitude in this time step
			double scalar_potential_max = std::max(scalar_potential_pt1, std::max(scalar_potential_pt2, scalar_potential_pt3));
			if (snapshot_potential_max < scalar_potential_max)
			{
				snapshot_potential_max = scalar_potential_max;
			}

			// Find the minimum field magnitude in this time step
			double scalar_potential_min = std::min(scalar_potential_pt1, std::min(scalar_potential_pt2, scalar_potential_pt3));
			if (snapshot_potential_min > scalar_potential_min)
			{
				snapshot_potential_min = scalar_potential_min;
			}
		}

		// Add the potential maximum at time step
		potentialmin_at_time_step.push_back(snapshot_potential_min);

		// Add the potential minimum at time step
		potentialmax_at_time_step.push_back(snapshot_potential_max);

	}


	// Create the vector (Copy the results)
	node_vector.clear_data(); // clear the node vector data
	for (const auto& nd_m : snap_shot_Electric_field)
	{
		int nd_id = nd_m.first;
		vector_data nd_vector = nd_m.second;

		// Add the node vector
		node_vector.add_vector(nd_id, nd_vector.vector_loc, nd_vector.vector_values, fieldmax_at_time_step, fieldmin_at_time_step);
	}

	// Create the potential contour (Copy the results)
	node_contour.clear_data(); // clear the node contour data
	for (const auto& tri_m : snap_shot_Electric_potential)
	{
		int tri_id = tri_m.first;
		tricontour_data tri_data = tri_m.second;

		// Add the triangle contour
		node_contour.add_tricontour(tri_id, tri_data.nd1, tri_data.nd2, tri_data.nd3,
			tri_data.nd1_values, tri_data.nd2_values, tri_data.nd3_values,
			potentialmax_at_time_step, potentialmin_at_time_step);
	}


	is_analysis_complete = true;

}

glm::vec2 charge_oscillation_solver::lienard_wiechert_field(const glm::vec2& grid_node_pt,
	const glm::vec2& v_at_t, const glm::vec2& a_at_t, const double& v_mag_at_t,
	const double& light_speed_c, const glm::vec2& w_vector, const glm::vec2& ref_zero)
{
	// Lienard Wiechert Field
	// Find the r-vector
	glm::vec2 r_vector = grid_node_pt - ref_zero;

	// Find the r dash-vector
	glm::vec2 r_dash_vector = r_vector - w_vector;

	// Normalize the r dash vector
	glm::vec2 norm_r_dash_vector = glm::normalize(r_dash_vector);
	double magnitude_r_dash = glm::length(r_dash_vector);

	// Find the u vector (u = cr' - v)
	glm::vec2 u_vector = (static_cast<float>(light_speed_c) * norm_r_dash_vector) - v_at_t;

	// Find the E const = r' / (r' dot u)^3
	double e_vec_const = 0.0f;

	if (magnitude_r_dash != 0.0)
	{
		e_vec_const = magnitude_r_dash / std::pow(glm::dot(r_dash_vector, u_vector), 3);
	}

	// E Vector 1
	glm::vec2 e_vec1 = static_cast<float>(std::pow(light_speed_c, 2) - std::pow(v_mag_at_t, 2)) * u_vector;

	// Cross product u x a
	glm::vec3 u_cross_a = glm::cross(glm::vec3(u_vector, 0), glm::vec3(a_at_t, 0));

	// E Vector 2
	glm::vec3 e_vec2_dash = glm::cross(glm::vec3(norm_r_dash_vector, 0), u_cross_a);
	glm::vec2 e_vec2 = glm::vec2(e_vec2_dash.x, e_vec2_dash.y);

	// Constant 1
	float k1 = 1.0;

	return k1 * (e_vec1 + e_vec2);
}


glm::vec2 charge_oscillation_solver::larmour_field(const glm::vec2& grid_node_pt,
	const glm::vec2& v_at_t, const glm::vec2& a_at_t, const double& v_mag_at_t,
	const double& light_speed_c, const glm::vec2& w_vector, const glm::vec2& ref_zero)
{
	// Larmour Field
	// Find the r-vector
	glm::vec2 r_vector = grid_node_pt - ref_zero;

	// Find the r dash-vector
	glm::vec2 r_dash_vector = r_vector - w_vector;

	// Normalize the r dash vector
	glm::vec2 norm_r_dash_vector = glm::normalize(r_dash_vector);
	double magnitude_r_dash = glm::length(r_dash_vector);

	if (magnitude_r_dash == 0)
	{
		// To avoid 0/0
		return glm::vec2(0);
	}

	// Cross 1 (r x a)
	glm::vec3 r_cross_a = glm::cross(glm::vec3(norm_r_dash_vector, 0), glm::vec3(a_at_t, 0));

	// Cross 2 a_perp = r x (r x a)
	glm::vec3 a_perp_dash = glm::cross(glm::vec3(norm_r_dash_vector, 0), r_cross_a);
	glm::vec2 a_perp = glm::vec2(a_perp_dash.x, a_perp_dash.y);

	// Constant 1
	float k1 = 1.0;

	// Constant 2
	float k2 = static_cast<float>(1.0 / (magnitude_r_dash *std::pow(light_speed_c, 2.0)));


	return k1 * k2 * (a_perp);
}


void charge_oscillation_solver::get_charge_location_data(const int& curve_type,
	const double& displ_mag,
	const double& velocity_mag,
	const double& acceleration_mag, glm::vec2& loc_at_t,
	glm::vec2& velo_at_t,
	glm::vec2& accl_at_t)
{
	// Return charge location, velocity and acceleartion at time

	if (curve_type == 0)
	{
		// Linear curve
		// get the param_t by converting -1 to 1 data to 0 to 1 data
		double param_t = (displ_mag * 0.5) + 0.5;

		loc_at_t = glm::vec2(0.0, (-100.0 * (1.0 - param_t)) + (100.0 * param_t));
		velo_at_t = glm::vec2(0.0, -200.0 * velocity_mag);
		accl_at_t = glm::vec2(0.0, 200.0 * acceleration_mag);
	}
	else if (curve_type == 1)
	{
		// Circular curve
		double param_t = (displ_mag * 0.5) + 0.5;

		loc_at_t = glm::vec2(100.0 * std::cos(param_t * 4.0 * m_pi),
							 100.0 * std::sin(param_t * 4.0 * m_pi));

		velo_at_t = glm::vec2(-100.0 * std::sin(param_t * 4.0 * m_pi) * velocity_mag, 
							 100.0 * std::cos(param_t * 4.0 * m_pi) * velocity_mag);
		accl_at_t = glm::vec2(-100.0 * std::cos(param_t * 4.0 * m_pi) * acceleration_mag, 
							   -100.0 * std::sin(param_t * 4.0 * m_pi) * acceleration_mag);
	}



}


int charge_oscillation_solver::get_delayed_index(const glm::vec2& node_pt, const int& step_i,
	const double& time_interval, const double& light_speed_c)
{
	// Delayed time
	double location_from_origin = glm::length(node_pt); // Location from length

	double delayed_time = (step_i * time_interval) - (location_from_origin / light_speed_c);

	if (delayed_time < 0)
	{
		delayed_time = 0;
	}

	// Find the index of acceleration at delayed time
	return static_cast<int>(std::round(delayed_time / time_interval));

}