//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class MouseMoveEvent : public Event {
	public:

		MouseMoveEvent(int32_t cursorX, int32_t cursorY);

		[[nodiscard]] int32_t cursorX() const;

		[[nodiscard]] int32_t cursorY() const;

		[[nodiscard]] bool operator==(const MouseMoveEvent &other) const;

	private:

		int32_t m_cursorX;
		int32_t m_cursorY;

	};

}
