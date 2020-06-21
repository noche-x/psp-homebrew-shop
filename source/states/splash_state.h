#pragma once

#include "../state.h"
#include <Graphics/2D/SpriteBase.h>
#include <Utilities/Timer.h>
#include <Graphics/UI/MarqueeText.h>

class splash_state : public state {
public:
	void init();
	void destroy();
	void update();
	void draw();
	bool should_change();
private:
    bool m_should_change;

	Stardust::Utilities::Timer* m_animation_timer;
	Stardust::Graphics::Render2D::Sprite* stardust_logo_sprite;
	Stardust::Graphics::Render2D::Sprite* app_logo_sprite;
};