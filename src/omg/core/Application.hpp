//
// Created by masy on 09.11.2023.
//

#pragma once

#if defined(WIN32) or defined(_WIN32)

#include "omg/core/ApplicationWin32.hpp"
namespace OMG {

	using Application = ApplicationWin32;

}

#endif
