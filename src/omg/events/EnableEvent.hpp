//
// Created by masy on 29.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class EnableEvent : public Event {
	public:

		EnableEvent(bool enabled);

		[[nodiscard]] bool enabled() const;

	private:

		bool m_enabled;

	};

}
