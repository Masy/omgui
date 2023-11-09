//
// Created by masy on 05.10.2023.
//

#include "AbstractWindow.hpp"

using namespace OMG::Widgets;

AbstractWindow::AbstractWindow() : Widget() {
	m_parent = this;
}

void AbstractWindow::addToRenderQueue() {
	// todo: add to render queue
}

