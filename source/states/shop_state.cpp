#include "shop_state.h"
#include "../global_variables.h"
#include "../definitions.h"
#include <Utilities/Logger.h>
#include <Network/NetworkDriver.h>
#include <Graphics/Dialogs.h>
#include <Network/NetworkDriver.h>
#include <Core/Core.h>
#include <pspaudio.h>

using namespace Stardust;

void shop_state::init()
{
    m_should_change = false;

    
}

void shop_state::destroy()
{
}

void shop_state::update()
{
}

bool shop_state::should_change()
{
    return m_should_change;
}

void shop_state::draw()
{
    
}
