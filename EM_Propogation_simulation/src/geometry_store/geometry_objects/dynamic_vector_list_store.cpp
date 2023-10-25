#include "dynamic_vector_list_store.h"

dynamic_vector_list_store::dynamic_vector_list_store()
{
	// Empty constructor
}

dynamic_vector_list_store::~dynamic_vector_list_store()
{
	// Empty destructor
}

void dynamic_vector_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Create the point shader
	std::filesystem::path shadersPath = geom_param_ptr->resourcePath;

	dyn_vector_shader.create_shader((shadersPath.string() + "/resources/shaders/dynvectorpoint_vert_shader.vert").c_str(),
		(shadersPath.string() + "/resources/shaders/dynvectorpoint_frag_shader.frag").c_str());

	// Delete all the vectors
	dyn_vector_count = 0;
	dyn_vectorMap.clear();
}

void dynamic_vector_list_store::add_vector(int& vector_id, std::vector<glm::vec2>& vector_startpt_loc, std::vector<glm::vec2>& vector_endpt_loc, std::vector<glm::vec3>& vector_color)
{
	// Create a temporary points
	dynamic_vector_store dyn_temp_vec;
	dyn_temp_vec.vector_id = vector_id;

	// Dynamic Vector points
	dyn_temp_vec.vector_startpt_loc = vector_startpt_loc;
	dyn_temp_vec.vector_endpt_loc = vector_endpt_loc;

	// Vector Color
	dyn_temp_vec.vector_color = vector_color;

	// Reserve space for the new element
	dyn_vectorMap.reserve(dyn_vectorMap.size() + 1);

	// Add to the list
	dyn_vectorMap.push_back(dyn_temp_vec);

	// Iterate the vector count
	dyn_vector_count++;
}

void dynamic_vector_list_store::set_buffer()
{


}

void dynamic_vector_list_store::paint_vectors(const int& dyn_index)
{
}

void dynamic_vector_list_store::update_buffer(const int& dyn_index)
{
}

void dynamic_vector_list_store::clear_vectors()
{
	// Delete all the vectors
	dyn_vector_count = 0;
	dyn_vectorMap.clear();
}

void dynamic_vector_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	if (set_modelmatrix == true)
	{
		// set the model matrix
		dyn_vector_shader.setUniform("geom_scale", static_cast<float>(geom_param_ptr->geom_scale));
		dyn_vector_shader.setUniform("transparency", 1.0f);

		dyn_vector_shader.setUniform("modelMatrix", geom_param_ptr->modelMatrix, false);
	}

	if (set_pantranslation == true)
	{
		// set the pan translation
		dyn_vector_shader.setUniform("panTranslation", geom_param_ptr->panTranslation, false);
	}

	if (set_zoomtranslation == true)
	{
		// set the zoom translation
		dyn_vector_shader.setUniform("zoomscale", static_cast<float>(geom_param_ptr->zoom_scale));
	}

	if (set_transparency == true)
	{
		// set the alpha transparency
		dyn_vector_shader.setUniform("transparency", static_cast<float>(geom_param_ptr->geom_transparency));
	}

	if (set_deflscale == true)
	{
		// set the deflection scale
		dyn_vector_shader.setUniform("normalized_deflscale", static_cast<float>(geom_param_ptr->normalized_defl_scale));
		dyn_vector_shader.setUniform("deflscale", static_cast<float>(geom_param_ptr->defl_scale));
	}
}

void dynamic_vector_list_store::get_vector_vertex_buffer(dynamic_vector_store& ln, const int& dyn_index, float* dyn_vector_vertices, unsigned int& dyn_vector_v_index)
{
}

void dynamic_vector_list_store::get_vector_index_buffer(unsigned int* dyn_vector_vertex_indices, unsigned int& dyn_vector_i_index)
{
}
