#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include "../geom_parameters.h"
#include "../geometry_buffers/gBuffers.h"
#include "../geometry_objects/point_list_store.h"
#include "../geometry_objects/line_list_store.h"
#include "../geometry_objects/dynamic_texture_list_store.h"

struct charge_path_points
{
	int path_id = 0;
	glm::vec2 path_pts = glm::vec2(0);
};

class charge_path_store
{
public:
	unsigned int path_point_count = 0;
	std::vector<charge_path_points> chargePathMap; // Create an unordered_map to store nodes with ID as key
	int path_type = -1; // Path type is closed path or open path
	// double charge_oscillation_freq = 0;
	std::vector<double> cummulative_segment_length; // Individual path cummulative length
	std::vector<double> segment_length; // Individual path length
	double charge_total_length = 0.0;
	bool is_pathset = false;

	charge_path_store();
	~charge_path_store();
	void init(geom_parameters* geom_param_ptr);
	void add_path(std::vector<std::string> curve_paths,int path_type);
	void add_charge_oscillation(glm::vec2& charge_origin, std::vector<glm::vec2>& charge_path_pts);
	void set_buffer();
	void set_path_buffer();
	glm::vec2 get_charge_path_location_at_t(const double& param_t);
	void paint_charge_path();
	void paint_charge_oscillation(const int& dyn_index);
	void update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale);

private:
	geom_parameters* geom_param_ptr = nullptr;
	point_list_store path_points;
	line_list_store path_lines;
	dynamic_texture_list_store path_tracks;

};