#pragma once
#include <iostream>
#include "../geometry_store/geom_parameters.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

class solver_window
{
public:
	bool is_show_window = false;
	bool execute_open = false;
	bool execute_close = false;
	bool execute_solve = false;

	// Analysis complete
	bool is_analysis_complete = false;

	// View option
	bool show_undeformed_model = true;
	bool show_contour_plot = true;
	bool show_vector_plot = true;

	// Charge oscillation frequency
	double charge_oscillation_freq = 1.0f;

	// Simulation time control variable
	double total_simulation_time = 10.0f;
	double time_interval = 0.01f;

	// Animation control
	bool animate_play = true;
	bool animate_pause = false;
	double deformation_scale_max = 20.0;
	double animation_speed = 1.0;

	// Time step control
	double time_interval_atrun = 0.0; // Value of time interval used in the pulse response 
	int time_step_count = 0;
	int time_step = 0;

	solver_window();
	~solver_window();
	void init();
	void render_window();
private:
	Stopwatch stopwatch;
};
