#include "loading_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Graphics/Dialogs.h>
#include <Network/NetworkDriver.h>
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

    if (alpha > 0.f) {
        alpha -= 2;
    }
    else if (alpha <= 0.f) {
        alpha = 255;
    }

    input_text->setOptions({1.f, GU_ARGB(alpha, 255, 255, 255), INTRAFONT_ALIGN_CENTER});

    if (g::network_init || m_content_init)
    {
        if (g::network_init)
            loading_what_text->setContent("content");
        if (m_content_init)
            loading_what_text->setContent("ready");

        scale += 19.f;
        bar_sprite->Scale(scale, 1.f);

        g::network_init = false;
        m_content_init = false;
    }

    if (!m_content_init && !once)
    {
        Network::g_NetworkDriver.Connect(35700, "192.168.2.190", false);

        Stardust::Network::PacketOut *p = new Stardust::Network::PacketOut();
        p->ID = PacketIDS::GET_APPS;
        Network::encodeString("fafa", *p);
        
        Network::g_NetworkDriver.AddPacket(p);
        Network::g_NetworkDriver.SendPackets();

        Network::g_NetworkDriver.ReceivePacket();
        Network::g_NetworkDriver.HandlePackets();

        // int thid = 0;
		// thid = sceKernelCreateThread("temp_connection_thread", connection_thread, 0x11, 0xFA0, 0, 0);
		// if (thid >= 0)
		// {
		// 	sceKernelStartThread(thid, 0, 0);
		// }
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
