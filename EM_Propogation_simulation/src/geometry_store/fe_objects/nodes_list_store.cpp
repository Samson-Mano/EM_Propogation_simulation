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

	// Node points
	//__________________________
	glm::vec3 temp_color = geom_param_ptr->geom_colors.node_color;

	//__________________________ Add the  points
	node_points.add_point(node_id, node_pt, temp_color);
	
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

void nodes_list_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update model openGL uniforms
	node_points.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
}
