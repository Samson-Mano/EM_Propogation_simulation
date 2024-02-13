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
	geom_colors.node_color = glm::vec3(0.54509f, 0.0f, 0.4f); // Dark Red
	geom_colors.line_color = glm::vec3(1.0f, 0.54901f, 0.6f); // Dark Orange
	geom_colors.constraint_color = glm::vec3(0.6f, 0.0f, 0.6f);
	geom_colors.load_color = glm::vec3(0.0f, 1.0f, 0.0f);
	geom_colors.ptmass_color = glm::vec3(0.0f, 1.0f, 0.0f);


	// Color of the charge path
	geom_colors.charge_path_color = glm::vec3(0.54f, 0.06f, 0.31f);

	// Traingle mesh
	geom_colors.triangle_color = glm::vec3(0.90196f, 0.90196f, 0.98039f); // Lavender
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



 Stopwatch_events::Stopwatch_events()
 {
	 // Empty constructor
 }

 Stopwatch_events::~Stopwatch_events()
 {
	 // Empty destructor
 }

 void Stopwatch_events::start()
 {
	 // Start of the stop watch
	 m_StartTime = std::chrono::high_resolution_clock::now();
	 m_bRunning = true;
 }

 void Stopwatch_events::stop()
 {
	 // Stop the watch
	 m_EndTime = std::chrono::high_resolution_clock::now();
	 m_bRunning = false;
 }

 double Stopwatch_events::elapsed()
 {
	 std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

	 if (m_bRunning == true)
	 {
		 // Stop watch is running (so no end time)
		 endTime = std::chrono::high_resolution_clock::now();
	 }
	 else
	 {
		 // Stop watch is stopped
		 endTime = m_EndTime;
	 }

	 // Returns the value in seconds
	 return std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count() / 1000000.0;
 }
