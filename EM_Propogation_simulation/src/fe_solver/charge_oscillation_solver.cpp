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
	bool& is_analysis_complete)
{
	is_analysis_complete = false;

	// Step 1: Find the charge location, velocity and acceleration
	std::vector<glm::vec2> charge_loc_at_t; // Location at time t
	std::vector<glm::vec2> charge_velcoty_at_t; // Velocity at time t
	std::vector<glm::vec2> charge_acceleration_at_t; // Acceleration at time t

	double param_t = 0.0;
	double angular_freq = 2.0 * m_pi * charge_oscillation_freq; // Angular charge oscillation frequency
	int cycle_n = 1; // number of cycle for closed loop
	double t1 = 0.0; // variable to aid in loop cycle time skip
	double time_t = 0.0; // time t

	for (time_t = 0.0; time_t <= total_simulation_time; time_t = time_t + time_interval)
	{
		double velocity_mag = 0.0; // Velocity magnitude
		double acceleration_mag = 0.0; // Acceleration magnitude

		if (charge_path.path_type == 0)
		{
			// Check to cycle the loop
			if ((cycle_n * m_pi) < (angular_freq * time_t))
			{
				t1 = time_t;
				cycle_n++;
			}
			param_t = 0.5 * (1.0 - std::cos(angular_freq * (time_t - t1)));

			// Velocity
			velocity_mag = 0.5 * angular_freq * std::sin(angular_freq * (time_t - t1));
			// Acceleration
			acceleration_mag = 0.5 * angular_freq * angular_freq * std::cos(angular_freq * (time_t - t1));
		}
		else
		{
			param_t = 0.5 * (1.0 - std::cos(angular_freq * time_t));

			// Velocity
			velocity_mag = 0.5 * angular_freq * std::sin(angular_freq * time_t);
			// Acceleration
			acceleration_mag = 0.5 * angular_freq * angular_freq * std::cos(angular_freq * time_t);
		}

		// get the charge location
		glm::vec2 loc_at_t = charge_path.get_charge_path_location_at_t(param_t).first;
		glm::vec2 dir_vector = charge_path.get_charge_path_location_at_t(param_t).second;

		// velocity and acceleration vector
		glm::vec2 velo_at_t = dir_vector * static_cast<float>(velocity_mag);
		glm::vec2 accl_at_t = dir_vector * static_cast<float>(acceleration_mag);

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
	const double light_speed_c = 300000000; // Light speed c
	node_vector.clear_data(); // clear the node vector data
	glm::vec2 ref_zero = glm::vec2(0); // Reference zero

	for (step_i = 0; step_i < this->time_step_count; step_i++)
	{
		time_t = step_i * time_interval; // current time t

		// Charge parameters
		glm::vec2 loc_at_t = charge_path.ch_location_at_t[step_i]; // charge location at t
		glm::vec2 v_at_t = charge_path.ch_velocity_at_t[step_i]; // charge velocity at t
		glm::vec2 a_at_t = charge_path.ch_acceleration_at_t[step_i]; // charge acceleration at t

		double v_mag = glm::length(v_at_t); // Magnitude of velocity

		// Find the w-vector
		glm::vec2 w_vector = loc_at_t - ref_zero;

		// Loop through every individual grid nodes
		for (const auto& nd_m : grid_nodes.nodeMap)
		{
			int node_id = nd_m.second.node_id; // Get the node id of the grid node
			glm::vec2 grid_node_pt = nd_m.second.node_pt; // Get the node pt of the grid point

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
			double e_vec_const = magnitude_r_dash / std::pow( glm::dot(r_dash_vector, u_vector),3);




		}


		


	}




	is_analysis_complete = true;

}


