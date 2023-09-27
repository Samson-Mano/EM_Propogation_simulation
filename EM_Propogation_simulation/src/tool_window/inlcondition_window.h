#pragma once
#include <iostream>
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

class inlcondition_window
{
public:
	bool is_show_window = false;
	bool execute_apply_path = false;
	

	// Initial Condition values
		
	double time_period = 10.0;

	inlcondition_window();
	~inlcondition_window();
	void init();
	void render_window();
private:

};