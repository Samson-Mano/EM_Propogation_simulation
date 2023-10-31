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
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Create the point shader
	std::filesystem::path shadersPath = geom_param_ptr->resourcePath;

	dyn_contour_shader.create_shader((shadersPath.string() + "/resources/shaders/dyncontourpoint_vert_shader.vert").c_str(),
		(shadersPath.string() + "/resources/shaders/dyncontourpoint_frag_shader.frag").c_str());

	// Delete all the contours
	dyn_tri_count = 0;
	dyn_triMap.clear();
}

void dynamictricontour_list_store::add_dyntricontour(int& tri_id, glm::vec2 tri_coord1, glm::vec2 tri_coord2, glm::vec2 tri_coord3, 
	std::vector<double>& tri_displ1, std::vector<double>& tri_displ2, std::vector<double>& tri_displ3,
	std::vector<glm::vec3>& tri_pt1_color, std::vector<glm::vec3>& tri_pt2_color, std::vector<glm::vec3>& tri_pt3_color)
{
	// Create a temporary contours
	dynamictricontour_store dyn_temp_contour;

	dyn_temp_contour.tri_id = tri_id;

	dyn_temp_contour.tri_coord1 = tri_coord1; // Dynamic triangle pt1
	dyn_temp_contour.tri_coord2 = tri_coord2; // Dynamic triangle pt2
	dyn_temp_contour.tri_coord3 = tri_coord3; // Dynamic triangle pt3

	// Displacement
	dyn_temp_contour.tri_displ1 = tri_displ1; // Dynamic Displacement list of pt1
	dyn_temp_contour.tri_displ2 = tri_displ2;// Dynamic Displacement list of pt2
	dyn_temp_contour.tri_displ3 = tri_displ3;// Dynamic Displacement list of pt3

	// Color
	dyn_temp_contour.tri_pt1_color = tri_pt1_color; // Dynamic Color list of pt1
	dyn_temp_contour.tri_pt2_color = tri_pt2_color; // Dynamic Color list of pt2
	dyn_temp_contour.tri_pt3_color = tri_pt3_color; // Dynamic Color list of pt3

	// Reserve space for the new element
	dyn_triMap.reserve(dyn_triMap.size() + 1);

	// Add to the list
	dyn_triMap.push_back(dyn_temp_contour);

	// Iterate the triangle contour count
	dyn_tri_count++;
}

void dynamictricontour_list_store::set_buffer()
{
	// Set the buffer for index
	unsigned int dyn_contour_indices_count = 3 * dyn_tri_count; // 1 indices for a point (3 points for triangle)
	unsigned int* dyn_contour_vertex_indices = new unsigned int[dyn_contour_indices_count];

	unsigned int dyn_contour_i_index = 0;

	// Set the contour index buffers
	for (auto& tri : dyn_triMap)
	{
		// Create the index buffers
		get_contour_index_buffer(dyn_contour_vertex_indices, dyn_contour_i_index);
	}

	//Set the layout of the dynamic triangle contour
	VertexBufferLayout contour_pt_layout;
	contour_pt_layout.AddFloat(2); // Triangle point
	contour_pt_layout.AddFloat(3); // Triangle color

	// Define the triangle contour vertices of the model for a point (2 position, 3 color) 
	const unsigned int dyn_contour_vertex_count = 5 * 3 * dyn_tri_count;
	unsigned int dyn_contour_vertex_size = dyn_contour_vertex_count * sizeof(float); // Size of the vector_vertex

	// Create a dynamic buffer to allocate space for the points
	dyn_contour_buffer.CreateDynamicBuffers(dyn_contour_vertex_size, dyn_contour_vertex_indices, dyn_contour_indices_count, contour_pt_layout);

	// Delete the dynamic array
	delete[] dyn_contour_vertex_indices;

}

void dynamictricontour_list_store::paint_dyntricontour(const int& dyn_index)
{
	// Paint all the contours
	dyn_contour_shader.Bind();
	dyn_contour_buffer.Bind();

	// Update the vector buffer data for dynamic drawing
	update_buffer(dyn_index);

	// Paint the vector
	glDrawElements(GL_TRIANGLES, (3 * dyn_tri_count), GL_UNSIGNED_INT, 0);

	dyn_contour_buffer.UnBind();
	dyn_contour_shader.UnBind();

}

void dynamictricontour_list_store::update_buffer(const int& dyn_index)
{
	//  Define the contour vertices of the model for a point (2 position, 3 color) 
	const unsigned int dyn_contour_vertex_count = 5 * 3 * dyn_tri_count;
	float* dyn_contour_vertices = new float[dyn_contour_vertex_count];

	unsigned int dyn_contour_v_index = 0;

	// Set the contour point vertices
	for (auto& tri : dyn_triMap)
	{
		// Add contour points buffers
		get_contour_vertex_buffer(tri, dyn_index, dyn_contour_vertices, dyn_contour_v_index);
	}

	unsigned int dyn_contour_vertex_size = dyn_contour_vertex_count * sizeof(float); // Size of the line point vertex

	// Update the buffer
	dyn_contour_buffer.UpdateDynamicVertexBuffer(dyn_contour_vertices, dyn_contour_vertex_size);

	// Delete the dynamic array
	delete[] dyn_contour_vertices;
}

