#include "inlcondition_window.h"

inlcondition_window::inlcondition_window()
{
	// Empty Constructor
}

inlcondition_window::~inlcondition_window()
{
	// Empty Destructor
}

void inlcondition_window::init()
{
	is_show_window = false;

	previous_selected_option = 0;
	selected_model_option = 0;
	set_linear_curve();

}

void inlcondition_window::render_window()
{
	if (is_show_window == false)
		return;

	ImGui::Begin("Intial condition");

	ImGui::RadioButton("Linear oscillation", &selected_model_option, 0);
	ImGui::RadioButton("Circular oscillation type - I", &selected_model_option, 1);
	// ImGui::RadioButton("Circular oscillation type - II", &selected_model_option, 2);
	ImGui::RadioButton("Elliptical osillation", &selected_model_option, 3);

	// Routine to check whether the option changed
	if (previous_selected_option != selected_model_option)
	{
		// set the previous selected option
		previous_selected_option = selected_model_option;

		// Set the charge path points
		if (selected_model_option == 0)
		{
			set_linear_curve();
		}
		else if (selected_model_option == 1)
		{
			set_circular_curve();
		}

		execute_apply_path = true;
	}

	//_________________________________________________________________________________________

// Add a "Close" button (Close the solver window)
	if (ImGui::Button("Close"))
	{
		is_show_window = false;
	}


	ImGui::End();

}


void inlcondition_window::set_linear_curve()
{
	// Linear curve
	path_type = 1;
	this->curve_paths.clear();

	// Point 1
	glm::vec2 pt = glm::vec2(0.0f, -100.0f);
	this->curve_paths.push_back(pt);

	// Point 2
	pt = glm::vec2(0.0f, 100.0f);
	this->curve_paths.push_back(pt);
}

void inlcondition_window::set_circular_curve()
{
	// Circular curve
	path_type = 0;
	this->curve_paths.clear();
	int num_of_pts = 20;

	for (int i = 0; i < num_of_pts; i++)
	{
		double angle = (i / static_cast<float>(num_of_pts)) * 2.0 * 3.141592653589793238463;

		float x = 100.0 * std::cos(angle);
		float y = 100.0 * std::sin(angle);

		glm::vec2 pt = glm::vec2(x, y);
		this->curve_paths.push_back(pt);
	}


}