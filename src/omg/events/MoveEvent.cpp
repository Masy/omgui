//
// Created by masy on 28.12.2023.
//

#include "MoveEvent.hpp"

using namespace OMG::Events;

MoveEvent::MoveEvent(const int32_t offsetX, const int32_t offsetY)
		: m_offsetX(offsetX), m_offsetY(offsetY) {}

int32_t MoveEvent::offsetX() const {
	return m_offsetX;
}

int32_t MoveEvent::offsetY() const {
	return m_offsetY;
}

bool MoveEvent::operator==(const OMG::Events::MoveEvent &other) const {
	return m_offsetX == other.m_offsetX && m_offsetY == other.m_offsetY;
}
