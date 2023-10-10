#include "geom_parameters.h"

geom_parameters::geom_parameters()
{
	// Empty constructor
}

geom_parameters::~geom_parameters()
{
	// Empty Destructor
}

void geom_parameters::init()
{
	// Initialize the paramters
	resourcePath = std::filesystem::current_path();
	std::cout << "Current path of application is " << resourcePath << std::endl;

	// Create the font atlas
	main_font.create_atlas(resourcePath);

	// Initialize the color theme
	geom_colors.background_color = glm::vec3(0.62f, 0.62f, 0.62f);
	geom_colors.node_color = glm::vec3(0.0f, 0.0f, 0.4f);
	geom_colors.line_color = glm::vec3(0.0f, 0.2f, 0.6f);
	geom_colors.constraint_color = glm::vec3(0.6f, 0.0f, 0.6f);
	geom_colors.load_color = glm::vec3(0.0f, 1.0f, 0.0f);
	geom_colors.ptmass_color = glm::vec3(0.0f, 1.0f, 0.0f);


	// Color of the charge path
	geom_colors.charge_path_color = glm::vec3(0.54f, 0.06f, 0.31f);

	// Traingle mesh
	geom_colors.triangle_color = glm::vec3(0.82f, 0.77f, 0.92f);
	geom_colors.triangle_boundary = geom_colors.triangle_color*0.8f;
	geom_colors.triangle_node = geom_colors.triangle_color * 0.6f;
}


 glm::vec3 geom_parameters::getContourColor(float value)
{
	// return the contour color based on the value (0 to 1)
	glm::vec3 color;
	float r, g, b;

	// Rainbow color map
	float hue = value * 5.0f; // Scale the value to the range of 0 to 5
	float c = 1.0f;
	float x = c * (1.0f - glm::abs(glm::mod(hue / 2.0f, 1.0f) - 1.0f));
	float m = 0.0f;

	if (hue >= 0 && hue < 1) {
		r = c;
		g = x;
		b = m;
	}
	else if (hue >= 1 && hue < 2) {
		r = x;
		g = c;
		b = m;
	}
	else if (hue >= 2 && hue < 3) {
		r = m;
		g = c;
		b = x;
	}
	else if (hue >= 3 && hue < 4) {
		r = m;
		g = x;
		b = c;
	}
	else {
		r = x;
		g = m;
		b = c;
	}

	color = glm::vec3(r, g, b);
	return color;
}



 void Stopwatch::reset_time()
 {
	 m_startTime = std::chrono::high_resolution_clock::now();
 }

 double Stopwatch::current_elapsed() const
 {
	 return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_startTime).count() / 1000.0;
 }