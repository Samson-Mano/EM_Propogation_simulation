#include "geom_store.h"

geom_store::geom_store()
{
	// Empty constructor
}

geom_store::~geom_store()
{
	// Empty Destructor
}

void geom_store::init(options_window* op_window, solver_window* sol_window, model_window* md_window, inlcondition_window* inl_window)
{
	// Initialize
	// Initialize the geometry parameters
	geom_param.init();

	is_geometry_set = false;
	// is_modal_analysis_complete = false;

	// Add the window pointers
	this->md_window = md_window;
	this->inl_window = inl_window;
	this->op_window = op_window;
	this->sol_window = sol_window;

	// Intialize the model grid
	create_geometry();
}

void geom_store::fini()
{
	// Deinitialize
	is_geometry_set = false;

}


void geom_store::update_WindowDimension(const int& window_width, const int& window_height)
{
	// Update the window dimension
	this->geom_param.window_width = window_width;
	this->geom_param.window_height = window_height;

	if (is_geometry_set == true)
	{
		// Update the model matrix
		update_model_matrix();
		// !! Zoom to fit operation during window resize is handled in mouse event class !!
	}
}


void geom_store::update_model_matrix()
{
	// Set the model matrix for the model shader
	// Find the scale of the model (with 0.9 being the maximum used)
	int max_dim = geom_param.window_width > geom_param.window_height ? geom_param.window_width : geom_param.window_height;

	double normalized_screen_width = 1.8f * (static_cast<double>(geom_param.window_width) / static_cast<double>(max_dim));
	double normalized_screen_height = 1.8f * (static_cast<double>(geom_param.window_height) / static_cast<double>(max_dim));


	geom_param.geom_scale = std::min(normalized_screen_width / geom_param.geom_bound.x,
		normalized_screen_height / geom_param.geom_bound.y);

	// Translation
	glm::vec3 geom_translation = glm::vec3(-1.0f * (geom_param.max_b.x + geom_param.min_b.x) * 0.5f * geom_param.geom_scale,
		-1.0f * (geom_param.max_b.y + geom_param.min_b.y) * 0.5f * geom_param.geom_scale,
		0.0f);

	glm::mat4 g_transl = glm::translate(glm::mat4(1.0f), geom_translation);

	geom_param.modelMatrix = g_transl * glm::scale(glm::mat4(1.0f), glm::vec3(static_cast<float>(geom_param.geom_scale)));

	// Update the model matrix
	boundary_nodes.update_geometry_matrices(true, false, false, false, false);
	boundary_lines.update_geometry_matrices(true, false, false, false, false);
	model_labels.update_opengl_uniforms(true, false, false, false, false);

	grid_nodes.update_geometry_matrices(true, false, false, false, false);
	grid_trimesh.update_geometry_matrices(true, false, false, false, false);
	charge_path.update_geometry_matrices(true, false, false, false, false);

	// Result
	node_vector.update_geometry_matrices(true, false, false, false, false);
	//model_inlcond.update_geometry_matrices(true, false, false, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(true, false, false, false, false);
	//modal_result_nodes.update_geometry_matrices(true, false, false, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(true, false, false, false, false);
	//pulse_result_nodes.update_geometry_matrices(true, false, false, false, false);
}

void geom_store::update_model_zoomfit()
{
	if (is_geometry_set == false)
		return;

	// Set the pan translation matrix
	geom_param.panTranslation = glm::mat4(1.0f);

	// Set the zoom scale
	geom_param.zoom_scale = 1.0f;

	// Update the zoom scale and pan translation
	boundary_nodes.update_geometry_matrices(false, true, true, false, false);
	boundary_lines.update_geometry_matrices(false, true, true, false, false);
	model_labels.update_opengl_uniforms(false, true, true, false, false);

	grid_nodes.update_geometry_matrices(false, true, true, false, false);
	grid_trimesh.update_geometry_matrices(false, true, true, false, false);
	charge_path.update_geometry_matrices(false, true, true, false, false);

	// Result
	node_vector.update_geometry_matrices(false, true, true, false, false);
	//model_inlcond.update_geometry_matrices(false, true, true, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(false, true, true, false, false);
	//modal_result_nodes.update_geometry_matrices(false, true, true, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(false, true, true, false, false);
	//pulse_result_nodes.update_geometry_matrices(false, true, true, false, false);
}

