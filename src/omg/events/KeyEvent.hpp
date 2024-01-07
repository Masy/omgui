//
// Created by masy on 19.12.2023.
//

#pragma once

#include "omg/core/Constants.hpp"
#include "omg/events/Event.hpp"

namespace OMG::Events {

	class KeyEvent : public Event {
	public:

		KeyEvent(uint32_t keyCode, uint32_t modifiers, KeyState keyState);

		[[nodiscard]] uint32_t keyCode() const;

		[[nodiscard]] uint32_t modifiers() const;

		[[nodiscard]] KeyState keyState() const;

		bool operator==(const KeyEvent &other) const;

	private:

		uint32_t m_keyCode;
		uint32_t m_modifiers;
		KeyState m_keyState;

	};

}


