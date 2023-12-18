//
// Created by masy on 03.10.2023.
//

#pragma once

#include <cstdint>
#include <limits>
#include "omg/core/Constants.hpp"

namespace OMG::Widgets {

	/**
	 * Base class for all user interface elements.
	 */
	class Widget {
	public:

		/**
		 * Destroys the widget.
		 */
		virtual ~Widget() noexcept;

		/**
		 * Gets the x coordinate of the widget.
		 *
		 * If the widget is a window, the position is relative to the desktop.
		 *
		 * @return The x coordinate of the widget relative to the parent.
		 */
		[[nodiscard]] int32_t posX() const;

		/**
		 * Gets the y coordinate of the widget.
		 *
		 * If the widget is a window, the position is relative to the desktop.
		 *
		 * @return The y coordinate of the widget relative to the parent.
		 */
		[[nodiscard]] int32_t posY() const;

		/**
		 * Sets the position of the widget.
		 *
		 * If the widget is a window, the position will be the relative to the desktop.
		 *
		 * @param posX The new x coordinate of the widget relative to the parent.
		 * @param posY The new y coordinate of the widget relative to the parent.
		 */
		virtual void setPosition(int32_t posX, int32_t posY);

		/**
		 * Moves the widget by the given offset.
		 *
		 * @param offsetX The offset by which the widget will be moved along the x axis.
		 * @param offsetY The offset by which the widget will be moved along the y axis.
		 * @see setPosition(int32_t, int32_t)
		 */
		inline void move(const int32_t offsetX, const int32_t offsetY) {
			setPosition(m_posX + offsetX, m_posY + offsetY);
		}

		/**
		 * Sets the x coordinate of the widget.
		 *
		 * @param posX The new x coordinate of the widget relative to the parent.
		 * @see setPosition(int32_t, int32_t)
		 */
		inline void setPosX(const int32_t posX) {
			setPosition(posX, m_posY);
		}

		/**
		 * Moves the widget along the x axis by the given offset.
		 *
		 * @param offsetX The offset by which the widget will be moved along the x axis.
		 * @see setPosition(int32_t, int32_t)
		 */
		inline void moveX(const int32_t offsetX) {
			setPosition(m_posX + offsetX, m_posY);
		}

		/**
		 * Sets the y coordinate of the widget.
		 *
		 * @param posY The new y coordinate of the widget relative to the parent.
		 * @see setPosition(int32_t, int32_t)
		 */
		inline void setPosY(const int32_t posY) {
			setPosition(m_posX, posY);
		}

		/**
		 * Moves the widget along the y axis by the given offset.
		 *
		 * @param offsetY The offset by which the widget will be moved along the y axis.
		 */
		inline void moveY(const int32_t offsetY) {
			setPosition(m_posX, m_posY + offsetY);
		}

		/**
		 * Gets the width of the widget.
		 *
		 * @return The width of the widget in pixel.
		 */
		[[nodiscard]] int32_t width() const;

		/**
		 * Gets the height of the widget.
		 *
		 * @return The height of the widget in pixel.
		 */
		[[nodiscard]] int32_t height() const;

		/**
		 * Sets the size of the widget.
		 *
		 * The final width and height will be clamped based on the minimum and maximum size of the widget.
		 *
		 * @param width The new width of the widget in pixel.
		 * @param height The new height of the widget in pixel.
		 */
		virtual void setSize(int32_t width, int32_t height);

		/**
		 * Sets the width of the widget.
		 *
		 * @param width The new width of the widget in pixel.
		 * @see setSize(int32_t, int32_t)
		 */
		inline void setWidth(const int32_t width) {
			setSize(width, m_height);
		}

		/**
		 * Sets the height of the widget.
		 *
		 * @param height The new height of the widget in pixel.
		 * @see setSize(int32_t, int32_t)
		 */
		inline void setHeight(const int32_t height) {
			setSize(m_width, height);
		}

		/**
		 * Gets the minimum width of the widget.
		 *
		 * @return The minimum width of the widget in pixel.
		 */
		[[nodiscard]] int32_t minWidth() const;

		/**
		 * Gets the maximum width of the widget in pixel.
		 *
		 * @return The maximum width of the widget in pixel.
		 */
		[[nodiscard]] int32_t maxWidth() const;

		/**
		 * Gets the minimum height of the widget in pixel.
		 *
		 * @return The minimum height of the widget in pixel.
		 */
		[[nodiscard]] int32_t minHeight() const;

		/**
		 * Gets the maximum height of the widget in pixel.
		 *
		 * @return The maximum height of the widget in pixel.
		 */
		[[nodiscard]] int32_t maxHeight() const;

