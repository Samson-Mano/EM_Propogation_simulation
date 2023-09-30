#pragma once
#include <iostream>
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"

class options_window
{
public:
	bool is_show_gridnode = true;
	bool is_show_gridnodenumber = false;
	bool is_show_gridnodecoord = false;

	bool is_show_gridtris = true;
	bool is_show_gridtris_shrunk = false;
	bool is_show_gridboundary = true;

	bool is_show_window = false;

	options_window();
	~options_window();
	void init();
	void render_window();
private:

};
