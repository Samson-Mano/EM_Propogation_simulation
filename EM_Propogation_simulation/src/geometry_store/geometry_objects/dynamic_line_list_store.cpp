#include "dynamic_line_list_store.h"

dynamic_line_list_store::dynamic_line_list_store()
{
	// Empty constructor
}

dynamic_line_list_store::~dynamic_line_list_store()
{
	// Empty destructor
}

void dynamic_line_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Create the point shader
	std::filesystem::path shadersPath = geom_param_ptr->resourcePath;

	dyn_line_shader.create_shader((shadersPath.string() + "/resources/shaders/dynline_vert_shader.vert").c_str(),
		(shadersPath.string() + "/resources/shaders/dynline_frag_shader.frag").c_str());

	// Delete all the lines
	dyn_line_count = 0;
	dyn_lineMap.clear();

}

void dynamic_line_list_store::add_line(int& line_id, std::vector<glm::vec2>& line_start_pt, std::vector<glm::vec2>& line_end_pt, std::vector<glm::vec3>& line_color)
{
	dynamic_line_store dyn_temp_ln;
	dyn_temp_ln.line_id = line_id;
	dyn_temp_ln.line_start_pt = line_start_pt; // Dynamic line start point
	dyn_temp_ln.line_end_pt = line_end_pt; // Dynamic line end point
	dyn_temp_ln.line_color = line_color; // Dynamic line color

	//___________________________________________________________________
	// Reserve space for the new element
	dyn_lineMap.reserve(dyn_lineMap.size() + 1);

	// Add to the line list
	dyn_lineMap.push_back(dyn_temp_ln);

	// Iterate the line count
	dyn_line_count++;

}

void dynamic_line_list_store::set_buffer()
{
	// Set the buffer for index
	unsigned int line_indices_count = 2 * dyn_line_count; // 2 indices to form a line
	unsigned int* line_vertex_indices = new unsigned int[line_indices_count];

	unsigned int line_i_index = 0;

	// Set the line vertices
	for (auto& ln : dyn_lineMap)
	{
		// Add point index buffers
		get_line_index_buffer(line_vertex_indices, line_i_index);
	}

	VertexBufferLayout line_layout;
	line_layout.AddFloat(2);  // Point
	line_layout.AddFloat(3);  // Color

	// Define the line vertices of the model for a node (2 position, 3 color) 
	const unsigned int line_vertex_count = 2 * 5 * dyn_line_count;
	unsigned int line_vertex_size = line_vertex_count * sizeof(float); // Size of the node_vertex

	// Allocate space for Vertex buffer
	// Create the Line buffers
	dyn_line_buffer.CreateDynamicBuffers(line_vertex_size, line_vertex_indices, line_indices_count, line_layout);

	// Delete the dynamic array
	delete[] line_vertex_indices;
}

void dynamic_line_list_store::paint_lines(const int& dyn_index)
{
	// Paint all the lines
	dyn_line_shader.Bind();
	dyn_line_buffer.Bind();

	// Update the line buffer data for dynamic drawing
	update_buffer(dyn_index);

	glDrawElements(GL_LINES, 2 * dyn_line_count, GL_UNSIGNED_INT, 0);
	dyn_line_buffer.UnBind();
	dyn_line_shader.UnBind();
}

void dynamic_line_list_store::update_buffer(const int& dyn_index)
{
	// Define the line vertices of the model for a node (2 position, 3 color) 
	const unsigned int line_vertex_count = 2 * 5 * dyn_line_count;
	float* line_vertices = new float[line_vertex_count];

	unsigned int line_v_index = 0;

	// Set the line vertices
	for (auto& ln : dyn_lineMap)
	{
		// Add line buffers
		get_line_vertex_buffer(ln, dyn_index, line_vertices, line_v_index);
	}

	unsigned int line_vertex_size = line_vertex_count * sizeof(float); // Size of the line vertex

	// Update the buffer
	dyn_line_buffer.UpdateDynamicVertexBuffer(line_vertices, line_vertex_size);

	// Delete the dynamic array
	delete[] line_vertices;

}

void dynamic_line_list_store::clear_lines()
{
	// Delete all the lines
	dyn_line_count = 0;
	dyn_lineMap.clear();
}

void dynamic_line_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	if (set_modelmatrix == true)
	{
		// set the model matrix
		dyn_line_shader.setUniform("geom_scale", static_cast<float>(geom_param_ptr->geom_scale));
		dyn_line_shader.setUniform("transparency", 1.0f);

		dyn_line_shader.setUniform("modelMatrix", geom_param_ptr->modelMatrix, false);
	}

	if (set_pantranslation == true)
	{
		// set the pan translation
		dyn_line_shader.setUniform("panTranslation", geom_param_ptr->panTranslation, false);
	}

	if (set_zoomtranslation == true)
	{
		// set the zoom translation
		dyn_line_shader.setUniform("zoomscale", static_cast<float>(geom_param_ptr->zoom_scale));
	}

	if (set_transparency == true)
	{
		// set the alpha transparency
		dyn_line_shader.setUniform("transparency", static_cast<float>(geom_param_ptr->geom_transparency));
	}

	if (set_deflscale == true)
	{
		// set the deflection scale
		dyn_line_shader.setUniform("normalized_deflscale", static_cast<float>(geom_param_ptr->normalized_defl_scale));
		dyn_line_shader.setUniform("deflscale", static_cast<float>(geom_param_ptr->defl_scale));
	}
}

void dynamic_line_list_store::get_line_vertex_buffer(dynamic_line_store& ln, const int& dyn_index, float* dyn_line_vertices, unsigned int& dyn_line_v_index)
{
	// Get the node buffer for the shader
	// Point 1
	// Point location
	dyn_line_vertices[dyn_line_v_index + 0] = ln.line_start_pt[dyn_index].x;
	dyn_line_vertices[dyn_line_v_index + 1] = ln.line_start_pt[dyn_index].y;

	// Point color
	dyn_line_vertices[dyn_line_v_index + 2] = ln.line_color[dyn_index].x;
	dyn_line_vertices[dyn_line_v_index + 3] = ln.line_color[dyn_index].y;
	dyn_line_vertices[dyn_line_v_index + 4] = ln.line_color[dyn_index].z;

	// Iterate
	dyn_line_v_index = dyn_line_v_index + 5;

	// Point 2
	// Point location
	dyn_line_vertices[dyn_line_v_index + 0] = ln.line_end_pt[dyn_index].x;
	dyn_line_vertices[dyn_line_v_index + 1] = ln.line_end_pt[dyn_index].y;

	// Point color
	dyn_line_vertices[dyn_line_v_index + 2] = ln.line_color[dyn_index].x;
	dyn_line_vertices[dyn_line_v_index + 3] = ln.line_color[dyn_index].y;
	dyn_line_vertices[dyn_line_v_index + 4] = ln.line_color[dyn_index].z;

	// Iterate
	dyn_line_v_index = dyn_line_v_index + 5;

}

void dynamic_line_list_store::get_line_index_buffer(unsigned int* dyn_line_indices, unsigned int& dyn_line_i_index)
{
	//__________________________________________________________________________
	// Add the indices
	dyn_line_indices[dyn_line_i_index] = dyn_line_i_index;

	dyn_line_i_index = dyn_line_i_index + 1;

	dyn_line_indices[dyn_line_i_index] = dyn_line_i_index;

	dyn_line_i_index = dyn_line_i_index + 1;

}
