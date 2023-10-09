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

}

void inlcondition_window::render_window()
{
	if (is_show_window == false)
		return;

	ImGui::Begin("Intial condition");

	// Import the pulse curve
	if (ImGui::Button("Import curve dxf file"))
	{
		// Execute curve import only if the curve is imported properly
		// Import the oscillation curve points
		import_dxfdata_geometry();
	}



	ImGui::Spacing();
	ImGui::Spacing();

	// Options for the path (closed curve or open curve)
	if (ImGui::RadioButton("Closed curve", selected_curvepath_option == 0))
	{
		selected_curvepath_option = 0;
	}
	// ImGui::SameLine();
	if (ImGui::RadioButton("Open curve", selected_curvepath_option == 1))
	{
		selected_curvepath_option = 1;
	}

	// Display the selected option
	ImGui::Text("Selected Option: %s", (selected_curvepath_option == 0) ? "Closed curve" : "Open curve");

	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool scale_input_mode = false;
	static char path_scale_str[8] = ""; // buffer to store input path scale string
	static int path_scale_input = path_scale_factor; // buffer to store input path scale value

	// Button to switch to input mode
	if (!scale_input_mode)
	{
		if (ImGui::Button("Input Path Scale Factor"))
		{
			scale_input_mode = true;
			snprintf(path_scale_str, 8, "%i", path_scale_input); // set the buffer to path scale factor
		}
	}
	else // input mode
	{
		// Text box to input angle value
		ImGui::SetNextItemWidth(60.0f);
		if (ImGui::InputText("##InputPathScale", path_scale_str, IM_ARRAYSIZE(path_scale_str), ImGuiInputTextFlags_CharsDecimal)) // ImGuiInputTextFlags_CharsDecimal
		{
			// convert the input string to float
			path_scale_input = static_cast<int>(atoi(path_scale_str));
			// limit the value to 0 - 1000 range
			path_scale_input = fmaxf(0.0f, fminf(path_scale_input, 1000));
			// set the angle to input value
			path_scale_factor = path_scale_input;
		}

		// Button to switch back to slider mode
		ImGui::SameLine();
		if (ImGui::Button("OK"))
		{
			scale_input_mode = false;
		}
	}

	// Slider for angle
	path_scale_input = path_scale_factor;

	ImGui::Text("Path Scale Factor");
	ImGui::SameLine();
	ImGui::SliderInt("Scale Factor", &path_scale_input, 0, 1000, "%i");

	path_scale_factor = path_scale_input;
	
	//_________________________________________________________________________________________
	
	// Oscillation frequency
	// Text for Oscillation frequenyc
	//_________________________________________________________________________________________
	// Input box to give input via text
	static bool oscillationfreq_input_mode = false;
	static char oscillationfreq_str[16] = ""; // buffer to store input Oscillation frequency string
	static float oscillationfreq_input = static_cast<float>(oscillation_freq); // buffer to store input Oscillation frequency

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
			oscillation_freq = oscillationfreq_input;
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
	ImGui::Text("Oscillation frequency = %.3f", oscillation_freq);

	ImGui::Spacing();
	ImGui::Spacing();


	// Execute the import path
	if (ImGui::Button("Apply path"))
	{
		// Execute curve import only if the curve is imported properly
		if (curve_imported == true)
		{
			execute_apply_path = true;
		}
	}

	ImGui::SameLine();

	// Add a "Close" button (Close the initial condition window)
	if (ImGui::Button("Close"))
	{
		// curve_imported = false;
		is_show_window = false;
	}


	ImGui::End();

}


std::string inlcondition_window::ShowOpenFileDialog_dxf()
{
	OPENFILENAMEW ofn;                         // Structure to store the file dialog options (wide-character version)
	wchar_t fileName[MAX_PATH];                // Buffer to store the selected file path (wide-character version)

	// Initialize the OPENFILENAMEW structure
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"AutoCAD R12 LT2 (*.dxf)\0*.dxf\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Show the file dialog
	if (GetOpenFileNameW(&ofn))             // Note the 'W' suffix for wide-character version of the function
	{
		// Convert the wide-character string to narrow-character string (UTF-8)
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, nullptr, 0, nullptr, nullptr);
		std::string fileName(bufferSize, '\0');
		WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, &fileName[0], bufferSize, nullptr, nullptr);
		return fileName;
	}

	return "";  // Return an empty string if the file dialog was cancelled or an error occurred
}



