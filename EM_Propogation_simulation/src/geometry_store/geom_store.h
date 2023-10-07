#pragma once
#include "geom_parameters.h"
// File system
#include <fstream>
#include <sstream>
#include <iomanip>

// Window includes
#include "../tool_window/inlcondition_window.h"
#include "../tool_window/model_window.h"
#include "../tool_window/options_window.h"
#include "../tool_window/solver_window.h"

// FE Objects
#include "fe_objects/nodes_list_store.h"
#include "fe_objects/elementline_list_store.h"
#include "fe_objects/elementtri_list_store.h"
#include "fe_objects/charge_path_store.h"

// Solver
#include "../fe_solver/charge_oscillation_solver.h"

// Geometry objects
#include "geometry_objects/label_list_store.h"

class geom_store
{
public:
	const double m_pi = 3.14159265358979323846;
	bool is_geometry_set = false;

	// Analysis complete
	bool is_analysis_complete = false;

	// Main Variable to strore the geometry parameters
	geom_parameters geom_param;

	geom_store();
	~geom_store();
	void init(options_window* op_window, solver_window* sol_window,
		model_window* md_window, inlcondition_window* inl_window);
	void fini();

	// Functions to control the origin
	void update_WindowDimension(const int& window_width, const int& window_height);
	void update_model_matrix();
	void update_model_zoomfit();
	void update_model_pan(glm::vec2& transl);
	void update_model_zoom(double& z_scale);
	void update_model_transperency(bool is_transparent);

	// Functions to paint the geometry and results
	void paint_geometry();

private:
	// General geometry properties
	double gird_length = 0.0;
	double gird_spacing = 0.0;
	double space_permittivity = 0.0;
	double material_density = 0.0;

	// geomerty object
	charge_path_store charge_path;
	elementtri_list_store grid_trimesh;
	nodes_list_store grid_nodes;
	nodes_list_store boundary_nodes;
	elementline_list_store boundary_lines;
	label_list_store model_labels;

	// Result mesh



	// View options ptr and Material window ptr
	options_window* op_window = nullptr;
	solver_window* sol_window = nullptr;
	model_window* md_window = nullptr;
	inlcondition_window* inl_window = nullptr;

	// Create geometry
	void create_geometry();

	std::pair<glm::vec2, glm::vec2> findMinMaxXY(const std::unordered_map<int, node_store>& model_nodes);
	glm::vec2 findGeometricCenter(const std::unordered_map<int, node_store>& model_nodes);


	void paint_model(); // Paint the model

	void paint_postprocess(); // Paint the results
};