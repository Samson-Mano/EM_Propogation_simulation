#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sstream>
#include "geometry_buffers/font_atlas.h"
#include <chrono>

class Stopwatch
{
public:
	void reset_time();
	double current_elapsed() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime = std::chrono::high_resolution_clock::time_point();
	// std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
};

struct geom_color_theme
{
	glm::vec3 background_color = glm::vec3(0);
	glm::vec3 node_color = glm::vec3(0);
	glm::vec3 line_color = glm::vec3(0);
	glm::vec3 charge_path_color = glm::vec3(0);
	glm::vec3 triangle_color = glm::vec3(0);
	glm::vec3 triangle_boundary = glm::vec3(0);
	glm::vec3 triangle_node = glm::vec3(0);
	glm::vec3 constraint_color = glm::vec3(0);
	glm::vec3 load_color = glm::vec3(0);
	glm::vec3 ptmass_color = glm::vec3(0);
};


class geom_parameters
{
public:
	// Standard sizes
	const float font_size = static_cast<float>(16.0f * std::pow(10, -5));
	const float node_circle_radii = 0.005f;
	const float charge_circle_radii = 0.002f;

	// Precision for various values
	const int length_precision = 3;
	const int coord_precision = 3;
	const int load_precision = 2;
	const int inlcond_precision = 3;
	const int defl_precision = 6;

	// Triangle mesh shrunk factor
	const double traingle_shrunk_factor = 0.8;

	// File path
	std::filesystem::path resourcePath = "";

	// Window size
	int window_width = 0;
	int window_height = 0;

	glm::vec2 min_b = glm::vec2(0); // (min_x, min_y)
	glm::vec2 max_b = glm::vec2(0); // (max_x, max_y)
	glm::vec2 geom_bound = glm::vec2(0); // Bound magnitude
	glm::vec2 center = glm::vec2(0); // center of the geometry
	glm::mat4 modelMatrix = glm::mat4(0); // Geometry model matrix
	double geom_scale = 0.0; // Scale of the geometry
	double geom_transparency = 0.0; // Value to control the geometry transparency
	double normalized_defl_scale = 0.0f; // Value of deflection scale
	double defl_scale = 0.0f; // Value of deflection scale

	// Screen transformations
	glm::mat4 panTranslation = glm::mat4(0); // Pan translataion
	double zoom_scale = 0.0; // Zoom scale

	// Standard colors
	geom_color_theme geom_colors;

	font_atlas main_font;

	geom_parameters();
	~geom_parameters();
	void init();

	static glm::vec3 getContourColor(float value);
private:

};
