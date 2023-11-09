//
// Created by masy on 03.10.2023.
//

#include <algorithm>
#include <stdexcept>
#include "Widget.hpp"

using namespace OMG;

Widgets::Widget::Widget() = default;

Widgets::Widget::~Widget() noexcept = default;

int32_t Widgets::Widget::posX() const {
	return m_posX;
}

int32_t Widgets::Widget::posY() const {
	return m_posY;
}

void Widgets::Widget::setPosition(const int32_t posX, const int32_t posY) {
	if (m_posX == posX && m_posY == posY)
		return;

	m_posX = posX;
	m_posY = posY;

	addToRenderQueue();
}

int32_t Widgets::Widget::width() const {
	return m_width;
}

int32_t Widgets::Widget::height() const {
	return m_height;
}

void Widgets::Widget::setSize(int32_t width, int32_t height) {
	width = std::clamp(width, m_minWidth, m_maxWidth);
	height = std::clamp(height, m_minHeight, m_maxHeight);

	if (m_width == width && m_height == height)
		return;

	m_width = width;
	m_height = height;
	addToRenderQueue();
}

int32_t Widgets::Widget::minWidth() const {
	return m_minWidth;
}

int32_t Widgets::Widget::maxWidth() const {
	return m_maxWidth;
}

int32_t Widgets::Widget::minHeight() const {
	return m_minHeight;
}

int32_t Widgets::Widget::maxHeight() const {
	return m_maxHeight;
}

void Widgets::Widget::setSizeLimits(const int32_t minWidth, const int32_t maxWidth,
									const int32_t minHeight, const int32_t maxHeight) {
	if (minWidth > maxWidth)
		throw std::invalid_argument("failed to set size limits of widget:"
									"min width can't be greater than max width!");

	if (minHeight > maxHeight)
		throw std::invalid_argument("failed to set size limits of widget:"
									"min height can't be greater than max height!");

	m_minWidth = minWidth;
	m_maxWidth = maxWidth;
	m_minHeight = minHeight;
	m_maxHeight = maxHeight;

	// since the size limits have changed, we need to check if the widget needs to be resized to fit the new boundaries.
	// the setSize() method will clamp the width and height based on the limits again, and if the widget doesn't need to
	// be resized, it will early return and do nothing.
	setSize(m_width, m_height);
}

bool Widgets::Widget::visible() const {
	return m_visible;
}

void Widgets::Widget::setVisible(const bool visible) {
	if (m_visible == visible)
		return;

	m_visible = visible;
	addToRenderQueue();
}

bool Widgets::Widget::enabled() const {
	return m_enabled;
}

void Widgets::Widget::setEnabled(const bool enabled) {
	if (m_enabled == enabled)
		return;

	m_enabled = enabled;
	addToRenderQueue();
}

bool Widgets::Widget::focused() const {
	return m_focused;
}

void Widgets::Widget::setFocused(bool focused) {
	if (m_focused == focused)
		return;

	m_focused = focused;
	addToRenderQueue();
}

int64_t Widgets::Widget::tabIndex() const {
	return m_tabIndex;
}

void Widgets::Widget::setTabIndex(const int64_t tabIndex) {
	m_tabIndex = tabIndex;
}

int64_t Widgets::Widget::zOrder() const {
	return m_zOrder;
}

void Widgets::Widget::setZOrder(const int64_t zOrder) {
	if (m_zOrder == zOrder)
		return;

	addToRenderQueue();
}

InteractState Widgets::Widget::interactState() const {
	return m_interactState;
}

void Widgets::Widget::setInteractState(const OMG::InteractState interactState) {
	if (m_interactState == interactState)
		return;

	m_interactState = interactState;
	addToRenderQueue();
}

void Widgets::Widget::addToRenderQueue() {
	if (!m_visible || m_parent == nullptr || m_parent == this)
		return;

	return m_parent->addToRenderQueue();
}
