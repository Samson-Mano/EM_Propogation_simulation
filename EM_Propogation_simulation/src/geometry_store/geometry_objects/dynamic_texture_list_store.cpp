#include "dynamic_texture_list_store.h"

dynamic_texture_list_store::dynamic_texture_list_store()
{
	// Empty constructor
}

dynamic_texture_list_store::~dynamic_texture_list_store()
{
	// Empty destructor
}

void dynamic_texture_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Create the shader and Texture for the drawing the constraints
	std::filesystem::path shadersPath = geom_param_ptr->resourcePath;

	dyn_texture_shader.create_shader((shadersPath.string() + "/resources/shaders/dyntexture_vert_shader.vert").c_str(),
		(shadersPath.string() + "/resources/shaders/dyntexture_frag_shader.frag").c_str());

	// Set texture uniform
	dyn_texture_shader.setUniform("u_Texture", 0);

	// Load the texture
	dyn_tx_texture.LoadTexture((shadersPath.string() + "/resources/images/pic_3D_circle.png").c_str());

	// Clear the data
	dyn_texture_count = 0;
	max_texture_value = 0.0;
	dyn_textureMap.clear();
}

void dynamic_texture_list_store::add_texture(int& dyn_texture_id, std::vector<glm::vec2>& dyn_texture_loc,double& texture_value)
{
	dynamic_texture_store dyn_temp_tx;
	dyn_temp_tx.dyn_texture_id = dyn_texture_id;
	dyn_temp_tx.dyn_texture_loc = dyn_texture_loc; // Dynamic point location
	dyn_temp_tx.texture_value = texture_value; // Texture value

	if (std::abs(texture_value) > max_texture_value)
	{
		// Fix the maximum texture value
		this->max_texture_value = std::abs(texture_value);
	}

	//___________________________________________________________________
	// Add to the list
	dyn_textureMap.push_back(dyn_temp_tx);

	// Iterate the texture count
	dyn_texture_count++;
}

void dynamic_texture_list_store::set_buffer()
{
	// Set the buffer for texture
	if (dyn_texture_count == 0)
	{
		// No texture to paint
		return;
	}


	unsigned int texture_indices_count = 6 * dyn_texture_count;
	unsigned int* texture_vertex_indices = new unsigned int[texture_indices_count];

	unsigned int texture_i_index = 0;

	// Set the texture index
	for (auto& dyn_tx : dyn_textureMap)
	{
		// Add the texture index buffer
		get_texture_index_buffer(texture_vertex_indices, texture_i_index);
	}

	VertexBufferLayout texture_layout;
	texture_layout.AddFloat(2);  // Position
	texture_layout.AddFloat(2);  // Center
	texture_layout.AddFloat(3);  // Color
	texture_layout.AddFloat(2);  // Texture co-ordinate

	// Define the texture vertices of the model for texture (2 position, 2 center, 3 color & 2 texture coord) 
	unsigned int texture_vertex_count = 4 * 9 * dyn_texture_count;
	unsigned int texture_vertex_size = texture_vertex_count * sizeof(float);

	// Allocate space for vertex buffers
	dyn_texture_buffer.CreateDynamicBuffers(texture_vertex_size, texture_vertex_indices,
		texture_indices_count, texture_layout);

	// Delete the Dynamic arrays
	delete[] texture_vertex_indices;

}

void dynamic_texture_list_store::paint_textures(const int& dyn_index)
{
	// Paint all the textures
	dyn_tx_texture.Bind();
	dyn_texture_shader.Bind();
	dyn_texture_buffer.Bind();

	// Update the point buffer data for dynamic drawing
	update_buffer(dyn_index);

	glDrawElements(GL_TRIANGLES, 6 * dyn_texture_count, GL_UNSIGNED_INT, 0);

	dyn_texture_buffer.UnBind();
	dyn_texture_shader.UnBind();
	dyn_tx_texture.UnBind();
}

