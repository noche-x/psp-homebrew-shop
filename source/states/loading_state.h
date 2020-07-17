#pragma once

#include "../state.h"
#include <Graphics/2D/SpriteBase.h>
#include <Utilities/Timer.h>
#include <Graphics/UI/MarqueeText.h>

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

	int m_loading_text_alpha;
	int m_bar_scale;

	Stardust::Utilities::Timer* m_animation_timer;
	Stardust::Graphics::UI::UIText* loading_what_text;
	Stardust::Graphics::UI::UIText* input_text;
	Stardust::Graphics::Render2D::Sprite* bar_sprite;
	Stardust::Graphics::Render2D::Sprite* dark_bar_sprite;
};