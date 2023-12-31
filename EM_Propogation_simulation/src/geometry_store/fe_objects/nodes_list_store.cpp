#include "nodes_list_store.h"

nodes_list_store::nodes_list_store()
{
	// Empty constructor
}

nodes_list_store::~nodes_list_store()
{
	// Empty destructor
}

void nodes_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Set the geometry parameters for the points and labels (and clear the labels)
	node_points.init(geom_param_ptr);

	// Clear the nodes
	node_count = 0;
	nodeMap.clear();
}

void nodes_list_store::add_node(int& node_id, glm::vec2& node_pt)
{
	// Add the node to the list
	node_store temp_node;
	temp_node.node_id = node_id;
	temp_node.node_pt = node_pt;
	temp_node.node_color = geom_param_ptr->geom_colors.node_color;

	//// Check whether the node_id is already there
	//if (nodeMap.find(node_id) != nodeMap.end())
	//{
	//	// Node ID already exist (do not add)
	//	return;
	//}

	// Insert to the nodes
	nodeMap.insert({ node_id, temp_node });
	node_count++;

	
}

void nodes_list_store::set_buffer()
{
	// Set the buffers for the Model
	node_points.set_buffer();
}


void nodes_list_store::paint_model_nodes()
{
	// Paint the model nodes
	node_points.paint_points();
}

int nodes_list_store::is_node_hit(glm::vec2& loc)
{
	// Return the node id of node which is clicked
	// Covert mouse location to screen location
	int max_dim = geom_param_ptr->window_width > geom_param_ptr->window_height ? geom_param_ptr->window_width : geom_param_ptr->window_height;

	// Transform the mouse location to openGL screen coordinates
	double screen_x = 2.0f * ((loc.x - (geom_param_ptr->window_width * 0.5f)) / max_dim);
	double screen_y = 2.0f * (((geom_param_ptr->window_height * 0.5f) - loc.y) / max_dim);

	// Nodal location
	glm::mat4 scaling_matrix = glm::mat4(1.0) * static_cast<float>(geom_param_ptr->zoom_scale);
	scaling_matrix[3][3] = 1.0f;

	glm::mat4 scaledModelMatrix = scaling_matrix * geom_param_ptr->modelMatrix;

	// Loop through all nodes in map and update min and max values
	for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it)
	{
		const auto& node = it->second.node_pt;
		glm::vec4 finalPosition = scaledModelMatrix * glm::vec4(node.x, node.y, 0, 1.0f) * geom_param_ptr->panTranslation;

		double node_position_x = finalPosition.x;
		double node_position_y = finalPosition.y;

		if ((((node_position_x - screen_x) * (node_position_x - screen_x)) +
			((node_position_y - screen_y) * (node_position_y - screen_y))) < (16 * geom_param_ptr->node_circle_radii * geom_param_ptr->node_circle_radii))
		{
			// Return the id of the node
			// 4 x Radius is the threshold of hit (2 * Diameter)
			return it->first;
		}
	}

	// None found
	return -1;
}

void nodes_list_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update model openGL uniforms
	node_points.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
}
