//
// Created by masy on 28.12.2023.
//

#include "ResizeEvent.hpp"

using namespace OMG::Events;

ResizeEvent::ResizeEvent(const int32_t deltaWidth, const int32_t deltaHeight)
		: m_deltaWidth(deltaWidth), m_deltaHeight(deltaHeight) {}

int32_t ResizeEvent::deltaWidth() const {
	return m_deltaWidth;
}

int32_t ResizeEvent::deltaHeight() const {
	return m_deltaHeight;
}

bool ResizeEvent::operator==(const OMG::Events::ResizeEvent &other) const {
	return m_deltaWidth == other.m_deltaWidth && m_deltaHeight == other.m_deltaHeight;
}
