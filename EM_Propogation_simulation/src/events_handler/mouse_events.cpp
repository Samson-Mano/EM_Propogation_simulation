#include "mouse_events.h"

mouse_events::mouse_events()
{
	// Empty constructor
}

mouse_events::~mouse_events()
{
	// Empty destructor
}

void mouse_events::init(geom_store* geom)
{
	// Intialize the geometry and tool window pointers
	this->geom = geom;

	// Tool windows

}

void mouse_events::mouse_location(glm::vec2& loc)
{
	// Copy the current mouse location only when for pan or zoom operation
	if (is_pan == true)
	{
		// Pan operation in progress
		glm::vec2 delta_d = click_pt - loc;
		// pan
		// std::cout << "Pan translation "<<delta_d.x<<", " << delta_d.y << std::endl;
		glm::vec2 current_translataion = delta_d / (std::max((geom->geom_param.window_width), (geom->geom_param.window_height)) * 0.5f);
		pan_operation(current_translataion);
	}
}

void mouse_events::pan_operation_start(glm::vec2& loc)
{
	// Pan operation start
	is_pan = true;
	// Note the click point when the pan operation start
	click_pt = loc;
	//std::cout << "Pan Operation Start" << std::endl;
}

void mouse_events::pan_operation(glm::vec2& current_translataion)
{
	// Pan operation in progress
	total_translation = (prev_translation + current_translataion);

	geom->update_model_pan(total_translation);
}

void mouse_events::pan_operation_ends()
{
	// Pan operation complete
	prev_translation = total_translation;
	is_pan = false;
	//std::cout << "Pan Operation End" << std::endl;
}

void mouse_events::zoom_operation(double& e_delta, glm::vec2& loc)
{
	// Screen point before zoom
	glm::vec2 screen_pt_b4_scale = intellizoom_normalized_screen_pt(loc);

	// Zoom operation
	if ((e_delta) > 0)
	{
		// Scroll Up
		if (zoom_val < 1000)
		{
			zoom_val = zoom_val + 0.1f;
		}
	}
	else if ((e_delta) < 0)
	{
		// Scroll Down
		if (zoom_val > 0.101)
		{
			zoom_val = zoom_val - 0.1f;
		}
	}

	// Hypothetical Screen point after zoom
	glm::vec2 screen_pt_a4_scale = intellizoom_normalized_screen_pt(loc);
	glm::vec2 g_tranl = -0.5f * static_cast<float>(zoom_val) * (screen_pt_b4_scale - screen_pt_a4_scale);

	// Set the zoom
	geom->update_model_zoom(zoom_val);

	// Perform Translation for Intelli Zoom
	pan_operation(g_tranl);
	pan_operation_ends();
}

void mouse_events::zoom_to_fit()
{
	// Zoom to fit the model
	prev_translation = glm::vec2(0);
	zoom_val = 1.0f;
	geom->update_model_zoomfit();
	// std::cout << "Zoom val: " << zoom_val << std::endl;
}

void mouse_events::left_mouse_click(glm::vec2& loc)
{
	// Left mouse single click
	

	// std::cout << "Left mouse single click" << std::endl;
}

void mouse_events::left_mouse_doubleclick(glm::vec2& loc)
{
	// Left mouse double click
	

	// std::cout << "Left mouse double click" << std::endl;
}

void mouse_events::right_mouse_click(glm::vec2& loc)
{
	// Right mouse single click


	// std::cout << "Right mouse single click" << std::endl;
}

void mouse_events::right_mouse_doubleclick(glm::vec2& loc)
{
	// Right mouse double click
// std::cout << "Right mouse double click" << std::endl;
}

glm::vec2 mouse_events::intellizoom_normalized_screen_pt(glm::vec2 loc)
{
	// Function returns normalized screen point for zoom operation
	glm::vec2 mid_pt = glm::vec2((geom->geom_param.window_width), (geom->geom_param.window_height)) * 0.5f;
	int min_size = std::min((geom->geom_param.window_width), (geom->geom_param.window_height));

	glm::vec2 mouse_pt = (-1.0f * (loc - mid_pt)) / (static_cast<float>(min_size) * 0.5f);

	return (mouse_pt - (2.0f * prev_translation)) / static_cast<float>(zoom_val);
}
