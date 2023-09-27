//
// Created by masy on 27.09.2023.
//

#pragma once

namespace OMG {

	class Application {
	public:

		static Application *getInstance();

		~Application();

		void start(int argc, const char **argv);

		void stop();

	private:

		Application();

	};

}
