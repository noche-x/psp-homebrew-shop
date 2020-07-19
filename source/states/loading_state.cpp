#include "loading_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Network/NetworkDriver.h>
#include <Core/Core.h>
#include <pspaudio.h>

using namespace Stardust;

void loading_state::init()
{
    m_should_change = false;
    m_content_init = false;
    
    m_loading_text_alpha = 255;
    m_bar_scale = 1.5f;
    
    //input_text = new Stardust::GFX::UI::UIText({240, 106}, "loading");
    //loading_what_text = new Stardust::GFX::UI::UIText({240, 180}, "network");

    //loading_what_text->setOptions({0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_CENTER});

    // NOTE: if it can't find the png files check where its loading from,
    //      example: (for me atleast ) on windows usbhostfs_pc doesn't change dirs
    //      so i have to start it from the dir i want it to be at. 
    bar_sprite = new Stardust::GFX::Render2D::Sprite(g::textures::bar);
    dark_bar_sprite = new Stardust::GFX::Render2D::Sprite(g::textures::dark_bar);

    bar_sprite->setPosition(240, 136);
    dark_bar_sprite->setPosition(240, 136);

    bar_sprite->setScale(1.f, 1.f);
    dark_bar_sprite->setScale(40.f, 1.f);
}

void loading_state::destroy()
{
    delete bar_sprite;
    delete dark_bar_sprite;
}

int connection_thread(SceSize args, void *argp)
{

    return 0;
}
std::string what = "";
void loading_state::update()
{
    static bool once = false;

    if (m_loading_text_alpha > 0.f)
    {
        m_loading_text_alpha -= 2;
    }
    else if (m_loading_text_alpha <= 0.f)
    {
        m_loading_text_alpha = 255;
    }

    if (g::network_init || m_content_init)
    {
        if (g::network_init)
            what = "content";
        if (m_content_init) {
            what =  "ready";
            m_should_change = true;
        }

        m_bar_scale += 2.f;
        bar_sprite->setScale(m_bar_scale, 1.f);

        g::network_init = false;
        m_content_init = false;
    }

    if (!m_content_init && !once && g::network_init)
    {
        std::string base_url = "http://psphbshop.s3.eu-central-1.amazonaws.com/";
        std::string req_file = base_url + "cache/apps.json";

        Utilities::app_Logger->log(req_file);
        if (!Network::g_NetworkDriver.GetFileHTTP(req_file.c_str(), "cache/apps.json"))
            Utilities::app_Logger->log("unable to get apps.json (check low level core logs)");
        else {
            Utilities::app_Logger->log("recieved cache/apps.json");

            Json::Value v = Utilities::JSON::openJSON("cache/apps.json");
            Utilities::app_Logger->log("apps array size: " + std::to_string(v["apps"].size()));
            for (int i = 0; i < v["apps"].size(); i++)
            {
                if (v["apps"][i].isObject())
                {
                    std::ifstream f(v["apps"][i].asString() + "/large");
                    if (!f.good())
                    {
                        req_file = base_url + v["apps"][i].asString() + "/large";
                        Utilities::app_Logger->log(req_file);

                        if (Network::g_NetworkDriver.GetFileHTTP(req_file.c_str(), std::string(v["apps"][i].asString() + "/large").c_str()))
                            Utilities::app_Logger->log("recieved " + v["apps"][i].asString() + "/large");
                        else
                            Utilities::app_Logger->log("unable to get " + v["apps"][i].asString() + "/large" + " (check low level core logs)");
                    }
                    else
                    {
                        Utilities::app_Logger->log(v["apps"][i].asString() + "/large" + " exists. skiping.");
                    }
                }
            }
            
            Network::g_NetworkDriver.Cleanup();
        }

        m_content_init = true;
        once = true;
    }
    else if (!m_content_init && !once && !g::network_init) {
        what = "failed or skipped network init, loading from cache";

        bar_sprite->setScale(40.f, 1.f);

        m_should_change = true;

        once = true;
    }
}

bool loading_state::should_change()
{
    return m_should_change;
}

void loading_state::draw()
{
    if (m_should_change)
        return;

    g::font_renderer->setStyle({255, 255, 255, m_loading_text_alpha, 1.f, -1, INTRAFONT_ALIGN_CENTER, 0.0f, true});
    g::font_renderer->draw("loading", {240, 106});
    g::font_renderer->setStyle({255, 255, 255, 255, 0.5f, -1, INTRAFONT_ALIGN_CENTER, 0.0f, true});
    g::font_renderer->draw(what, {240, 180});
    
    dark_bar_sprite->draw();
    bar_sprite->draw();
}
