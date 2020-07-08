#include "app_logic.h"
#include <Utilities/Logger.h>

void app_logic::init() {
    m_state_manager = new state_manager();

    m_splash_state = new splash_state();
    m_loading_state = new loading_state();

    m_state_manager->change_state(m_loading_state);
};

void app_logic::run() {
    m_state_manager->update();
    m_state_manager->draw();
    if (m_state_manager->current_state() == m_splash_state && m_state_manager->should_change()) {
        m_state_manager->change_state(m_loading_state);
        Stardust::Graphics::g_RenderCore.SetClearColor(40, 40, 40, 255);
    }
};

bool app_logic::is_running() {
    return this->m_state_manager->is_running();
};