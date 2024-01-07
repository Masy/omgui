//
// Created by masy on 05.10.2023.
//

#include <stdexcept>
#include <string>
#include "omg/widgets/WindowWin32.hpp"

using namespace OMG::Widgets;

WindowWin32::WindowWin32(const std::string_view &title, const uint32_t style) : AbstractWindow() {
	m_visible = false;
	LONG dwStyle = 0;
	LONG exStyle = 0;
	if (style & OMG_WINDOW_STYLE_RESIZABLE)
		dwStyle |= WS_THICKFRAME;
	else if (style & OMG_WINDOW_STYLE_BORDER)
		dwStyle |= WS_BORDER;

	if (style & OMG_WINDOW_STYLE_TITLE_BAR)
		dwStyle |= WS_CAPTION | WS_SYSMENU;
	if (style & OMG_WINDOW_STYLE_MAXIMIZE_BOX)
		dwStyle |= WS_MAXIMIZEBOX | WS_SYSMENU;
	if (style & OMG_WINDOW_STYLE_MINIMIZE_BOX)
		dwStyle |= WS_MINIMIZEBOX | WS_SYSMENU;

	if (style & OMG_WINDOW_STYLE_MENU) {
		dwStyle |= WS_POPUP;
		exStyle |= WS_EX_NOACTIVATE;
		m_isPopup = true;
	} else if (style & OMG_WINDOW_STYLE_POPUP) {
		dwStyle |= WS_POPUP;
		exStyle |= WS_EX_ACCEPTFILES;
		m_isPopup = true;
	} else {
		exStyle |= WS_EX_ACCEPTFILES;
	}

	m_width = m_isPopup ? 1 : 854;
	m_height = m_isPopup ? 1 : 480;

	RECT clientArea{};
	clientArea.top = 0;
	clientArea.left = 0;
	clientArea.bottom = m_height;
	clientArea.right = m_width;

	if (AdjustWindowRectEx(&clientArea, dwStyle, false, 0) == 0)
		throw std::runtime_error("Failed to calculate size of content area!");

	int wideStrLen = MultiByteToWideChar(CP_UTF8, 0, title.data(), -1, nullptr, 0);
	std::wstring wTitle(wideStrLen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, title.data(), -1, wTitle.data(), wideStrLen);

	// we need to initialize the application before creating the window
	OMG::ApplicationWin32::getInstance();

	m_hwnd = CreateWindowExW(
			0,
			L"omGUI",
			(style & OMG_WINDOW_STYLE_TITLE_BAR) ? wTitle.data() : nullptr,
			0,
			CW_USEDEFAULT, CW_USEDEFAULT, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top,
			nullptr,
			nullptr,
			GetModuleHandleW(nullptr), // hinstance
			this);

	if (m_hwnd == nullptr) {
		throw std::runtime_error("Failed to create window handle! " + std::to_string(GetLastError()));
	}

	SetWindowLongW(m_hwnd, GWL_STYLE, dwStyle);
	SetWindowLongW(m_hwnd, GWL_EXSTYLE, exStyle);

	m_contentScale = (float) GetDpiForWindow(m_hwnd) / USER_DEFAULT_SCREEN_DPI;
}

WindowWin32::WindowWin32(const std::wstring_view &title, const uint32_t style) : AbstractWindow() {
	m_visible = false;
	LONG dwStyle = 0;
	LONG exStyle = 0;
	if (style & OMG_WINDOW_STYLE_RESIZABLE)
		dwStyle |= WS_THICKFRAME;
	else if (style & OMG_WINDOW_STYLE_BORDER)
		dwStyle |= WS_BORDER;

	if (style & OMG_WINDOW_STYLE_TITLE_BAR)
		dwStyle |= WS_CAPTION | WS_SYSMENU;
	if (style & OMG_WINDOW_STYLE_MAXIMIZE_BOX)
		dwStyle |= WS_MAXIMIZEBOX | WS_SYSMENU;
	if (style & OMG_WINDOW_STYLE_MINIMIZE_BOX)
		dwStyle |= WS_MINIMIZEBOX | WS_SYSMENU;

	if (style & OMG_WINDOW_STYLE_MENU) {
		dwStyle |= WS_POPUP;
		exStyle |= WS_EX_NOACTIVATE;
		m_isPopup = true;
	} else if (style & OMG_WINDOW_STYLE_POPUP) {
		dwStyle |= WS_POPUP;
		exStyle |= WS_EX_ACCEPTFILES;
		m_isPopup = true;
	} else {
		exStyle |= WS_EX_ACCEPTFILES;
	}

	m_width = m_isPopup ? 1 : 854;
	m_height = m_isPopup ? 1 : 480;

	RECT clientArea{};
	clientArea.top = 0;
	clientArea.left = 0;
	clientArea.bottom = m_height;
	clientArea.right = m_width;

	if (AdjustWindowRectEx(&clientArea, dwStyle, false, 0) == 0)
		throw std::runtime_error("Failed to calculate size of content area!");

	// we need to initialize the application before creating the window
	OMG::ApplicationWin32::getInstance();

	m_hwnd = CreateWindowExW(
			0,
			L"omGUI",
			(style & OMG_WINDOW_STYLE_TITLE_BAR) ? title.data() : nullptr,
			0,
			CW_USEDEFAULT, CW_USEDEFAULT, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top,
			nullptr,
			nullptr,
			GetModuleHandleW(nullptr),
			this);

	if (m_hwnd == nullptr) {
		throw std::runtime_error("Failed to create window handle! " + std::to_string(GetLastError()));
	}

	SetWindowLongW(m_hwnd, GWL_STYLE, dwStyle);
	SetWindowLongW(m_hwnd, GWL_EXSTYLE, exStyle);

	m_contentScale = (float) GetDpiForWindow(m_hwnd) / USER_DEFAULT_SCREEN_DPI;
}

void WindowWin32::setPosition(const int32_t posX, const int32_t posY) {
	RECT windowRect{};
	GetWindowRect(m_hwnd, &windowRect);
	MoveWindow(m_hwnd, windowRect.left + (posX - m_posX), windowRect.top + (posY - m_posY), m_width, m_height, false);
}

void WindowWin32::setSize(const int32_t width, const int32_t height) {
	RECT windowRect{};
	GetWindowRect(m_hwnd, &windowRect);

	MoveWindow(m_hwnd, windowRect.left, windowRect.bottom,
			   (windowRect.right - windowRect.left) + (width - m_width),
			   (windowRect.bottom - windowRect.top) + (height - m_height), true);
}

void WindowWin32::setVisible(const bool visible) {
	if (m_visible == visible)
		return;

	ShowWindow(m_hwnd, visible ? SW_SHOW : SW_HIDE);
}

void WindowWin32::setEnabled(const bool enabled) {
	if (m_enabled == enabled)
		return;

	EnableWindow(m_hwnd, enabled);
}

void WindowWin32::setFocused(const bool focused) {
	if (m_focused == focused)
		return;

	SetFocus(focused ? m_hwnd : nullptr);
}

void WindowWin32::minimize() {
	if (m_windowState == OMG_WINDOW_MINIMIZED)
		return;

	ShowWindow(m_hwnd, SW_MINIMIZE);
}

void WindowWin32::maximize() {
	if (m_windowState == OMG_WINDOW_MAXIMIZED)
		return;

	ShowWindow(m_hwnd, SW_MAXIMIZE);
}

void WindowWin32::restore() {
	if (m_windowState == OMG_WINDOW_WINDOWED)
		return;

	ShowWindow(m_hwnd, SW_RESTORE);
}

HWND WindowWin32::hwnd() const {
	return m_hwnd;
}
