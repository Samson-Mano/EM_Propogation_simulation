#include "model_window.h"

model_window::model_window()
{
	// Empty Constructor
}

model_window::~model_window()
{
	// Empty Destructor
}

void model_window::init(double grid_length,
	double segment_length,
	double space_permittivity,
	double material_density)
{
	is_show_window = false;
	is_execute_apply = false;

	// Add to the input
	this->grid_length = grid_length; // Grid length
	this->segment_length = segment_length; // Segement length
	this->space_permittivity = space_permittivity; // Space permittivity (electric constant)
	this->material_density = material_density; // Material density
}

void model_window::render_window(bool& isWindowSizeChanging)
{
	if (is_show_window == false)
		return;


	ImGui::Begin("Edit Model");

	// Inputs
	// Text for Grid Length
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool gridlength_input_mode = false;
	static char gridlength_str[16] = ""; // buffer to store input Gird total length string
	static float gridlength_input = static_cast<float>(grid_length); // buffer to store input Gird total length

	// Button to switch to input mode
	if (!gridlength_input_mode)
	{
		if (ImGui::Button("Grid length"))
		{
			gridlength_input_mode = true;
			snprintf(gridlength_str, 16, "%.3f", gridlength_input); // set the buffer to current model length
		}
	}
	else // input mode
	{
		// Text box to input grid length
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputGridLength", gridlength_str, IM_ARRAYSIZE(gridlength_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			gridlength_input = static_cast<float>(atof(gridlength_str));
			// set the model grid length to input value
			grid_length = gridlength_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			gridlength_input_mode = false;
		}
	}

	// Text for Gird length
	ImGui::SameLine();
	ImGui::Text("Total gird length = %.3f", grid_length);



	// Text for Segment Length
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool seglength_input_mode = false;
	static char seglength_str[16] = ""; // buffer to store input segment length string
	static float seglength_input = static_cast<float>(segment_length); // buffer to store input segment length

	// Button to switch to input mode
	if (!seglength_input_mode)
	{
		if (ImGui::Button("Grid spacing"))
		{
			seglength_input_mode = true;
			snprintf(seglength_str, 16, "%.3f", seglength_input); // set the buffer to current number of nodes
		}
	}
	else // input mode
	{
		// Text box to input segment length
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputGridSpacing", seglength_str, IM_ARRAYSIZE(seglength_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			seglength_input = static_cast<float>(atof(seglength_str));
			// set the segment length to input value
			segment_length = seglength_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			seglength_input_mode = false;
		}
	}

	// Text for Grid spacing (segment length)
	ImGui::SameLine();
	ImGui::Text("Grid spacing length = %.3f", segment_length);


	// Text for Electric constant
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool electricconstant_input_mode = false;
	static char electricconstant_str[16] = ""; // buffer to store input Electric constant string
	static float electricconstant_input = static_cast<float>(space_permittivity); // buffer to store input Electric constant

	// Button to switch to input mode
	if (!electricconstant_input_mode)
	{
		if (ImGui::Button("Electric constant"))
		{
			electricconstant_input_mode = true;
			snprintf(electricconstant_str, 16, "%.3f", electricconstant_input); // set the buffer to current Electric constant
		}
	}
	else // input mode
	{
		// Text box to input Tension
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputLineTension", electricconstant_str, IM_ARRAYSIZE(electricconstant_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			electricconstant_input = static_cast<float>(atof(electricconstant_str));
			// set the Space permittivity to input value
			space_permittivity = electricconstant_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			electricconstant_input_mode = false;
		}
	}

	// Text for Space Permittivity
	ImGui::SameLine();
	ImGui::Text("Electric constant = %.3f", space_permittivity);


	// Text for Density values
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool matdensity_input_mode = false;
	static char matdensity_str[16] = ""; // buffer to store input Density string
	static float matdensity_input = static_cast<float>(material_density); // buffer to store input material Density

	// Button to switch to input mode
	if (!matdensity_input_mode)
	{
		if (ImGui::Button("Material Density"))
		{
			matdensity_input_mode = true;
			snprintf(matdensity_str, 16, "%.3f", matdensity_input); // set the buffer to current material Density
		}
	}
	else // input mode
	{
		// Text box to input Tension
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputMatDensity", matdensity_str, IM_ARRAYSIZE(matdensity_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			matdensity_input = static_cast<float>(atof(matdensity_str));
			// set the Density to input value
			material_density = matdensity_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			matdensity_input_mode = false;
		}
	}

	// Text for material Density
	ImGui::SameLine();
	ImGui::Text("material Density = %.3f", material_density);


	ImGui::Spacing();
	//_________________________________________________________________________________________

	// Apply button
	if (ImGui::Button("Apply"))
	{
		//___________________________________________________________________________________
		// Create a file to keep track of model data
		std::ofstream model_file;
		model_file.open("model_data.txt");

		// Print the model data
		model_file << grid_length << std::endl;  // Line length
		model_file << segment_length << std::endl;  // Number of nodes
		model_file << space_permittivity << std::endl;  // Line tension
		model_file << material_density << std::endl; // Material density

		// Close the file
		model_file.close();

		// Execute apply
		is_execute_apply = true;
		isWindowSizeChanging = true;
	}

	ImGui::SameLine();
	// Close button
	if (ImGui::Button("Close"))
	{
		is_show_window = false; // set the flag to close the window
	}



	ImGui::End();
}
