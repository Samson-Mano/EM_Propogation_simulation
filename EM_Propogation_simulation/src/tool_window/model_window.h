#pragma once
#include <iostream>
#include <fstream>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include <chrono>

class model_window
{
public:
	bool is_show_window = false;
	bool is_execute_apply = false;

	// Input for the window
	double grid_length = 100.0; // Line length
	double segment_length = 1.0; // Number of nodes
	double space_permittivity = 1.0; // Line tension
	double material_density = 1.0; // Material density

	model_window();
	~model_window();
	void init(double grid_length,
		double segment_length,
		double space_permittivity,
		double material_density);
	void render_window(bool& isWindowSizeChanging);
private:

};