#include "loading_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Graphics/Dialogs.h>
#include <Network/NetworkDriver.h>
#include <Core/Core.h>
#include <pspaudio.h>

using namespace Stardust;

void loading_state::init()
{
    m_should_change = false;
    m_content_init = false;
    m_content = "network";

    input_text = new Stardust::Graphics::UI::UIText({240, 106}, "loading");
    loading_what_text = new Stardust::Graphics::UI::UIText({240, 180}, "a");

    loading_what_text->setOptions({0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_CENTER});

    bar_sprite = new Stardust::Graphics::Render2D::Sprite(Stardust::Graphics::TextureUtil::LoadPng("assets/images/bar.png"));
    dark_bar_sprite = new Stardust::Graphics::Render2D::Sprite(Stardust::Graphics::TextureUtil::LoadPng("assets/images/dark_bar.png"));

    bar_sprite->SetPosition(240, 136);
    dark_bar_sprite->SetPosition(240, 136);

    bar_sprite->Scale(1.f, 1.f);
    dark_bar_sprite->Scale(40.f, 1.f);
}

void loading_state::destroy()
{
}

int connection_thread(SceSize args, void *argp)
{

    return 0;
}

void loading_state::update()
{
    static float scale = 1.5f;
    static bool once = false;
    static int alpha = 255;

    if (alpha > 0.f)
    {
        alpha -= 2;
    }
    else if (alpha <= 0.f)
    {
        alpha = 255;
    }

    input_text->setOptions({1.f, GU_ARGB(alpha, 255, 255, 255), INTRAFONT_ALIGN_CENTER});

    if (g::network_init || m_content_init)
    {
        if (g::network_init)
            loading_what_text->setContent("content");
        if (m_content_init)
            loading_what_text->setContent("ready");

        scale += 2.f;
        bar_sprite->Scale(scale, 1.f);

        g::network_init = false;
        m_content_init = false;
    }

    if (!m_content_init && !once)
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
                    std::ifstream f(v["apps"][i]["large"].asString());
                    if (!f.good())
                    {
                        req_file = base_url + v["apps"][i]["large"].asString();
                        Utilities::app_Logger->log(req_file);

                        if (Network::g_NetworkDriver.GetFileHTTP(req_file.c_str(), v["apps"][i]["large"].asString().c_str()))
                            Utilities::app_Logger->log("recieved " + v["apps"][i]["large"].asString());
                        else
                            Utilities::app_Logger->log("unable to get " + v["apps"][i]["large"].asString() + " (check low level core logs)");
                    }
                    else
                    {
                        Utilities::app_Logger->log(v["apps"][i]["large"].asString() + " exists. skiping.");
                    }                    
                }
            }
            
            Network::g_NetworkDriver.Cleanup();
        }

        m_content_init = true;
        once = true;
    }
}

bool loading_state::should_change()
{
    return m_should_change;
}

void loading_state::draw()
{
    input_text->draw();
    loading_what_text->draw();
    dark_bar_sprite->Draw();
    bar_sprite->Draw();
}
