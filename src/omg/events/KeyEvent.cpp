//
// Created by masy on 19.12.2023.
//
#include "KeyEvent.hpp"


using namespace OMG::Events;

KeyEvent::KeyEvent(uint32_t keyCode, uint32_t modifiers, OMG::KeyState keyState)
		: m_keyCode(keyCode), m_modifiers(modifiers), m_keyState(keyState) {}

uint32_t KeyEvent::keyCode() const {
	return m_keyCode;
}

uint32_t KeyEvent::modifiers() const {
	return m_modifiers;
}

OMG::KeyState KeyEvent::keyState() const {
	return m_keyState;
}

bool KeyEvent::operator==(const OMG::Events::KeyEvent &other) const {
	return m_keyCode == other.m_keyCode && m_modifiers == other.m_modifiers && m_keyState == other.m_keyState;
}