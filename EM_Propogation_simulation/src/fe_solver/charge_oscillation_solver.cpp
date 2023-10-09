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
	charge_path_store& charge_path, 
	const double total_simulation_time, 
	const double time_interval, 
	nodevector_list_store& node_vector, 
	bool& is_analysis_complete)
{
	is_analysis_complete = false;

	// Step 1: Find the charge location, velocity and acceleration
	std::vector<glm::vec2> charge_loc_at_t;

	//// get all the points
	//std::vector<glm::vec2> chargePathPts;

	//// Iterate through chargePathMap and extract path_pts
	//for (const charge_path_points& point : charge_path.chargePathMap) 
	//{
	//	chargePathPts.push_back(point.path_pts);
	//}

	//double total_travel_length= charge_path.charge_total_length; // Total charge length
	//double charge_oscillation_freq = charge_path.charge_oscillation_freq; // Charge oscillation frequency
	//int loop_type = charge_path.path_type; // Path type 0 - closed loop, 1 - open loop
	//std::vector<double> segment_length = charge_path.segment_length; // Individual path segment length

	double param_t = 0.0;
	double angular_freq = 2.0 * m_pi * charge_path.charge_oscillation_freq; // Angular charge oscillation frequency
	int cycle_n = 1;
	double t1 = 0.0;

	for (double time_t = 0.0; time_t <= total_simulation_time; time_t = time_t + time_interval)
	{
		if (charge_path.path_type == 0)
		{
			// Check to cycle the loop
			if ((cycle_n * m_pi) < (angular_freq * time_t))
			{
				t1 = time_t;
				cycle_n++;
			}
			param_t = 0.25 * (1.0 - std::cos(angular_freq * (time_t - t1)));

		}
		else
		{
			param_t = 0.5 * (1.0 - std::cos(angular_freq * time_t));
		}
		
		glm::vec2 ch_at_t = charge_path.get_charge_path_location_at_t(param_t);

		std::cout << ch_at_t.x << "," << ch_at_t.y << std::endl;


	}



	is_analysis_complete = true;

}


