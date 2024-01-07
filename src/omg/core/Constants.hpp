//
// Created by masy on 03.10.2023.
//

#pragma once

#include <cstdint>

namespace OMG {

	/**
	 * Enum describing the current interaction state of a widget.
	 */
	enum InteractState : uint32_t {
		/**
		 * State for when the widget is neither hovered or pressed.
		 */
		OMG_INTERACT_DEFAULT,
		/**
		 * State for when the widget is hovered.
		 */
		OMG_INTERACT_HOVERED,
		/**
		 * State for when the widget is pressed.
		 */
		OMG_INTERACT_PRESSED
	};

	/**
	 * Enum describing the current state of a window.
	 */
	enum WindowState : uint32_t {
		/**
		 * State for when the window is neither maximized or minimized.
		 */
		OMG_WINDOW_WINDOWED,
		/**
		 * State for when the window minimized (also sometimes known as iconized).
		 */
		OMG_WINDOW_MINIMIZED,
		/**
		 * State for when the window is maximized.
		 */
		OMG_WINDOW_MAXIMIZED,
	};

	/**
	 * Enum describing the state of a keyboard key or mouse button.
	 */
	enum KeyState : uint32_t {
		/**
		 * State for when the key or button was released.
		 */
		OMG_RELEASED,
		/**
		 * State for when the key was just pressed.
		 */
		OMG_PRESSED,
		/**
		 * State for when the key is held down.
		 */
		OMG_REPEAT
	};

	/**
	 * Enum describing modifiers that can be toggled while performing key actions.
	 */
	enum KeyModifier : uint32_t {
		/**
		 * Modifier bit for the shift-key.
		 */
		OMG_MOD_SHIFT = 0x01,
		/**
		 * Modifier bit for the control-key.
		 */
		OMG_MOD_CONTROL = 0x02,
		/**
		 * Modifier bit for the alt-key.
		 */
		OMG_MOD_ALT = 0x04,
		/**
		 * Modifier bit for the super-key.
		 *
		 * On Windows, this is known as the windows-key.
		 */
		OMG_MOD_SUPER = 0x08,
		/**
		 * Modifier bit for the caps lock.
		 */
		OMG_MOD_CAPS_LOCK = 0x10,
		/**
		 * Modifier bit for the num lock.
		 */
		OMG_MOD_NUM_LOCK = 0x20
	};

	/**
	 * Key code of an unknown key.
	 */
	inline constexpr uint32_t OMG_KEY_UNKNOWN = 0xFFFFFFFFu;
	/**
	 * Key code for the left mouse button.
	 */
	inline constexpr uint32_t OMG_MB_LEFT = 0x00u;
	/**
	 * Key code for the right mouse button.
	 */
	inline constexpr uint32_t OMG_MB_RIGHT = 0x01u;
	/**
	 * Key code for the middle mouse button.
	 */
	inline constexpr uint32_t OMG_MB_MIDDLE = 0x02u;
	/**
	 * Key code for the forward button on a mouse.
	 */
	inline constexpr uint32_t OMG_MB_FORWARD = 0x03u;
	/**
	 * Key code for the backward button on a mouse.
	 */
	inline constexpr uint32_t OMG_MB_BACKWARD = 0x04u;

}
