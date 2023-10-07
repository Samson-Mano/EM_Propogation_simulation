#include "nodevector_list_store.h"

nodevector_list_store::nodevector_list_store()
{
	// Empty constructor
}

nodevector_list_store::~nodevector_list_store()
{
	// Empty destructor
}

void nodevector_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Set the geometry parameters for the vectors
	vector_lines.init(geom_param_ptr);
	vector_arrow_left.init(geom_param_ptr);
	vector_arrow_right.init(geom_param_ptr);


	// Clear the vector data
	vector_count = 0;
	vectorMap.clear();
}

void nodevector_list_store::add_vector(int& vector_id, glm::vec2& vector_loc, std::vector<glm::vec2>& vector_values, std::vector<double>& vector_max_mag)
{




}

void nodevector_list_store::delete_vector(int& vector_id)
{
}

void nodevector_list_store::clear_data()
{
	// Clear all the data
	vector_lines.clear_lines();
	vector_arrow_left.clear_lines();
	vector_arrow_right.clear_lines();

	// Clear the vector data
	vector_count = 0;
	vectorMap.clear();
}

void nodevector_list_store::set_buffer()
{


}

void nodevector_list_store::paint_vectors(const int& dyn_index)
{


}

void nodevector_list_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update the geometry matrices 
	vector_lines.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency,set_deflscale);
	vector_arrow_left.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
	vector_arrow_right.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);

}
