#pragma once

#include "../state.h"
#include <GFX/2D/Sprite.h>
#include <Utilities/Timer.h>

using namespace Stardust;

class shop_state : public state {
public:
	void init();
	void destroy();
	void update();
	void draw();
	bool should_change();
private:
	bool m_should_change;

	int m_box_width;
	int m_box_height;
	
	int m_selection_index;
	bool m_selected;
	bool m_go_back;

	int m_page;

	const char* m_description;

	GFX::Render2D::Sprite* box_sprite;
	GFX::Render2D::Sprite* bar_box_sprite;
	GFX::Render2D::Sprite* image_sprite;
	std::vector< std::pair<std::string, GFX::Render2D::Sprite*>> json_vector;
};