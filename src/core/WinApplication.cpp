//
// Created by masy on 27.09.2023.
//

#include <stdexcept>
#include <windows.h>
#include "Application.hpp"

using namespace OMG;

static inline WNDCLASSW windowClass{};
static inline uint32_t eventMsgID = 0;

LRESULT handleMessage(HWND hwnd, const  UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

Application *Application::getInstance() {
	static auto *instance = new Application();
	return instance;
}

Application::Application() {
	windowClass.lpfnWndProc = handleMessage;
	windowClass.hInstance = GetModuleHandleW(L"omGUI");
	windowClass.lpszClassName = L"omGUI";
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.style = CS_DBLCLKS;

	RegisterClassW(&windowClass);

	eventMsgID = RegisterWindowMessageW(L"omGUIEventMessage");
	if (eventMsgID == 0)
		throw std::runtime_error("failed to register omGUI event message!");

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

Application::~Application() {

}

void Application::start([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
	MSG message{};

	while (GetMessageW(&message, nullptr, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}

void Application::stop() {
	PostQuitMessage(0);
}
