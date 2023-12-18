//
// Created by masy on 04.10.2023.
//

#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif /* not defined NOMINMAX */
#include <Windows.h>
#include "omg/widgets/AbstractWindow.hpp"
#include "omg/core/ApplicationWin32.hpp"

namespace OMG::Widgets {

	/**
	 * Window widget for the windows operating system.
	 */
	class WindowWin32 : public AbstractWindow {
	public:

		/**
		 * Creates a new window widget.
		 *
		 * @param title A const reference to the title of the window.
		 * @param style A bitmask defining the style of the window.
		 */
		WindowWin32(const std::string_view &title, uint32_t style = OMG_WINDOW_STYLE_DEFAULT);

		/**
		 * Creates a new window widget.
		 *
		 * @param title A const reference to the title of the window.
		 * @param style A bitmask defining the style of the window.
		 */
		WindowWin32(const std::wstring_view &title, uint32_t style = OMG_WINDOW_STYLE_DEFAULT);

		/**
		 * Sets the position of the window.
		 *
		 * @param posX The new x coordinate of the widget relative to the desktop.
		 * @param posY The new y coordinate of the widget relative to the desktop.
		 */
		void setPosition(int32_t posX, int32_t posY) override;

		/**
		 * Sets the size of the window's content area.
		 *
		 * The final width and height will be clamped based on the minimum and maximum size of the window.
		 *
		 * @param width The new width of the window's content area in pixel.
		 * @param height The new height of the widget's content area in pixel.
		 */
		void setSize(int32_t width, int32_t height) override;

		/**
		 * Sets the visibility of the window.
		 *
		 * @param visible Whether the window will be visible or not.
		 */
		void setVisible(bool visible) override;

		/**
		 * Enables or disables the window.
		 *
		 * @param enabled Whether the window will be enabled or disabled.
		 */
		void setEnabled(bool enabled) override;

		/**
		 * Sets whether the window is focused or not.
		 *
		 * @param focused Whether the window will be focused or unfocused.
		 */
		void setFocused(bool focused) override;

	private:

		/**
		 * The handle to the win32 window.
		 */
		HWND m_hwnd = nullptr;

		friend class OMG::ApplicationWin32;
	};

}
