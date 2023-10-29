#include "dynamictricontour_list_store.h"

dynamictricontour_list_store::dynamictricontour_list_store()
{
	// Empty constructor
}

dynamictricontour_list_store::~dynamictricontour_list_store()
{
	// Empty destructor
}

void dynamictricontour_list_store::init(geom_parameters* geom_param_ptr)
{
}

void dynamictricontour_list_store::add_dyntricontour(int& tri_id, glm::vec2 tri_coord1, glm::vec2 tri_coord2, glm::vec2 tri_coord3, std::vector<glm::vec2>& tri_displ1, std::vector<glm::vec2>& tri_displ2, std::vector<glm::vec2>& tri_displ3, std::vector<glm::vec3>& tri_pt1_color, std::vector<glm::vec3>& tri_pt2_color, std::vector<glm::vec3>& tri_pt3_color, std::vector<double> contour_level_list)
{
}

void dynamictricontour_list_store::set_buffer()
{
}

void dynamictricontour_list_store::paint_dyntricontour(const int& dyn_index)
{
}

void dynamictricontour_list_store::update_buffer(const int& dyn_index)
{
}

void dynamictricontour_list_store::clear_dyntricontours()
{
}

void dynamictricontour_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
}

void dynamictricontour_list_store::get_contour_vertex_buffer(dynamictricontour_store& dyntri, const int& dyn_index, float* dyn_contour_vertices, unsigned int& dyn_contour_v_index)
{
}

void dynamictricontour_list_store::get_contour_index_buffer(unsigned int* dyn_contour_vertex_indices, unsigned int& dyn_contour_i_index)
{
}
