#include "geom_store.h"

geom_store::geom_store()
{
	// Empty constructor
}

geom_store::~geom_store()
{
	// Empty Destructor
}

void geom_store::init(options_window* op_window,solver_window* sol_window, model_window* md_window, inlcondition_window* inl_window)
{
	// Initialize
	// Initialize the geometry parameters
	geom_param.init();

	is_geometry_set = false;
	// is_modal_analysis_complete = false;

	//// Initialize the model nodes and lines
	//model_nodes.init(&geom_param);
	//model_lineelements.init(&geom_param);
	//model_constarints.init(&geom_param);
	//model_loads.init(&geom_param);
	//model_ptmass.init(&geom_param);
	//model_inlcond.init(&geom_param);

	//// Initialize the modal analysis result nodes and lines
	//modal_results.clear_data();
	//modal_result_nodes.init(&geom_param);
	//modal_result_lineelements.init(&geom_param);

	//// Initialize the pulse analysis result nodes and lines
	//pulse_response_result.clear_results();
	//pulse_result_nodes.init(&geom_param);
	//pulse_result_lineelements.init(&geom_param);

	// Add the window pointers
	this->op_window = op_window;
	this->sol_window = sol_window;
}

void geom_store::fini()
{
	// Deinitialize
	is_geometry_set = false;

}



void geom_store::update_WindowDimension(const int& window_width, const int& window_height)
{
	// Update the window dimension
	this->geom_param.window_width = window_width;
	this->geom_param.window_height = window_height;

	if (is_geometry_set == true)
	{
		// Update the model matrix
		update_model_matrix();
		// !! Zoom to fit operation during window resize is handled in mouse event class !!
	}
}


void geom_store::update_model_matrix()
{
	// Set the model matrix for the model shader
	// Find the scale of the model (with 0.9 being the maximum used)
	int max_dim = geom_param.window_width > geom_param.window_height ? geom_param.window_width : geom_param.window_height;

	double normalized_screen_width = 1.8f * (static_cast<double>(geom_param.window_width) / static_cast<double>(max_dim));
	double normalized_screen_height = 1.8f * (static_cast<double>(geom_param.window_height) / static_cast<double>(max_dim));


	geom_param.geom_scale = std::min(normalized_screen_width / geom_param.geom_bound.x,
		normalized_screen_height / geom_param.geom_bound.y);

	// Translation
	glm::vec3 geom_translation = glm::vec3(-1.0f * (geom_param.max_b.x + geom_param.min_b.x) * 0.5f * geom_param.geom_scale,
		-1.0f * (geom_param.max_b.y + geom_param.min_b.y) * 0.5f * geom_param.geom_scale,
		0.0f);

	glm::mat4 g_transl = glm::translate(glm::mat4(1.0f), geom_translation);

	geom_param.modelMatrix = g_transl * glm::scale(glm::mat4(1.0f), glm::vec3(static_cast<float>(geom_param.geom_scale)));

	//// Update the model matrix
	//model_nodes.update_geometry_matrices(true, false, false, false, false);
	//model_lineelements.update_geometry_matrices(true, false, false, false, false);
	//model_constarints.update_geometry_matrices(true, false, false, false, false);
	//model_loads.update_geometry_matrices(true, false, false, false, false);
	//model_ptmass.update_geometry_matrices(true, false, false, false, false);
	//model_inlcond.update_geometry_matrices(true, false, false, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(true, false, false, false, false);
	//modal_result_nodes.update_geometry_matrices(true, false, false, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(true, false, false, false, false);
	//pulse_result_nodes.update_geometry_matrices(true, false, false, false, false);
}

void geom_store::update_model_zoomfit()
{
	if (is_geometry_set == false)
		return;

	// Set the pan translation matrix
	geom_param.panTranslation = glm::mat4(1.0f);

	// Set the zoom scale
	geom_param.zoom_scale = 1.0f;

	//// Update the zoom scale and pan translation
	//model_nodes.update_geometry_matrices(false, true, true, false, false);
	//model_lineelements.update_geometry_matrices(false, true, true, false, false);
	//model_constarints.update_geometry_matrices(false, true, true, false, false);
	//model_loads.update_geometry_matrices(false, true, true, false, false);
	//model_ptmass.update_geometry_matrices(false, true, true, false, false);
	//model_inlcond.update_geometry_matrices(false, true, true, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(false, true, true, false, false);
	//modal_result_nodes.update_geometry_matrices(false, true, true, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(false, true, true, false, false);
	//pulse_result_nodes.update_geometry_matrices(false, true, true, false, false);
}

void geom_store::update_model_pan(glm::vec2& transl)
{
	if (is_geometry_set == false)
		return;

	// Pan the geometry
	geom_param.panTranslation = glm::mat4(1.0f);

	geom_param.panTranslation[0][3] = -1.0f * transl.x;
	geom_param.panTranslation[1][3] = transl.y;

	//// Update the pan translation
	//model_nodes.update_geometry_matrices(false, true, false, false, false);
	//model_lineelements.update_geometry_matrices(false, true, false, false, false);
	//model_constarints.update_geometry_matrices(false, true, false, false, false);
	//model_loads.update_geometry_matrices(false, true, false, false, false);
	//model_ptmass.update_geometry_matrices(false, true, false, false, false);
	//model_inlcond.update_geometry_matrices(false, true, false, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(false, true, false, false, false);
	//modal_result_nodes.update_geometry_matrices(false, true, false, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(false, true, false, false, false);
	//pulse_result_nodes.update_geometry_matrices(false, true, false, false, false);
}

void geom_store::update_model_zoom(double& z_scale)
{
	if (is_geometry_set == false)
		return;

	// Zoom the geometry
	geom_param.zoom_scale = z_scale;

	//// Update the Zoom
	//model_nodes.update_geometry_matrices(false, false, true, false, false);
	//model_lineelements.update_geometry_matrices(false, false, true, false, false);
	//model_constarints.update_geometry_matrices(false, false, true, false, false);
	//model_loads.update_geometry_matrices(false, false, true, false, false);
	//model_ptmass.update_geometry_matrices(false, false, true, false, false);
	//model_inlcond.update_geometry_matrices(false, false, true, false, false);

	//// Update the modal analysis result matrix
	//modal_result_lineelements.update_geometry_matrices(false, false, true, false, false);
	//modal_result_nodes.update_geometry_matrices(false, false, true, false, false);

	//// Update the pulse analysis result matrix
	//pulse_result_lineelements.update_geometry_matrices(false, false, true, false, false);
	//pulse_result_nodes.update_geometry_matrices(false, false, true, false, false);
}

void geom_store::update_model_transperency(bool is_transparent)
{
	if (is_geometry_set == false)
		return;

	if (is_transparent == true)
	{
		// Set the transparency value
		geom_param.geom_transparency = 0.2f;
	}
	else
	{
		// remove transparency
		geom_param.geom_transparency = 1.0f;
	}

	//// Update the model transparency
	//model_nodes.update_geometry_matrices(false, false, false, true, false);
	//model_lineelements.update_geometry_matrices(false, false, false, true, false);
	//model_constarints.update_geometry_matrices(false, false, false, true, false);
	//model_loads.update_geometry_matrices(false, false, false, true, false);
	//model_ptmass.update_geometry_matrices(false, false, false, true, false);
	//model_inlcond.update_geometry_matrices(false, false, false, true, false);

}


void geom_store::paint_geometry()
{


}