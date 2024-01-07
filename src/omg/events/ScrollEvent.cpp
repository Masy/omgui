//
// Created by masy on 28.12.2023.
//

#include "ScrollEvent.hpp"

using namespace OMG::Events;

ScrollEvent::ScrollEvent(const int32_t cursorX, const int32_t cursorY,
						 const int32_t offsetX, const int32_t offsetY, const uint32_t modifiers)
		: m_cursorX(cursorX), m_cursorY(cursorY), m_offsetX(offsetX), m_offsetY(offsetY), m_modifiers(modifiers) {}

int32_t ScrollEvent::cursorX() const {
	return m_cursorX;
}

int32_t ScrollEvent::cursorY() const {
	return m_cursorY;
}

int32_t ScrollEvent::offsetX() const {
	return m_offsetX;
}

int32_t ScrollEvent::offsetY() const {
	return m_offsetY;
}

uint32_t ScrollEvent::modifiers() const {
	return m_modifiers;
}

bool ScrollEvent::operator==(const OMG::Events::ScrollEvent &other) const {
	return m_cursorX == other.m_cursorX && m_cursorY == other.m_cursorY &&
	       m_offsetX == other.m_offsetX && m_offsetY == other.m_offsetY &&
		   m_modifiers == other.m_modifiers;
}
