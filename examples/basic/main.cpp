//
// Created by masy on 27.09.2023.
//

#include "omg/core/Application.hpp"
#include "omg/widgets/Window.hpp"

int main(const int argc, const char **args) {
	auto *window = new OMG::Widgets::Window("omGUI test window");

	window->show();
	window->setSizeLimits(426, 1280, 240, 720);

	OMG::Application::getInstance()->start(argc, args);
	return 0;
}