void geom_store::update_model_pan(glm::vec2& transl)
{
	if (is_geometry_set == false)
		return;

	// Pan the geometry
	geom_param.panTranslation = glm::mat4(1.0f);

	geom_param.panTranslation[0][3] = -1.0f * transl.x;
	geom_param.panTranslation[1][3] = transl.y;

	// Update the pan translation
	boundary_nodes.update_geometry_matrices(false, true, false, false, false);
	boundary_lines.update_geometry_matrices(false, true, false, false, false);
	model_labels.update_opengl_uniforms(false, true, false, false, false);

	grid_nodes.update_geometry_matrices(false, true, false, false, false);
	grid_trimesh.update_geometry_matrices(false, true, false, false, false);
	charge_path.update_geometry_matrices(false, true, false, false, false);

	// Result
	node_vector.update_geometry_matrices(false, true, false, false, false);
	//model_inlcond.update_geometry_matrices(false, true, false, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(false, true, false, false, false);
	//modal_result_nodes.update_geometry_matrices(false, true, false, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(false, true, false, false, false);
	//pulse_result_nodes.update_geometry_matrices(false, true, false, false, false);
}

void geom_store::update_model_zoom(double& z_scale)
{
	if (is_geometry_set == false)
		return;

	// Zoom the geometry
	geom_param.zoom_scale = z_scale;

	// Update the Zoom
	boundary_nodes.update_geometry_matrices(false, false, true, false, false);
	boundary_lines.update_geometry_matrices(false, false, true, false, false);
	model_labels.update_opengl_uniforms(false, false, true, false, false);

	grid_nodes.update_geometry_matrices(false, false, true, false, false);
	grid_trimesh.update_geometry_matrices(false, false, true, false, false);
	charge_path.update_geometry_matrices(false, false, true, false, false);

	// Result
	node_vector.update_geometry_matrices(false, false, true, false, false);
	//model_inlcond.update_geometry_matrices(false, false, true, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(false, false, true, false, false);
	//modal_result_nodes.update_geometry_matrices(false, false, true, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(false, false, true, false, false);
	//pulse_result_nodes.update_geometry_matrices(false, false, true, false, false);
}

void geom_store::update_model_transperency(bool is_transparent)
{
	if (is_geometry_set == false)
		return;

	if (is_transparent == true)
	{
		// Set the transparency value
		geom_param.geom_transparency = 0.2f;
	}
	else
	{
		// remove transparency
		geom_param.geom_transparency = 1.0f;
	}

	// Update the model transparency
	boundary_nodes.update_geometry_matrices(false, false, false, true, false);
	boundary_lines.update_geometry_matrices(false, false, false, true, false);
	model_labels.update_opengl_uniforms(false, false, false, true, false);

	grid_nodes.update_geometry_matrices(false, false, false, true, false);
	grid_trimesh.update_geometry_matrices(false, false, false, true, false);
	charge_path.update_geometry_matrices(false, false, false, true, false);
	
	// Result
	node_vector.update_geometry_matrices(false, false, false, true, false);
	//model_ptmass.update_geometry_matrices(false, false, false, true, false);
	//model_inlcond.update_geometry_matrices(false, false, false, true, false);

}


