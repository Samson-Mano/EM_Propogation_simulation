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

void dynamic_vector_list_store::add_vector(int& vector_id, glm::vec2 vector_origin, std::vector<glm::vec2>& vector_endpt_loc, std::vector<glm::vec3>& vector_color)
{
	// Create a temporary points
	dynamic_vector_store dyn_temp_vec;
	dyn_temp_vec.vector_id = vector_id;

	// Dynamic Vector points
	dyn_temp_vec.vector_origin = vector_origin;
	dyn_temp_vec.vector_endpt_loc = vector_endpt_loc;

	// Vector Color
	dyn_temp_vec.vector_color = vector_color;

	// Add to the list
	dyn_vectorMap.push_back(dyn_temp_vec);

	// Iterate the vector count
	dyn_vector_count++;
}

void dynamic_vector_list_store::set_buffer()
{
	// Set the buffer for index
	unsigned int vector_indices_count = 6 * dyn_vector_count; // 1 indices for a point (1 line for vector, 2 line for arrow)
	unsigned int* vector_vertex_indices = new unsigned int[vector_indices_count];

	unsigned int vector_i_index = 0;

	// Set the vector index buffers
	for (auto& vec : dyn_vectorMap)
	{
		// Create the index buffers
		get_vector_index_buffer(vector_vertex_indices, vector_i_index);
	}

	//Set the layout of the dynamic vector
	VertexBufferLayout vector_pt_layout;
	vector_pt_layout.AddFloat(2); // Vector origin
	vector_pt_layout.AddFloat(2); // Vector end point
	vector_pt_layout.AddFloat(3); // Vector color
	vector_pt_layout.AddFloat(1); // Type of point

	// Define the vector vertices of the model for a point (2 origin, 2 position, 3 color  & 1 point type) 
	const unsigned int vector_vertex_count = 8 * 6 * dyn_vector_count;
	unsigned int vector_vertex_size = vector_vertex_count * sizeof(float); // Size of the vector_vertex

	// Create a dynamic buffer to allocate space for the points
	dyn_vector_buffer.CreateDynamicBuffers(vector_vertex_size, vector_vertex_indices, vector_indices_count, vector_pt_layout);

	// Delete the dynamic array
	delete[] vector_vertex_indices;
}

void dynamic_vector_list_store::paint_vectors(const int& dyn_index)
{
	// Paint all the vectors
	dyn_vector_shader.Bind();
	dyn_vector_buffer.Bind();

	// Update the vector buffer data for dynamic drawing
	update_buffer(dyn_index);

	// Paint the vector
	glDrawElements(GL_LINES, (6 * dyn_vector_count), GL_UNSIGNED_INT, 0);

	dyn_vector_buffer.UnBind();
	dyn_vector_shader.UnBind();
}

