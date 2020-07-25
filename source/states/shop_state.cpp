#include "shop_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Network/NetworkDriver.h>
#include <Core/Core.h>
#include <Utilities/Input.h>
#include <cmath>
#include <psppower.h>

using namespace Stardust;

void shop_state::init()
{
    m_should_change = false;
    
    m_box_width = 110;
    m_box_height = 67;

    m_selection_index = 0;

    m_page = 0;
    m_description = "";

    json_vector.clear();
    
    box_sprite = new GFX::Render2D::Sprite(g::textures::bar);
    bar_box_sprite = new GFX::Render2D::Sprite(g::textures::bar);

    box_sprite->setColor(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f);
    box_sprite->setScale((float)m_box_width / 16.0f, (float)m_box_height / 16.0f);
    
    unsigned int sex = GFX::g_TextureManager->loadTex("cache/mcpsp/icon512.png", GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, false);
    image_sprite = new GFX::Render2D::Sprite(sex, {0, 0}, {512, 291});

    image_sprite->setScale((float)m_box_width / 512.0f, (float)m_box_height / 291.0f);
    bar_box_sprite->setScale(100, 2.2f);

    bar_box_sprite->setColor(64 / 255.f, 123 / 255.f, 181 / 255.f, 1.f);
    box_sprite->setColor(66 / 255.f, 66 / 255.f, 66 / 255.f, 1.f);
    //bar_box_sprite->setPosition((5 * 100) / 2, (5 * 2) / 4);
}

void shop_state::destroy()
{
}

void shop_state::update()
{
    if (m_page == 0) {
        if (Utilities::KeyPressed(PSP_CTRL_UP))
        {
            m_selection_index -= 4;
            if (m_selection_index < 0)
                m_selection_index = 0;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_DOWN))
        {
            m_selection_index += 4;
            if (m_selection_index > json_vector.size() - 1)
                m_selection_index = json_vector.size() - 1;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_RIGHT))
        {
            m_selection_index += 1;
            if (m_selection_index > json_vector.size() - 1)
                m_selection_index = json_vector.size() - 1;
        }
        else if (Utilities::KeyPressed(PSP_CTRL_LEFT))
        {
            m_selection_index -= 1;
            if (m_selection_index < 0)
                m_selection_index = 0;
        }
    }
    if (Utilities::KeyPressed(PSP_CTRL_CROSS))
    {
        m_selected = true;
    }
    else if (Utilities::KeyPressed(PSP_CTRL_CIRCLE)) {
        m_go_back = true;
    } 

    if (m_selected) {
        m_selected = false;

        m_page = 1;

        Json::Value v = Utilities::JSON::openJSON("cache/apps.json");
        m_description = v["apps"][m_selection_index]["desc"].asString().c_str();
    }

    if (m_go_back) {
        m_go_back = false;

        m_page -= 1;
        if (m_page < 0)
            m_page = 0;
    }

    static bool once = true; 
    if (once) {
        once = false;
        
        Json::Value v = Utilities::JSON::openJSON("cache/apps.json");
        for (int i = 0; i < v["apps"].size(); i++)
        {
            if (v["apps"][i].isObject())
            {
                unsigned int sextillion = GFX::g_TextureManager->loadTex(v["apps"][i]["large"].asString(), GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, false);
                GFX::Render2D::Sprite* temp_sprite = new GFX::Render2D::Sprite(sextillion, {0, 0}, {480, 272});
                temp_sprite->setScale((float)m_box_width / 480.0f, (float)m_box_height / 272.0f);
                
                std::pair<std::string, GFX::Render2D::Sprite*> p_temp(v["apps"][i]["name"].asString(), temp_sprite);
                json_vector.push_back(p_temp);
            }
        }
    }
}

bool shop_state::should_change()
{
    return m_should_change;
}

