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

    stardust_logo_sprite = new Stardust::Graphics::Render2D::Sprite(Stardust::Graphics::TextureUtil::LoadTex("./assets/images/stardust_logo.png"));
    app_logo_sprite = new Stardust::Graphics::Render2D::Sprite(Stardust::Graphics::TextureUtil::LoadTex("./assets/images/app_logo.png"));
    stardust_logo_sprite->SetPosition(240, 136);
    app_logo_sprite->SetPosition(240, 136);

    m_animation_timer = new Stardust::Utilities::Timer();
    m_animation_timer->reset();
}

void splash_state::destroy()
{
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
        alpha = easeInOutExpo(m_animation_timer->elapsed() / 3) * 255;
        stardust_logo_sprite->Alpha(alpha);
        stardust_logo_sprite->Draw();
    }

    if (m_animation_timer->elapsed() > 3.f && m_animation_timer->elapsed() < 4.f)
    {
        alpha = 0.f;
    }

    if (m_animation_timer->elapsed() > 4.f && m_animation_timer->elapsed() < 6.1f)
    {
        alpha = easeInOutExpo((m_animation_timer->elapsed() - 4.1f) / 3) * 255;
        app_logo_sprite->Alpha(alpha);
        app_logo_sprite->Draw();
    }
    if (m_animation_timer->elapsed() > 6.1f) {
        alpha = 255.f;
        app_logo_sprite->Alpha(alpha);
        app_logo_sprite->Draw();
    }
}
