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
#include <Graphics/RendererCore.h>
#include <Graphics/UI/UIText.h>
#include <Graphics/UI/UIButton.h>
#include <Graphics/Dialogs.h>
#include <Network/NetworkDriver.h>

#include "global_variables.h"
#include "app_logic.h"
#include "definitions.h"

PSP_MODULE_INFO("PSP-Homebrew-Shop", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;

int main()
{
    Platform::initPlatform("psp-homebrew-shop");
    Utilities::app_Logger->log("version: " + std::to_string(VERSION));

    // NOTE: add trace logs to stardust
    Utilities::detail::core_Logger->currentLevel = Utilities::LoggerLevel::LOGGER_LEVEL_TRACE;

#ifndef SKIP_NET_INIT
    if (!Network::g_NetworkDriver.Init())
    {
        Utilities::app_Logger->log("[!] network init failed. could be that the network is down or user canceled.");
        Graphics::ShowMessageError("Failed to initalize the network driver.", 0x80020001);
        Graphics::ShowMessage("Failed to initalize the network driver.");
        
        Platform::exitPlatform();
    }
    
    Utilities::app_Logger->log("network init done.");
#else
    g::network_init = false;
    Utilities::app_Logger->log("skipping network init...");
#endif

    app_logic *g_logic = new app_logic();
    g_logic->init();

    Graphics::g_RenderCore.Set2DMode();
    Graphics::g_RenderCore.SetClearColor(30, 30, 30, 255);

#ifdef INTERNAL_DEV
    const char* dev_ver = "DEV VERSION";
    Graphics::UI::UIText* dev_version_text = new Graphics::UI::UIText({480 - (int)(intraFontMeasureText(Graphics::UI::g_DefaultFont, dev_ver) * 0.7f), 267}, dev_ver);
    dev_version_text->setOptions({0.7f, 0xFFFFFFFF, INTRAFONT_ALIGN_LEFT});
#endif

    while (g_logic->is_running())
    {
        Graphics::g_RenderCore.BeginCommands();
        Graphics::g_RenderCore.Clear();

        g_logic->run();

#ifdef INTERNAL_DEV
        dev_version_text->draw();
#endif

        Platform::platformUpdate();
        Graphics::g_RenderCore.EndCommands();
    }

    Platform::exitPlatform();

    return 0;
}