void geom_store::create_geometry()
{
	// Create the geometry based on the input
	this->gird_length = md_window->grid_length; // Grid length
	this->gird_spacing = md_window->segment_length; // Grid spacing
	this->space_permittivity = md_window->space_permittivity; // Space permittivity
	this->material_density = md_window->material_density;  // Material density

	// Reinitialize the model geometry
	is_geometry_set = false;

	// Initialize the model items
	// Boundary
	this->boundary_nodes.init(&geom_param);
	this->boundary_lines.init(&geom_param);
	this->model_labels.init(&geom_param);

	// Triangle mesh
	this->grid_trimesh.init(&geom_param);
	this->grid_nodes.init(&geom_param);
	this->charge_path.init(&geom_param);

	// Result initialization
	this->node_vector.init(&geom_param);

	// Initialize the boundary pts
	int node_id = 0;
	int line_id = 0;
	glm::vec2 node_pt = glm::vec2(0.0, 0.0);
	double percent_increase = 0.05;
	double pt = (gird_length + (gird_length * percent_increase)) * 0.5;

	// Add the origin pt and four boundary nodes
	// Orgin point
	this->boundary_nodes.add_node(node_id, node_pt);

	// Boundary pt 1
	node_id = 1;
	node_pt = glm::vec2(-pt, -pt);
	this->boundary_nodes.add_node(node_id, node_pt);

	// Boundary pt 2
	node_id = 2;
	node_pt = glm::vec2(-pt, pt);
	this->boundary_nodes.add_node(node_id, node_pt);

	// Boundary pt 3
	node_id = 3;
	node_pt = glm::vec2(pt, pt);
	this->boundary_nodes.add_node(node_id, node_pt);

	// Boundary pt 4
	node_id = 4;
	node_pt = glm::vec2(pt, -pt);
	this->boundary_nodes.add_node(node_id, node_pt);

	// Add the four boundary lines
	line_id = 0;
	this->boundary_lines.add_elementline(line_id, &boundary_nodes.nodeMap[1], &boundary_nodes.nodeMap[2]);

	line_id = 1;
	this->boundary_lines.add_elementline(line_id, &boundary_nodes.nodeMap[2], &boundary_nodes.nodeMap[3]);

	line_id = 2;
	this->boundary_lines.add_elementline(line_id, &boundary_nodes.nodeMap[3], &boundary_nodes.nodeMap[4]);

	line_id = 3;
	this->boundary_lines.add_elementline(line_id, &boundary_nodes.nodeMap[4], &boundary_nodes.nodeMap[1]);

	// Add the model general detail
	this->model_labels.clear_labels();

	std::stringstream ss;
	ss << std::fixed << std::setprecision(geom_param.length_precision) << gird_length;

	std::string temp_str = "Gird Length = " + ss.str();

	ss.str("");
	ss << std::fixed << std::setprecision(geom_param.length_precision) << gird_spacing;

	temp_str = temp_str + " (Gird spacing = " + ss.str() + ")";
	node_pt = glm::vec2(0.0, -pt);
	this->model_labels.add_text(temp_str, node_pt, glm::vec2(0), geom_param.geom_colors.node_color, 0, false, false);

	//_______________________________________________________________________________________________________________
	// Create the mesh
	// Calculate the number of rows and columns
	int numRows = static_cast<int>(gird_length / gird_spacing);
	int numCols = numRows; // Assuming a square grid

	// Calculate the origin (center) of the grid
	double originX = gird_length / 2.0f;
	double originY = gird_length / 2.0f;

	// Iterate over the rows and columns to create triangle nodes
	node_id = 0;
	for (int row = 0; row <= numRows; ++row)
	{
		for (int col = 0; col <= numCols; ++col)
		{
			// Calculate the coordinates of the nodes
			double left_x = (col * gird_spacing) - originX;
			double bottom_y = originY - (row * gird_spacing);

			node_pt = glm::vec2(left_x, bottom_y);
			// Create the node
			this->grid_nodes.add_node(node_id, node_pt);

			node_id++;
		}
	}

	// Iterate over the rows and columns to create triangles
	node_id = 0;
	int tri_id = 0;
	for (int row = 0; row <= numRows - 1; ++row)
	{
		for (int col = 0; col <= numCols - 1; ++col)
		{
			// Calculate the IDs of the four corner nodes of the square
			int bottomLeftNode = node_id;
			int bottomRightNode = node_id + 1;
			int topLeftNode = node_id + (numCols + 1);
			int topRightNode = (node_id + (numCols + 1)) + 1;

			// Create the lower triangle
			this->grid_trimesh.add_elementtriangle(tri_id,
				&grid_nodes.nodeMap[bottomLeftNode],
				&grid_nodes.nodeMap[bottomRightNode],
				&grid_nodes.nodeMap[topLeftNode]);
			tri_id++;

			// Create the upper triangle
			this->grid_trimesh.add_elementtriangle(tri_id,
				&grid_nodes.nodeMap[topRightNode],
				&grid_nodes.nodeMap[topLeftNode],
				&grid_nodes.nodeMap[bottomRightNode]);
			tri_id++;

			node_id++;
		}
		node_id++; // Skip the last node in each row
	}

	// Create a simple initial oscillation path
	std::ostringstream scaled_dxf_input; // Create an ostringstream object for scaled input

	scaled_dxf_input << 0 << ", " << 0.0 << ", " << 100.0 << "\n";
	scaled_dxf_input << 0 << ", " << 0.0 << ", " << -100.0 << "\n";


	std::string currve_pt;
	std::vector<std::string> curve_paths;
	std::string input_str = scaled_dxf_input.str();
	std::istringstream iss_temp(input_str);

	while (std::getline(iss_temp, currve_pt))
	{
		curve_paths.push_back(currve_pt);
	}

	inl_window->curve_imported = true;

	// Add the path
	this->charge_path.add_path(curve_paths, 0);


	// Geometry is loaded
	is_geometry_set = true;

	// Set the boundary of the geometry
	std::pair<glm::vec2, glm::vec2> result = findMinMaxXY(boundary_nodes.nodeMap);
	this->geom_param.min_b = result.first;
	this->geom_param.max_b = result.second;
	this->geom_param.geom_bound = geom_param.max_b - geom_param.min_b;

	// Set the center of the geometry
	this->geom_param.center = findGeometricCenter(boundary_nodes.nodeMap);

	// Set the geometry
	update_model_matrix();
	update_model_zoomfit();

	// Set the geometry buffers
	// Boundary
	this->boundary_nodes.set_buffer();
	this->boundary_lines.set_buffer();
	this->model_labels.set_buffer();

	// Triangle mesh
	this->grid_trimesh.set_buffer();
	this->grid_nodes.set_buffer();
	this->charge_path.set_buffer();
}


