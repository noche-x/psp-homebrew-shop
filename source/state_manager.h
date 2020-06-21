#pragma once
#include "state.h"

class state_manager {
public:
    state_manager();
    void change_state(state* next_state);
    void update();
    void draw();
    state* current_state();
    bool should_change();
    bool is_running();
private:
    state* m_current_state;
    bool m_running;
};
