//
// Created by masy on 27.09.2023.
//

#include "omg/core/Application.hpp"
#include "omg/widgets/Window.hpp"
#include "omg/events/AbstractEvent.hpp"
#include "omg/events/MouseEvent.hpp"

using namespace OMG;

int main(const int argc, const char **args) {
	auto *window = new Widgets::Window("omGUI test window");

	window->show();
	window->setSizeLimits(426, 1280, 240, 720);

	Events::connect(window, [] (Widgets::Widget *window, const std::shared_ptr<Events::MouseEvent> &event) {
		printf("x: %d; y: %d\n", window->posX(), window->posY());
	});

	Application::getInstance()->start(argc, args);
	return 0;
}
