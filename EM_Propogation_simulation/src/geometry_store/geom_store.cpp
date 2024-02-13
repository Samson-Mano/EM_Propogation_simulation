#include "geom_store.h"

geom_store::geom_store()
{
	// Empty constructor
}

geom_store::~geom_store()
{
	// Empty Destructor
}

void geom_store::init(options_window* op_window, solver_window* sol_window,inlcondition_window* inl_window, std::ifstream& grid_file)
{
	// Initialize
	// Initialize the geometry parameters
	geom_param.init();

	is_geometry_set = false;
	// is_modal_analysis_complete = false;

	// Add the window pointers
	this->inl_window = inl_window;
	this->op_window = op_window;
	this->sol_window = sol_window;

	// Intialize the model grid
	create_geometry(grid_file);
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
	grid_vector_nodes.update_geometry_matrices(true, false, false, false, false);
	grid_lines.update_geometry_matrices(true, false, false, false, false);
	grid_trimesh.update_geometry_matrices(true, false, false, false, false);
	charge_path.update_geometry_matrices(true, false, false, false, false);

	// Result
	node_vector.update_geometry_matrices(true, false, false, false, false);
	node_contour.update_geometry_matrices(true, false, false, false, false);

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
	grid_vector_nodes.update_geometry_matrices(false, true, true, false, false);
	grid_lines.update_geometry_matrices(false, true, true, false, false);
	grid_trimesh.update_geometry_matrices(false, true, true, false, false);
	charge_path.update_geometry_matrices(false, true, true, false, false);

	// Result
	node_vector.update_geometry_matrices(false, true, true, false, false);
	node_contour.update_geometry_matrices(false, true, true, false, false);

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
	grid_vector_nodes.update_geometry_matrices(false, true, false, false, false);
	grid_lines.update_geometry_matrices(false, true, false, false, false);
	grid_trimesh.update_geometry_matrices(false, true, false, false, false);
	charge_path.update_geometry_matrices(false, true, false, false, false);

	// Result
	node_vector.update_geometry_matrices(false, true, false, false, false);
	node_contour.update_geometry_matrices(false, true, false, false, false);

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
	grid_vector_nodes.update_geometry_matrices(false, false, true, false, false);
	grid_lines.update_geometry_matrices(false, false, true, false, false);
	grid_trimesh.update_geometry_matrices(false, false, true, false, false);
	charge_path.update_geometry_matrices(false, false, true, false, false);

	// Result
	node_vector.update_geometry_matrices(false, false, true, false, false);
	node_contour.update_geometry_matrices(false, false, true, false, false);

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
	grid_vector_nodes.update_geometry_matrices(false, false, false, true, false);
	grid_lines.update_geometry_matrices(false, false, false, true, false);
	grid_trimesh.update_geometry_matrices(false, false, false, true, false);
	charge_path.update_geometry_matrices(false, false, false, true, false);

	// Result
	node_vector.update_geometry_matrices(false, false, false, true, false);
	node_contour.update_geometry_matrices(false, false, false, true, false);

}


