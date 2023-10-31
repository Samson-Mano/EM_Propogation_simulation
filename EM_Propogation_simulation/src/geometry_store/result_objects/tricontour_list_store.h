#pragma once
#include "../fe_objects/nodes_list_store.h"
#include "../geometry_objects/dynamictricontour_list_store.h"

struct  tricontour_data
{
	int tri_id = 0; // ID of the triangle element
	node_store* nd1 = nullptr; // node 1
	node_store* nd2 = nullptr; // node 2
	node_store* nd3 = nullptr; // node 2

	// Node values at each time step 
	std::vector<double> nd1_values;
	std::vector<double> nd2_values;
	std::vector<double> nd3_values;

	// ratio with maximum and minimum = (nd_values - min)/ (max - min)
	std::vector<double> nd1_mag_ratio;
	std::vector<double> nd2_mag_ratio;
	std::vector<double> nd3_mag_ratio;

	// Nodal colors at each time step
	std::vector<glm::vec3> nd1_colors;
	std::vector<glm::vec3> nd2_colors;
	std::vector<glm::vec3> nd3_colors;
};


class tricontour_list_store
{
public:
	int tricontour_count = 0;
	std::unordered_map<int, tricontour_data> tricontourMap;


	tricontour_list_store();
	~tricontour_list_store();
	void init(geom_parameters* geom_param_ptr);
	void add_tricontour(int& tri_id, node_store* nd1, node_store* nd2, node_store* nd3,
		std::vector<double>& nd1_values, std::vector<double>& nd2_values, std::vector<double>& nd3_values,
		std::vector<double>& contour_timestep_max_mag, std::vector<double>& contour_timestep_min_mag);

	void clear_data();
	void set_buffer();
	void paint_tricontour(const int& dyn_index);
	void update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale);

private:
	geom_parameters* geom_param_ptr = nullptr;
	dynamictricontour_list_store tri_contours;

};
