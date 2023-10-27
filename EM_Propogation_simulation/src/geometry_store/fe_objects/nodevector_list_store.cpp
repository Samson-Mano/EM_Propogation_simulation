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

	// Clear the vector data
	vector_count = 0;
	vectorMap.clear();
}

void nodevector_list_store::add_vector(int& vector_id, glm::vec2& vector_loc,
	std::vector<glm::vec2>& vector_values, std::vector<double>& vector_timestep_max_mag, std::vector<double>& vector_timestep_min_mag)
{
	// Create a temporary vector
	vector_data temp_vector;
	temp_vector.vector_id = vector_id;
	temp_vector.vector_loc = vector_loc;
	temp_vector.vector_values = vector_values;
	// temp_vector.vector_timestep_max_mag = vector_timestep_max_mag;
	// temp_vector.vector_timestep_min_mag = vector_timestep_min_mag;


	// Create the vector color list
	// Vector color
	std::vector<glm::vec3> vector_colors;
	std::vector<double> vec_ratio;

	// Add Vector values scaled to 1;
	std::vector<glm::vec2> vector_values_scaled;

	int i = 0;
	for (auto& vec_value : vector_values)
	{
		// Get the vector magnitude
		double vec_mag_ratio = (glm::length(vec_value) - vector_timestep_min_mag[i]) / (vector_timestep_max_mag[i] - vector_timestep_min_mag[i]);

		// std::cout << vec_mag_ratio << std::endl;
		vec_ratio.push_back(vec_mag_ratio);

		// get the vector color
		glm::vec3 vec_color = geom_parameters::getContourColor(static_cast<float>(1.0f - vec_mag_ratio));

		// Scale the vector valued
		vector_values_scaled.push_back(vector_loc + static_cast<float>(vec_mag_ratio) * glm::normalize( vec_value - vector_loc));

		// Add to the vector color values
		vector_colors.push_back(vec_color);

		i++;
	}

	// add to the struct
	temp_vector.vec_ratio = vec_ratio;
	temp_vector.vector_values_scaled = vector_values_scaled;
	temp_vector.vector_colors = vector_colors;

	// Insert to the nodes
	vectorMap.insert({ vector_id, temp_vector });
	vector_count++;
}

void nodevector_list_store::delete_vector(int& vector_id)
{
}

void nodevector_list_store::clear_data()
{
	// Clear all the data
	vector_lines.clear_vectors();
	
	// Clear the vector data
	vector_count = 0;
	vectorMap.clear();
}

void nodevector_list_store::set_buffer()
{
	// Clear all the lines
	vector_lines.clear_vectors();

	//_____________________ Add the Dynamic Vectors
	for (auto& vec_m : vectorMap)
	{
		vector_data vec = vec_m.second;

		// Scale the vector values befor adding

		vector_lines.add_vector(vec.vector_id,vec.vector_loc, vec.vector_values_scaled, vec.vector_colors);
	}

	// Set the buffer (Only the index buffer is set because its a dynamic paint)
	vector_lines.set_buffer();
}

void nodevector_list_store::paint_vectors(const int& dyn_index)
{
	// Paint the vector
	vector_lines.paint_vectors(dyn_index);
}

void nodevector_list_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update the geometry matrices 
	vector_lines.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
}
