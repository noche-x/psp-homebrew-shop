#pragma once

#include "../state.h"
#include <Graphics/2D/SpriteBase.h>
#include <Utilities/Timer.h>
#include <Graphics/UI/MarqueeText.h>

class shop_state : public state {
public:
	void init();
	void destroy();
	void update();
	void draw();
	bool should_change();
private:
    bool m_should_change;
};