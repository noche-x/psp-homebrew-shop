#include "splash_state.h"
#include <Utilities/Logger.h>

double easeInOutExpo(double t)
{
    if (t < 0.5)
    {
        return (pow(2, 16 * t) - 1) / 510;
    }
    else
    {
        return 1 - 0.5 * pow(2, -16 * (t - 0.5));
    }
}

void splash_state::init()
{
    m_should_change = false;
    
    stardust_logo_tex = Stardust::GFX::g_TextureManager->loadTex("assets/images/stardust_logo.png", GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, false);
    app_logo_tex = Stardust::GFX::g_TextureManager->loadTex("assets/images/app_logo.png", GFX_FILTER_NEAREST, GFX_FILTER_NEAREST, false);

    stardust_logo_sprite = new Stardust::GFX::Render2D::Sprite(stardust_logo_tex);
    app_logo_sprite = new Stardust::GFX::Render2D::Sprite(app_logo_tex);

    stardust_logo_sprite->setScale(2.f, 2.f);
    app_logo_sprite->setScale(2.f, 2.f);

    stardust_logo_sprite->setPosition(120, 68);
    app_logo_sprite->setPosition(120, 68);

    m_animation_timer = new Stardust::Utilities::Timer();
    m_animation_timer->reset();
}

void splash_state::destroy()
{
    Stardust::GFX::g_TextureManager->deleteTex(stardust_logo_tex);
    Stardust::GFX::g_TextureManager->deleteTex(app_logo_tex);

    delete stardust_logo_sprite;
    delete app_logo_sprite;
    delete m_animation_timer;
}

void splash_state::update()
{
    m_animation_timer->deltaTime();
    if (m_animation_timer->elapsed() > 8.f) {
        m_should_change = true;
    }
}

bool splash_state::should_change() {
    return m_should_change;
}

float alpha = 0.f;

void splash_state::draw()
{
    if (m_should_change)
        return;
        
    if (m_animation_timer->elapsed() > 0.f && m_animation_timer->elapsed() < 3.f)
    {
        alpha = easeInOutExpo(m_animation_timer->elapsed() / 3);
        stardust_logo_sprite->setColor(1.0f, 1.0f, 1.0f, alpha);
        stardust_logo_sprite->draw();
    }

    if (m_animation_timer->elapsed() > 3.f && m_animation_timer->elapsed() < 4.f)
    {
        alpha = 0.f;
    }

    if (m_animation_timer->elapsed() > 4.f && m_animation_timer->elapsed() < 6.1f)
    {
        alpha = easeInOutExpo((m_animation_timer->elapsed() - 4.1f) / 3);
        app_logo_sprite->setColor(1.0f, 1.0f, 1.0f, alpha);
        app_logo_sprite->draw();
    }
    if (m_animation_timer->elapsed() > 6.1f) {
        alpha = 1.f;
        app_logo_sprite->setColor(1.0f, 1.0f, 1.0f, alpha);
        app_logo_sprite->draw();
    }
}
