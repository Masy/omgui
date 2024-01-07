//
// Created by masy on 27.09.2023.
//

#include <stdexcept>
#include <map>
#include "omg/core/ApplicationWin32.hpp"
#include "omg/widgets/Window.hpp"
#include "omg/events/KeyEvent.hpp"
#include "omg/events/MouseEvent.hpp"
#include "omg/events/MoveEvent.hpp"
#include "omg/events/ResizeEvent.hpp"
#include "omg/events/WindowStateEvent.hpp"
#include "omg/events/FocusEvent.hpp"
#include "omg/events/ActivationEvent.hpp"
#include "omg/events/EnableEvent.hpp"
#include "omg/events/VisibilityEvent.hpp"
#include "omg/events/CharEvent.hpp"
#include "omg/events/ScrollEvent.hpp"
#include "omg/events/FileDropEvent.hpp"
#include "omg/events/MouseMoveEvent.hpp"
#include "omg/events/ContentScaleEvent.hpp"
#include "omg/events/MouseHoverEvent.hpp"
#include "omg/events/DoubleClickEvent.hpp"

using namespace OMG;

static inline WNDCLASSW _windowClass{};
static inline uint32_t _eventMsgID = 0;
static inline uint32_t _threadId = 0;
static inline std::map<HWND, OMG::Widgets::Window*> WINDOWS;

