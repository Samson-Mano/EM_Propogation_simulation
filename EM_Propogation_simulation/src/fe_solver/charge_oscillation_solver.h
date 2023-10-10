#pragma once
#include <iostream>
#include <fstream>

// FE Objects
#include "../geometry_store/fe_objects/nodes_list_store.h"
#include "../geometry_store/fe_objects/charge_path_store.h"

// Result Objects
#include "../geometry_store/fe_objects/nodevector_list_store.h"

#pragma warning(push)
#pragma warning (disable : 26451)
#pragma warning (disable : 26495)
#pragma warning (disable : 6255)
#pragma warning (disable : 6294)
#pragma warning (disable : 26813)
#pragma warning (disable : 26454)

// Optimization for Eigen Library
// 1) OpenMP (Yes (/openmp)
//	 Solution Explorer->Configuration Properties -> C/C++ -> Language -> Open MP Support
// 2) For -march=native, choose "AVX2" or the latest supported instruction set.
//   Solution Explorer->Configuration Properties -> C/C++ -> Code Generation -> Enable Enhanced Instruction Set 

#include <Eigen/Dense>
#include <Eigen/Sparse>
// Define the sparse matrix type for the reduced global stiffness matrix
typedef Eigen::SparseMatrix<double> SparseMatrix;
#pragma warning(pop)


class charge_oscillation_solver
{
public:
	const double m_pi = 3.14159265358979323846;
	double time_interval = 0.0;
	int time_step_count = 0;

	charge_oscillation_solver();
	~charge_oscillation_solver();
	void charge_oscillation_analysis_start(const nodes_list_store& grid_nodes,
										   charge_path_store& charge_path,
											const double charge_oscillation_freq,
											const double total_simulation_time,
											const double time_interval,
											nodevector_list_store& node_vector,
											bool& is_analysis_complete);


private:

};
