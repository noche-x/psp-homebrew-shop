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

	GFX::Render2D::Sprite* box_sprite;
	GFX::Render2D::Sprite* bar_box_sprite;
	std::vector<const char*> names_vector;
};