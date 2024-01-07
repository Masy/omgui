//
// Created by masy on 18.12.2023.
//

#pragma once

#include <condition_variable>
#include "AbstractEvent.hpp"

namespace OMG::Events {

	class EventWin32 : public AbstractEvent {
	public:

		struct _Wrapper {
			std::shared_ptr<AbstractEvent> event;
			Widgets::Widget *widget = nullptr;
			std::pair<AbstractEvent::handlerMultiMap_t::iterator, AbstractEvent::handlerMultiMap_t::iterator> handlers;
			std::condition_variable *cv = nullptr;
			bool *finished = nullptr;
		};

	public:

		EventWin32();

		void emit(Widgets::Widget *widget) override;

		void emitAsync(Widgets::Widget *widget) override;

	};

}
