//
// Created by masy on 28.12.2023.
//

#include "CharEvent.hpp"

using namespace OMG::Events;

CharEvent::CharEvent(const uint32_t codePoint) : m_codePoint(codePoint) {}

uint32_t CharEvent::codePoint() const {
	return m_codePoint;
}
