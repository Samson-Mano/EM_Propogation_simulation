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
    is_show_nodenumber = true;
    is_show_nodecoord = false;
    is_show_linenumber = true;
    is_show_linelength = false;
    is_show_loadvalue = true;
    is_show_window = false;
}

void options_window::render_window()
{
    if (is_show_window == false)
        return;

    // Create a new ImGui options window
    ImGui::Begin("View Options");

    // Add 5 checkboxes
    ImGui::Checkbox("Show Node ID", &is_show_nodenumber);
    ImGui::Checkbox("Show Node Coordinates", &is_show_nodecoord);
    ImGui::Checkbox("Show line ID", &is_show_linenumber);
    ImGui::Checkbox("Show line length", &is_show_linelength);
    ImGui::Checkbox("Show load value", &is_show_loadvalue);

    // Add a "Close" button
    if (ImGui::Button("Close"))
    {
        is_show_window = false;
    }

    ImGui::End();
}
