//
// Created by masy on 29.12.2023.
//

#include "WindowStateEvent.hpp"

using namespace OMG::Events;

WindowStateEvent::WindowStateEvent(const OMG::WindowState prevState, const OMG::WindowState newState)
		: m_prevState(prevState), m_newState(newState) {}

OMG::WindowState WindowStateEvent::prevState() const {
	return m_prevState;
}

OMG::WindowState WindowStateEvent::newState() const {
	return m_newState;
}

bool WindowStateEvent::operator==(const OMG::Events::WindowStateEvent &other) const {
	return m_prevState == other.m_prevState && m_newState == other.m_newState;
}
