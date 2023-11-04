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
	path_accls.init(geom_param_ptr);

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
	path_accls.clear_lines();

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
		this->charge_total_length = this->charge_total_length + segment_length;
		this->segment_length.push_back(segment_length);

		// Add the first point as end point to complete the loop
		path_points.add_point(path_points.pointMap[0].point_id, path_points.pointMap[0].point_loc, path_points.pointMap[0].point_offset, temp_color, false);
	}

	// Assign Path type & charge oscillation
	this->path_type = path_type;
	is_pathset = true;

	// Set the buffer
	set_buffer();
}

void charge_path_store::add_charge_oscillation(std::vector<glm::vec2>& charge_path_pts, std::vector<glm::vec2>& charge_velocity, std::vector<glm::vec2>& charge_acceleration)
{
	// Charge oscillation path
	path_tracks.clear_textures();

	// Add the values points, velocity & acceleration
	ch_location_at_t = charge_path_pts;
	ch_velocity_at_t = charge_velocity;
	ch_acceleration_at_t = charge_acceleration;

	// Add the path points to the texture
	int id = 0;
	double value = 10.0;
	path_tracks.add_texture(id, charge_path_pts, value);

	//_____________________ Add the Dynamic Vectors
	double max_accl = 0.0;
	for (auto& vec_m : charge_acceleration)
	{
		if (max_accl < std::abs(glm::length(vec_m)))
		{
			// Maximum acceleration
			max_accl = std::abs(glm::length(vec_m));
		}
	}


	std::vector<glm::vec2> accl_start_pt;
	std::vector<glm::vec2> accl_end_pt;
	std::vector<glm::vec3> accl_colors_list;

	id = 0;
	for (auto& vec_m : charge_acceleration)
	{
		// Add the start point
		accl_start_pt.push_back(charge_path_pts[id]);

		// Add the end point
		double defl_ratio = geom_param_ptr->defl_scale * (0.005f / geom_param_ptr->geom_scale);

		double mag_ratio = glm::length(vec_m) / max_accl;
		accl_end_pt.push_back(charge_path_pts[id] + static_cast<float>(1000 * mag_ratio) * glm::normalize(vec_m));

		// Color list
		accl_colors_list.push_back(geom_parameters::getContourColor(1.0 - mag_ratio));

		id++;
	}

	id = 0;
	//for (auto& vec_m : charge_acceleration)
	//{
		// Scale the vector values befor adding
	path_accls.add_line(id, accl_start_pt, accl_end_pt, accl_colors_list);

	//	id++;
	//}

	// Set the buffer (Only the index buffer is set because its a dynamic paint)


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
	path_accls.set_buffer();
}

std::pair<glm::vec2, glm::vec2>  charge_path_store::get_charge_path_location_at_t(const double& param_t)
{
	// Charge path point
	double length_at_t = 0.0;

	// Amount of length travelled based on param t (0 - 1)
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

	if ((seg_index + 1) != pt_count)
	{
		pt_index2 = pt_index1 + 1;
	}


	// Point at t
	double pt_at_t_x = (chargePathMap[pt_index1].path_pts.x * (1 - segment_t_ratio)) + (chargePathMap[pt_index2].path_pts.x * segment_t_ratio);
	double pt_at_t_y = (chargePathMap[pt_index1].path_pts.y * (1 - segment_t_ratio)) + (chargePathMap[pt_index2].path_pts.y * segment_t_ratio);


	// Make the pair (1 - Location of the charge, 2 - Direction vector of the charge)
	glm::vec2 location_at_t = glm::vec2(pt_at_t_x, pt_at_t_y); // Location

	// Find the direction vector from two point
	const double dt_at_t = 0.0001;
	double pt_at_t_x_dt1 = (chargePathMap[pt_index1].path_pts.x * (1 - (segment_t_ratio - dt_at_t))) + 
		(chargePathMap[pt_index2].path_pts.x * (segment_t_ratio - dt_at_t));
	double pt_at_t_y_dt1 = (chargePathMap[pt_index1].path_pts.y * (1 - (segment_t_ratio - dt_at_t))) +
		(chargePathMap[pt_index2].path_pts.y * (segment_t_ratio - dt_at_t));

	glm::vec2 location_at_t_dt1 = glm::vec2(pt_at_t_x_dt1, pt_at_t_y_dt1); // Location dt1

	double pt_at_t_x_dt2 = (chargePathMap[pt_index1].path_pts.x * (1 - (segment_t_ratio + dt_at_t))) +
		(chargePathMap[pt_index2].path_pts.x * (segment_t_ratio + dt_at_t));
	double pt_at_t_y_dt2 = (chargePathMap[pt_index1].path_pts.y * (1 - (segment_t_ratio + dt_at_t))) +
		(chargePathMap[pt_index2].path_pts.y * (segment_t_ratio + dt_at_t));

	glm::vec2 location_at_t_dt2 = glm::vec2(pt_at_t_x_dt2, pt_at_t_y_dt2); // Location dt2

	glm::vec2 direction_vector = location_at_t_dt2 - location_at_t_dt1;

	// Normalized the direction vector 
	glm::vec2 normalized_direction_vector = glm::normalize(direction_vector);


	return std::make_pair(location_at_t, normalized_direction_vector);
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
	path_accls.paint_lines(dyn_index);
}

void charge_path_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update model openGL uniforms
	path_points.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
	path_lines.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
	path_tracks.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, false, set_deflscale);
	path_accls.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, false, set_deflscale);
}