std::pair<glm::vec2, glm::vec2> geom_store::findMinMaxXY(const std::unordered_map<int, node_store>& model_nodes)
{
	// Initialize min and max values to first node in map
	glm::vec2 firstNode = model_nodes.begin()->second.node_pt;
	glm::vec2 minXY = glm::vec2(firstNode.x, firstNode.y);
	glm::vec2 maxXY = minXY;

	// Loop through all nodes in map and update min and max values
	for (auto it = model_nodes.begin(); it != model_nodes.end(); ++it)
	{
		const auto& node = it->second.node_pt;
		if (node.x < minXY.x)
		{
			minXY.x = node.x;
		}
		if (node.y < minXY.y)
		{
			minXY.y = node.y;
		}
		if (node.x > maxXY.x)
		{
			maxXY.x = node.x;
		}
		if (node.y > maxXY.y)
		{
			maxXY.y = node.y;
		}
	}

	// Return pair of min and max values
	return { minXY, maxXY };
}

glm::vec2 geom_store::findGeometricCenter(const std::unordered_map<int, node_store>& model_nodes)
{
	// Function returns the geometric center of the nodes
		// Initialize the sum with zero
	glm::vec2 sum(0);

	// Sum the points
	for (auto it = model_nodes.begin(); it != model_nodes.end(); ++it)
	{
		sum += it->second.node_pt;
	}
	return sum / static_cast<float>(model_nodes.size());
}


void geom_store::paint_geometry()
{
	if (is_geometry_set == false)
		return;

	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	// Paint the model
	paint_model();

	// Paint the postprocessing results
	paint_postprocess();

}



void geom_store::paint_model()
{
	//____________________________________________________________
	// Postprocessing is in progress

	if (sol_window->is_show_window == true && sol_window->show_undeformed_model == false)
	{
		return;
	}

	//____________________________________________________________

	// Paint the model
	// Boundary
	boundary_nodes.paint_model_nodes();
	boundary_lines.paint_elementlines();
	model_labels.paint_text();

	// Traingle mesh
	if (op_window->is_show_gridtris == true)
	{
		// Paint the triangle mesh
		if (op_window->is_show_gridtris_shrunk == false)
		{
			grid_trimesh.paint_elementtriangles();
		}
		else
		{
			// Paint the traingle mesh shrunk
			grid_trimesh.paint_elementtriangles_shrunk();
		}
	}

	if (op_window->is_show_gridboundary == true)
	{
		// Paint the triangle mesh boundary
		grid_trimesh.paint_elementtriangles_boundarylines();
	}

	if (op_window->is_show_gridnode == true)
	{
		// Paint the grid nodes
		grid_nodes.paint_model_nodes();

	}

	if (inl_window->execute_apply_path == true)
	{
		// Execute the application of new path
		inl_window->execute_apply_path = false;

		// Create new path
		this->charge_path.add_path(inl_window->curve_paths, inl_window->selected_curvepath_option);
	}

	if (md_window->is_show_window == true)
	{
		// Edit model window is open
		if (md_window->is_execute_apply == true)
		{
			// Apply model changes
			create_geometry();
			md_window->is_execute_apply = false;
		}
	}


	// Paint the charge path
	charge_path.paint_charge_path();

}


