#if defined(SKIP_NET_INIT) | defined(SKIP_SPLASH)
    #define INTERNAL_DEV
#endif

#include <iostream>
#include <string>

#include <Platform/Platform.h>
#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
#include <Utilities/JSON.h>
#include <GFX/RenderCore.h>
#include <GFX/UI/TextRenderer.h>
#ifndef SKIP_NET_INIT
    #include <Platform/PSP/Dialogs.h>
#endif
#include <Network/NetworkDriver.h>

#include "global_variables.h"
#include "app_logic.h"
#include "definitions.h"

using namespace Stardust;

#if (CURRENT_PLATFORM == PLATFORM_WIN) || (CURRENT_PLATFORM == PLATFORM_NIX)
namespace std
{
    template <typename T>
    std::string to_string(const T &n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
} // namespace std
#endif

int main()
{
    Platform::initPlatform("psp-homebrew-shop");
    Utilities::app_Logger->log("version: " + std::to_string(VERSION));

    // NOTE: add trace logs to stardust
    Utilities::detail::core_Logger->currentLevel = Utilities::LoggerLevel::LOGGER_LEVEL_TRACE;
    Utilities::app_Logger->currentLevel = Utilities::LoggerLevel::LOGGER_LEVEL_TRACE;
    
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
    g::font_renderer->init("flash0:/font/ltn0.pgf");
    g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, true});

    while (g_logic->is_running())
    {
        GFX::g_RenderCore->beginFrame();
        GFX::g_RenderCore->clear();

        g_logic->run();

#ifdef INTERNAL_DEV
        g::font_renderer->setStyle({255, 255, 255, 255, 0.7f, TEXT_RENDERER_RIGHT, TEXT_RENDERER_RIGHT, 0, false});
        g::font_renderer->draw("DEV VERSION", {480, 265});
        g::font_renderer->setStyle({255, 255, 255, 255, 1.f, TEXT_RENDERER_CENTER, TEXT_RENDERER_CENTER, 0, true});
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