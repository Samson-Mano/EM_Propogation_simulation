#include "charge_path_store.h"

charge_path_store::charge_path_store()
{
	// Empty constructor
}

charge_path_store::~charge_path_store()
{
	// Empty destructor
}

void charge_path_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Set the geometry parameters points and lines of the charge path
	path_points.init(geom_param_ptr);
	path_lines.init(geom_param_ptr);

	// Clear the charge path points
	is_pathset = false;
	path_point_count = 0;
	chargePathMap.clear();
}

void charge_path_store::add_path(std::vector<std::string> curve_paths, int path_type, double charge_oscillation_freq)
{
	// Check the input
	if (curve_paths.size() < 2 || charge_oscillation_freq < 0.001)
	{
		// Input error
		return;
	}

	int path_id = 0;

	// Clear the existing path
	path_point_count = 0;
	chargePathMap.clear();
	path_points.clear_points();
	path_lines.clear_lines();

	glm::vec3 temp_color = geom_param_ptr->geom_colors.charge_path_color;

	// Loop through all the path
	for (const auto& str_path : curve_paths)
	{
		std::istringstream iss(str_path);
		std::string id_str, x_str, y_str;

		// Split the line using the comma as the delimiter
		if (std::getline(iss, id_str, ',') &&
			std::getline(iss, x_str, ',') &&
			std::getline(iss, y_str))
		{

			// Convert the extracted strings to the appropriate data types
			int pt_id = std::stoi(id_str);
			double scaled_x = std::stod(x_str);
			double scaled_y = std::stod(y_str);

			// Add the charge path point to the list
			charge_path_points temp_chargepath;
			temp_chargepath.path_id = pt_id;
			temp_chargepath.path_pts = glm::vec2(scaled_x, scaled_y);

			// Insert to the points to the charge path map
			chargePathMap.insert({ pt_id, temp_chargepath });
			path_point_count++;



			//__________________________ Add the path points
			glm::vec2 path_pt_offset = glm::vec2(0);

			path_points.add_point(pt_id, temp_chargepath.path_pts, path_pt_offset, temp_color, false);
		}
	}

	// ---------------- Add the path lines
	int i = 0;
	int line_id = 0;
	glm::vec2 point1 = path_points.pointMap[0].point_loc;

	for (i = 1; i < path_points.point_count; i++)
	{
		glm::vec2 point2 = path_points.pointMap[i].point_loc;

		path_lines.add_line(line_id, point1, point2, glm::vec2(0), glm::vec2(0), temp_color, temp_color, false);
		line_id++;

		// re initialize point
		point1 = point2;
	}

	if (path_type == 0)
	{
		// closed path so close the loop
		glm::vec2 point2 = path_points.pointMap[0].point_loc;
		path_lines.add_line(line_id, point1, point2, glm::vec2(0), glm::vec2(0), temp_color, temp_color, false);
	}

	is_pathset = true;

	// Set the buffer
	set_buffer();
}

void charge_path_store::set_buffer()
{
	// Set the buffers for the Model
	path_points.set_buffer();
	path_lines.set_buffer();
}

void charge_path_store::paint_charge_path()
{
	// Paint the charge path
	path_points.paint_points();
	path_lines.paint_lines();
}

void charge_path_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update model openGL uniforms
	path_points.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
	path_lines.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);

}
