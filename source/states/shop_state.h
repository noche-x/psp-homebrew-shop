#pragma once

#include "../state.h"
#include <Graphics/2D/SpriteBase.h>
#include <Utilities/Timer.h>
#include <Graphics/UI/MarqueeText.h>

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

	Graphics::Render2D::Sprite* box_sprite;
	Graphics::Render2D::Sprite* bar_box_sprite;
	Graphics::UI::UIText* app_name_text;
	std::vector<const char*> names_vector;
};