void dynamictricontour_list_store::clear_dyntricontours()
{
	// Delete all the contours
	dyn_tri_count = 0;
	dyn_triMap.clear();
}

void dynamictricontour_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{

	if (set_modelmatrix == true)
	{
		// set the model matrix
		dyn_contour_shader.setUniform("geom_scale", static_cast<float>(geom_param_ptr->geom_scale));
		dyn_contour_shader.setUniform("transparency", 1.0f);

		dyn_contour_shader.setUniform("modelMatrix", geom_param_ptr->modelMatrix, false);
	}

	if (set_pantranslation == true)
	{
		// set the pan translation
		dyn_contour_shader.setUniform("panTranslation", geom_param_ptr->panTranslation, false);
	}

	if (set_zoomtranslation == true)
	{
		// set the zoom translation
		dyn_contour_shader.setUniform("zoomscale", static_cast<float>(geom_param_ptr->zoom_scale));
	}

	if (set_transparency == true)
	{
		// set the alpha transparency
		dyn_contour_shader.setUniform("transparency", static_cast<float>(geom_param_ptr->geom_transparency));
	}

	if (set_deflscale == true)
	{
		// set the deflection scale
		dyn_contour_shader.setUniform("normalized_deflscale", static_cast<float>(geom_param_ptr->normalized_defl_scale));
		dyn_contour_shader.setUniform("deflscale", static_cast<float>(geom_param_ptr->defl_scale));
	}
}

void dynamictricontour_list_store::get_contour_vertex_buffer(dynamictricontour_store& dyntri, const int& dyn_index, float* dyn_contour_vertices, unsigned int& dyn_contour_v_index)
{
	// Get the three node buffer for the shader
	// Point 1
	// Point location
	dyn_contour_vertices[dyn_contour_v_index + 0] = dyntri.tri_coord1.x;
	dyn_contour_vertices[dyn_contour_v_index + 1] = dyntri.tri_coord1.y;

	// Point color
	dyn_contour_vertices[dyn_contour_v_index + 2] = dyntri.tri_pt1_color[dyn_index].x;
	dyn_contour_vertices[dyn_contour_v_index + 3] = dyntri.tri_pt1_color[dyn_index].y;
	dyn_contour_vertices[dyn_contour_v_index + 4] = dyntri.tri_pt1_color[dyn_index].z;

	// Iterate
	dyn_contour_v_index = dyn_contour_v_index + 5;

	// Point 2
	// Point location
	dyn_contour_vertices[dyn_contour_v_index + 0] = dyntri.tri_coord2.x;
	dyn_contour_vertices[dyn_contour_v_index + 1] = dyntri.tri_coord2.y;

	// Point color
	dyn_contour_vertices[dyn_contour_v_index + 2] = dyntri.tri_pt2_color[dyn_index].x;
	dyn_contour_vertices[dyn_contour_v_index + 3] = dyntri.tri_pt2_color[dyn_index].y;
	dyn_contour_vertices[dyn_contour_v_index + 4] = dyntri.tri_pt2_color[dyn_index].z;

	// Iterate
	dyn_contour_v_index = dyn_contour_v_index + 5;

	// Point 3
	// Point location
	dyn_contour_vertices[dyn_contour_v_index + 0] = dyntri.tri_coord3.x;
	dyn_contour_vertices[dyn_contour_v_index + 1] = dyntri.tri_coord3.y;

	// Point color
	dyn_contour_vertices[dyn_contour_v_index + 2] = dyntri.tri_pt3_color[dyn_index].x;
	dyn_contour_vertices[dyn_contour_v_index + 3] = dyntri.tri_pt3_color[dyn_index].y;
	dyn_contour_vertices[dyn_contour_v_index + 4] = dyntri.tri_pt3_color[dyn_index].z;

	// Iterate
	dyn_contour_v_index = dyn_contour_v_index + 5;
}

void dynamictricontour_list_store::get_contour_index_buffer(unsigned int* dyn_contour_vertex_indices, unsigned int& dyn_contour_i_index)
{
	//__________________________________________________________________________
	// Add the indices
	// Index 1
	dyn_contour_vertex_indices[dyn_contour_i_index] = dyn_contour_i_index;

	dyn_contour_i_index = dyn_contour_i_index + 1;

	// Index 2
	dyn_contour_vertex_indices[dyn_contour_i_index] = dyn_contour_i_index;

	dyn_contour_i_index = dyn_contour_i_index + 1;

	// Index 3
	dyn_contour_vertex_indices[dyn_contour_i_index] = dyn_contour_i_index;

	dyn_contour_i_index = dyn_contour_i_index + 1;

}
