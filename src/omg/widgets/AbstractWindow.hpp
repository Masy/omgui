//
// Created by masy on 03.10.2023.
//

#pragma once

#include <string_view>
#include "omg/widgets/Widget.hpp"

namespace OMG::Widgets {

	enum WindowStyle {
		/**
		 * If this flag is set, the window will have a border around the content area.
		 */
		OMG_WINDOW_STYLE_BORDER = 0x01,
		/**
		 * If this flag is set, the window can be resized by the user.
		 */
		OMG_WINDOW_STYLE_RESIZABLE = 0x03,
		/**
		 * If this flag is set, the window will have it's title displayed in the title bar.
		 */
		OMG_WINDOW_STYLE_TITLE_BAR = 0x05,
		/**
		 * If this flag is set, the window will have a maximize button in the title bar.
		 */
		OMG_WINDOW_STYLE_MAXIMIZE_BOX = 0x08,
		/**
		 * If this flag is set, the window will have a minimize button in the title bar.
		 */
		OMG_WINDOW_STYLE_MINIMIZE_BOX = 0x10,
		/**
		 * If this flag is set, the window will be a popup.
		 */
		OMG_WINDOW_STYLE_POPUP = 0x20,
		/**
		 * If this flag is set, the window will be a context menu.
		 */
		OMG_WINDOW_STYLE_MENU = 0x40,
		/**
		 * The style of a standard window.
		 */
		OMG_WINDOW_STYLE_DEFAULT = (OMG_WINDOW_STYLE_TITLE_BAR | OMG_WINDOW_STYLE_RESIZABLE | OMG_WINDOW_STYLE_MAXIMIZE_BOX | OMG_WINDOW_STYLE_MINIMIZE_BOX)
	};

	/**
	 * Abstract base class for the operating dependent window widget.
	 */
	class AbstractWindow : public Widget {
	public:

		/**
		 * Adds the whole window the the render queue.
		 */
		void addToRenderQueue() override;

	protected:

		/**
		 * Creates a new abstract window widget.
		 */
		AbstractWindow();

	protected:

		/**
		 * Whether the window is a popup or not.
		 */
		bool m_isPopup = false;
		/**
		 * The content scale of the window.
		 *
		 * The size of al widgets in the window should be multiplied by the content scale.
		 */
		float m_contentScale = 1.0f;

	};

}