void dynamic_texture_list_store::update_buffer(const int& dyn_index)
{
	// Define the texture vertices of the model for texture (2 position, 2 center, 3 color & 2 texture coord) 
	const unsigned int texture_vertex_count = 4 * 9 * dyn_texture_count;
	float* texture_vertices = new float[texture_vertex_count];

	unsigned int texture_v_index = 0;

	// Set the texture vertices
	for (auto& dyn_tx : dyn_textureMap)
	{
		// Add texture buffers
		get_texture_vertex_buffer(dyn_tx, dyn_index, texture_vertices, texture_v_index);
	}

	unsigned int texture_vertex_size = texture_vertex_count * sizeof(float); // Size of the texture vertex

	// Update the buffer
	dyn_texture_buffer.UpdateDynamicVertexBuffer(texture_vertices, texture_vertex_size);

	// Delete the dynamic array
	delete[] texture_vertices;
}

void dynamic_texture_list_store::clear_textures()
{
	// Delete all the textures
	dyn_texture_count = 0;
	max_texture_value = 0.0;
	dyn_textureMap.clear();
}

void dynamic_texture_list_store::update_opengl_uniforms(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	if (set_modelmatrix == true)
	{
		// set the model matrix
		dyn_texture_shader.setUniform("geom_scale", static_cast<float>(geom_param_ptr->geom_scale));
		dyn_texture_shader.setUniform("transparency", 1.0f);

		dyn_texture_shader.setUniform("modelMatrix", geom_param_ptr->modelMatrix, false);
	}

	if (set_pantranslation == true)
	{
		// set the pan translation
		dyn_texture_shader.setUniform("panTranslation", geom_param_ptr->panTranslation, false);
	}

	if (set_zoomtranslation == true)
	{
		// set the zoom translation
		dyn_texture_shader.setUniform("zoomscale", static_cast<float>(geom_param_ptr->zoom_scale));
	}

	if (set_transparency == true)
	{
		// set the alpha transparency
		dyn_texture_shader.setUniform("transparency", static_cast<float>(geom_param_ptr->geom_transparency));
	}

	if (set_deflscale == true)
	{
		// set the deflection scale
		dyn_texture_shader.setUniform("normalized_deflscale", static_cast<float>(geom_param_ptr->normalized_defl_scale));
		dyn_texture_shader.setUniform("deflscale", static_cast<float>(geom_param_ptr->defl_scale));
	}
}

