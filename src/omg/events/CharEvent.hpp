//
// Created by masy on 28.12.2023.
//

#pragma once

#include "omg/events/Event.hpp"

namespace OMG::Events {

	class CharEvent : public Event {
	public:

		CharEvent(uint32_t codePoint);

		[[nodiscard]] uint32_t codePoint() const;

	private:

		uint32_t m_codePoint;

	};

}
