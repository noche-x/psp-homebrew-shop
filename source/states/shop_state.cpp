#include "shop_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Graphics/Dialogs.h>
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

    box_sprite = new Graphics::Render2D::Sprite(Graphics::TextureUtil::LoadPng("assets/images/dark_bar.png"));
    box_sprite->RGB(170, 170, 170);
    box_sprite->Scale(m_box_width / 10, m_box_height / 10);
    
    bar_box_sprite = new Graphics::Render2D::Sprite(Graphics::TextureUtil::LoadPng("assets/images/bar.png"));
    bar_box_sprite->Scale(100, 2);
    bar_box_sprite->SetPosition((5 * box_sprite->scaleX) / 2, ( 5 * box_sprite->scaleY) / 4);

    app_name_text = new Stardust::Graphics::UI::UIText({240, 106}, "loading");
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
            box_sprite->SetPosition(((x + 1) * 8) + (x * m_box_width) + (m_box_width / 2), 22 + ((y + 1) * 2) + (y * m_box_height) + (m_box_height / 2));
            box_sprite->Draw();
        }  
    }

    bar_box_sprite->Draw();
}