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

void shop_state::init()
{
    m_should_change = false;

    m_box_width = 110;
    m_box_height = 67;

    m_selection_index = 0;

    names_vector.clear();

    
    
    box_sprite = new GFX::Render2D::Sprite(g::dark_bar);
    box_sprite->setColor(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 255.0f);
    box_sprite->setScale(m_box_width / 10, m_box_height / 10);
    bar_box_sprite = new GFX::Render2D::Sprite(g::bar);
    bar_box_sprite->setScale(100, 2);
    bar_box_sprite->setPosition((5 * 100) / 2, (5 * 2) / 4);
}

void shop_state::destroy()
{
}

void shop_state::update()
{
    //battery_text->setContent(scePow);   

    //if (Utilities::KeyPressed(PSP_CTRL_CROSS));
    
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
    for (int y = 0; y < 3; y++) 
    {
        for (int x = 0; x < 4; x++)
        {
            box_sprite->setPosition(((x + 1) * 8) + (x * m_box_width) + (m_box_width / 2), 22 + ((y + 1) * 2) + (y * m_box_height) + (m_box_height / 2));
            box_sprite->draw();
        }  
    }

    bar_box_sprite->draw();
}