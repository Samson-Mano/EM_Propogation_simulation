#include "options_window.h"

options_window::options_window()
{
	// Empty constructor
}

options_window::~options_window()
{
	// Empty destructor
}

void options_window::init()
{
    // Initialize the options
     is_show_gridnode = true;
     is_show_gridtris = true;
     is_show_gridtris_shrunk = false;
     is_show_gridboundary = true;

     is_show_window = false;
}

void options_window::render_window()
{
    if (is_show_window == false)
        return;

    // Create a new ImGui options window
    ImGui::Begin("View Options");

    // Add 5 checkboxes
    ImGui::Checkbox("Show Grid Node", &is_show_gridnode);
    ImGui::Checkbox("Show Grid Mesh", &is_show_gridtris);
    ImGui::Checkbox("Show Grid Mesh Triangle shrunk", &is_show_gridtris_shrunk);
    ImGui::Checkbox("Show Grid Boundary", &is_show_gridboundary);

    // Add a "Close" button
    if (ImGui::Button("Close"))
    {
        is_show_window = false;
    }

    ImGui::End();
}
