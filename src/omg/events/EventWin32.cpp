//
// Created by masy on 18.12.2023.
//

#include <stdexcept>
#include "omg/events/EventWin32.hpp"
#include "omg/core/Application.hpp"

using namespace OMG::Events;

EventWin32::EventWin32() = default;

void EventWin32::emit(Widgets::Widget *widget) {
	if (widget == nullptr)
		throw std::invalid_argument("Failed to emit event: Widget can't be a nullptr!");

	auto *app = Application::getInstance();

	if (GetCurrentThreadId() == app->threadId()) {
		std::pair<handlerMultiMap_t::iterator, handlerMultiMap_t::iterator> range;
		try {
			range = eventHandlers.at(widget).equal_range(std::type_index(typeid(*this)));;
		} catch (const std::exception &) {
			return;
		}

		for (auto &handler = range.first; handler != range.second; handler++) {
			if (this->cancelled())
				break;

			handler->second(widget, shared_from_this());
		}
	} else {
		std::mutex mutex;
		std::condition_variable cv;
		bool finished = false;

		auto *winEvent = new _Wrapper();
		winEvent->event = shared_from_this();
		winEvent->widget = widget;
		winEvent->cv = &cv;
		winEvent->finished = &finished;

		try {
			winEvent->handlers = eventHandlers.at(widget).equal_range(std::type_index(typeid(*this)));
		} catch (const std::exception &) {
			return;
		}

		PostThreadMessageW(app->threadId(), app->eventMsgId(), 0, (LPARAM) winEvent);

		while (!finished) {
			std::unique_lock lock(mutex);
			cv.wait(lock);
		}
	}
}

void EventWin32::emitAsync(Widgets::Widget *widget) {
	if (widget == nullptr)
		throw std::invalid_argument("failed to asynchronously emit event:"
									"widget can't be a nullptr!");

	auto *winEvent = new _Wrapper();
	winEvent->event = shared_from_this();
	winEvent->widget = widget;

	try {
		winEvent->handlers = eventHandlers.at(widget).equal_range(std::type_index(typeid(*this)));
	} catch (const std::exception &) {
		return;
	}

	auto *app = OMG::Application::getInstance();
	PostThreadMessageW(app->threadId(), app->eventMsgId(), 0, (LPARAM) winEvent);
}
