//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class ResizeEvent : public Event {
	public:

		ResizeEvent(int32_t deltaWidth, int32_t deltaHeight);

		[[nodiscard]] int32_t deltaWidth() const;

		[[nodiscard]] int32_t deltaHeight() const;

		[[nodiscard]] bool operator==(const ResizeEvent &other) const;

	private:

		int32_t m_deltaWidth;
		int32_t m_deltaHeight;

	};

}
