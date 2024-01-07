
//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class DoubleClickEvent : public Event{
	public:

		DoubleClickEvent(int32_t cursorX, int32_t cursorY, uint32_t mouseButton, uint32_t modifiers);

		[[nodiscard]] int32_t cursorX() const;

		[[nodiscard]] int32_t cursorY() const;

		[[nodiscard]] uint32_t mouseButton() const;

		[[nodiscard]] uint32_t modifiers() const;

		[[nodiscard]] bool operator==(const DoubleClickEvent &other) const;

	private:

		int32_t m_cursorX;
		int32_t m_cursorY;
		uint32_t m_mouseButton;
		uint32_t m_modifiers;

	};

}
