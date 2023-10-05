#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include "../geom_parameters.h"
#include "../geometry_buffers/gBuffers.h"
#include "../geometry_objects/point_list_store.h"
#include "../geometry_objects/line_list_store.h"

class charge_path_store
{
public:
	unsigned int path_point_count = 0;
	
	charge_path_store();
	~charge_path_store();
	void init(geom_parameters* geom_param_ptr);
	void add_path(std::vector<std::string> curve_paths);


private:
	geom_parameters* geom_param_ptr = nullptr;
	point_list_store path_points;
	line_list_store path_lines;

};