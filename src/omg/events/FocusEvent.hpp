//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class FocusEvent : public Event{
	public:

		FocusEvent(bool focused);

		[[nodiscard]] bool focused() const;

	private:

		bool m_focused;

	};

}
