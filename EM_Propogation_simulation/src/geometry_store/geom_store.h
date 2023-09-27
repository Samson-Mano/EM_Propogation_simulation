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

class geom_store
{
public:
	const double m_pi = 3.14159265358979323846;
	bool is_geometry_set = false;

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

	// View options ptr and Material window ptr
	options_window* op_window = nullptr;
	solver_window* sol_window = nullptr;
	model_window* md_window = nullptr;
	inlcondition_window* inl_window = nullptr;

};