LRESULT ApplicationWin32::handleMessage(HWND hwnd, const  UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
	static thread_local uint32_t HIGH_SURROGATE = UNICODE_NOCHAR;

	if (uMsg == WM_CREATE) {
		auto *createInfo = reinterpret_cast<CREATESTRUCTW*>(lParam);
		auto pWindow = reinterpret_cast<OMG::Widgets::Window*>(createInfo->lpCreateParams);
		WINDOWS[hwnd] = pWindow;

		RECT clientArea{};
		GetClientRect(hwnd, &clientArea);
		ClientToScreen(hwnd, reinterpret_cast<LPPOINT>(&clientArea.left));
		ClientToScreen(hwnd, reinterpret_cast<LPPOINT>(&clientArea.right));
		pWindow->m_posX = clientArea.left;
		pWindow->m_posY = clientArea.top;
		pWindow->m_width = clientArea.right - clientArea.left;
		pWindow->m_height = clientArea.bottom - clientArea.top;
		return 0;
	} else if (uMsg == _eventMsgID) {
		auto *winEvent = reinterpret_cast<Events::EventWin32::_Wrapper*>(lParam);

		for (auto &handler = winEvent->handlers.first; handler != winEvent->handlers.second; handler++) {
			if (winEvent->event->cancelled())
				break;

			handler->second(winEvent->widget, winEvent->event);
		}

		if (winEvent->cv != nullptr) {
			*winEvent->finished = true;
			winEvent->cv->notify_all();
		}

		delete winEvent;
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

		case WM_MOVE: {
			POINT screenPos = {(int32_t) (short) LOWORD(lParam),
							   (int32_t) (short) HIWORD(lParam)};
			if (screenPos.x == -32000 || screenPos.y == -32000)
				return 0;

			int32_t offsetX = screenPos.x - pWindow->posX();
			int32_t offsetY = screenPos.y - pWindow->posY();

			pWindow->m_posX = screenPos.x;
			pWindow->m_posY = screenPos.y;

			auto event = std::make_shared<Events::MoveEvent>(offsetX, offsetY);
			event->emit(pWindow);

			break;
		}

		case WM_SIZE: {
			auto width = (int32_t) LOWORD(lParam);
			auto height = (int32_t) HIWORD(lParam);

			int32_t deltaWidth = width - pWindow->width();
			int32_t deltaHeight = height - pWindow->height();

			pWindow->m_width = width;
			pWindow->m_height = height;

			if (deltaWidth != 0 || deltaHeight != 0) {
				auto event = std::make_shared<Events::ResizeEvent>(deltaWidth, deltaHeight);
				event->emit(pWindow);
			}

			if (wParam <= SIZE_MAXIMIZED && wParam != pWindow->m_windowState) {
				auto prevState = pWindow->m_windowState;
				pWindow->m_windowState = (WindowState) wParam;

				auto event = std::make_shared<Events::WindowStateEvent>(prevState, pWindow->m_windowState);
				event->emit(pWindow);
			}

			break;
		}

		case WM_ACTIVATE: {
			bool activated = LOWORD(wParam) != WA_INACTIVE;
			HWND otherWindow = (HWND) lParam;

			auto it = WINDOWS.find(otherWindow);

			auto event = std::make_shared<Events::ActivationEvent>(activated, it != WINDOWS.end() ? it->second : nullptr);
			event->emit(pWindow);

			break;
		}

		case WM_SETFOCUS: {
			if (!pWindow->m_focused) {
				pWindow->m_focused = true;

				auto event = std::make_shared<Events::FocusEvent>(true);
				event->emit(pWindow);
			}

			break;
		}

		case WM_KILLFOCUS: {
			if (pWindow->m_focused) {
				pWindow->m_focused = false;

				auto event = std::make_shared<Events::FocusEvent>(false);
				event->emit(pWindow);
			}

			break;
		}

		case WM_ENABLE: {
			auto enabled = wParam == TRUE;
			pWindow->m_enabled = enabled;

			auto event = std::make_shared<Events::EnableEvent>(enabled);
			event->emit(pWindow);

			break;
		}

		case WM_SHOWWINDOW: {
			auto visible = wParam == TRUE;
			pWindow->m_visible = visible;

			auto event = std::make_shared<Events::VisibilityEvent>(visible);
			event->emit(pWindow);

			break;
		}

		case WM_MOUSEACTIVATE : {
			if (pWindow->isPopup())
				return MA_NOACTIVATE;

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

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			auto keyCode = (uint32_t) wParam;
			bool repeat = lParam & 0x40000000u;
			auto event = std::make_shared<Events::KeyEvent>(keyCode, getModifierBitMask(), repeat ? OMG_REPEAT : OMG_PRESSED);
			event->emit(pWindow);

			break;
		}

		case WM_KEYUP:
		case WM_SYSKEYUP: {
			auto keyCode = (uint32_t) wParam;
			auto event = std::make_shared<Events::KeyEvent>(keyCode, getModifierBitMask(), OMG_RELEASED);
			event->emit(pWindow);

			break;
		}

		case WM_CHAR: {
			if (wParam == UNICODE_NOCHAR)
				break;

			uint16_t highSurrogate = HIWORD(wParam);
			uint16_t lowSurrogate = LOWORD(wParam);
			uint32_t codePoint = 0;
			if (highSurrogate >= 0xD800 && highSurrogate <= 0xDBFF && lowSurrogate >= 0xDC00 && lowSurrogate <= 0xDFFF) {
				codePoint = ((highSurrogate - 0xD800) << 10) + (lowSurrogate - 0xDC00) + 0x10000;
			} else {
				codePoint = (uint32_t) wParam;
			}

			auto event = std::make_shared<Events::CharEvent>(codePoint);
			event->emit(pWindow);

			break;
		}

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN: {
			int x = (int) (short) LOWORD(lParam);
			int y = (int) (short) HIWORD(lParam);
			uint32_t mouseButton;

			switch(uMsg) {
				case WM_RBUTTONDOWN:
					mouseButton = OMG_MB_RIGHT;
					break;
				case WM_MBUTTONDOWN:
					mouseButton = OMG_MB_MIDDLE;
					break;
				case WM_XBUTTONDOWN:
					if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
						mouseButton = OMG_MB_FORWARD;
					else
						mouseButton = OMG_MB_BACKWARD;
					break;
				default:
					mouseButton = OMG_MB_LEFT;
					pWindow->m_interactState = OMG_INTERACT_PRESSED;
					break;
			}

			auto event = std::make_shared<Events::MouseEvent>(x, y, mouseButton, getModifierBitMask(), OMG_PRESSED);
			event->emit(pWindow);
			break;
		}

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP: {
			int x = (int) (short) LOWORD(lParam);
			int y = (int) (short) HIWORD(lParam);
			uint32_t mouseButton;

			switch(uMsg) {
				case WM_RBUTTONUP:
					mouseButton = OMG_MB_RIGHT;
					break;
				case WM_MBUTTONUP:
					mouseButton = OMG_MB_MIDDLE;
					break;
				case WM_XBUTTONUP:
					if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
						mouseButton = OMG_MB_FORWARD;
					else
						mouseButton = OMG_MB_BACKWARD;
					break;
				default: {
					mouseButton = OMG_MB_LEFT;

					bool insideClientArea = x >= 0 && x <= pWindow->width() && y >= 0 && y <= pWindow->height();
					pWindow->m_interactState = insideClientArea ? OMG_INTERACT_HOVERED : OMG_INTERACT_DEFAULT;
					break;
				}
			}

			auto event = std::make_shared<Events::MouseEvent>(x, y, mouseButton, getModifierBitMask(), OMG_RELEASED);
			event->emit(pWindow);

			break;
		}

		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK: {
			int x = (int) (short) LOWORD(lParam);
			int y = (int) (short) HIWORD(lParam);
			uint32_t mouseButton;

			switch(uMsg) {
				case WM_RBUTTONDBLCLK:
					mouseButton = OMG_MB_RIGHT;
					break;
				case WM_MBUTTONDBLCLK:
					mouseButton = OMG_MB_MIDDLE;
					break;
				case WM_XBUTTONDBLCLK:
					if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
						mouseButton = OMG_MB_FORWARD;
					else
						mouseButton = OMG_MB_BACKWARD;
					break;
				default:
					mouseButton = OMG_MB_LEFT;
					break;
			}

			auto event = std::make_shared<Events::DoubleClickEvent>(x, y, mouseButton, getModifierBitMask());
			event->emit(pWindow);
		} break;

		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL: {
			auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			POINT cursorPos = {(int) (short) LOWORD(lParam), (int) (short) HIWORD(lParam)};
			ScreenToClient(hwnd, &cursorPos);

			auto event = std::make_shared<Events::ScrollEvent>(cursorPos.x, cursorPos.y,
															   uMsg == WM_MOUSEHWHEEL ? zDelta : 0,
															   uMsg == WM_MOUSEWHEEL ? zDelta : 0,
															   getModifierBitMask());
			event->emit(pWindow);

			break;
		}

		case WM_DROPFILES: {
			auto hDrop = (HDROP) wParam;
			POINT cursor;
			DragQueryPoint(hDrop, &cursor);

			UINT count = DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);
			std::vector<std::filesystem::path> paths(count);
			UINT n = 0;
			for (auto &path : paths) {
				const int size = (int) DragQueryFileW(hDrop, n, nullptr, 0) + 1;
				std::vector<WCHAR> buffer(size);
				DragQueryFileW(hDrop, n, buffer.data(), size);
				path = std::filesystem::path(buffer.data());
				n++;
			}

			DragFinish(hDrop);

			auto event = std::make_shared<Events::FileDropEvent>(cursor.x, cursor.y, std::move(paths));
			event->emit(pWindow);

			break;
		}

		case WM_MOUSEMOVE: {
			auto cursorX = (short) LOWORD(lParam);
			auto cursorY = (short) HIWORD(lParam);

			auto event = std::make_shared<Events::MouseMoveEvent>(cursorX, cursorY);
			event->emit(pWindow);

			TRACKMOUSEEVENT tracker{};
			tracker.cbSize = sizeof(tracker);
			tracker.dwFlags = TME_LEAVE | TME_HOVER;
			tracker.hwndTrack = hwnd;
			tracker.dwHoverTime = HOVER_DEFAULT;

			TrackMouseEvent(&tracker);

			break;
		}

		case WM_MOUSEHOVER: {
			if (pWindow->m_interactState != OMG_INTERACT_DEFAULT)
				break;

			pWindow->m_interactState = OMG_INTERACT_HOVERED;
			auto cursorX = (int32_t) (short) LOWORD(lParam);
			auto cursorY = (int32_t) (short) HIWORD(lParam);

			auto event = std::make_shared<Events::MouseHoverEvent>(cursorX, cursorY);
			event->emit(pWindow);

			break;
		}

		case WM_MOUSELEAVE: {
			if (pWindow->m_interactState != OMG_INTERACT_PRESSED)
				pWindow->m_interactState = OMG_INTERACT_DEFAULT;
			break;
		}

		case WM_DPICHANGED: {
			auto prevScale = pWindow->contentScale();
			auto newScale = (float) HIWORD(wParam) / USER_DEFAULT_SCREEN_DPI;

			auto event = std::make_shared<Events::ContentScaleEvent>(prevScale, newScale);
			event->emit(pWindow);

			RECT *const prcNewWindow = (RECT *) lParam;
			SetWindowPos(hwnd, nullptr, prcNewWindow->left, prcNewWindow->top,
						 prcNewWindow->right - prcNewWindow->left,
						 prcNewWindow->bottom - prcNewWindow->top,
						 SWP_NOZORDER | SWP_NOACTIVATE);

			break;
		}

		default:
			break;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

uint32_t ApplicationWin32::getModifierBitMask() {
	uint32_t bitmask = 0;

	if (GetKeyState(VK_SHIFT) < 0)
		bitmask |= OMG_MOD_SHIFT;
	if (GetKeyState(VK_CONTROL) < 0)
		bitmask |= OMG_MOD_CONTROL;
	if (GetKeyState(VK_MENU) < 0)
		bitmask |= OMG_MOD_ALT;
	if (GetKeyState(VK_LWIN) < 0 || GetKeyState(VK_RWIN) < 0)
		bitmask |= OMG_MOD_SUPER;
	if (GetKeyState(VK_CAPITAL) == 1)
		bitmask |= OMG_MOD_CAPS_LOCK;
	if (GetKeyState(VK_NUMLOCK) == 1)
		bitmask |= OMG_MOD_NUM_LOCK;

	return bitmask;
}

ApplicationWin32 *ApplicationWin32::getInstance() {
	static auto *instance = new ApplicationWin32();
	return instance;
}

ApplicationWin32::ApplicationWin32() {
	_windowClass.lpfnWndProc = handleMessage;
	_windowClass.hInstance = GetModuleHandleW(nullptr);
	_windowClass.lpszClassName = L"omGUI";
	_windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	_windowClass.style = CS_DBLCLKS;

	RegisterClassW(&_windowClass);

	_eventMsgID = RegisterWindowMessageW(L"omGUIEventMessage");
	if (_eventMsgID == 0)
		throw std::runtime_error("failed to register omGUI event message!");

	_threadId = GetCurrentThreadId();

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

uint32_t ApplicationWin32::eventMsgId() const {
	return _eventMsgID;
}

uint32_t ApplicationWin32::threadId() const {
	return _threadId;
}
