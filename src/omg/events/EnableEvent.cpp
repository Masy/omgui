//
// Created by masy on 29.12.2023.
//

#include "EnableEvent.hpp"

using namespace OMG::Events;

EnableEvent::EnableEvent(const bool enabled) : m_enabled(enabled) {}

bool EnableEvent::enabled() const {
	return m_enabled;
}
