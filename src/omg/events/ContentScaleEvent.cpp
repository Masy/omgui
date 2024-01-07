//
// Created by masy on 28.12.2023.
//

#include "ContentScaleEvent.hpp"

using namespace OMG::Events;

ContentScaleEvent::ContentScaleEvent(const float prevScale, const float newScale)
		: m_prevScale(prevScale), m_newScale(newScale) {}

float ContentScaleEvent::prevScale() const {
	return m_prevScale;
}

float ContentScaleEvent::newScale() const {
	return m_newScale;
}
