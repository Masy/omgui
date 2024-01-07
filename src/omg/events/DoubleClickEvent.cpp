//
// Created by masy on 28.12.2023.
//

#include "DoubleClickEvent.hpp"

using namespace OMG::Events;

DoubleClickEvent::DoubleClickEvent(const int32_t cursorX, const int32_t cursorY,
								   const uint32_t mouseButton, const uint32_t modifiers)
		: m_cursorX(cursorX), m_cursorY(cursorY), m_mouseButton(mouseButton), m_modifiers(modifiers) {}

int32_t DoubleClickEvent::cursorX() const {
	return m_cursorX;
}

int32_t DoubleClickEvent::cursorY() const {
	return m_cursorY;
}

uint32_t DoubleClickEvent::mouseButton() const {
	return m_mouseButton;
}

uint32_t DoubleClickEvent::modifiers() const {
	return m_modifiers;
}

bool DoubleClickEvent::operator==(const OMG::Events::DoubleClickEvent &other) const {
	return m_cursorX == other.m_cursorX && m_cursorY == other.m_cursorY &&
		   m_mouseButton == other.m_mouseButton && m_modifiers == other.m_modifiers;
}