		/**
		 * Sets the minimum and maximum size of the widget.
		 *
		 * If the current size of the widget exceeds the new boundaries, the widget will be resized.
		 *
		 * @param minWidth The minimum width of the widget in pixel.
		 * @param maxWidth The maximum width of the widget in pixel.
		 * @param minHeight The minimum height of the widget in pixel.
		 * @param maxHeight The maximum height of the widget in pixel.
		 * @see setSize(int32_t, int32_t)
		 * @throws std::invalid_argument if the minimum width is greater than the maximum width
		 * @throws std::invalid_argument if the minimum height is greater than the maximum height
		 */
		virtual void setSizeLimits(int32_t minWidth, int32_t maxWidth, int32_t minHeight, int32_t maxHeight);

		/**
		 * Sets the minimum width of the widget.
		 *
		 * @param minWidth The new minimum width of the widget in pixel.
		 * @see setSizeLimits(int32_t, int32_t, int32_t, int32_t)
		 */
		inline void setMinWidth(const int32_t minWidth) {
			setSizeLimits(minWidth, m_maxWidth, m_minHeight, m_maxHeight);
		}

		/**
		 * Sets the maximum width of the widget.
		 *
		 * @param maxWidth The new maximum width of the widget in pixel.
		 * @see setSizeLimits(int32_t, int32_t, int32_t, int32_t)
		 */
		inline void setMaxWidth(const int32_t maxWidth) {
			setSizeLimits(m_minWidth, maxWidth, m_minHeight, m_maxHeight);
		}

		/**
		 * Sets the minimum and maximum width of the widget.
		 *
		 * @param minWidth The new minimum width of the widget in pixel.
		 * @param maxWidth The new maximum width of the widget in pixel.
		 * @see setSizeLimits(int32_t, int32_t, int32_t, int32_t)
		 */
		inline void setWidthLimits(const int32_t minWidth, const int32_t maxWidth) {
			setSizeLimits(minWidth, maxWidth, m_minHeight, m_maxHeight);
		}

		/**
		 * Sets the minimum height of the widget.
		 *
		 * @param minHeight The new minimum height of the widget in pixel.
		 * @see setSizeLimits(int32_t, int32_t, int32_t, int32_t)
		 */
		inline void setMinHeight(const int32_t minHeight) {
			setSizeLimits(m_minWidth, m_maxWidth, minHeight, m_maxHeight);
		}

		/**
		 * Sets the maximum height of the widget.
		 *
		 * @param maxHeight The new maximum height of the widget in pixel.
		 * @see setSizeLimits(int32_t, int32_t, int32_t, int32_t)
		 */
		inline void setMaxHeight(const int32_t maxHeight) {
			setSizeLimits(m_minWidth, m_maxWidth, m_minHeight, maxHeight);
		}

		/**
		 * Sets the minimum and maximum height of the widget.
		 *
		 * @param minHeight The new minimum height of the widget in pixel.
		 * @param maxHeight The new maximum height of the widget in pixel.
		 * @see setSizeLimits(int32_t, int32_t, int32_t, int32_t)
		 */
		inline void setHeightLimits(const int32_t minHeight, const int32_t maxHeight) {
			setSizeLimits(m_minWidth, m_maxWidth, minHeight, maxHeight);
		}

		/**
		 * Checks if the widget is visible or not.
		 *
		 * If the widget isn't visible, it won't receive input events from the window system.
		 *
		 * @return Whether the widget is visible or not.
		 */
		[[nodiscard]] bool visible() const;

		/**
		 * Sets the visibility of the widget.
		 *
		 * @param visible Whether the widget will be visible or not.
		 */
		virtual void setVisible(bool visible);

		/**
		 * Convenience method to make the widget visible.
		 *
		 * @see visible(bool)
		 */
		inline void show() {
			setVisible(true);
		}

		/**
		 * Convenience method to make the widget invisible.
		 *
		 * @see visible(bool)
		 */
		inline void hide() {
			setVisible(false);
		}

		/**
		 * Checks if the widget is enabled or not.
		 *
		 * If the widget is disabled, it won't receive input events from the window system but still be rendered.
		 *
		 * @return Whether the widget is enabled or not.
		 */
		[[nodiscard]] bool enabled() const;

		/**
		 * Enables or disables the widget.
		 *
		 * @param enabled Whether the widget will be enabled or disabled.
		 */
		virtual void setEnabled(bool enabled);

		/**
		 * Convenience method to enable the widget.
		 *
		 * @see setEnabled(bool)
		 */
		inline void enable() {
			setEnabled(true);
		}

		/**
		 * Convenience method to disable the widget.
		 *
		 * @see setEnabled(bool)
		 */
		inline void disable() {
			setEnabled(false);
		}

