#include <iostream>
#include <string>

#include <pspsdk.h>
#include <pspkernel.h>

#include <Platform/Platform.h>
#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
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

int send_apps_handler(Stardust::Network::PacketIn *packet)
{
    std::string apps_file = Stardust::Network::decodeString(*packet);
    Stardust::Utilities::app_Logger->log(apps_file);

    return 0;
}

int main()
{
    Platform::initPlatform("psp-homebrew-shop");
    Utilities::app_Logger->log("version: " + std::to_string(VERSION));

    if (!Network::g_NetworkDriver.Init())
    {
        if (Graphics::ShowMessageYesNo("Failed to initalize the network driver. You will be unable to load the app list. App will exit when you make a selection."))
            Platform::exitPlatform();
        else
            Platform::exitPlatform();
    }
    else
        g::network_init = true;

    Network::g_NetworkDriver.AddPacketHandler(PacketIDS::SEND_APPS, send_apps_handler);

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