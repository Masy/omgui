//
// Created by masy on 28.12.2023.
//

#include "FocusEvent.hpp"

using namespace OMG::Events;

FocusEvent::FocusEvent(const bool focused) : m_focused(focused) {}

bool FocusEvent::focused() const {
	return m_focused;
}
