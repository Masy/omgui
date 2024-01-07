//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Widgets {
	class AbstractWindow;
}

namespace OMG::Events {

	class ActivationEvent : public Event {
	public:

		ActivationEvent(bool activated, Widgets::AbstractWindow *otherWindow);

		[[nodiscard]] bool activated() const;

		[[nodiscard]] Widgets::AbstractWindow *otherWindow() const;

	private:

		bool m_activated;
		Widgets::AbstractWindow *m_otherWindow;

	};

}
