//
// Created by masy on 29.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class VisibilityEvent : public Event {
	public:

		VisibilityEvent(bool visible);

		[[nodiscard]] bool visible() const;

		[[nodiscard]] bool operator==(const VisibilityEvent &other) const;

	private:

		bool m_visible;

	};

}
