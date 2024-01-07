//
// Created by masy on 28.12.2023.
//

#include "MouseMoveEvent.hpp"

using namespace OMG::Events;

MouseMoveEvent::MouseMoveEvent(const int32_t cursorX, const int32_t cursorY)
		: m_cursorX(cursorX), m_cursorY(cursorY) {}

int32_t MouseMoveEvent::cursorX() const {
	return m_cursorX;
}

int32_t MouseMoveEvent::cursorY() const {
	return m_cursorY;
}

bool MouseMoveEvent::operator==(const OMG::Events::MouseMoveEvent &other) const {
	return m_cursorX == other.m_cursorX && m_cursorY == other.m_cursorY;
}
