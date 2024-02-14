#include "charge_oscillation_solver.h"

charge_oscillation_solver::charge_oscillation_solver()
{
	// Empty constructor
}

charge_oscillation_solver::~charge_oscillation_solver()
{
	// Empty destructor
}

void charge_oscillation_solver::charge_oscillation_analysis_start(const nodes_list_store& grid_vector_nodes,
	const nodes_list_store& grid_nodes,
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
		// Calculate the cycle count
		if ((angular_freq * time_t) > (cycle_n * angular_freq))
		{
			cycle_n++;
		}

		// get the charge location
		glm::vec2 loc_at_t = glm::vec2(0); // charge_path.get_charge_path_location_at_t(param_t).first;
		
		// velocity and acceleration vector
		glm::vec2 velo_at_t = glm::vec2(0);
		glm::vec2 accl_at_t = glm::vec2(0);

		get_charge_location_data(charge_path.curve_type,
			time_t,
			angular_freq,
			cycle_n,
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
	std::unordered_map<int, vector_data> snap_shot_Electric_potential; // Electric potential

	int node_id = 0;
	glm::vec2 node_pt = glm::vec2(0);

	// Create a Frame work for the vectors
	for (const auto& nd_m : grid_vector_nodes.nodeMap)
	{
		node_store nd = nd_m.second;

		node_id = nd.node_id;
		node_pt = nd.node_pt;

		// Set the vector id & vector point
		snap_shot_Electric_field[node_id].vector_id = node_id;
		snap_shot_Electric_field[node_id].vector_loc.resize(this->time_step_count, node_pt);
	}

	// Create a Frame work for the contours
	for (const auto& nd_m : grid_nodes.nodeMap)
	{
		node_store nd = nd_m.second;

		node_id = nd.node_id;
		node_pt = nd.node_pt;

		// Set the vector id & vector point
		snap_shot_Electric_potential[node_id].vector_id = node_id;
		snap_shot_Electric_potential[node_id].vector_loc.resize(this->time_step_count, node_pt);

	}


	// Variable store the maximum and minimum potential in this time step
	std::vector<double> potentialmax_at_time_step;
	std::vector<double> potentialmin_at_time_step;

	// Local variables
	double snapshot_potential_max = DBL_MIN;
	double snapshot_potential_min = DBL_MAX;

	int delayed_index = 0;

	glm::vec2 loc_at_t = glm::vec2(0); // charge location at t
	glm::vec2 v_at_t = glm::vec2(0); // charge velocity at t
	glm::vec2 a_at_t = glm::vec2(0); // charge acceleration at t

	double v_mag_at_t = 0.0; // Magnitude of velocity

	// Find the w-vector
	glm::vec2 w_vector = glm::vec2(0);

	// E Vector
	glm::vec2 e_vec = glm::vec2(0);

	double e_vec_mag = 0.0; // Magnitude of e vector

	for (step_i = 0; step_i < this->time_step_count; step_i++)
	{
		time_t = step_i * time_interval; // current time t

		// Loop through every individual grid nodes
		for (const auto& nd_m : grid_vector_nodes.nodeMap)
		{
			node_store nd = nd_m.second;

			node_id = nd.node_id; // Get the node id of the grid node
			node_pt = nd.node_pt; // Get the node pt of the grid point

			// Find the index of acceleration at delayed time
			delayed_index = get_delayed_index(node_pt,step_i,time_interval,light_speed_c);

			// Charge parameters
			loc_at_t = charge_path.ch_location_at_t[delayed_index]; // charge location at t
			v_at_t = charge_path.ch_velocity_at_t[delayed_index]; // charge velocity at t
			a_at_t = charge_path.ch_acceleration_at_t[delayed_index]; // charge acceleration at t

			v_mag_at_t = glm::length(v_at_t); // Magnitude of velocity

			// Find the w-vector
			w_vector = loc_at_t - ref_zero;

			// E Vector
			// e_vec = lienard_wiechert_field(grid_node_pt, v_at_t, a_at_t, v_mag_at_t, light_speed_c, w_vector, ref_zero);
			e_vec = larmour_field(node_pt, v_at_t, a_at_t, v_mag_at_t, light_speed_c, w_vector, ref_zero);

			e_vec_mag = glm::length(e_vec); // Magnitude of e vector

			// Add the vector at time step
			snap_shot_Electric_field[node_id].vector_values.push_back(e_vec);

		}

		// Potential max
		snapshot_potential_max = DBL_MIN;
		snapshot_potential_min = DBL_MAX;


		// Loop through every individual mesh triangle's nodes
		for (const auto& nd_m : grid_nodes.nodeMap)
		{
			node_store nd = nd_m.second;

			node_id = nd.node_id; // Get the node id of the mesh node int
			node_pt = nd.node_pt; // Get the node pt of the mesh node point

			// Find the index of acceleration at delayed time
			delayed_index = get_delayed_index(node_pt, step_i, time_interval, light_speed_c);

			// Charge parameters
			loc_at_t = charge_path.ch_location_at_t[delayed_index]; // charge location at t
			v_at_t = charge_path.ch_velocity_at_t[delayed_index]; // charge velocity at t
			a_at_t = charge_path.ch_acceleration_at_t[delayed_index]; // charge acceleration at t

			v_mag_at_t = glm::length(v_at_t); // Magnitude of velocity

			// Find the w-vector
			w_vector = loc_at_t - ref_zero;

			// E Vector
			// e_vec = lienard_wiechert_field(grid_node_pt, v_at_t, a_at_t, v_mag_at_t, light_speed_c, w_vector, ref_zero);
			e_vec = larmour_field(node_pt, v_at_t, a_at_t, v_mag_at_t, light_speed_c, w_vector, ref_zero);

			e_vec_mag = glm::length(e_vec); // Magnitude of e vector

			// Add the vector at time step
			snap_shot_Electric_potential[node_id].vec_ratio.push_back(e_vec_mag);
			//_____________________________________________________________________

			// Find the maximum potential magnitude in this time step
			if (snapshot_potential_max < e_vec_mag)
			{
				snapshot_potential_max = e_vec_mag;
			}

			// Find the minimum potential magnitude in this time step
			if (snapshot_potential_min > e_vec_mag)
			{
				snapshot_potential_min = e_vec_mag;
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
		node_vector.add_vector(nd_id, nd_vector.vector_loc, nd_vector.vector_values, potentialmax_at_time_step, potentialmin_at_time_step);
	}

	// Create the potential contour (Copy the results)
	node_contour.clear_data(); // clear the node contour data
	for (const auto& tri_m : grid_trimesh.elementtriMap)
	{
		elementtri_store tri = tri_m.second;
		int nd1_id = tri.nd1->node_id;
		int nd2_id = tri.nd2->node_id;
		int nd3_id = tri.nd3->node_id;

		// Add the triangle contour
		node_contour.add_tricontour(tri.tri_id, tri.nd1, tri.nd2, tri.nd3,
			snap_shot_Electric_potential[nd1_id].vec_ratio,
			snap_shot_Electric_potential[nd2_id].vec_ratio,
			snap_shot_Electric_potential[nd3_id].vec_ratio,
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
	const double& time_t,
	const double& angular_freq,
	const int& cycle_count,
	glm::vec2& loc_at_t,
	glm::vec2& velo_at_t,
	glm::vec2& accl_at_t)
{
	// Return charge location, velocity and acceleartion at time

	if (curve_type == 0)
	{


		// Displacement
		double displ_mag = std::sin(angular_freq * time_t);

		// Velocity
		double velocity_mag = angular_freq * std::cos(angular_freq * time_t);

		// Acceleration
		double acceleration_mag = -1.0 * angular_freq * angular_freq * std::sin(angular_freq * time_t);

		// Linear curve
		// get the param_t by converting -1 to 1 data to 0 to 1 data
		double param_t = (displ_mag * 0.5) + 0.5;

		loc_at_t = glm::vec2(0.0, (-100.0 * (1.0 - param_t)) + (100.0 * param_t));
		velo_at_t = -1.0f * glm::vec2(0.0, 200.0 * velocity_mag);
		accl_at_t = glm::vec2(0.0, 200.0 * acceleration_mag);
	}
	else if (curve_type == 1)
	{
		// Circular curve

		loc_at_t = glm::vec2(100.0 * std::cos(time_t * 2.0 * m_pi),
							 100.0 * std::sin(time_t * 2.0 * m_pi));

		velo_at_t = - 1.0f * glm::vec2(-100.0 * std::sin(time_t * 2.0 * m_pi),
							 100.0 * std::cos(time_t * 2.0 * m_pi));
		accl_at_t = glm::vec2(-100.0 * std::cos(time_t * 2.0 * m_pi),
							   -100.0 * std::sin(time_t * 2.0 * m_pi));
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