void shop_state::draw()
{
    if (m_page == 0) {
        // x, y for the selection box
        int y = m_selection_index / 4;
        int x = m_selection_index - y * 4;

        // for maxing alpha of the selection box when switching
        static int prev_x = x;
        static int prev_y = y;
        static int select_box_alpha = 255;

        // flashing
        if (select_box_alpha > 0.f)
        {
            select_box_alpha -= 4;
        }
        else if (select_box_alpha <= 0.f)
        {
            select_box_alpha = 255;
        }

        // max alpha when switching
        if (prev_x != x || prev_y != y)
        {
            select_box_alpha = 255;
            prev_x = x;
            prev_y = y;
        }
        else
        {
            prev_x = x;
            prev_y = y;
        }

        // set the color of the box white, change alpha, scale it
        box_sprite->setColor(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, select_box_alpha / 255.f);
        box_sprite->setScale((float)(m_box_width + 6) / 16.0f, (float)(m_box_height + 6) / 16.0f);

        // calculations for the selection box
        float x_padding = (x + 1) * 8.5f;
        float x_box_width_pos = x * m_box_width;
        float x_center_to_left_align = m_box_width / 2;

        float y_padding = (y + 1) * 6.5f;
        float y_name_padding = y * 8.f;
        float y_box_width_pos = y * m_box_height;
        float y_center_to_left_align = m_box_height / 2;

        // set the selection box position to be on the selection index
        box_sprite->setPosition(x_box_width_pos + x_center_to_left_align + x_padding, 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding));

        box_sprite->draw();

        // make fonts 0.5f size
        g::font_renderer->setStyle({255, 255, 255, 255, 0.5f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});

        int y_size = json_vector.size() / 4;
        for (int y = 0; y <= y_size; y++)
        {
            int x_size = (y < y_size) ? 4 : json_vector.size() - y * 4;
            for (int x = 0; x < x_size; x++)
            {
                float x_padding = (x + 1) * 8.5f;
                float x_box_width_pos = x * m_box_width;
                float x_center_to_left_align = m_box_width / 2;

                float y_padding = (y + 1) * 6.5f;
                float y_name_padding = y * 8.f;
                float y_box_width_pos = y * m_box_height;
                float y_center_to_left_align = m_box_height / 2;

                json_vector.at(y * 4 + x).second->setScale((float)m_box_width / 480.0f, (float)m_box_height / 272.0f);
                json_vector.at(y * 4 + x).second->setPosition(x_box_width_pos + x_center_to_left_align + x_padding, 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding));
                json_vector.at(y * 4 + x).second->draw();

                g::font_renderer->draw(json_vector.at(y * 4 + x).first, {x_box_width_pos + x_center_to_left_align + x_padding, 40 + 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding)});
            }
        }

        // return style back to normal
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});
    }
    else if (m_page == 1) {
        float x_padding = 1 * 8.5f;
        float x_box_width_pos = 0 * m_box_width * 2;
        float x_center_to_left_align = m_box_width;

        float y_padding = 1 * 6.5f;
        float y_name_padding = 0 * 8.f;
        float y_box_width_pos = 0 * m_box_height * 2;
        float y_center_to_left_align = m_box_height;

        json_vector.at(m_selection_index).second->setScale((float)m_box_width / 240.0f, (float)m_box_height / 136.0f);
        json_vector.at(m_selection_index).second->setPosition(x_box_width_pos + x_center_to_left_align + x_padding, 18 + y_name_padding + (y_box_width_pos + y_center_to_left_align + y_padding));
        json_vector.at(m_selection_index).second->draw();

        g::font_renderer->setStyle({255, 255, 255, 255, 0.4f, TEXT_RENDERER_LEFT, TEXT_RENDERER_LEFT, 0, false});
        g::font_renderer->draw(m_description, {x_box_width_pos * 2 + x_center_to_left_align + x_padding, 18 + y_name_padding + (y_box_width_pos + y_padding)});
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});
    }
    {
        // status bar
        bar_box_sprite->draw();

        // battery life calculation
        int life = scePowerGetBatteryLifeTime();
        int h = (life / 100) + 2;
        int m = (life / 10);
        std::string battery_life = "";
        if ((life / 100) != 0)
            battery_life = std::to_string(h) + "h" + std::to_string(m) + "m";
        else if ((life / 100) != 0)
            battery_life = "full";

        // get clock
        char time_text[25];
        pspTime time;
        sceRtcGetCurrentClockLocalTime(&time);

        // set clock text
        int hour = time.hour % 12;
        int am_pm = time.hour / 12;
        snprintf(time_text, 25, "%2i:%02i %s", (hour == 0) ? 12 : hour, time.minutes, am_pm ? "pm" : "am");

        // render time
        g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, TEXT_RENDERER_RIGHT, TEXT_RENDERER_LEFT, 0, false});
        g::font_renderer->draw(time_text, {5, 10});
        
        // render charging indicator
        g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, TEXT_RENDERER_RIGHT, TEXT_RENDERER_RIGHT, 0, false});
        g::font_renderer->draw((scePowerIsBatteryCharging() ? "charging " : "") + battery_life + " " + std::to_string(scePowerGetBatteryLifePercent()) + "%", {475, 10});
        
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, false});
    }
}