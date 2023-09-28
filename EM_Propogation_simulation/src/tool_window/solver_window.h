#pragma once

class solver_window
{
public:
	bool is_show_window = false;
	bool show_undeformed_model = true;

	solver_window();
	~solver_window();
	void init();
	void render_window();
private:

};
