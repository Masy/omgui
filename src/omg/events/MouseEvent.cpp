//
// Created by masy on 19.12.2023.
//
#include "MouseEvent.hpp"

using namespace OMG::Events;

MouseEvent::MouseEvent(const int32_t cursorX, const int32_t cursorY, const uint32_t mouseButton,
					   const uint32_t modifiers, const OMG::KeyState keyState)
		: m_cursorX(cursorX), m_cursorY(cursorY), m_mouseButton(mouseButton),
		  m_modifiers(modifiers), m_keyState(keyState) {}

int32_t MouseEvent::cursorX() const {
	return m_cursorX;
}

int32_t MouseEvent::cursorY() const {
	return m_cursorY;
}

uint32_t MouseEvent::mouseButton() const {
	return m_mouseButton;
}

uint32_t MouseEvent::modifiers() const {
	return m_modifiers;
}

OMG::KeyState MouseEvent::keyState() const {
	return m_keyState;
}

bool MouseEvent::operator==(const OMG::Events::MouseEvent &other) const {
	return m_cursorX == other.m_cursorX && m_cursorY == other.m_cursorY &&
		   m_mouseButton == other.m_mouseButton && m_modifiers == other.m_modifiers &&
		   m_keyState == other.m_keyState;
}
