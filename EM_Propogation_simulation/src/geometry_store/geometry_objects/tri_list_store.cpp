#include "tri_list_store.h"

tri_list_store::tri_list_store()
{
	// Empty constructor
}

tri_list_store::~tri_list_store()
{
	// Empty destructor
}

void tri_list_store::init(geom_parameters* geom_param_ptr)
{
}

void tri_list_store::add_tri(int& tri_id, glm::vec2& tript1_loc, glm::vec2& tript2_loc, glm::vec2& tript3_loc, glm::vec2 tript1_offset, glm::vec2 tript2_offset, glm::vec2 tript3_offset, glm::vec3& tript1_color, glm::vec3& tript2_color, glm::vec3& tript3_color, bool is_offset)
{
}

void tri_list_store::set_buffer()
{
}

void tri_list_store::paint_triangles()
{
}

void tri_list_store::clear_triangles()
{
}

void tri_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
}

void tri_list_store::get_line_buffer(tri_store& tri, float* tri_vertices, unsigned int& tri_v_index, unsigned int* tri_vertex_indices, unsigned int& tri_i_index)
{
}
