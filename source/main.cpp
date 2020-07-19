#if defined(SKIP_NET_INIT) | defined(SKIP_SPLASH)
    #define INTERNAL_DEV
#endif

#include <iostream>
#include <string>

#include <pspsdk.h>
#include <pspkernel.h>

#include <Platform/Platform.h>
#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
#include <Utilities/JSON.h>
#include <GFX/RenderCore.h>
#include <GFX/UI/TextRenderer.h>
#include <Platform/PSP/Dialogs.h>
#include <Network/NetworkDriver.h>

#include "global_variables.h"
#include "app_logic.h"
#include "definitions.h"

using namespace Stardust;

int main()
{
    Platform::initPlatform("psp-homebrew-shop");
    Utilities::app_Logger->log("version: " + std::to_string(VERSION));

    // NOTE: add trace logs to stardust
    Utilities::detail::core_Logger->currentLevel = Utilities::LoggerLevel::LOGGER_LEVEL_TRACE;
    
    g::textures::dark_bar = GFX::g_TextureManager->loadTex("assets/images/dark_bar.png", GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, false);
    g::textures::bar = GFX::g_TextureManager->loadTex("assets/images/bar.png", GFX_FILTER_LINEAR, GFX_FILTER_LINEAR, false);

#ifndef SKIP_NET_INIT
    if (!Network::g_NetworkDriver.Init())
    {
        Utilities::app_Logger->log("[!] network init failed. could be that the network is down or user canceled.");
        Platform::PSP::ShowMessageError("Failed to initalize the network driver.", 0x80020001);
        Platform::PSP::ShowMessage("Failed to initalize the network driver.");
        
        Platform::exitPlatform();
    }

    Utilities::app_Logger->log("network init done.");
#else
    g::network_init = false;
    Utilities::app_Logger->log("skipping network init...");
#endif

    app_logic *g_logic = new app_logic();
    g_logic->init();

    GFX::g_RenderCore->setClearColor(30.0f/255.0f, 30.0f/255.0f, 30.0f/255.0f, 1.0f);
    
    g::font_renderer = new GFX::UI::TextRenderer();
    g::font_renderer->init("./assets/font.pgf");
    g::font_renderer->setStyle({255, 255, 255, 255, 1.f, INTRAFONT_ALIGN_CENTER, INTRAFONT_ALIGN_CENTER, 0, true});

    while (g_logic->is_running())
    {
        GFX::g_RenderCore->beginFrame();
        GFX::g_RenderCore->clear();

        g_logic->run();

#ifdef INTERNAL_DEV
        g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, INTRAFONT_ALIGN_RIGHT, INTRAFONT_ALIGN_RIGHT, 0, false});
        g::font_renderer->draw("DEV VERSION", {480, 265});
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, INTRAFONT_ALIGN_CENTER, INTRAFONT_ALIGN_CENTER, 0, true});
#endif

        Platform::platformUpdate();

        Utilities::app_Logger->flushLog();
        Utilities::detail::core_Logger->flushLog();
        
        GFX::g_RenderCore->endFrame();
    }

    Utilities::app_Logger->flushLog();
    Utilities::detail::core_Logger->flushLog();

    Platform::exitPlatform();

    return 0;
}