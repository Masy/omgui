//
// Created by masy on 04.10.2023.
//

#pragma once

#if defined(WIN32) or defined(_WIN32)

#include "omg/widgets/WindowWin32.hpp"
namespace OMG::Widgets {

	using Window = WindowWin32;

}

#endif
