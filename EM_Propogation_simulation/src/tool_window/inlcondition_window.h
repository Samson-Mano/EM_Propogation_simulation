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
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

class inlcondition_window
{
public:
	bool is_show_window = false;
	bool execute_apply_path = false;
	bool curve_imported = false;
	
	// Initial Condition values
	// Store the path points as list of string	
	std::vector<std::string> curve_paths;
	int selected_curvepath_option = 0; // type of curve path
	int path_scale_factor = 100;
	// double oscillation_freq = 1.0; // Oscillation frequency
	int selected_model_option = 0; // Option to control the model


	inlcondition_window();
	~inlcondition_window();
	void init();
	void render_window();
private:
	std::string ShowOpenFileDialog_dxf();
	void import_dxfdata_geometry();


};