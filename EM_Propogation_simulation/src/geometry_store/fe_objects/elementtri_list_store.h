#pragma once
#include "nodes_list_store.h"
#include "../geometry_objects/line_list_store.h"
#include "../geometry_objects/tri_list_store.h"

struct elementtri_store
{
	int tri_id = 0; // ID of the triangle element
	node_store* nd1 = nullptr; // node 1
	node_store* nd2 = nullptr; // node 2
	node_store* nd3 = nullptr; // node 2
};


class elementtri_list_store
{
public:
	unsigned int elementtri_count = 0;
	std::unordered_map<int, elementtri_store> elementtriMap; // Create an unordered_map to store Triangles with ID as key

	elementtri_list_store();
	~elementtri_list_store();
	void init(geom_parameters* geom_param_ptr);
	void add_elementtriangle(int& tri_id, node_store* nd1, node_store* nd2, node_store* nd3);
	void set_buffer();
	void paint_elementtriangles();

	void update_geometry_matrices(bool set_modelmatrix, bool set_pantranslation, bool set_zoomtranslation, bool set_transparency, bool set_deflscale);

private:
	geom_parameters* geom_param_ptr = nullptr;
	line_list_store element_boundarylines;
	point_list_store element_boundarypts;
	tri_list_store element_tris;
};
