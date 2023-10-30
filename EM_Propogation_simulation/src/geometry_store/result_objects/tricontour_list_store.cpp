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



		tri_contours.add_dyntricontour(dyntri.tri_id, tri_pt1, tri_pt2, tri_pt3,
			);
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
