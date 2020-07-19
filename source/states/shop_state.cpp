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

    if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
        x_a += 0.1f;
    }
    if (Utilities::KeyPressed(PSP_CTRL_CIRCLE)) {
        y_a += 0.1f;
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

            //                                V                       vvvvvvvvv                       vvvvvv                   vv                                             vvvvvvv
            box_sprite->setPosition((((x + 1) / 8) + (x * m_box_width / 1.0625f) + (m_box_width / 2)) / 6.0f, (22 + ((y + 1) * 40) + (y * m_box_height) + (m_box_height / 2)) / 6.0f);
            box_sprite->setPosition((x_box_width_pos + x_center_to_left_align + x_padding) / x_a, 5 + ((y_box_width_pos + y_center_to_left_align + y_padding) / y_a));
            
            //box_sprite->setPosition((x_padding / 16 + x_box_width_pos + x_center_to_left_align) / 6.0f, (22 + y_padding / 20 + y_box_width_pos + y_center_to_left_align) / 6.0f);
            box_sprite->draw();
        }  
    }

    bar_box_sprite->draw();
}