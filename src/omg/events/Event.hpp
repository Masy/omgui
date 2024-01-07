//
// Created by masy on 18.12.2023.
//

#pragma once

#if defined(WIN32) or defined(_WIN32)

#include "omg/events/EventWin32.hpp"
namespace OMG::Events {

	using Event = EventWin32;

}

#endif