void geom_store::create_geometry(std::ifstream& grid_file)
{
	// Reinitialize the model geometry
	is_geometry_set = false;

	// Initialize the model items
	// Boundary
	this->boundary_nodes.init(&geom_param);
	this->boundary_lines.init(&geom_param);
	this->model_labels.init(&geom_param);

	// Triangle mesh
	this->grid_trimesh.init(&geom_param);
	this->grid_lines.init(&geom_param);
	this->grid_nodes.init(&geom_param);
	this->grid_vector_nodes.init(&geom_param);
	this->charge_path.init(&geom_param);

	// Result initialization
	this->node_vector.init(&geom_param);
	this->node_contour.init(&geom_param);


	// Initialize the boundary pts
	int node_id = 0;
	int line_id = 0;
	glm::vec2 node_pt = glm::vec2(0.0, 0.0);
	double percent_increase = 0.05;
	double pt = (grid_size + (grid_size * percent_increase)) * 0.5;

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
	ss << std::fixed << std::setprecision(geom_param.length_precision) << grid_size;

	std::string temp_str = "Gird Size = " + ss.str() + "x " + ss.str();

	node_pt = glm::vec2(0.0, -pt);
	this->model_labels.add_text(temp_str, node_pt, glm::vec2(0), geom_param.geom_colors.node_color, 0, false, false);

	//_______________________________________________________________________________________________________________
	// Create the mesh

	std::cout << "Grid creation started" << std::endl;

	// Read the Raw Data
	// Read the entire file into a string
	std::string file_contents((std::istreambuf_iterator<char>(grid_file)),
		std::istreambuf_iterator<char>());

	// Split the string into lines
	std::istringstream iss(file_contents);
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(iss, line))
	{
		lines.push_back(line);
	}

	// Create stopwatch
	Stopwatch_events stopwatch;
	stopwatch.start();
	std::stringstream stopwatch_elapsed_str;
	stopwatch_elapsed_str << std::fixed << std::setprecision(6);

	stopwatch_elapsed_str << stopwatch.elapsed();
	std::cout << "Lines loaded at " << stopwatch_elapsed_str.str() << " secs" << std::endl;

	//Node Point list
	std::vector<glm::vec2> node_pts_list;

	int j = 0;
	for (int x_i = 0; x_i <= grid_size; x_i += 50)
	{
		for (int y_i = 0; y_i <= grid_size; y_i += 50)
		{
			double x = -1000 + x_i;
			double y = -1000 + y_i;

			glm::vec2 node_pt = glm::vec2(x, y);
			this->grid_vector_nodes.add_node(j, node_pt);
			j++;
		}
	}

	stopwatch_elapsed_str.str("");
	stopwatch_elapsed_str << stopwatch.elapsed();
	std::cout << "Grid nodes created at " << stopwatch_elapsed_str.str() << " secs" << std::endl;

	// Process the lines
	j = 0;
	while (j < lines.size())
	{
		std::string line = lines[j];
		std::string type = line.substr(0, 4);  // Extract the first 4 characters of the line

		// Split the line into comma-separated fields
		std::istringstream iss(line);
		std::string field;
		std::vector<std::string> fields;
		while (std::getline(iss, field, ','))
		{
			fields.push_back(field);
		}

		if (type == "node")
		{
			// Read the nodes
			int node_id = std::stoi(fields[1]); // node ID
			double x = std::stod(fields[2]); // Node coordinate x
			double y = std::stod(fields[3]); // Node coordinate y

			// Add to node Map
			glm::vec2 node_pt = glm::vec2(x, y);
			this->grid_nodes.add_node(node_id, node_pt);

			//Add to the node list
			node_pts_list.push_back(node_pt);

		}
		else if (type == "line")
		{
			int line_id = std::stoi(fields[1]); // line ID
			int start_node_id = std::stoi(fields[2]); // line id start node
			int end_node_id = std::stoi(fields[3]); // line id end node

			// Add to line Map (Note that Nodes needed to be added before the start of line addition !!!!)
			this->grid_lines.add_elementline(line_id, &grid_nodes.nodeMap[start_node_id], &grid_nodes.nodeMap[end_node_id]);
		}
		else if (type == "tria")
		{
			int tri_id = std::stoi(fields[1]); // triangle ID
			int nd1_id = std::stoi(fields[2]); // triangle node 1
			int nd2_id = std::stoi(fields[3]); // triangle node 2
			int nd3_id = std::stoi(fields[4]); // triangle node 3

			// Add to constraint map
			this->grid_trimesh.add_elementtriangle(tri_id, 
				&grid_nodes.nodeMap[nd1_id], 
				&grid_nodes.nodeMap[nd2_id],
				&grid_nodes.nodeMap[nd3_id]);
		}
		
		// Iterate line
		j++;
	}

	stopwatch_elapsed_str.str("");
	stopwatch_elapsed_str << stopwatch.elapsed();
	std::cout << "Grid mesh created at " << stopwatch_elapsed_str.str() << " secs" << std::endl;

	// Create a simple initial oscillation path
	// Add the path
	this->charge_path.add_path(inl_window->curve_paths, inl_window->selected_model_option, inl_window->path_type);


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
	this->grid_lines.set_buffer();
	this->grid_nodes.set_buffer();
	this->grid_vector_nodes.set_buffer();
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
		grid_trimesh.paint_elementtriangles();
	}

	if (op_window->is_show_gridboundary == true)
	{
		// Paint the triangle mesh boundary
		grid_lines.paint_elementlines();
	}

	if (op_window->is_show_gridnode == true)
	{
		// Paint the grid nodes
		grid_vector_nodes.paint_model_nodes();


	}

	if (inl_window->execute_apply_path == true)
	{
		// Execute the application of new path
		inl_window->execute_apply_path = false;

		// Create new path
		this->charge_path.add_path(inl_window->curve_paths, inl_window->selected_model_option, inl_window->path_type);
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
		node_contour.update_geometry_matrices(false, false, false, true, true);

		// ______________________________________________________________________________________
		// Paint the charge path
		charge_path.paint_charge_oscillation(sol_window->time_step);

		// Paint the Electric field vector
		if (sol_window->show_vector_plot == true)
		{
			node_vector.paint_vectors(sol_window->time_step);
		}

		// Paint the Electric potential
		if (sol_window->show_contour_plot == true)
		{
			node_contour.paint_tricontour(sol_window->time_step);
		}
	}


	// Execute open for the solver
	if (sol_window->execute_open == true)
	{
		// Execute the open sequence
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
		sol_window->execute_open = false;
	}

	// Execute the solve
	if (sol_window->execute_solve == true)
	{
		// Execute the Charge oscillation solve
		charge_oscillation_solver ch_solver;

		ch_solver.charge_oscillation_analysis_start(grid_vector_nodes,
			grid_nodes,
			grid_trimesh,
			charge_path,
			sol_window->charge_oscillation_freq,
			sol_window->total_simulation_time,
			sol_window->time_interval,
			node_vector,
			node_contour,
			is_analysis_complete);

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
			node_contour.update_geometry_matrices(false, false, false, true, true);

			charge_path.set_path_buffer();
			node_vector.set_buffer();
			node_contour.set_buffer();

			// Charge analysis is complete
			update_model_transperency(true);
		}
		sol_window->execute_solve = false;
	}


}