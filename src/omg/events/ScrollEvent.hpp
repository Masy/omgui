//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class ScrollEvent : public Event {
	public:

		ScrollEvent(int32_t cursorX, int32_t cursorY, int32_t offsetX, int32_t offsetY, uint32_t modifiers);

		[[nodiscard]] int32_t cursorX() const;

		[[nodiscard]] int32_t cursorY() const;

		[[nodiscard]] int32_t offsetX() const;

		[[nodiscard]] int32_t offsetY() const;

		[[nodiscard]] uint32_t modifiers() const;

		[[nodiscard]] bool operator==(const ScrollEvent &other) const;

	private:

		int32_t m_cursorX;
		int32_t m_cursorY;
		int32_t m_offsetX;
		int32_t m_offsetY;
		uint32_t m_modifiers;

	};

}
