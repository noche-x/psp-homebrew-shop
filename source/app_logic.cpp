#include "app_logic.h"
#include <Utilities/Logger.h>

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

    if (m_state_manager->current_state() == m_splash_state && m_state_manager->should_change()) {
        m_state_manager->change_state(m_loading_state);
        Stardust::Graphics::g_RenderCore.SetClearColor(40, 40, 40, 255);
    }
    else if (m_state_manager->current_state() == m_loading_state && m_state_manager->should_change()) {
        m_state_manager->change_state(m_shop_state);
        Stardust::Graphics::g_RenderCore.SetClearColor(50, 50, 50, 255);
    }
    
    m_state_manager->update();
    m_state_manager->draw(); 
};

bool app_logic::is_running() {
    return this->m_state_manager->is_running();
};