void dynamic_vector_list_store::update_buffer(const int& dyn_index)
{
	//  Define the vector vertices of the model for a point (2 origin, 2 position, 3 color  & 1 point type) 
	const unsigned int vector_vertex_count = 8 * 6 * dyn_vector_count;
	float* vector_vertices = new float[vector_vertex_count];

	unsigned int vector_v_index = 0;

	// Set the vector point vertices
	for (auto& vec : dyn_vectorMap)
	{
		// Add vector points buffers
		get_vector_vertex_buffer(vec, dyn_index, vector_vertices, vector_v_index);
	}

	unsigned int vector_vertex_size = vector_vertex_count * sizeof(float); // Size of the line point vertex

	// Update the buffer
	dyn_vector_buffer.UpdateDynamicVertexBuffer(vector_vertices, vector_vertex_size);

	// Delete the dynamic array
	delete[] vector_vertices;
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

void dynamic_vector_list_store::get_vector_vertex_buffer(dynamic_vector_store& vec, const int& dyn_index,
	float* dyn_vector_vertices, unsigned int& dyn_vector_v_index)
{
	// Get the node buffer for the shader
	// Vector Origin Point
	// Vector Origin
	dyn_vector_vertices[dyn_vector_v_index + 0] = vec.vector_origin.x;
	dyn_vector_vertices[dyn_vector_v_index + 1] = vec.vector_origin.y;

	// Vector end point
	dyn_vector_vertices[dyn_vector_v_index + 2] = vec.vector_origin.x;
	dyn_vector_vertices[dyn_vector_v_index + 3] = vec.vector_origin.y;

	// Point color
	dyn_vector_vertices[dyn_vector_v_index + 4] = vec.vector_color[dyn_index].x;
	dyn_vector_vertices[dyn_vector_v_index + 5] = vec.vector_color[dyn_index].y;
	dyn_vector_vertices[dyn_vector_v_index + 6] = vec.vector_color[dyn_index].z;

	// Type of Point (origin point = 0)
	dyn_vector_vertices[dyn_vector_v_index + 7] = 0;

	// Iterate
	dyn_vector_v_index = dyn_vector_v_index + 8;

	glm::vec2 vec_end_pt = glm::vec2(vec.vector_origin.x + vec.vector_endpt_loc[dyn_index].x,
		vec.vector_origin.y - vec.vector_endpt_loc[dyn_index].y);

	// Vector End Point
	// Vector Origin
	dyn_vector_vertices[dyn_vector_v_index + 0] = vec.vector_origin.x;
	dyn_vector_vertices[dyn_vector_v_index + 1] = vec.vector_origin.y;

	// Vector end point
	dyn_vector_vertices[dyn_vector_v_index + 2] = vec_end_pt.x;
	dyn_vector_vertices[dyn_vector_v_index + 3] = vec_end_pt.y;

	// Point color
	dyn_vector_vertices[dyn_vector_v_index + 4] = vec.vector_color[dyn_index].x;
	dyn_vector_vertices[dyn_vector_v_index + 5] = vec.vector_color[dyn_index].y;
	dyn_vector_vertices[dyn_vector_v_index + 6] = vec.vector_color[dyn_index].z;

	// Type of Point (End point = 1)
	dyn_vector_vertices[dyn_vector_v_index + 7] = 1;

	// Iterate
	dyn_vector_v_index = dyn_vector_v_index + 8;

	//______________________________________________________________________________________
	// Arrow head line 1
	// Vector End Point
	// Vector Origin
	dyn_vector_vertices[dyn_vector_v_index + 0] = vec.vector_origin.x;
	dyn_vector_vertices[dyn_vector_v_index + 1] = vec.vector_origin.y;

	// Vector end point
	dyn_vector_vertices[dyn_vector_v_index + 2] = vec_end_pt.x;
	dyn_vector_vertices[dyn_vector_v_index + 3] = vec_end_pt.y;

	// Point color
	dyn_vector_vertices[dyn_vector_v_index + 4] = vec.vector_color[dyn_index].x;
	dyn_vector_vertices[dyn_vector_v_index + 5] = vec.vector_color[dyn_index].y;
	dyn_vector_vertices[dyn_vector_v_index + 6] = vec.vector_color[dyn_index].z;

	// Type of Point (End point = 1)
	dyn_vector_vertices[dyn_vector_v_index + 7] = 1;

	// Iterate
	dyn_vector_v_index = dyn_vector_v_index + 8;

	// Vector End Point
	// Vector Origin
	dyn_vector_vertices[dyn_vector_v_index + 0] = vec.vector_origin.x;
	dyn_vector_vertices[dyn_vector_v_index + 1] = vec.vector_origin.y;

	// Vector end point
	dyn_vector_vertices[dyn_vector_v_index + 2] = vec_end_pt.x;
	dyn_vector_vertices[dyn_vector_v_index + 3] = vec_end_pt.y;

	// Point color
	dyn_vector_vertices[dyn_vector_v_index + 4] = vec.vector_color[dyn_index].x;
	dyn_vector_vertices[dyn_vector_v_index + 5] = vec.vector_color[dyn_index].y;
	dyn_vector_vertices[dyn_vector_v_index + 6] = vec.vector_color[dyn_index].z;

	// Type of Point (arrow head point 1 = 2)
	dyn_vector_vertices[dyn_vector_v_index + 7] = 2;

	// Iterate
	dyn_vector_v_index = dyn_vector_v_index + 8;

	//______________________________________________________________________________________
	// Arrow head line 2
	// Vector End Point
	// Vector Origin
	dyn_vector_vertices[dyn_vector_v_index + 0] = vec.vector_origin.x;
	dyn_vector_vertices[dyn_vector_v_index + 1] = vec.vector_origin.y;

	// Vector end point
	dyn_vector_vertices[dyn_vector_v_index + 2] = vec_end_pt.x;
	dyn_vector_vertices[dyn_vector_v_index + 3] = vec_end_pt.y;

	// Point color
	dyn_vector_vertices[dyn_vector_v_index + 4] = vec.vector_color[dyn_index].x;
	dyn_vector_vertices[dyn_vector_v_index + 5] = vec.vector_color[dyn_index].y;
	dyn_vector_vertices[dyn_vector_v_index + 6] = vec.vector_color[dyn_index].z;

	// Type of Point (End point = 1)
	dyn_vector_vertices[dyn_vector_v_index + 7] = 1;

	// Iterate
	dyn_vector_v_index = dyn_vector_v_index + 8;

	// Vector End Point
	// Vector Origin
	dyn_vector_vertices[dyn_vector_v_index + 0] = vec.vector_origin.x;
	dyn_vector_vertices[dyn_vector_v_index + 1] = vec.vector_origin.y;

	// Vector end point
	dyn_vector_vertices[dyn_vector_v_index + 2] = vec_end_pt.x;
	dyn_vector_vertices[dyn_vector_v_index + 3] = vec_end_pt.y;

	// Point color
	dyn_vector_vertices[dyn_vector_v_index + 4] = vec.vector_color[dyn_index].x;
	dyn_vector_vertices[dyn_vector_v_index + 5] = vec.vector_color[dyn_index].y;
	dyn_vector_vertices[dyn_vector_v_index + 6] = vec.vector_color[dyn_index].z;

	// Type of Point (arrow head point 2 = 3)
	dyn_vector_vertices[dyn_vector_v_index + 7] = 3;

	// Iterate
	dyn_vector_v_index = dyn_vector_v_index + 8;
}

void dynamic_vector_list_store::get_vector_index_buffer(unsigned int* dyn_vector_vertex_indices, unsigned int& dyn_vector_i_index)
{
	//__________________________________________________________________________
	// Add the indices
	// Vector line
	// Vector origin
	dyn_vector_vertex_indices[dyn_vector_i_index] = dyn_vector_i_index;

	dyn_vector_i_index = dyn_vector_i_index + 1;

	// Vector end point
	dyn_vector_vertex_indices[dyn_vector_i_index] = dyn_vector_i_index;

	dyn_vector_i_index = dyn_vector_i_index + 1;

	//__________________________________________________________________________
	// Vector arrow head line 1
	// Vector end point
	dyn_vector_vertex_indices[dyn_vector_i_index] = dyn_vector_i_index;

	dyn_vector_i_index = dyn_vector_i_index + 1;

	// Arrow point 1
	dyn_vector_vertex_indices[dyn_vector_i_index] = dyn_vector_i_index;

	dyn_vector_i_index = dyn_vector_i_index + 1;

	//__________________________________________________________________________
	// Vector arrow head line 2
	// Vector end point
	dyn_vector_vertex_indices[dyn_vector_i_index] = dyn_vector_i_index;

	dyn_vector_i_index = dyn_vector_i_index + 1;

	// Arrow point 2
	dyn_vector_vertex_indices[dyn_vector_i_index] = dyn_vector_i_index;

	dyn_vector_i_index = dyn_vector_i_index + 1;
}
