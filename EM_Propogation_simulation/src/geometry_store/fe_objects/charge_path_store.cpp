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
	path_tracks.init(geom_param_ptr);

	// Clear the charge path points
	is_pathset = false;
	path_point_count = 0;
	chargePathMap.clear();
}

void charge_path_store::add_path(std::vector<std::string> curve_paths, int path_type)
{
	// Check the input
	if (curve_paths.size() < 2)
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
			chargePathMap.push_back(temp_chargepath);
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
	this->charge_total_length = 0.0;
	this->cummulative_segment_length.clear(); // cummulative segment length
	this->segment_length.clear(); // clear the individual path
	double segment_length = 0.0;

	for (i = 1; i < path_points.point_count; i++)
	{
		glm::vec2 point2 = path_points.pointMap[i].point_loc;

		path_lines.add_line(line_id, point1, point2, glm::vec2(0), glm::vec2(0), temp_color, temp_color, false);
		line_id++;

		// Calculate the length
		segment_length = std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2));
		this->charge_total_length = this->charge_total_length + segment_length;
		this->segment_length.push_back(segment_length);

		// re initialize point
		point1 = point2;
	}

	if (path_type == 0)
	{
		// closed path so close the loop
		glm::vec2 point2 = path_points.pointMap[0].point_loc;
		path_lines.add_line(line_id, point1, point2, glm::vec2(0), glm::vec2(0), temp_color, temp_color, false);

		// Calculate the length
		segment_length = std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2));
		// this->charge_total_length = this->charge_total_length + std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2));
		// this->segment_length.push_back(segment_length);

	}

	// Assign Path type & charge oscillation
	this->path_type = path_type;
	// this->charge_oscillation_freq = charge_oscillation_freq;

	is_pathset = true;

	// Set the buffer
	set_buffer();
}

void charge_path_store::add_charge_oscillation(glm::vec2& charge_origin, std::vector<glm::vec2>& charge_path_pts)
{
	// Charge oscillation path
	path_tracks.clear_textures();

	// Add the path points to the texture
	int id = 0;
	double value = 10.0;
	path_tracks.add_texture(id, charge_origin, charge_path_pts, value);

}


void charge_path_store::set_buffer()
{
	// Set the buffers for the Model
	path_points.set_buffer();
	path_lines.set_buffer();
}

void charge_path_store::set_path_buffer()
{
	// Set the path buffer
	path_tracks.set_buffer();
}

glm::vec2 charge_path_store::get_charge_path_location_at_t(const double& param_t)
{
	// Charge path point
	double length_at_t = 0.0;
	
	//if (path_type == 0)
	//{
	//	// closed loop
	//	length_at_t = 0.5 * param_t * this->charge_total_length;
	//}
	//else
	//{
	//	// open curve
	//	length_at_t = param_t * this->charge_total_length;
	//}

	length_at_t = param_t * this->charge_total_length;

	int pt_count = static_cast<int>(chargePathMap.size()); // point count

	// Check the location at
	double cummulative_length = 0;
	int seg_index = 0; // index segment

	double segment_t_ratio = 0.0; // parameter t for the segment length

	for (double seg_l : segment_length)
	{
		// Check where the length at t lies in the cummulative length
		if (length_at_t >= cummulative_length && length_at_t <= (cummulative_length + seg_l))
		{
			// find the t ratio at this segment
			segment_t_ratio = (length_at_t - cummulative_length) / seg_l;
			break;
		}
		else
		{
			seg_index++;
			// sum the cummulative length
			cummulative_length = cummulative_length + seg_l;
		}
	}

	//--------------------------------------------------------------------------------------------
	int pt_index1 = seg_index; // point index 1
	int pt_index2 = 0;

	if ((seg_index+1) != pt_count)
	{
		pt_index2 = pt_index1 + 1;
	}


	// Point at t
	double pt_at_t_x = (chargePathMap[pt_index1].path_pts.x * (1 - segment_t_ratio)) + (chargePathMap[pt_index2].path_pts.x * segment_t_ratio);
	double pt_at_t_y = (chargePathMap[pt_index1].path_pts.y * (1 - segment_t_ratio)) + (chargePathMap[pt_index2].path_pts.y * segment_t_ratio);

	return glm::vec2(pt_at_t_x, pt_at_t_y);
}


void charge_path_store::paint_charge_path()
{
	// Paint the charge path
	path_points.paint_points();
	path_lines.paint_lines();
}

void charge_path_store::paint_charge_oscillation(const int& dyn_index)
{
	// Paint the charge oscillation
	path_tracks.paint_textures(dyn_index);
}

void charge_path_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update model openGL uniforms
	path_points.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
	path_lines.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
	path_tracks.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, false, set_deflscale);

}
