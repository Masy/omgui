//
// Created by masy on 28.12.2023.
//

#include "MouseHoverEvent.hpp"

using namespace OMG::Events;

MouseHoverEvent::MouseHoverEvent(const int32_t cursorX, const int32_t cursorY)
		: m_cursorX(cursorX), m_cursorY(cursorY) {}

int32_t MouseHoverEvent::cursorX() const {
	return m_cursorX;
}

int32_t MouseHoverEvent::cursorY() const {
	return m_cursorY;
}
