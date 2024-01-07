//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class MoveEvent : public Event {
	public:

		MoveEvent(int32_t offsetX, int32_t offsetY);

		[[nodiscard]] int32_t offsetX() const;

		[[nodiscard]] int32_t offsetY() const;

		[[nodiscard]] bool operator==(const MoveEvent &other) const;

	private:

		int32_t m_offsetX;
		int32_t m_offsetY;

	};

}
