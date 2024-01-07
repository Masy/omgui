//
// Created by masy on 28.12.2023.
//

#include "ActivationEvent.hpp"

using namespace OMG::Events;

ActivationEvent::ActivationEvent(const bool activated, Widgets::AbstractWindow *otherWindow)
		: m_activated(activated), m_otherWindow(otherWindow) {}

bool ActivationEvent::activated() const {
	return m_activated;
}

OMG::Widgets::AbstractWindow *ActivationEvent::otherWindow() const {
	return m_otherWindow;
}
