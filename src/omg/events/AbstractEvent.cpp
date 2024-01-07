//
// Created by masy on 18.12.2023.
//

#include "omg/events/AbstractEvent.hpp"

using namespace OMG::Events;

std::unordered_map<OMG::Widgets::Widget*, AbstractEvent::handlerMultiMap_t> AbstractEvent::eventHandlers{};

AbstractEvent::~AbstractEvent() = default;

bool AbstractEvent::cancelled() const {
	return m_cancelled;
}

void AbstractEvent::setCancelled(const bool cancelled) {
	m_cancelled = cancelled;
}

void OMG::Events::disconnect(Widgets::Widget *widget) {
	AbstractEvent::eventHandlers.erase(widget);
}