void inlcondition_window::import_dxfdata_geometry()
{
	std::string file_path = ShowOpenFileDialog_dxf();
	if (file_path == "")
	{
		// Exit if no valid file path is selected
		return;
	}


	std::cout << "Selected File: " << file_path << std::endl;


	// Open the input file
	std::ifstream input_file(file_path, std::ifstream::in);

	// reads R12 LT AutoCAD Dxf file
	// Read the varai2D
	// Read the entire file into a string
	std::string file_contents((std::istreambuf_iterator<char>(input_file)),
		std::istreambuf_iterator<char>());

	// Split the string into lines
	std::istringstream iss(file_contents);
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(iss, line))
	{
		lines.push_back(line);
	}

	// Data type to store the Lines and Points
	struct dxf_points
	{
		int pt_id = 0;
		double pt_x = 0.0;
		double pt_y = 0.0;

		static int is_exist(std::vector<dxf_points>& all_pts, dxf_points& this_pt)
		{
			for (auto& pt : all_pts)
			{
				if (std::abs(pt.pt_x - this_pt.pt_x) < 1e-6 &&
					std::abs(pt.pt_y - this_pt.pt_y) < 1e-6)
				{
					return pt.pt_id;
				}
			}
			return -1;
		}
	};

	struct dxf_lines
	{
		int startpt_id = 0;
		int endpt_id = 0;

		static bool is_exist(std::vector<dxf_lines>& all_lines, dxf_lines& this_ln)
		{
			for (auto& ln : all_lines)
			{
				if (ln.startpt_id == this_ln.startpt_id && ln.endpt_id == this_ln.endpt_id)
				{
					return true;
				}

				if (ln.startpt_id == this_ln.endpt_id && ln.endpt_id == this_ln.startpt_id)
				{
					return true;
				}
			}
			return false;
		}
	};

	std::vector<dxf_points> all_dxf_points;
	std::vector<dxf_lines> all_dxf_lines;

	int j = 0;

	// Process the lines
	while (j < lines.size())
	{
		// Read the LINE Data
		if (lines[j] == "LINE")
		{
			dxf_points ln_start_pt; // line start pt
			dxf_points ln_end_pt; // line end pt

			j = j + 5; // Move to the next five line
			while (lines[j] != "  0")
			{
				if (lines[j] == " 10")
				{
					// Process the x-coordinate (start point)
					ln_start_pt.pt_x = std::stod(lines[j + 1]);
				}
				else if (lines[j] == " 20")
				{
					// Process the y-coordinate (start point)
					ln_start_pt.pt_y = std::stod(lines[j + 1]);
				}
				else if (lines[j] == " 11")
				{
					// Process the x-coordinate (end point)
					ln_end_pt.pt_x = std::stod(lines[j + 1]);
				}
				else if (lines[j] == " 21")
				{
					// Process the y-coordinate (end point)
					ln_end_pt.pt_y = std::stod(lines[j + 1]);
				}

				j++; // Move to the next two line
			}

			// Add to point list
			// Start Point
			int start_ptid = dxf_points::is_exist(all_dxf_points, ln_start_pt);
			if (start_ptid != -1)
			{
				// Point already exists
				// No need to add

			}
			else
			{
				// Point does not exists (Add start point)
				start_ptid = static_cast<unsigned int>(all_dxf_points.size());
				ln_start_pt.pt_id = start_ptid;
				all_dxf_points.push_back(ln_start_pt);
			}

			// End Point
			int end_ptid = dxf_points::is_exist(all_dxf_points, ln_end_pt);
			if (end_ptid != -1)
			{
				// Point already exists
				// No need to add

			}
			else
			{
				// Point does not exists (Add End point)
				end_ptid = static_cast<unsigned int>(all_dxf_points.size());
				ln_end_pt.pt_id = end_ptid;
				all_dxf_points.push_back(ln_end_pt);
			}

			// Add line
			if (start_ptid != end_ptid)
			{
				dxf_lines line_segment;
				line_segment.startpt_id = start_ptid;
				line_segment.endpt_id = end_ptid;

				// Add line to the list
				if (dxf_lines::is_exist(all_dxf_lines, line_segment) == false)
				{
					all_dxf_lines.push_back(line_segment);
				}
			}
		}

		// Read the POLYLINE Data
		if (lines[j] == "POLYLINE")
		{
			std::vector<dxf_points> polyline_points; // polyline point
			dxf_points pln_pt;
			bool first_pt = true;

			j++; // Move to the next line
			while (lines[j] != "SEQEND")
			{
				if (lines[j] == " 10")
				{
					// Process the polyline x-coordinate
					pln_pt.pt_x = std::stod(lines[j + 1]);

				}
				else if (lines[j] == " 20")
				{
					// Process the polyline y-coordinate
					pln_pt.pt_y = std::stod(lines[j + 1]);

					if (first_pt == false)
					{
						// Add to polyline points list
						if (dxf_points::is_exist(polyline_points, pln_pt) == -1)
						{
							// No points exists so add to the list
							polyline_points.push_back(pln_pt);
						}
					}
					else
					{
						first_pt = false;
					}
				}

				j++; // Move to the next line
			}

			// Iterate through the polyline points and add to the list
			for (int i = 0; i < (static_cast<int>(polyline_points.size()) - 1); i++)
			{
				// Get i th point and i+1 th point

				dxf_points ln_start_pt = polyline_points[i];
				dxf_points ln_end_pt = polyline_points[i + 1];

				// Add to point list
				// Start Point
				int start_ptid = dxf_points::is_exist(all_dxf_points, ln_start_pt);
				if (start_ptid != -1)
				{
					// Point already exists
					// No need to add

				}
				else
				{
					// Point does not exists (Add start point)
					start_ptid = static_cast<unsigned int>(all_dxf_points.size());
					ln_start_pt.pt_id = start_ptid;
					all_dxf_points.push_back(ln_start_pt);
				}

				// End Point
				int end_ptid = dxf_points::is_exist(all_dxf_points, ln_end_pt);
				if (end_ptid != -1)
				{
					// Point already exists
					// No need to add

				}
				else
				{
					// Point does not exists (Add End point)
					end_ptid = static_cast<unsigned int>(all_dxf_points.size());
					ln_end_pt.pt_id = end_ptid;
					all_dxf_points.push_back(ln_end_pt);
				}

				// Add line
				if (start_ptid != end_ptid)
				{
					dxf_lines line_segment;
					line_segment.startpt_id = start_ptid;
					line_segment.endpt_id = end_ptid;

					// Add line to the list
					if (dxf_lines::is_exist(all_dxf_lines, line_segment) == false)
					{
						all_dxf_lines.push_back(line_segment);
					}
				}
			}
		}
		// iterate line
		j++;
	}

	// Print all the points to ostring stream
	std::ostringstream dxf_input; // Create an ostringstream object

	// variables for the midpoint and scaling factor
	double sum_x = 0.0;
	double sum_y = 0.0;
	double min_x = all_dxf_points[0].pt_x;
	double min_y = all_dxf_points[0].pt_y;
	double max_x = all_dxf_points[0].pt_x;
	double max_y = all_dxf_points[0].pt_y;


	dxf_input << std::fixed << std::setprecision(9); // Set precision to 6 decimal places

	for (const auto& pt : all_dxf_points)
	{
		dxf_input << "node, " << pt.pt_id << ", " << pt.pt_x << ", " << pt.pt_y << "\n";

		// Sum of points
		sum_x += pt.pt_x;
		sum_y += pt.pt_y;
		// minimum of points
		min_x = std::min(min_x, pt.pt_x);
		min_y = std::min(min_y, pt.pt_y);
		// maximum of points
		max_x = std::max(max_x, pt.pt_x);
		max_y = std::max(max_y, pt.pt_y);
	}

	// Mid point and scale factor
	double mid_x = (min_x + max_x) / 2.0;
	double mid_y = (min_y + max_y) / 2.0;
	double scale_factor = path_scale_factor / std::max(max_x - min_x, max_y - min_y);


	// Save the values
	if (all_dxf_points.size() > 0)
	{
		// Clear the previous path
		this->curve_paths.clear();

		std::ostringstream scaled_dxf_input; // Create an ostringstream object for scaled input

		scaled_dxf_input << std::fixed << std::setprecision(6); // Set precision to 6 decimal places

		for (auto& pt : all_dxf_points)
		{
			// Scaled x & y
			double scaled_x = (pt.pt_x - mid_x) * scale_factor;
			double scaled_y = (pt.pt_y - mid_y) * scale_factor;

			scaled_dxf_input << pt.pt_id << ", " << scaled_x << ", " << scaled_y << "\n";
		}

		std::string line;
		std::string input_str = scaled_dxf_input.str();
		std::istringstream iss_temp(input_str);

		while (std::getline(iss_temp, line))
		{
			this->curve_paths.push_back(line);
		}


		// Curve imported 
		curve_imported = true;
	}





	int line_id = 0;
	for (auto& ln : all_dxf_lines)
	{
		dxf_input << "line, " << line_id << "," << ln.startpt_id << ", " << ln.endpt_id << ", " << "0" << "\n";
		line_id++;
	}

	// std::string dxf_inputStr = dxf_input.str(); // Get the string from the ostringstream
	//  std::cout << dxf_inputStr; // Print on console to check

	input_file.close();
}