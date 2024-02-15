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

	ImGui::RadioButton("Linear path oscillation", &selected_model_option, 0);
	ImGui::RadioButton("Circular path oscillation", &selected_model_option, 1);
	ImGui::RadioButton("Elliptical path oscillation", &selected_model_option, 2);
	ImGui::RadioButton("Cardioid path oscillation", &selected_model_option, 3);
	ImGui::RadioButton("TriFolliate path oscillation", &selected_model_option, 4);

	// get_param_c_value_input();
	// get_param_b_value_input();

	// Routine to check whether the option changed
	if (previous_selected_option != selected_model_option)
	{
		// set the previous selected option
		previous_selected_option = selected_model_option;

		// Set the charge path points
		if (selected_model_option == 0)
		{
			// Linear
			set_linear_curve();
		}
		else if (selected_model_option == 1)
		{
			// Circular
			set_circular_curve();
		}
		else if (selected_model_option == 2)
		{
			// Elliptical
			set_elliptical_curve();
		}
		else if (selected_model_option == 3)
		{
			// Cardioid
			set_cardioid_curve();
		}
		else if (selected_model_option == 4)
		{
			// Tri Folliate
			set_trifolliate_curve();
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

	for (int i = 0; i < num_of_pts; i++)
	{
		double angle = (i / static_cast<float>(num_of_pts)) * 2.0 * 3.141592653589793238463;

		float x = 100.0 * std::cos(angle);
		float y = 100.0 * std::sin(angle);

		glm::vec2 pt = glm::vec2(x, y);
		this->curve_paths.push_back(pt);
	}


}

void inlcondition_window::set_elliptical_curve()
{
	// Elliptical curve

	path_type = 0;
	this->curve_paths.clear();

	for (int i = 0; i < num_of_pts; i++)
	{
		double angle = (i / static_cast<float>(num_of_pts)) * 2.0 * 3.141592653589793238463;

		float x = 150.0 * std::cos(angle);
		float y = 75.0 * std::sin(angle);

		glm::vec2 pt = glm::vec2(x, y);
		this->curve_paths.push_back(pt);
	}

}


void inlcondition_window::set_cardioid_curve()
{
	// Cardioid curve

	path_type = 0;
	this->curve_paths.clear();

	for (int i = 0; i < num_of_pts; i++)
	{
		double angle = (i / static_cast<float>(num_of_pts)) * 2.0 * 3.141592653589793238463;

		float x = 100.0 *(1.0 - std::cos(angle)) * std::cos(angle);
		float y = 100.0 * (1.0 - std::cos(angle)) * std::sin(angle);

		glm::vec2 pt = glm::vec2(x, y);
		this->curve_paths.push_back(pt);
	}


}


void inlcondition_window::set_trifolliate_curve()
{
	// Tri-Folliate curve

	path_type = 0;
	this->curve_paths.clear();

	for (int i = 0; i < num_of_pts; i++)
	{
		double angle = (i / static_cast<float>(num_of_pts)) * 1.0* 3.141592653589793238463;

		float x = 100.0 * std::cos(angle) * std::cos(3.0 * angle);
		float y = 100.0 * std::sin(angle) * std::cos(3.0 * angle);

		glm::vec2 pt = glm::vec2(x, y);
		this->curve_paths.push_back(pt);
	}

}

void inlcondition_window::get_param_c_value_input()
{
	// Input box to give input via text
	static bool displx_input_mode = false;
	static char displx_str[16] = ""; // buffer to store input Displacement X string
	static float displx_input = 0; // buffer to store input Displacement X value

	// Button to switch to input mode
	if (!displx_input_mode)
	{
		if (ImGui::Button("Param C"))
		{
			displx_input_mode = true;
			snprintf(displx_str, 16, "%.1f", param_c); // set the buffer to current Displacement X
		}
	}
	else // input mode
	{
		// Text box to input value
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##Displacement X", displx_str, IM_ARRAYSIZE(displx_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			param_c = atof(displx_str);
			// set the load value to input value
			// deformation_scale_max = defscale_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			displx_input_mode = false;
		}
	}

	// Text for load value
	ImGui::SameLine();
	ImGui::Text(" %.1f", param_c);

}


void inlcondition_window::get_param_b_value_input()
{
	// Input box to give input via text
	static bool displx_input_mode = false;
	static char displx_str[16] = ""; // buffer to store input Displacement X string
	static float displx_input = 0; // buffer to store input Displacement X value

	// Button to switch to input mode
	if (!displx_input_mode)
	{
		if (ImGui::Button("Param B"))
		{
			displx_input_mode = true;
			snprintf(displx_str, 16, "%.1f", param_b); // set the buffer to current Displacement X
		}
	}
	else // input mode
	{
		// Text box to input value
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##Displacement X", displx_str, IM_ARRAYSIZE(displx_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			param_b = atof(displx_str);
			// set the load value to input value
			// deformation_scale_max = defscale_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			displx_input_mode = false;
		}
	}

	// Text for load value
	ImGui::SameLine();
	ImGui::Text(" %.1f", param_b);



}