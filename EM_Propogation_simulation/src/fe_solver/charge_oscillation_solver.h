#pragma once
#include <iostream>
#include <fstream>

// FE Objects
#include "../geometry_store/fe_objects/nodes_list_store.h"
#include "../geometry_store/fe_objects/elementtri_list_store.h"
#include "../geometry_store/fe_objects/charge_path_store.h"

// Result Objects
#include "../geometry_store/result_objects/nodevector_list_store.h"
#include "../geometry_store/result_objects/tricontour_list_store.h"

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
										   const elementtri_list_store& grid_trimesh,
										   charge_path_store& charge_path,
											const double charge_oscillation_freq,
											const double total_simulation_time,
											const double time_interval,
											nodevector_list_store& node_vector,
											tricontour_list_store& node_contour,
											bool& is_analysis_complete);


private:
	glm::vec2 lienard_wiechert_field(const glm::vec2& grid_node_pt,
		const glm::vec2& v_at_t,
		const glm::vec2& a_at_t,
		const double& v_mag_at_t,
		const double& light_speed_c,
		const glm::vec2& w_vector,
		const glm::vec2& ref_zero);

	glm::vec2 larmour_field(const glm::vec2& grid_node_pt,
		const glm::vec2& v_at_t,
		const glm::vec2& a_at_t,
		const double& v_mag_at_t,
		const double& light_speed_c,
		const glm::vec2& w_vector,
		const glm::vec2& ref_zero);

	void get_charge_location_data(const int& curve_type, 
		const double& displ_mag, 
		const double& velocity_mag,
		const double& acceleration_mag, glm::vec2& loc_at_t,
		glm::vec2& velo_at_t,
		glm::vec2& accl_at_t);


	int get_delayed_index(const glm::vec2& node_pt, const int& step_i, 
		const double& time_interval, const double& light_speed_c);

};
