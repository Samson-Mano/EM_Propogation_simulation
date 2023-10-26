#pragma once
#include "../geometry_buffers/gBuffers.h"
#include "../geom_parameters.h"

struct dynamic_vector_store
{
	// store the individual point
	int vector_id = 0;
	glm::vec2 vector_origin; // Vector start pt
	std::vector < glm::vec2> vector_endpt_loc; // Dynamic Vector end pt
	std::vector<glm::vec3> vector_color; // Dynamic Vector magnitude color
};


class dynamic_vector_list_store
{
public:
	geom_parameters* geom_param_ptr = nullptr;
	unsigned int dyn_vector_count = 0;
	std::vector<dynamic_vector_store> dyn_vectorMap;

	dynamic_vector_list_store();
	~dynamic_vector_list_store();
	void init(geom_parameters* geom_param_ptr);
	void add_vector(int& vector_id, glm::vec2 vector_origin, std::vector<glm::vec2>& vector_endpt_loc,
		std::vector<glm::vec3>& vector_color);
	void set_buffer();
	void paint_vectors(const int& dyn_index);
	void update_buffer(const int& dyn_index);
	void clear_vectors();
	void update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale);
private:
	gBuffers dyn_vector_buffer;
	Shader dyn_vector_shader;

	void get_vector_vertex_buffer(dynamic_vector_store& vec, const int& dyn_index,
		float* dyn_vector_vertices, unsigned int& dyn_vector_v_index);

	void get_vector_index_buffer(unsigned int* dyn_vector_vertex_indices, unsigned int& dyn_vector_i_index);
};