void geom_store::paint_postprocess()
{
	// Check closing sequence for Solver window
	if (sol_window->execute_close == true)
	{
		// Execute the close sequence
		if (is_analysis_complete == true)
		{
			// Analysis is complete (but clear the results anyway beacuse results will be loaded at open)
			sol_window->is_analysis_complete = false;

			// Solver analysis is complete
			update_model_transperency(false);
		}

		sol_window->execute_close = false;
	}

	// Check whether the solver window is open or not
	if (sol_window->is_show_window == false)
	{
		return;
	}

	// Paint the analysis result
	if (is_analysis_complete == true)
	{
		// Update the deflection scale
		geom_param.normalized_defl_scale = 1.0f;
		geom_param.defl_scale = sol_window->deformation_scale_max;

		charge_path.update_geometry_matrices(false, false, false, true, true);
		node_vector.update_geometry_matrices(false, false, false, true, true);

		// ______________________________________________________________________________________

		// Paint the charge path
		charge_path.paint_charge_oscillation(sol_window->time_step);

		// Paint the Electric field vector
		node_vector.paint_vectors(sol_window->time_step);
	}


	// Execute open for the solver
	if (sol_window->execute_open == true)
	{
		// Execute the open sequence
		if (inl_window->curve_imported == false)
		{
			// Exit the window (when modal analysis is not complete)
			sol_window->is_show_window = false;
		}
		else
		{

			// Modal analysis is complete (check whether frequency response analysis is complete or not)
			if (is_analysis_complete == true)
			{
				// Set the charge oscillation analysis result
				sol_window->is_analysis_complete = true;

				// Reset the buffers for charge analysis (charge path)
				// pulse_result_lineelements.set_buffer();
				//pulse_result_nodes.set_buffer();

				// Charge analysis is complete
				update_model_transperency(true);
			}

		}
		sol_window->execute_open = false;
	}

	// Execute the solve
	if (sol_window->execute_solve == true)
	{
		// Execute the Charge oscillation solve
		charge_oscillation_solver ch_solver;

		ch_solver.charge_oscillation_analysis_start(grid_nodes,
			grid_trimesh,
			charge_path,
			sol_window->charge_oscillation_freq,
			sol_window->total_simulation_time,
			sol_window->time_interval,
			node_vector,
			is_analysis_complete);


		//pulse_analysis_solver pulse_solver;
		//pulse_solver.pulse_analysis_start(model_nodes,
		//	model_lineelements,
		//	model_constarints,
		//	model_loads,
		//	model_ptmass,
		//	model_inlcond,
		//	mat_window->material_list,
		//	sol_modal_window->is_include_consistent_mass_matrix,
		//	md_solver,
		//	modal_results,
		//	sol_pulse_window->total_simulation_time,
		//	sol_pulse_window->time_interval,
		//	sol_pulse_window->damping_ratio,
		//	pulse_response_result,
		//	pulse_result_nodes,
		//	pulse_result_lineelements,
		//	is_pulse_analysis_complete);

		// Check whether the modal analysis is complete or not
		if (is_analysis_complete == true)
		{
			// Set the charge oscillation analysis result
			sol_window->is_analysis_complete = true;
			sol_window->time_interval_atrun = ch_solver.time_interval;
			sol_window->time_step_count = ch_solver.time_step_count;

			// Reset the buffers for pulse result nodes and lines
			charge_path.update_geometry_matrices(false, false, false, true, true);
			node_vector.update_geometry_matrices(false, false, false, true, true);

			charge_path.set_path_buffer();
			node_vector.set_buffer();
			//pulse_result_nodes.set_buffer();

			// Charge analysis is complete
			update_model_transperency(true);
		}
		sol_window->execute_solve = false;
	}


}