		/**
		 * Checks whether the widget is focused or not.
		 *
		 * A focused widget will receive keyboard input events.
		 *
		 * @return Whether the widget is focused or not.
		 */
		[[nodiscard]] bool focused() const;

		/**
		 * Sets whether the widget is focused or not.
		 *
		 * @param focused Whether the widget will be focused or unfocused.
		 */
		virtual void setFocused(bool focused);

		/**
		 * Convenience method to focuses the widget.
		 *
		 * @see setFocused(bool)
		 */
		inline void focus() {
			setFocused(true);
		}

		/**
		 * Convenience method to unfocus the widget.
		 *
		 * @see setFocus(bool)
		 */
		inline void unfocus() {
			setFocused(false);
		}

		/**
		 * Gets the tab index of the widget.
		 *
		 * The tab index determines the order in which the focus is cycled when pressing the <code>tab</code> key.
		 *
		 * @return The tab index of the widget.
		 */
		[[nodiscard]] int64_t tabIndex() const;

		/**
		 * Sets the tab index of the widget.
		 *
		 * @param tabIndex The new tab index of the widget.
		 */
		virtual void setTabIndex(int64_t tabIndex);

		/**
		 * Gets the z-order of the widget.
		 *
		 * The z-order determines the order in which widgets are rendered.
		 *
		 * @return The z order of the widget.
		 */
		[[nodiscard]] int64_t zOrder() const;

		/**
		 * Sets the z-order of the widget.
		 *
		 * @param zOrder The new z-order of the widget.
		 */
		virtual void setZOrder(int64_t zOrder);

		/**
		 * Gets the interact state of the widget.
		 *
		 * @return The interact state of the widget.
		 */
		[[nodiscard]] InteractState interactState() const;

		/**
		 * Sets the interact state of the widget.
		 *
		 * @param interactState The new interact state of the widget.
		 */
		virtual void setInteractState(OMG::InteractState interactState);

		/**
		 * Adds the widget to the render queue of the window it is currently displayed in.
		 */
		virtual void addToRenderQueue();

	protected:

		/**
		 * Creates a new widget.
		 */
		Widget();

	protected:

		/**
		 * A pointer to the parent widget.
		 */
		Widget *m_parent = nullptr;
		/**
		 * The x coordinate of the widget relative to the parent.
		 */
		int32_t m_posX = 0;
		/**
		 * The y coordinate of the widget relative to the parent.
		 */
		int32_t m_posY = 0;
		/**
		 * The width of the widget in pixel.
		 */
		int32_t m_width = 80;
		/**
		 * The height of the widget in pixel.
		 */
		int32_t m_height = 24;
		/**
		 * The minimum width of the widget in pixel.
		 */
		int32_t m_minWidth = 0;
		/**
		 * The maximum width of the widget in pixel.
		 */
		int32_t m_maxWidth = (std::numeric_limits<int32_t>::max)();
		/**
		 * The minimum height of the widget in pixel.
		 */
		int32_t m_minHeight = 0;
		/**
		 * The maximum height of the widget in pixel.
		 */
		int32_t m_maxHeight = (std::numeric_limits<int32_t>::max)();
		/**
		 * Whether the widget is currently visible or not.
		 *
		 * If the parent or any parent up the chain is invisible, this widget will be treated as invisible as well,
		 * even if the bool is <code>true</code>.
		 *
		 * Invisible widgets aren't displayed in the window and won't receive input events from the window system.
		 */
		bool m_visible = true;
		/**
		 * Whether the widget is currently enabled or not.
		 *
		 * If the parent or any parent up the chain in disabled, this widget will be treated as disabled as well,
		 * even if the bool is <code>true</code>.
		 *
		 * Disabled widgets won't receive input events from the window system, even if they are visible.
		 */
		bool m_enabled = true;
		/**
		 * Whether the widget is currently focused or not.
		 *
		 * Focused widgets will receive keyboard events and mouse move events, regardless if the mouse was actually
		 * moved over the widget or not.
		 *
		 * When a widget is about to be focused, the currently focused widget will lose it's focus first, before the new
		 * widget is focused.
		 */
		bool m_focused = false;
		/**
		 * The tab index of the widget.
		 *
		 * The tab index determines the order in which the focus of widgets is cycled inside a parent widget.
		 */
		int64_t m_tabIndex = 0;
		/**
		 * The z order of the widget.
		 *
		 * The z order determines the order in which widgets are rendered.
		 * Widgets with a lower z order are rendered first, while widgets with a higher z order are rendered
		 * on top of the others.
		 */
		int64_t m_zOrder = 0;
		/**
		 * The current interact state of the widget.
		 *
		 * The interact state determines whether the widget is currently hovered, pressed down none of the previous states.
		 */
		InteractState m_interactState = OMG_INTERACT_DEFAULT;

	};

}
