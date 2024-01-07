//
// Created by masy on 19.12.2023.
//

#pragma once

#include "omg/core/Constants.hpp"
#include "omg/events/Event.hpp"

namespace OMG::Events {

	class MouseEvent : public Event {
	public:

		MouseEvent(int32_t cursorX, int32_t cursorY, uint32_t mouseButton, uint32_t modifiers, KeyState keyState);

		[[nodiscard]] int32_t cursorX() const;

		[[nodiscard]] int32_t cursorY() const;

		[[nodiscard]] uint32_t mouseButton() const;

		[[nodiscard]] uint32_t modifiers() const;

		[[nodiscard]] KeyState keyState() const;

		[[nodiscard]] bool operator==(const MouseEvent &other) const;

	private:

		int32_t m_cursorX;
		int32_t m_cursorY;
		uint32_t m_mouseButton;
		uint32_t m_modifiers;
		KeyState m_keyState;

	};

}
