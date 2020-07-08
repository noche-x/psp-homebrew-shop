#include <iostream>
#include <string>

#include <pspsdk.h>
#include <pspkernel.h>

#include <Platform/Platform.h>
#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
#include <Utilities/JSON.h>
#include <Graphics/Dialogs.h>
#include <Graphics/RendererCore.h>
#include <Graphics/UI/UIText.h>
#include <Graphics/UI/UIButton.h>
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

    Utilities::detail::core_Logger->currentLevel = Utilities::LoggerLevel::LOGGER_LEVEL_TRACE;

    if (!Network::g_NetworkDriver.Init())
    {
        Utilities::app_Logger->log("tess");
        Graphics::ShowMessage("Failed to initalize the network driver. App will exit now.");
        Platform::exitPlatform();
    }
    else
        g::network_init = true;
    
    Utilities::app_Logger->log("tess");

    app_logic *g_logic = new app_logic();
    g_logic->init();

    Graphics::g_RenderCore.Set2DMode();
    Graphics::g_RenderCore.SetClearColor(30, 30, 30, 255);

    while (g_logic->is_running())
    {
        Graphics::g_RenderCore.BeginCommands();
        Graphics::g_RenderCore.Clear();

        g_logic->run();

        Platform::platformUpdate();
        Graphics::g_RenderCore.EndCommands();
    }

    Platform::exitPlatform();

    return 0;
}