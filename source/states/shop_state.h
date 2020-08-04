#pragma once

#include "../state.h"
#include <GFX/2D/Sprite.h>
#include <Utilities/Timer.h>

using namespace Stardust;

struct app_data {
	std::string short_name;
	std::string long_name;
	GFX::Render2D::Sprite* sprite;
};

class shop_state : public state {
public:
	void init();
	void destroy();
	void update();
	void draw();
	bool should_change();
private:
	void load_showcase_data();
	
	bool m_should_change;

	int m_box_width;
	int m_box_height;
	
	int m_selection_index;
	int m_selection_index_s;
	bool m_selected;
	bool m_go_back;
	bool m_uninstall;

	int m_page;

	std::string m_description;

	GFX::Render2D::Sprite* box_sprite;
	GFX::Render2D::Sprite* bar_box_sprite;
	std::vector<app_data> json_vector;
};