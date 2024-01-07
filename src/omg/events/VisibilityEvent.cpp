//
// Created by masy on 29.12.2023.
//

#include "VisibilityEvent.hpp"

using namespace OMG::Events;

VisibilityEvent::VisibilityEvent(const bool visible) : m_visible(visible) {}

bool VisibilityEvent::visible() const {
	return m_visible;
}

bool VisibilityEvent::operator==(const OMG::Events::VisibilityEvent &other) const {
	return m_visible == other.m_visible;
}
