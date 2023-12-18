//
// Created by masy on 09.11.2023.
//

#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif /* not defined NOMINMAX */
#include <Windows.h>
#include "omg/core/AbstractApplication.hpp"

namespace OMG {

	/**
	 * Application class for the windows operating system.
	 */
	class ApplicationWin32 : public AbstractApplication {
	public:

		/**
		 * Gets the singleton instance of the application class.
		 *
		 * @return A pointer to the singleton instance of the application class.
		 */
		static ApplicationWin32 *getInstance();

	public:

		/**
		 * Destroys the application.
		 */
		~ApplicationWin32() override;

		/**
		 * Starts the application.
		 *
		 * @param argc The number of program arguments.
		 * @param argv A pointer to the array of program arguments.
		 */
		void start(int argc, const char **argv) override;

		/**
		 * Stops the application.
		 */
		void stop() override;

	protected:

		/**
		 * Event handler for the win32 message system.
		 *
		 * @param hwnd The handle of the window that emitted the message.
		 * @param uMsg The id of the message that will be handled by the event handler.
		 * @param wParam The first parameter of the message.
		 * @param lParam The second parameter of the message.
		 * @return The status code of the handled message.
		 */
		static LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:

		/**
		 * Creates a new application instance.
		 *
		 * Since the application class is realized as a singleton, to obtain the instance of the singleton you have to
		 * call the {@link getInstance()} method.
		 */
		ApplicationWin32();

	};

}
