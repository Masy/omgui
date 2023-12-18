//
// Created by masy on 27.09.2023.
//

#pragma once

namespace OMG {

	/**
	 * Abstract base class for the main application class.
	 */
	class AbstractApplication {
	public:

		/**
		 * Destroys the application.
		 */
		virtual ~AbstractApplication() = default;

		/**
		 * Starts the application.
		 *
		 * @param argc The number of program arguments.
		 * @param argv A pointer to the array of program arguments.
		 */
		virtual void start(int argc, const char **argv) = 0;

		/**
		 * Stops the application.
		 */
		virtual void stop() = 0;

	protected:

		/**
		 * Creates a new application.
		 */
		AbstractApplication() = default;

	};

}
