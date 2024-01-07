//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class ContentScaleEvent : public Event {
	public:

		ContentScaleEvent(float prevScale, float newScale);

		[[nodiscard]] float prevScale() const;

		[[nodiscard]] float newScale() const;

	private:

		float m_prevScale;
		float m_newScale;

	};

}
