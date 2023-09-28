#include "geom_store.h"

geom_store::geom_store()
{
	// Empty constructor
}

geom_store::~geom_store()
{
	// Empty Destructor
}

void geom_store::init(options_window* op_window,solver_window* sol_window, model_window* md_window, inlcondition_window* inl_window)
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
	//model_loads.update_geometry_matrices(true, false, false, false, false);
	//model_ptmass.update_geometry_matrices(true, false, false, false, false);
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
	//model_loads.update_geometry_matrices(false, true, true, false, false);
	//model_ptmass.update_geometry_matrices(false, true, true, false, false);
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
	//model_loads.update_geometry_matrices(false, true, false, false, false);
	//model_ptmass.update_geometry_matrices(false, true, false, false, false);
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
	//model_loads.update_geometry_matrices(false, false, true, false, false);
	//model_ptmass.update_geometry_matrices(false, false, true, false, false);
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
	//model_loads.update_geometry_matrices(false, false, false, true, false);
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
	this->boundary_nodes.init(&geom_param);
	this->boundary_lines.init(&geom_param);
	this->model_labels.init(&geom_param);

	// Initialize the boundary pts
	int node_id = 0;
	int line_id = 0;
	glm::vec2 node_pt = glm::vec2(0.0, 0.0);
	double percent_increase = 0.1;
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
	this->boundary_nodes.set_buffer();
	this->boundary_lines.set_buffer();
	this->model_labels.set_buffer();

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
	boundary_nodes.paint_model_nodes();
	boundary_lines.paint_elementlines();
	model_labels.paint_text();

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
}