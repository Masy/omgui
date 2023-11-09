//
// Created by masy on 27.09.2023.
//

#include <stdexcept>
#include <Windows.h>
#include <map>
#include "ApplicationWin32.hpp"
#include "omg/widgets/Window.hpp"

using namespace OMG;

static inline WNDCLASSW windowClass{};
static inline uint32_t eventMsgID = 0;
static inline std::map<HWND, OMG::Widgets::Window*> WINDOWS;

LRESULT ApplicationWin32::handleMessage(HWND hwnd, const  UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
	if (uMsg == WM_CREATE) {
		auto *createInfo = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WINDOWS[hwnd] = reinterpret_cast<OMG::Widgets::Window*>(createInfo->lpCreateParams);
		return 0;
	} else if (uMsg == eventMsgID) {
		return 0;
	}

	if (uMsg == WM_QUIT)
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);


	OMG::Widgets::Window *pWindow;
	try {
		pWindow = WINDOWS.at(hwnd);
	} catch (const std::exception &) {
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg) {
		case WM_DESTROY: {
			delete pWindow;
			WINDOWS.erase(hwnd);
			PostQuitMessage(0);
			break;
		}

		case WM_SIZE: {
			pWindow->m_width = (int32_t) LOWORD(lParam);
			pWindow->m_height = (int32_t) HIWORD(lParam);
			break;
		}

		case WM_ACTIVATE: {
			pWindow->m_focused = wParam != WA_INACTIVE;
		}

		case WM_ENABLE: {
			pWindow->m_enabled = wParam == TRUE;
			break;
		}

		case WM_SHOWWINDOW: {
			pWindow->m_visible = wParam == TRUE;
			break;
		}

		case WM_GETMINMAXINFO: {
			auto sizeLimits = (LPMINMAXINFO) lParam;

			sizeLimits->ptMinTrackSize.x = pWindow->minWidth();
			sizeLimits->ptMinTrackSize.y = pWindow->minHeight();
			sizeLimits->ptMaxTrackSize.x = pWindow->maxWidth();
			sizeLimits->ptMaxTrackSize.y = pWindow->maxHeight();
			break;
		}

		default:
			break;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

ApplicationWin32 *ApplicationWin32::getInstance() {
	static auto *instance = new ApplicationWin32();
	return instance;
}

ApplicationWin32::ApplicationWin32() {
	windowClass.lpfnWndProc = handleMessage;
	windowClass.hInstance = GetModuleHandleW(nullptr);
	windowClass.lpszClassName = L"omGUI";
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.style = CS_DBLCLKS;

	RegisterClassW(&windowClass);

	eventMsgID = RegisterWindowMessageW(L"omGUIEventMessage");
	if (eventMsgID == 0)
		throw std::runtime_error("failed to register omGUI event message!");

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

ApplicationWin32::~ApplicationWin32() {
	while (!WINDOWS.empty()) {
		DestroyWindow(WINDOWS.rbegin()->first);
	}
}

void ApplicationWin32::start([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
	MSG message{};

	while (GetMessageW(&message, nullptr, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}

void ApplicationWin32::stop() {
	PostQuitMessage(0);
}
