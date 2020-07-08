#pragma once
#include "state_manager.h"
#include "states/splash_state.h"
#include "states/loading_state.h"

class app_logic {
public:
    void init();
    void run();
    bool is_running();
private:
	state_manager* m_state_manager;
    splash_state* m_splash_state;
    loading_state* m_loading_state;
};