#include "state_manager.h"
#include "state.h"

state_manager::state_manager() {
    m_running = true;
    m_current_state = nullptr;
}

void state_manager::change_state(state* next_state) {
    if (m_current_state != nullptr) {
        m_current_state->destroy();
    }
    next_state->init();
    m_current_state = next_state;
}

void state_manager::update() {
    m_current_state->update();
}

void state_manager::draw() {
    m_current_state->draw();
}

state* state_manager::current_state() {
    return m_current_state;
}

bool state_manager::should_change() {
    return m_current_state->should_change();
}

bool state_manager::is_running() {
    return m_running;
}