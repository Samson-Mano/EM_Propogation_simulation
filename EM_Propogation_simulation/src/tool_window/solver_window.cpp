#include "solver_window.h"

solver_window::solver_window()
{
	// Empty constructor
}

solver_window::~solver_window()
{
	// Empty destructor
}


void solver_window::init()
{
    // Initialize the options
    is_show_window = false;
}

void solver_window::render_window()
{
    if (is_show_window == false)
        return;

    ImGui::Begin("Solver window");

	//_________________________________________________________________________________________
	// Add a Modal Analysis button
	if (ImGui::Button("Run Analysis"))
	{
		execute_solve = true;
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// Show Modal analysis results
//________________________________________________________________________________________

	// Oscillation frequency
	// Text for Oscillation frequenyc
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool oscillationfreq_input_mode = false;
	static char oscillationfreq_str[16] = ""; // buffer to store input Oscillation frequency string
	static float oscillationfreq_input = static_cast<float>(charge_oscillation_freq); // buffer to store input Oscillation frequency

	// Button to switch to input mode
	if (!oscillationfreq_input_mode)
	{
		if (ImGui::Button("Oscillation Frequency"))
		{
			oscillationfreq_input_mode = true;
			snprintf(oscillationfreq_str, 16, "%.3f", oscillationfreq_input); // set the buffer to current Oscillation frequency
		}
	}
	else // input mode
	{
		// Text box to input Oscillation Frequency
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputOscillationFreq", oscillationfreq_str, IM_ARRAYSIZE(oscillationfreq_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			oscillationfreq_input = static_cast<float>(atof(oscillationfreq_str));
			// set the Oscillation Frequency to input value
			charge_oscillation_freq = oscillationfreq_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			oscillationfreq_input_mode = false;
		}
	}

	//___________________________________________________________________________________________________________________________

	// Text for Gird length
	ImGui::SameLine();
	ImGui::Text("Oscillation frequency = %.3f", charge_oscillation_freq);

	// Inputs
	// Text for total simulation time
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool totaltime_input_mode = false;
	static char totaltime_str[16] = ""; // buffer to store input load string
	static float totaltime_input = static_cast<float>(total_simulation_time); // buffer to store input load End Time

	// Button to switch to input mode
	if (!totaltime_input_mode)
	{
		if (ImGui::Button("Total simulation time"))
		{
			totaltime_input_mode = true;
			snprintf(totaltime_str, 16, "%.3f", totaltime_input); // set the buffer to current load End Time
		}
	}
	else // input mode
	{
		// Text box to input load start time
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputTotalTime", totaltime_str, IM_ARRAYSIZE(totaltime_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			totaltime_input = static_cast<float>(atof(totaltime_str));
			// set the load start time to input value
			total_simulation_time = totaltime_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			totaltime_input_mode = false;
		}
	}

	// Text for Total simulation time
	ImGui::SameLine();
	ImGui::Text("Total simulation time = %.3f", total_simulation_time);

	// Text for time interval
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool timeinterval_input_mode = false;
	static char timeinterval_str[16] = ""; // buffer to store input load string
	static float timeinterval_input = static_cast<float>(time_interval); // buffer to store input load End Time

	// Button to switch to input mode
	if (!timeinterval_input_mode)
	{
		if (ImGui::Button("Time Interval"))
		{
			timeinterval_input_mode = true;
			snprintf(timeinterval_str, 16, "%.3f", timeinterval_input); // set the buffer to current load End Time
		}
	}
	else // input mode
	{
		// Text box to input load start time
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputTimeInterval", timeinterval_str, IM_ARRAYSIZE(timeinterval_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			timeinterval_input = static_cast<float>(atof(timeinterval_str));
			// set the load start time to input value
			time_interval = timeinterval_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			timeinterval_input_mode = false;
		}
	}


	// Text for Time Interval
	ImGui::SameLine();
	ImGui::Text("Time Interval = %.3f", time_interval);
	//_________________________________________________________________________________________


	// Add check boxes to show the Deformed model, contour plot and vector plot
	ImGui::Checkbox("Show Model", &show_undeformed_model);
	ImGui::Checkbox("Show Contour plot", &show_contour_plot);
	ImGui::Checkbox("Show Vector plot", &show_vector_plot);
	ImGui::Spacing();
	//_________________________________________________________________________________________

	// Input box to give input via text
	static bool defscale_input_mode = false;
	static char defscale_str[16] = ""; // buffer to store input deformation scale string
	static double defscale_input = 0; // buffer to store input deformation scale value

	// Button to switch to input mode
	if (!defscale_input_mode)
	{
		if (ImGui::Button("Deformation Scale"))
		{
			defscale_input_mode = true;
			snprintf(defscale_str, 16, "%.1f", deformation_scale_max); // set the buffer to current deformation scale value
		}
	}
	else // input mode
	{
		// Text box to input value
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##Deformation Scale", defscale_str, IM_ARRAYSIZE(defscale_str), ImGuiInputTextFlags_CharsDecimal))
		{
			// convert the input string to int
			defscale_input = atoi(defscale_str);
			// set the load value to input value
			deformation_scale_max = defscale_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			defscale_input_mode = false;
		}
	}

	// Text for load value
	ImGui::SameLine();
	ImGui::Text(" %.1f", deformation_scale_max);

	// Slider for Deflection scale
	float deformation_scale_flt = static_cast<float>(deformation_scale_max);

	ImGui::Text("Deformation Scale");
	ImGui::SameLine();
	ImGui::SliderFloat(".", &deformation_scale_flt, 0.0f, 100.0f, "%.1f");
	deformation_scale_max = deformation_scale_flt;

	////Set the deformation scale
	//normailzed_defomation_scale = 1.0f;
	//deformation_scale = deformation_scale_max;

	ImGui::Spacing();

	//_________________________________________________________________________________________

	if (ImGui::CollapsingHeader("Animate"))
	{
		// Animate the solution
		// Start a horizontal layout
		ImGui::BeginGroup();

		// Play button active

		if (animate_pause == true)
		{
			// Add the Play button
			if (ImGui::Button("Play"))
			{
				// Handle Play button click
				animate_play = !animate_play;
				animate_pause = false;
			}
		}

		if (animate_play == true)
		{
			// Add the Pause button
			if (ImGui::Button("Pause"))
			{
				// Handle Pause button click
				animate_pause = !animate_pause;
				animate_play = false;
			}
		}

		// Add some spacing between buttons
		ImGui::SameLine();

		// Add the Stop button
		if (ImGui::Button("Stop"))
		{
			// Handle Stop button click
			animate_play = false;
			animate_pause = true;
		}

		// Animation speed control
		// Input box to give input via text
		static bool animation_speed_input_mode = false;
		static char animation_speed_str[16] = ""; // buffer to store input deformation scale string
		static float animation_speed_input = 0; // buffer to store input deformation scale value

		// Button to switch to input mode
		if (!animation_speed_input_mode)
		{
			if (ImGui::Button("Animation Speed"))
			{
				animation_speed_input_mode = true;
				snprintf(animation_speed_str, 16, "%.3f", animation_speed); // set the buffer to current deformation scale value
			}
		}
		else // input mode
		{
			// Text box to input value
			ImGui::SetNextItemWidth(60.0f);
			if (ImGui::InputText("##Animation Speed", animation_speed_str, IM_ARRAYSIZE(animation_speed_str), ImGuiInputTextFlags_CharsDecimal))
			{
				// convert the input string to int
				animation_speed_input = static_cast<float>(atof(animation_speed_str));
				// set the load value to input value
				animation_speed = animation_speed_input;
			}

			// Button to switch back to slider mode
			ImGui::SameLine();
			if (ImGui::Button("OK"))
			{
				animation_speed_input_mode = false;
			}
		}

		// Text for Animation speed value
		ImGui::SameLine();
		ImGui::Text(" %.3f", animation_speed);

		// Display the time step and time value
		ImGui::Text("Time = %.3f secs",
			time_interval_atrun * time_step);

		//ImGui::Text("Time value = %.3f secs",
		//	stopwatch.current_elapsed());

		// Display the frame rate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		// End the horizontal layout
		ImGui::EndGroup();
	}

	ImGui::Spacing();
	ImGui::Spacing();
	//_________________________________________________________________________________________

	// Add a "Close" button (Close the solver window)
	if (ImGui::Button("Close"))
	{
		execute_close = true;
		is_show_window = false;
	}


	ImGui::End();

	//______________________________________________ END of Imgui interface ___________________

	// Cycle through the pulse response time step
	if (is_analysis_complete == true)
	{

		if (animate_play == true)
		{
			// Stop watch
			if ((stopwatch.current_elapsed() * animation_speed) > time_interval_atrun)
			{
				stopwatch.reset_time(); // reset the time
				time_step++; // increment the time step

				// Adjust the time step such that it didnot exceed the time_step_total
				if (time_step >= time_step_count)
				{
					time_step = 0;
				}
			}
		}
		else if (animate_pause == true)
		{
			// Pause the animation
		}
		else
		{
			// Stop the animation (show the end of animation)
			time_step = time_step_count - 1;
		}
	}


}
