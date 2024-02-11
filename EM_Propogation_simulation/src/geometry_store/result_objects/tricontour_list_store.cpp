#include "tricontour_list_store.h"

tricontour_list_store::tricontour_list_store()
{
	// Empty constructor
}

tricontour_list_store::~tricontour_list_store()
{
	// Empty destructor
}

void tricontour_list_store::init(geom_parameters* geom_param_ptr)
{
	// Set the geometry parameters
	this->geom_param_ptr = geom_param_ptr;

	// Set the geometry parameters for the triangle contours
	tri_contours.init(geom_param_ptr);

	// Clear the triangle contour data
	tricontour_count = 0;
	tricontourMap.clear();
}

void tricontour_list_store::add_tricontour(int& tri_id, node_store* nd1, node_store* nd2, node_store* nd3,
	std::vector<double>& nd1_values, std::vector<double>& nd2_values, std::vector<double>& nd3_values,
	std::vector<double>& contour_timestep_max_mag, std::vector<double>& contour_timestep_min_mag)
{
	// ratio with maximum and minimum = (nd_values - min)/ (max - min)
	std::vector<double> nd1_mag_ratio;
	std::vector<double> nd2_mag_ratio;
	std::vector<double> nd3_mag_ratio;

	// Time step count
	int t_step_count = static_cast<int>(contour_timestep_max_mag.size());

	for (int i = 0; i < t_step_count; i++)
	{
		double temp_nd1_mag = (nd1_values[i] - contour_timestep_min_mag[i]) / (contour_timestep_max_mag[i] - contour_timestep_min_mag[i]);
		double temp_nd2_mag = (nd2_values[i] - contour_timestep_min_mag[i]) / (contour_timestep_max_mag[i] - contour_timestep_min_mag[i]);
		double temp_nd3_mag = (nd3_values[i] - contour_timestep_min_mag[i]) / (contour_timestep_max_mag[i] - contour_timestep_min_mag[i]);

		// Add to the mag_ratio list
		nd1_mag_ratio.push_back(temp_nd1_mag);
		nd2_mag_ratio.push_back(temp_nd2_mag);
		nd3_mag_ratio.push_back(temp_nd3_mag);

	}

	tricontour_data temp_tricontour;
	temp_tricontour.tri_id = tri_id;
	temp_tricontour.nd1 = nd1;
	temp_tricontour.nd2 = nd2;
	temp_tricontour.nd3 = nd3;

	// Add the node values data
	temp_tricontour.nd1_values = nd1_values;
	temp_tricontour.nd2_values = nd2_values;
	temp_tricontour.nd3_values = nd3_values;

	// Add to the tri_contour mag data
	temp_tricontour.nd1_mag_ratio = nd1_mag_ratio;
	temp_tricontour.nd2_mag_ratio = nd2_mag_ratio;
	temp_tricontour.nd3_mag_ratio = nd3_mag_ratio;

	//_____________________________________________________________________

	// Insert to the tricontourMap
	tricontourMap.insert({ tri_id, temp_tricontour });
	tricontour_count++;
}

void tricontour_list_store::clear_data()
{
	// Clear all the data
	tri_contours.clear_dyntricontours();

	// Clear the trianlge contour data
	tricontour_count = 0;
	tricontourMap.clear();
}

void tricontour_list_store::set_buffer()
{
	// Clear all the contours
	tri_contours.clear_dyntricontours();

	//_____________________ Add the Dynamic Tri Contours
	for (auto& dyntri_m : tricontourMap)
	{
		tricontour_data dyntri = dyntri_m.second;

		// Get the node pt
		glm::vec2 tri_pt1 = dyntri.nd1->node_pt; // Pt1
		glm::vec2 tri_pt2 = dyntri.nd2->node_pt; // Pt2
		glm::vec2 tri_pt3 = dyntri.nd3->node_pt; // Pt3


		// Add to the contour list
		tri_contours.add_dyntricontour(dyntri.tri_id, tri_pt1, tri_pt2, tri_pt3,
			dyntri.nd1_mag_ratio, dyntri.nd2_mag_ratio, dyntri.nd3_mag_ratio);
	}

	// Set the buffer (Only the index buffer is set because its a dynamic paint)
	tri_contours.set_buffer();
}

void tricontour_list_store::paint_tricontour(const int& dyn_index)
{
	// Paint the contour
	tri_contours.paint_dyntricontour(dyn_index);
}

void tricontour_list_store::update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale)
{
	// Update the geometry matrices 
	tri_contours.update_opengl_uniforms(set_modelmatrix, set_pantranslation, set_zoomtranslation, set_transparency, set_deflscale);
}
