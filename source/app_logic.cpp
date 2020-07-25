#include "app_logic.h"
#include <Utilities/Logger.h>
#include <GFX/RenderCore.h>

void app_logic::init() {
    m_state_manager = new state_manager();

    m_splash_state = new splash_state();
    m_loading_state = new loading_state();
    m_shop_state = new shop_state();

#ifndef SKIP_SPLASH
    m_state_manager->change_state(m_splash_state);
#else
    m_state_manager->change_state(m_loading_state);
    Utilities::app_Logger->log("skipping splash screen...");
#endif 
};

void app_logic::run() {
    m_state_manager->update();
    m_state_manager->draw(); 

    if (m_state_manager->current_state() == m_splash_state && m_state_manager->should_change()) {
        m_state_manager->change_state(m_loading_state);
        Utilities::app_Logger->log("loading loading state");
        Stardust::GFX::g_RenderCore->setClearColor(40.0f / 255.0f, 40.0f / 255.0f, 40.0f / 255.0f, 1.0f);
    }
    else if (m_state_manager->current_state() == m_loading_state && m_state_manager->should_change()) {
        m_state_manager->change_state(m_shop_state);
        Utilities::app_Logger->log("loading shop state");
        Stardust::GFX::g_RenderCore->setClearColor(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
    }
};

bool app_logic::is_running() {
    return this->m_state_manager->is_running();
};