void dynamic_texture_list_store::get_texture_vertex_buffer(dynamic_texture_store& dyn_tx, const int& dyn_index, float* dyn_point_vertices, unsigned int& dyn_point_v_index)
{
	// Constraint color
	glm::vec3 tx_color = geom_param_ptr->geom_colors.ptmass_color;
	float corner_size = static_cast<float>(-3.4 * (dyn_tx.texture_value / max_texture_value) * (geom_param_ptr->charge_circle_radii / geom_param_ptr->geom_scale));

	// Set the Point mass vertices Corner 1 Top Left
	dyn_point_vertices[dyn_point_v_index + 0] = dyn_tx.dyn_texture_loc[dyn_index].x - corner_size;
	dyn_point_vertices[dyn_point_v_index + 1] = dyn_tx.dyn_texture_loc[dyn_index].y + corner_size;

	// Set the node center
	dyn_point_vertices[dyn_point_v_index + 2] = dyn_tx.dyn_texture_loc[dyn_index].x;
	dyn_point_vertices[dyn_point_v_index + 3] = dyn_tx.dyn_texture_loc[dyn_index].y;

	// Set the node color
	dyn_point_vertices[dyn_point_v_index + 4] = tx_color.x;
	dyn_point_vertices[dyn_point_v_index + 5] = tx_color.y;
	dyn_point_vertices[dyn_point_v_index + 6] = tx_color.z;

	// Set the Texture co-ordinates
	dyn_point_vertices[dyn_point_v_index + 7] = 0.0;
	dyn_point_vertices[dyn_point_v_index + 8] = 0.0;


	// Increment
	dyn_point_v_index = dyn_point_v_index + 9;


	// Set the Point mass vertices Corner 2 Top Right
	dyn_point_vertices[dyn_point_v_index + 0] = dyn_tx.dyn_texture_loc[dyn_index].x + corner_size;
	dyn_point_vertices[dyn_point_v_index + 1] = dyn_tx.dyn_texture_loc[dyn_index].y + corner_size;

	// Set the node center
	dyn_point_vertices[dyn_point_v_index + 2] = dyn_tx.dyn_texture_loc[dyn_index].x;
	dyn_point_vertices[dyn_point_v_index + 3] = dyn_tx.dyn_texture_loc[dyn_index].y;

	// Set the node color
	dyn_point_vertices[dyn_point_v_index + 4] = tx_color.x;
	dyn_point_vertices[dyn_point_v_index + 5] = tx_color.y;
	dyn_point_vertices[dyn_point_v_index + 6] = tx_color.z;

	// Set the Texture co-ordinates
	dyn_point_vertices[dyn_point_v_index + 7] = 1.0;
	dyn_point_vertices[dyn_point_v_index + 8] = 0.0;

	// Increment
	dyn_point_v_index = dyn_point_v_index + 9;


	// Set the Point Mass vertices Corner 3 Bot Right
	dyn_point_vertices[dyn_point_v_index + 0] = dyn_tx.dyn_texture_loc[dyn_index].x + corner_size;
	dyn_point_vertices[dyn_point_v_index + 1] = dyn_tx.dyn_texture_loc[dyn_index].y - corner_size;

	// Set the node center
	dyn_point_vertices[dyn_point_v_index + 2] = dyn_tx.dyn_texture_loc[dyn_index].x;
	dyn_point_vertices[dyn_point_v_index + 3] = dyn_tx.dyn_texture_loc[dyn_index].y;

	// Set the node color
	dyn_point_vertices[dyn_point_v_index + 4] = tx_color.x;
	dyn_point_vertices[dyn_point_v_index + 5] = tx_color.y;
	dyn_point_vertices[dyn_point_v_index + 6] = tx_color.z;

	// Set the Texture co-ordinates
	dyn_point_vertices[dyn_point_v_index + 7] = 1.0;
	dyn_point_vertices[dyn_point_v_index + 8] = 1.0;


	// Increment
	dyn_point_v_index = dyn_point_v_index + 9;


	// Set the Constraint vertices Corner 4 Bot Left
	dyn_point_vertices[dyn_point_v_index + 0] = dyn_tx.dyn_texture_loc[dyn_index].x - corner_size;
	dyn_point_vertices[dyn_point_v_index + 1] = dyn_tx.dyn_texture_loc[dyn_index].y - corner_size;

	// Set the node center
	dyn_point_vertices[dyn_point_v_index + 2] = dyn_tx.dyn_texture_loc[dyn_index].x;
	dyn_point_vertices[dyn_point_v_index + 3] = dyn_tx.dyn_texture_loc[dyn_index].y;

	// Set the node color
	dyn_point_vertices[dyn_point_v_index + 4] = tx_color.x;
	dyn_point_vertices[dyn_point_v_index + 5] = tx_color.y;
	dyn_point_vertices[dyn_point_v_index + 6] = tx_color.z;

	// Set the Texture co-ordinates
	dyn_point_vertices[dyn_point_v_index + 7] = 0.0;
	dyn_point_vertices[dyn_point_v_index + 8] = 1.0;

	// Increment
	dyn_point_v_index = dyn_point_v_index + 9;

}

void dynamic_texture_list_store::get_texture_index_buffer(unsigned int* dyn_point_indices, unsigned int& dyn_point_i_index)
{
	// Set the Quad indices
	unsigned int t_id = ((dyn_point_i_index / 6) * 4);

	// Triangle 0,1,2
	dyn_point_indices[dyn_point_i_index + 0] = t_id + 0;
	dyn_point_indices[dyn_point_i_index + 1] = t_id + 1;
	dyn_point_indices[dyn_point_i_index + 2] = t_id + 2;

	// Triangle 2,3,0
	dyn_point_indices[dyn_point_i_index + 3] = t_id + 2;
	dyn_point_indices[dyn_point_i_index + 4] = t_id + 3;
	dyn_point_indices[dyn_point_i_index + 5] = t_id + 0;

	// Increment
	dyn_point_i_index = dyn_point_i_index + 6;
}
