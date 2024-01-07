//
// Created by masy on 30.12.2023.
//

#define NOMINMAX
#include <array>
#include <shtypes.h>
#include <shlobj_core.h>
#include "omg/core/Application.hpp"
#include "omg/widgets/Window.hpp"
#include "doctest/doctest.h"
#include "omg/events/AbstractEvent.hpp"
#include "omg/events/ActivationEvent.hpp"
#include "omg/events/FocusEvent.hpp"
#include "omg/events/CharEvent.hpp"
#include "omg/events/ContentScaleEvent.hpp"
#include "omg/events/DoubleClickEvent.hpp"
#include "omg/events/EnableEvent.hpp"
#include "omg/events/FileDropEvent.hpp"
#include "omg/events/KeyEvent.hpp"
#include "omg/events/MouseEvent.hpp"
#include "omg/events/MouseHoverEvent.hpp"
#include "omg/events/MouseMoveEvent.hpp"
#include "omg/events/MoveEvent.hpp"
#include "omg/events/VisibilityEvent.hpp"
#include "omg/events/WindowStateEvent.hpp"
#include "omg/events/ResizeEvent.hpp"
#include "omg/events/ScrollEvent.hpp"

using namespace OMG;

TEST_SUITE("Event Tests") {

	TEST_CASE("test window events") {
		auto *window = new Widgets::Window("omGUI test window");

		SUBCASE("check activation event") {
			bool activationEventHandled = false;
			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::ActivationEvent> &event) {
				activationEventHandled = event->activated();
			});

			SUBCASE("check activating") {
				SetActiveWindow(window->hwnd());
				REQUIRE(activationEventHandled);
			}

			SUBCASE("check deactivating") {
				SendMessageW(window->hwnd(), WM_ACTIVATE, WA_INACTIVE, 0);
				REQUIRE_FALSE(activationEventHandled);
			}

			Events::disconnect(window);

			SUBCASE("check disconnection") {
				SetActiveWindow(window->hwnd());
				REQUIRE_FALSE(activationEventHandled);
			}
		}

		SUBCASE("check focus event") {
			bool focusEventHandled = false;
			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::FocusEvent> &event) {
				focusEventHandled = event->focused();
			});

			SUBCASE("check focusing") {
				window->setFocused(true);
				REQUIRE(focusEventHandled);
			}

			SUBCASE("check unfocusing") {
				window->setFocused(false);
				REQUIRE_FALSE(focusEventHandled);
			}

			Events::disconnect(window);
		}

		SUBCASE("check char event") {
			std::vector<uint32_t> codepoints{
						0x41, 0x70, 0x70, 0x6C, 0x65, 0x20, 0xCEBC, 0xCEAE, 0xCEBB, 0xCEBF,
								0x20, 0xD0AF, 0xD0B1, 0xD0BB, 0xD0BE, 0xD0BA, 0xD0BE, 0x20,
								0x1F606, 0x1F7A3
			};
			std::vector<uint32_t> result;

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::CharEvent> &event) {
				result.push_back(event->codePoint());
			});

			for (uint32_t codepoint : codepoints) {
				WPARAM wParam;

				if (codepoint < 0x10000) {
					wParam = (WCHAR) codepoint;
				} else {
					uint16_t highSurrogate = (uint16_t) std::floor((codepoint - 0x10000) / 0x400) + 0xD800;
					uint16_t lowSurrogate = ((codepoint - 0x10000) % 0x400) + 0xDC00;
					wParam = (uint64_t) highSurrogate << 16 | (uint64_t) lowSurrogate;
				}

				SendMessageW(window->hwnd(), WM_CHAR, wParam, 0);
			}

			CHECK_EQ(codepoints.size(), result.size());
			if (result.size() == codepoints.size()) {
				for (std::size_t n = 0; n < codepoints.size(); n++) {
					CHECK_EQ(codepoints[n], result[n]);
				}
			}

			Events::disconnect(window);
		}

		SUBCASE("check content scale event") {
			bool contentScaleEventHandled = false;

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::ContentScaleEvent> &event) {
				contentScaleEventHandled = event->newScale() == 1.5f;
			});

			RECT rect;
			GetClientRect(window->hwnd(), &rect);
			auto width = rect.right - rect.left;
			auto height = rect.bottom - rect.top;
			rect.right = rect.left + (LONG) (width * 1.5);
			rect.bottom = rect.top + (LONG) (height * 1.5);
			SendMessageW(window->hwnd(), WM_DPICHANGED, (WPARAM) (USER_DEFAULT_SCREEN_DPI * 1.5) << 16, (LPARAM) &rect);
			CHECK(contentScaleEventHandled);
			Events::disconnect(window);
		}

		SUBCASE("check double click event") {
			std::array<BYTE, 256> keyStates = {0};

			Events::DoubleClickEvent checkEvent{0, 0, 0, 0};
			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::DoubleClickEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check double left click") {
				Events::DoubleClickEvent event{420, 1337, OMG_MB_LEFT, OMG_MOD_SHIFT | OMG_MOD_ALT};

				keyStates[VK_SHIFT] = -128;
				keyStates[VK_MENU] = -128;
				SetKeyboardState((BYTE*) keyStates.data());
				SendMessageW(window->hwnd(), WM_LBUTTONDBLCLK, 0, (1337 << 16) | 420);
				CHECK_EQ(checkEvent, event);
				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check double right click") {
				Events::DoubleClickEvent event{0, 0, OMG_MB_RIGHT, OMG_MOD_SHIFT | OMG_MOD_CONTROL};

				keyStates[VK_SHIFT] = -128;
				keyStates[VK_CONTROL] = -128;
				SetKeyboardState((BYTE*) keyStates.data());
				SendMessageW(window->hwnd(), WM_RBUTTONDBLCLK, 0, 0);
				CHECK_EQ(checkEvent, event);
				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check double middle click") {
				Events::DoubleClickEvent event{0, 0, OMG_MB_MIDDLE, OMG_MOD_ALT | OMG_MOD_CAPS_LOCK};

				keyStates[VK_MENU] = -128;
				keyStates[VK_CAPITAL] = 1;
				SetKeyboardState((BYTE*) keyStates.data());
				SendMessageW(window->hwnd(), WM_MBUTTONDBLCLK, 0, 0);
				CHECK_EQ(checkEvent, event);
				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check double forward click") {
				Events::DoubleClickEvent event{0, 0, OMG_MB_FORWARD, OMG_MOD_SUPER | OMG_MOD_NUM_LOCK};

				keyStates[VK_LWIN] = -128;
				keyStates[VK_NUMLOCK] = 1;
				SetKeyboardState((BYTE*) keyStates.data());
				SendMessageW(window->hwnd(), WM_XBUTTONDBLCLK, XBUTTON1 << 16, 0);
				CHECK_EQ(checkEvent, event);
				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check double backward click") {
				Events::DoubleClickEvent event{0, 0, OMG_MB_BACKWARD, OMG_MOD_SUPER | OMG_MOD_NUM_LOCK};

				keyStates[VK_LWIN] = -128;
				keyStates[VK_NUMLOCK] = 1;
				SetKeyboardState((BYTE*) keyStates.data());
				SendMessageW(window->hwnd(), WM_XBUTTONDBLCLK, XBUTTON2 << 16, 0);
				CHECK_EQ(checkEvent, event);
				memset(keyStates.data(), 0, keyStates.size());
			}

			Events::disconnect(window);
		}

		SUBCASE("check enable event") {
			bool enableEventHandled = true;

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::EnableEvent> &event) {
				enableEventHandled = event->enabled();
			});

			SUBCASE("check disabling") {
				window->setEnabled(false);
				CHECK_FALSE(enableEventHandled);
			}

			SUBCASE("check enabling") {
				window->setEnabled(true);
				CHECK(enableEventHandled);
			}

			Events::disconnect(window);
		}

		SUBCASE("check file drop event") {
			bool fileDropHandled = false;
			std::vector<std::filesystem::path> paths{
				R"(C:\Fake\Path\To\Some\File.txt)",
				R"(D:\Another\File.png)",
				R"(F:\The\last\File\is\actually\a\folder)"
			};

			std::vector<std::wstring> tmp;

			size_t totalSize = sizeof(DROPFILES);
			for (const auto &path : paths) {
				tmp.push_back(path.wstring());
				totalSize += (tmp.back().size() + 1) * sizeof(wchar_t);
			}

			HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE, totalSize);
			REQUIRE(hGlobal != nullptr);

			auto *pDropFiles = reinterpret_cast<DROPFILES*>(GlobalLock(hGlobal));
			REQUIRE(pDropFiles != nullptr);

			pDropFiles->pFiles = sizeof(DROPFILES);
			pDropFiles->pt.x = 420;
			pDropFiles->pt.y = 1337;
			pDropFiles->fNC = FALSE;
			pDropFiles->fWide = TRUE;

			auto *pData = reinterpret_cast<wchar_t*>(pDropFiles + 1);
			for (const auto &path : tmp) {
				wcscpy_s(pData, path.length() + 1, path.c_str());
				pData += path.length() + 1;
			}

			GlobalUnlock(hGlobal);
			auto hDrop = reinterpret_cast<HDROP>(hGlobal);

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::FileDropEvent> &event) {
				if (event->size() != paths.size() || event->cursorX() != 420 || event->cursorY() != 1337)
					return;

				uint32_t idx = 0;
				for (auto &path : event->paths()) {
					if (path != paths[idx++])
						return;
				}

				fileDropHandled = true;
			});

			SendMessageW(window->hwnd(), WM_DROPFILES, (WPARAM) hDrop, 0);
			CHECK(fileDropHandled);
			GlobalFree(hGlobal);
			Events::disconnect(window);
		}

		SUBCASE("check key event") {
			std::array<BYTE, 256> keyStates = {0};

			Events::KeyEvent checkEvent{0, 0, OMG_RELEASED};
			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::KeyEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check key A + CTRL + NUMLOCK") {
				Events::KeyEvent event1{0x41, OMG_MOD_CONTROL | OMG_MOD_NUM_LOCK, OMG_PRESSED};
				Events::KeyEvent event2{0x41, OMG_MOD_CONTROL | OMG_MOD_NUM_LOCK, OMG_REPEAT};
				Events::KeyEvent event3{0x41, OMG_MOD_CONTROL | OMG_MOD_NUM_LOCK, OMG_RELEASED};

				keyStates[VK_NUMLOCK] = 1;
				keyStates[VK_CONTROL] = -128;
				SetKeyboardState((BYTE*) keyStates.data());

				SendMessageW(window->hwnd(), WM_KEYDOWN, 0x41, 0);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_KEYDOWN, 0x41, 0x40000000);
				CHECK_EQ(event2, checkEvent);
				SendMessageW(window->hwnd(), WM_KEYUP, 0x41, 0x40000000);
				CHECK_EQ(event3, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check key NUM5 + SHIFT + CAPSLOCK") {
				Events::KeyEvent event1{VK_NUMPAD5, OMG_MOD_SHIFT | OMG_MOD_CAPS_LOCK, OMG_PRESSED};
				Events::KeyEvent event2{VK_NUMPAD5, OMG_MOD_SHIFT | OMG_MOD_CAPS_LOCK, OMG_REPEAT};
				Events::KeyEvent event3{VK_NUMPAD5, OMG_MOD_SHIFT | OMG_MOD_CAPS_LOCK, OMG_RELEASED};

				keyStates[VK_CAPITAL] = 1;
				keyStates[VK_SHIFT] = -128;
				SetKeyboardState((BYTE*) keyStates.data());

				SendMessageW(window->hwnd(), WM_KEYDOWN, VK_NUMPAD5, 0);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_KEYDOWN, VK_NUMPAD5, 0x40000000);
				CHECK_EQ(event2, checkEvent);
				SendMessageW(window->hwnd(), WM_KEYUP, VK_NUMPAD5, 0x40000000);
				CHECK_EQ(event3, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check key ALT + SUPER") {
				Events::KeyEvent event1{VK_MENU, OMG_MOD_ALT | OMG_MOD_SUPER, OMG_PRESSED};
				Events::KeyEvent event2{VK_MENU, OMG_MOD_ALT | OMG_MOD_SUPER, OMG_REPEAT};
				Events::KeyEvent event3{VK_MENU, OMG_MOD_ALT | OMG_MOD_SUPER, OMG_RELEASED};

				keyStates[VK_LWIN] = -128;
				keyStates[VK_MENU] = -128;
				SetKeyboardState((BYTE*) keyStates.data());

				SendMessageW(window->hwnd(), WM_KEYDOWN, VK_MENU, 0);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_KEYDOWN, VK_MENU, 0x40000000);
				CHECK_EQ(event2, checkEvent);
				SendMessageW(window->hwnd(), WM_KEYUP, VK_MENU, 0x40000000);
				CHECK_EQ(event3, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			Events::disconnect(window);
		}

		SUBCASE("check mouse event") {
			std::array<BYTE, 256> keyStates = {0};

			Events::MouseEvent checkEvent{0, 0, 0, 0, OMG_RELEASED};
			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::MouseEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check left MB + CTRL + CAPS") {
				Events::MouseEvent event1{123, 456, OMG_MB_LEFT, OMG_MOD_CONTROL | OMG_MOD_CAPS_LOCK, OMG_PRESSED};
				Events::MouseEvent event2{234, 567, OMG_MB_LEFT, OMG_MOD_CONTROL | OMG_MOD_CAPS_LOCK, OMG_RELEASED};

				keyStates[VK_CONTROL] = -128;
				keyStates[VK_CAPITAL] = 1;
				SetKeyboardState(keyStates.data());

				SendMessageW(window->hwnd(), WM_LBUTTONDOWN, 0, (456 << 16) | 123);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_LBUTTONUP, 0, (567 << 16) | 234);
				CHECK_EQ(event2, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check right MB + SHIFT") {
				Events::MouseEvent event1{432, 567, OMG_MB_RIGHT, OMG_MOD_SHIFT, OMG_PRESSED};
				Events::MouseEvent event2{567, 890, OMG_MB_RIGHT, OMG_MOD_SHIFT, OMG_RELEASED};

				keyStates[VK_SHIFT] = -128;
				SetKeyboardState(keyStates.data());

				SendMessageW(window->hwnd(), WM_RBUTTONDOWN, 0, (567 << 16) | 432);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_RBUTTONUP, 0, (890 << 16) | 567);
				CHECK_EQ(event2, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check middle MB + ALT") {
				Events::MouseEvent event1{123, 789, OMG_MB_MIDDLE, OMG_MOD_ALT, OMG_PRESSED};
				Events::MouseEvent event2{345, 543, OMG_MB_MIDDLE, OMG_MOD_ALT, OMG_RELEASED};

				keyStates[VK_MENU] = -128;
				SetKeyboardState(keyStates.data());

				SendMessageW(window->hwnd(), WM_MBUTTONDOWN, 0, (789 << 16) | 123);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_MBUTTONUP, 0, (543 << 16) | 345);
				CHECK_EQ(event2, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check forward MB + WIN") {
				Events::MouseEvent event1{123, 789, OMG_MB_FORWARD, OMG_MOD_SUPER, OMG_PRESSED};
				Events::MouseEvent event2{345, 543, OMG_MB_FORWARD, OMG_MOD_SUPER, OMG_RELEASED};

				keyStates[VK_LWIN] = -128;
				SetKeyboardState(keyStates.data());

				SendMessageW(window->hwnd(), WM_XBUTTONDOWN, XBUTTON1 << 16, (789 << 16) | 123);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_XBUTTONUP, XBUTTON1 << 16, (543 << 16) | 345);
				CHECK_EQ(event2, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check backward MB + WIN") {
				Events::MouseEvent event1{123, 789, OMG_MB_BACKWARD, OMG_MOD_SUPER, OMG_PRESSED};
				Events::MouseEvent event2{345, 543, OMG_MB_BACKWARD, OMG_MOD_SUPER, OMG_RELEASED};

				keyStates[VK_LWIN] = -128;
				SetKeyboardState(keyStates.data());

				SendMessageW(window->hwnd(), WM_XBUTTONDOWN, XBUTTON2 << 16, (789 << 16) | 123);
				CHECK_EQ(event1, checkEvent);
				SendMessageW(window->hwnd(), WM_XBUTTONUP, XBUTTON2 << 16, (543 << 16) | 345);
				CHECK_EQ(event2, checkEvent);

				memset(keyStates.data(), 0, keyStates.size());
			}

			Events::disconnect(window);
		}

		SUBCASE("check mouse hover event") {
			int32_t cursorX = 0;
			int32_t cursorY = 0;

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::MouseHoverEvent> &event) {
				cursorX = event->cursorX();
				cursorY = event->cursorY();
			});

			SendMessageW(window->hwnd(), WM_MOUSEHOVER, 0, (345 << 16) | 765);
			CHECK_EQ(765, cursorX);
			CHECK_EQ(345, cursorY);
			CHECK(window->interactState() == OMG_INTERACT_HOVERED);

			SendMessageW(window->hwnd(), WM_MOUSELEAVE, 0, 0);
			CHECK(window->interactState() == OMG_INTERACT_DEFAULT);

			Events::disconnect(window);
		}

		SUBCASE("check mouse move event") {
			Events::MouseMoveEvent checkEvent{0, 0};
			Events::MouseMoveEvent event1{std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::min()};
			Events::MouseMoveEvent event2{std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::max()};
			Events::MouseMoveEvent event3{-1243, 23765};

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::MouseMoveEvent> &event) {
				checkEvent = *event;
			});

			SendMessageW(window->hwnd(), WM_MOUSEMOVE, 0, (LPARAM) (event1.cursorY() << 16) | (event1.cursorX() & 0xFFFF));
			CHECK_EQ(event1, checkEvent);
			SendMessageW(window->hwnd(), WM_MOUSEMOVE, 0, (event2.cursorY() << 16) | (event2.cursorX() & 0xFFFF));
			CHECK_EQ(event2, checkEvent);
			SendMessageW(window->hwnd(), WM_MOUSEMOVE, 0, (event3.cursorY() << 16) | (event3.cursorX() & 0xFFFF));
			CHECK_EQ(event3, checkEvent);

			Events::disconnect(window);
		}

		SUBCASE("check move event") {
			Events::MoveEvent checkEvent{0, 0};

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::MoveEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check window and hwnd coordinates are equal") {
				RECT clientArea{};
				GetClientRect(window->hwnd(), &clientArea);
				ClientToScreen(window->hwnd(), reinterpret_cast<LPPOINT>(&clientArea.left));

				CHECK_EQ(clientArea.left, window->posX());
				CHECK_EQ(clientArea.top, window->posY());
			}

			SUBCASE("check setPosition") {
				Events::MoveEvent event{-37, 1489};
				auto newPosX = window->posX() + event.offsetX();
				auto newPosY = window->posY() + event.offsetY();
				window->setPosition(newPosX, newPosY);
				CHECK_EQ(event, checkEvent);

				RECT clientArea{};
				GetClientRect(window->hwnd(), &clientArea);
				ClientToScreen(window->hwnd(), reinterpret_cast<LPPOINT>(&clientArea.left));

				CHECK_EQ(clientArea.left, window->posX());
				CHECK_EQ(clientArea.top, window->posY());
			}

			SUBCASE("check setPosX") {
				Events::MoveEvent event{362, 0};
				window->setPosX(window->posX() + event.offsetX());
				CHECK_EQ(event, checkEvent);

				RECT clientArea{};
				GetClientRect(window->hwnd(), &clientArea);
				ClientToScreen(window->hwnd(), reinterpret_cast<LPPOINT>(&clientArea.left));

				CHECK_EQ(clientArea.left, window->posX());
				CHECK_EQ(clientArea.top, window->posY());
			}

			SUBCASE("check setPosY") {
				Events::MoveEvent event{0, -315};
				window->setPosY(window->posY() + event.offsetY());
				CHECK_EQ(event, checkEvent);

				RECT clientArea{};
				GetClientRect(window->hwnd(), &clientArea);
				ClientToScreen(window->hwnd(), reinterpret_cast<LPPOINT>(&clientArea.left));

				CHECK_EQ(clientArea.left, window->posX());
				CHECK_EQ(clientArea.top, window->posY());
			}

			SUBCASE("check move") {
				Events::MoveEvent event{-671, 177};
				window->move(event.offsetX(), event.offsetY());
				CHECK_EQ(event, checkEvent);

				RECT clientArea{};
				GetClientRect(window->hwnd(), &clientArea);
				ClientToScreen(window->hwnd(), reinterpret_cast<LPPOINT>(&clientArea.left));

				CHECK_EQ(clientArea.left, window->posX());
				CHECK_EQ(clientArea.top, window->posY());
			}

			SUBCASE("check moveX") {
				Events::MoveEvent event{213, 0};
				window->moveX(event.offsetX());
				CHECK_EQ(event, checkEvent);

				RECT clientArea{};
				GetClientRect(window->hwnd(), &clientArea);
				ClientToScreen(window->hwnd(), reinterpret_cast<LPPOINT>(&clientArea.left));

				CHECK_EQ(clientArea.left, window->posX());
				CHECK_EQ(clientArea.top, window->posY());
			}

			SUBCASE("check moveY") {
				Events::MoveEvent event{0, 365};
				window->moveY(event.offsetY());
				CHECK_EQ(event, checkEvent);

				RECT clientArea{};
				GetClientRect(window->hwnd(), &clientArea);
				ClientToScreen(window->hwnd(), reinterpret_cast<LPPOINT>(&clientArea.left));

				CHECK_EQ(clientArea.left, window->posX());
				CHECK_EQ(clientArea.top, window->posY());
			}

			Events::disconnect(window);
		}

		SUBCASE("check resize event") {
			Events::ResizeEvent checkEvent{0, 0};
			Events::ResizeEvent event1{50, 0};
			Events::ResizeEvent event2{0, 50};
			Events::ResizeEvent event3{-50, -50};

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::ResizeEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check setWidth") {
				window->setWidth(window->width() + event1.deltaWidth());
				CHECK_EQ(event1, checkEvent);
			}

			SUBCASE("check setHeight") {
				window->setHeight(window->height() + event2.deltaHeight());
				CHECK_EQ(event2, checkEvent);
			}

			SUBCASE("check setWidth") {
				window->setSize(window->width() + event3.deltaWidth(), window->height() + event3.deltaHeight());
				CHECK_EQ(event3, checkEvent);
			}

			Events::disconnect(window);
		}

		SUBCASE("check scroll event") {
			std::array<BYTE, 256> keyStates = {0};
			Events::ScrollEvent checkEvent{0, 0, 0, 0, 0};
			Events::ScrollEvent event1{0, 0, -1, 0, OMG_MOD_CONTROL};
			Events::ScrollEvent event2{-1337, -1337, 1, 0, OMG_MOD_SHIFT | OMG_MOD_SUPER};
			Events::ScrollEvent event3{1337, -1337, 0, -1, OMG_MOD_ALT};
			Events::ScrollEvent event4{1337, 1337, 0, 1, OMG_MOD_NUM_LOCK | OMG_MOD_CAPS_LOCK};

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::ScrollEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check negative x offset") {
				POINT cursor{event1.cursorX(), event1.cursorY()};
				ClientToScreen(window->hwnd(), &cursor);
				keyStates[VK_CONTROL] = -128;
				SetKeyboardState(keyStates.data());
				SendMessageW(window->hwnd(), WM_MOUSEHWHEEL, -WHEEL_DELTA << 16, (cursor.y << 16) | (cursor.x & 0xFFFF));
				CHECK_EQ(event1, checkEvent);
				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check positive x offset") {
				POINT cursor{event2.cursorX(), event2.cursorY()};
				ClientToScreen(window->hwnd(), &cursor);
				keyStates[VK_SHIFT] = -128;
				keyStates[VK_LWIN] = -128;
				SetKeyboardState(keyStates.data());
				SendMessageW(window->hwnd(), WM_MOUSEHWHEEL, WHEEL_DELTA << 16, (cursor.y << 16) | (cursor.x & 0xFFFF));
				CHECK_EQ(event2, checkEvent);
				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check negative y offset") {
				POINT cursor{event3.cursorX(), event3.cursorY()};
				ClientToScreen(window->hwnd(), &cursor);
				keyStates[VK_MENU] = -128;
				SetKeyboardState(keyStates.data());
				SendMessageW(window->hwnd(), WM_MOUSEWHEEL, -WHEEL_DELTA << 16 , (cursor.y << 16) | (cursor.x & 0xFFFF));
				CHECK_EQ(event3, checkEvent);
				memset(keyStates.data(), 0, keyStates.size());
			}

			SUBCASE("check positive y offset") {
				POINT cursor{event4.cursorX(), event4.cursorY()};
				ClientToScreen(window->hwnd(), &cursor);
				keyStates[VK_NUMLOCK] = 1;
				keyStates[VK_CAPITAL] = 1;
				SetKeyboardState(keyStates.data());
				SendMessageW(window->hwnd(), WM_MOUSEWHEEL, WHEEL_DELTA << 16, (cursor.y << 16) | (cursor.x & 0xFFFF));
				CHECK_EQ(event4, checkEvent);
				memset(keyStates.data(), 0, keyStates.size());
			}

			Events::disconnect(window);
		}

		SUBCASE("check visibility event") {
			Events::VisibilityEvent checkEvent{false};
			Events::VisibilityEvent event1{true};
			Events::VisibilityEvent event2{false};

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::VisibilityEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check show") {
				window->show();
				CHECK_EQ(event1, checkEvent);
			}

			SUBCASE("check hide") {
				window->hide();
				CHECK_EQ(event2, checkEvent);
			}

			Events::disconnect(window);
		}

		SUBCASE("check window state event") {
			Events::WindowStateEvent checkEvent{OMG_WINDOW_WINDOWED, OMG_WINDOW_WINDOWED};
			Events::WindowStateEvent event1{OMG_WINDOW_WINDOWED, OMG_WINDOW_MINIMIZED};
			Events::WindowStateEvent event2{OMG_WINDOW_MINIMIZED, OMG_WINDOW_WINDOWED};
			Events::WindowStateEvent event3{OMG_WINDOW_WINDOWED, OMG_WINDOW_MAXIMIZED};
			Events::WindowStateEvent event4{OMG_WINDOW_MAXIMIZED, OMG_WINDOW_WINDOWED};

			Events::connect(window, [&] (Widgets::Widget *window, const std::shared_ptr<Events::WindowStateEvent> &event) {
				checkEvent = *event;
			});

			SUBCASE("check minimize") {
				window->minimize();
				CHECK_EQ(event1, checkEvent);
				window->restore();
				CHECK_EQ(event2, checkEvent);
			}

			SUBCASE("check maximize") {
				window->maximize();
				CHECK_EQ(event3, checkEvent);
				window->restore();
				CHECK_EQ(event4, checkEvent);
			}
		}
	}

}