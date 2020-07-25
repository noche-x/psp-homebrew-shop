#pragma once

#include "../state.h"
#include <GFX/2D/Sprite.h>
#include <Utilities/Timer.h>

class loading_state : public state {
public:
	void init();
	void destroy();
	void update();
	void draw();
	bool should_change();
private:
    bool m_should_change;
    bool m_content_init;

	uint8_t m_loading_text_alpha;
	uint8_t m_bar_scale;

	Stardust::Utilities::Timer* m_animation_timer;
	Stardust::GFX::Render2D::Sprite* bar_sprite;
	Stardust::GFX::Render2D::Sprite* dark_bar_sprite;
};