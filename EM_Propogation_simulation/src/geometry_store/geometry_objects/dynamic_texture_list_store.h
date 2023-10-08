#pragma once
#include "../geometry_buffers/gBuffers.h"
#include "../geom_parameters.h"


struct dynamic_texture_store
{
	int dyn_texture_id = 0;
	glm::vec2 dyn_texture_loc = glm::vec2(0);
	std::vector<glm::vec2> dyn_point_offset; // Dynamic point offset
	double texture_value = 0.0;
};


class dynamic_texture_list_store
{
public:
	geom_parameters* geom_param_ptr = nullptr;
	unsigned int dyn_texture_count = 0;
	double max_texture_value = 0.0;
	std::vector<dynamic_texture_store> dyn_textureMap;

	dynamic_texture_list_store();
	~dynamic_texture_list_store();
	void init(geom_parameters* geom_param_ptr);
	void add_texture(int& dyn_texture_id, glm::vec2& dyn_texture_loc, std::vector<glm::vec2>& dyn_point_offset, double& texture_value);
	void set_buffer();
	void paint_points(const int& dyn_index);
	void update_buffer(const int& dyn_index);
	void clear_textures();
	void update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale);
private:
	gBuffers dyn_texture_buffer;
	Shader dyn_texture_shader;
	Texture dyn_tx_texture;

	void get_texture_vertex_buffer(dynamic_texture_store& dyn_tx, const int& dyn_index,
		float* dyn_point_vertices, unsigned int& dyn_point_v_index);

	void get_texture_index_buffer(unsigned int* dyn_point_indices, unsigned int& dyn_point_i_index);
};
