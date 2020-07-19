#pragma once

#include "../state.h"
#include <GFX/2D/Sprite.h>
#include <Utilities/Timer.h>

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
	
	unsigned int app_logo_tex, stardust_logo_tex;

	Stardust::GFX::Render2D::Sprite* stardust_logo_sprite;
	Stardust::GFX::Render2D::Sprite* app_logo_sprite;
};