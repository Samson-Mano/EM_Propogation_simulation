#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <iomanip>
#include <glm/glm.hpp>
#include "../geometry_store/geom_parameters.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

class inlcondition_window
{
public:
	const int num_of_pts = 50;
	bool is_show_window = false;
	bool execute_apply_path = false;

	double param_c = 1.0;
	double param_b = 1.0;

	// Initial Condition values
	// Store the path points as vec2
	int path_type = 0; // 0 - closed curve, 1 - open curve
	std::vector<glm::vec2> curve_paths;

	int path_scale_factor = 100;
	int previous_selected_option = 0;
	int selected_model_option = 0; // Option to control the model


	inlcondition_window();
	~inlcondition_window();
	void init();
	void render_window();
private:
	void set_linear_curve();
	void set_circular_curve();
	void set_elliptical_curve();
	void set_cardioid_curve();
	void set_trifolliate_curve();
	void get_param_c_value_input();
	void get_param_b_value_input();
};