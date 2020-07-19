#include "shop_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Network/NetworkDriver.h>
#include <Core/Core.h>
#include <Utilities/Input.h>
#include <pspaudio.h>
#include <psppower.h>
#include <cmath>

using namespace Stardust;

float x_a, y_a;
void shop_state::init()
{
    m_should_change = false;

    x_a = 6.9f;
    y_a = 4.2f;
    
    m_box_width = 110;
    m_box_height = 67;

    m_selection_index = 0;

    names_vector.clear();
    
    box_sprite = new GFX::Render2D::Sprite(g::textures::dark_bar);
    bar_box_sprite = new GFX::Render2D::Sprite(g::textures::bar);

    box_sprite->setColor(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f);
    box_sprite->setScale((float)m_box_width / 16.0f, (float)m_box_height / 16.0f);
    
    bar_box_sprite->setScale(100, 2);
    //bar_box_sprite->setPosition((5 * 100) / 2, (5 * 2) / 4);
}

void shop_state::destroy()
{
}

void shop_state::update()
{
    //battery_text->setContent(scePow);   

    if (Utilities::KeyPressed(PSP_CTRL_UP)) {
        m_selection_index -= 4;
        if (m_selection_index < 0)
            m_selection_index = 0;
    } 
    else if (Utilities::KeyPressed(PSP_CTRL_DOWN)) {
        m_selection_index += 4;
        if (m_selection_index > 15)
            m_selection_index = 15;
    } 
    else if (Utilities::KeyPressed(PSP_CTRL_RIGHT)) {
        m_selection_index += 1;
        if (m_selection_index > 15)
            m_selection_index = 15;
    } 
    else if (Utilities::KeyPressed(PSP_CTRL_LEFT)) {
        m_selection_index -= 1;
        if (m_selection_index < 0)
            m_selection_index = 0;
    } 

    static bool once = true; 
    if (once) {
        once = false;
        
        Json::Value v = Utilities::JSON::openJSON("cache/apps.json");
        for (int i = 0; i < v["apps"].size(); i++)
        {
            if (v["apps"][i].isObject())
            {
                //names_vector.push_back(v["apps"][i]["name"].asString().c_str());
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
    box_sprite->setColor(140.0f / 255.0f, 140.0f / 255.0f, 140.0f / 255.0f, 1.0f);
    box_sprite->setScale((float)(m_box_width + 2) / 16.0f, (float)(m_box_height + 2) / 16.0f);

    int y = std::floor(m_selection_index / 4);
    int x = m_selection_index - y * 4;
    
    float x_padding = (x + 1) * 8.5f;
    float x_box_width_pos = x * m_box_width;
    float x_center_to_left_align = m_box_width / 2;
            
    float y_padding = (y + 1) * 6.5f;
    float y_box_width_pos = y * m_box_height;
    float y_center_to_left_align = m_box_height / 2;
    
    box_sprite->setPosition((x_box_width_pos + x_center_to_left_align + x_padding) / x_a, 5 + ((y_box_width_pos + y_center_to_left_align + y_padding) / y_a));

    box_sprite->draw();

    box_sprite->setScale((float)m_box_width / 16.0f, (float)m_box_height / 16.0f);
    box_sprite->setColor(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f);

    for (int y = 0; y < 4; y++) 
    {
        for (int x = 0; x < 4; x++)
        {   
            float x_padding = (x + 1) * 8.5f;
            float x_box_width_pos = x * m_box_width;
            float x_center_to_left_align = m_box_width / 2;
            
            float y_padding = (y + 1) * 6.5f;
            float y_box_width_pos = y * m_box_height;
            float y_center_to_left_align = m_box_height / 2;

            box_sprite->setPosition((x_box_width_pos + x_center_to_left_align + x_padding) / x_a, 5 + ((y_box_width_pos + y_center_to_left_align + y_padding) / y_a));
            box_sprite->draw();
        }  
    }    

    bar_box_sprite->draw();
    g::font_renderer->draw(std::to_string(x), {20, 20});
    g::font_renderer->draw(std::to_string(y), {45, 20});
}