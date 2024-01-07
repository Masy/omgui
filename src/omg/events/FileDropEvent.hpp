//
// Created by masy on 28.12.2023.
//

#pragma once

#include <vector>
#include <filesystem>
#include "omg/events/Event.hpp"

namespace OMG::Events {

	class FileDropEvent : public Event {
	public:

		FileDropEvent(int32_t cursorX, int32_t cursorY, std::vector<std::filesystem::path> &&paths);

		[[nodiscard]] int32_t cursorX() const;

		[[nodiscard]] int32_t cursorY() const;

		[[nodiscard]] const std::vector<std::filesystem::path> &paths() const;

		[[nodiscard]] std::vector<std::filesystem::path>::size_type size() const;

	private:

		int32_t m_cursorX;
		int32_t m_cursorY;
		std::vector<std::filesystem::path> m_paths;

	};

}
