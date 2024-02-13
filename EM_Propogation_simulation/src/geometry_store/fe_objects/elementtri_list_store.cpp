#include "elementtri_list_store.h"

elementtri_list_store::elementtri_list_store()
{
	// Empty constructor
}

elementtri_list_store::~elementtri_list_store()
{
	// Empty destructor
}

void elementtri_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Set the geometry parameters for the labels (and clear the labels)
	element_tris.init(geom_param_ptr);

	// Clear the triangles
	elementtri_count = 0;
	elementtriMap.clear();
}

void elementtri_list_store::add_elementtriangle(int& tri_id, node_store* nd1, node_store* nd2, node_store* nd3)
{
	// Add the line to the list
	elementtri_store temp_tri;
	temp_tri.tri_id = tri_id; // Triangle ID
	temp_tri.nd1 = nd1;
	temp_tri.nd2 = nd2;
	temp_tri.nd3 = nd3;

	// Check whether the node_id is already there
	//if (elementtriMap.find(tri_id) != elementtriMap.end())
	//{
	//	// Element ID already exist (do not add)
	//	return;
	//}

	// Insert to the lines
	elementtriMap.insert({ tri_id, temp_tri });
	elementtri_count++;

	//__________________________ Add the Triangle
	int tri_level = this->tri_level;
	glm::vec2 node_pt1 = (*nd1).node_pt;
	glm::vec2 node_pt2 = (*nd2).node_pt;
	glm::vec2 node_pt3 = (*nd3).node_pt;

	//________________________________________________________
	glm::vec3 temp_tri_color = geom_param_ptr->geom_colors.triangle_color;
	// Main triangle
	element_tris.add_tri(tri_id, node_pt1, node_pt2, node_pt3,temp_tri_color);

}

void elementtri_list_store::set_buffer()
{
	// Set the buffers for the Model
	element_tris.set_buffer();
}

void elementtri_list_store::paint_elementtriangles()
{
	// Paint the triangles
	element_tris.paint_triangles();
}

void elementtri_list_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update model openGL uniforms
	element_tris.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
	
}
