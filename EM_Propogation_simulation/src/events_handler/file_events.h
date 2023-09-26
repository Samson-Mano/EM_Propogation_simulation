#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include "../geometry_store/geom_store.h"

enum menu_item
{
	import_varai2d,
	import_raw_data,
	import_dxf_data,
	export_raw_data
};

class file_events
{
public:
	file_events();
	~file_events();
	void filemenu_event(menu_item m_item, geom_store& geom);
private:
	std::string ShowOpenFileDialog();
	std::string ShowOpenFileDialog_dxf();
	std::string ShowSaveFileDialog();
	void import_varai2d_geometry(geom_store& geom);
	void export_rawdata_geometry(geom_store& geom);
	void import_rawdata_geometry(geom_store& geom);
	void import_dxfdata_geometry(geom_store& geom);
};