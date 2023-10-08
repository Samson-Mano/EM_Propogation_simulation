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
	const charge_path_store& charge_path, 
	const double total_simulation_time, 
	const double time_interval, 
	dynamic_texture_list_store& charge_path_data,
	nodevector_list_store& node_vector, 
	bool& is_analysis_complete)
{
	// Step 1: Find the charge location, velocity and acceleration
	std::vector<glm::vec2> charge_loc_at_t;

	double total_travel_length= charge_path.charge_total_length;

	for (double time_t = 0.0; time_t <= total_simulation_time; time_t = time_t + time_interval)
	{





	}



}


