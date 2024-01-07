//
// Created by masy on 29.12.2023.
//

#pragma once

#include "omg/core/Constants.hpp"
#include "omg/events/Event.hpp"

namespace OMG::Events {

	class WindowStateEvent : public Event {
	public:

		WindowStateEvent(WindowState prevState, WindowState newState);

		[[nodiscard]] WindowState prevState() const;

		[[nodiscard]] WindowState newState() const;

		[[nodiscard]] bool operator==(const WindowStateEvent &other) const;

	private:

		WindowState m_prevState;
		WindowState m_newState;

	};

}
