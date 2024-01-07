//
// Created by masy on 28.12.2023.
//

#include "FileDropEvent.hpp"

using namespace OMG::Events;

FileDropEvent::FileDropEvent(const int32_t cursorX, const int32_t cursorY, std::vector<std::filesystem::path> &&paths)
		: m_cursorX(cursorX), m_cursorY(cursorY), m_paths(std::move(paths)) {}

int32_t FileDropEvent::cursorX() const {
	return m_cursorX;
}

int32_t FileDropEvent::cursorY() const {
	return m_cursorY;
}

const std::vector<std::filesystem::path> &FileDropEvent::paths() const {
	return m_paths;
}

std::vector<std::filesystem::path>::size_type FileDropEvent::size() const {
	return m_paths.size();
}
