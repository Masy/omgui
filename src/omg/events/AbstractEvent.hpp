//
// Created by masy on 18.12.2023.
//

#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <typeindex>
#include <functional>

namespace OMG::Widgets {
	class Widget;
}

namespace OMG::Events {

	class AbstractEvent : public std::enable_shared_from_this<AbstractEvent> {
	public:

		typedef std::multimap<std::type_index, std::function<void(Widgets::Widget *, const std::shared_ptr<AbstractEvent> &)>> handlerMultiMap_t;

		static std::unordered_map<Widgets::Widget*, handlerMultiMap_t> eventHandlers;

	public:

		virtual ~AbstractEvent();

		virtual void emit(Widgets::Widget *widget) = 0;

		virtual void emitAsync(Widgets::Widget *widget) = 0;

		[[nodiscard]] bool cancelled() const;

		void setCancelled(bool cancelled);

	private:

		bool m_cancelled = false;

	};

	/**
	 * Struct to extract the type of the second argument of a function.
	 *
	 * This is used for event handlers to get the type of the event that is handled by the function.
	 */
	template <typename>
	struct extractSecondArgument;

	/**
	 * Extracts the type of the second argument of a function.
	 *
	 * @tparam A The type of the functions return value.
	 * @tparam B The type of the functions first parameter.
	 * @tparam C The type of the functions second parameter.
	 */
	template <typename A, typename B, typename C>
	struct extractSecondArgument<A (*)(B, C)> {
		using type = std::remove_reference_t<C>::element_type;
	};

	/**
	 * Extracts the type of the second argument of a std::function.
	 *
	 * @tparam A The type of the std::function's return value.
	 * @tparam B The type of the std::function's first parameter.
	 * @tparam C The type of the std::function's second parameter.
	 */
	template <typename A, typename B, typename C>
	struct extractSecondArgument<std::function<A(B, C)>> {
		using type = std::remove_reference_t<C>::element_type;
	};

	/**
	 * Extracts the type of the second argument of a lambda function.
	 *
	 * @tparam T The type of the lambda function.
	 */
	template <typename T>
	struct extractSecondArgument {
		/**
		 * Extracts the type of the second argument of a constant lambda function.
		 *
		 * @tparam P The type of the captured arguments.
		 * @tparam A The type of the lambda's return value.
		 * @tparam B The type of the lambda's first parameter.
		 * @tparam C The type of the lambda's second parameter.
		 * @param op The captured arguments.
		 * @return a reference to the second parameter of the lambda.
		 */
		template <typename P, typename A, typename B, typename C>
		static C& extractSecondArgumentFromLambda(A (P::* op)(B, C) const);

		/**
		 * Extracts the type of the second argument of a lambda function.
		 *
		 * @tparam P The type of the captured arguments.
		 * @tparam A The type of the lambda's return value.
		 * @tparam B The type of the lambda's first parameter.
		 * @tparam C The type of the lambda's second parameter.
		 * @param op The captured arguments.
		 * @return a reference to the second parameter of the lambda.
		 */
		template <typename P, typename A, typename B, typename C>
		static C& extractSecondArgumentFromLambda(A (P::* op)(B, C));

		using type = std::remove_reference_t<decltype(extractSecondArgumentFromLambda(&T::operator()))>::element_type;
	};

	/**
	 * Type declaration for the type of an extracted second argument of a function.
	 */
	template <typename T>
	using extractSecondArgument_t = typename extractSecondArgument<T>::type;

	template<typename Widget_t, typename F>
	void connect(Widget_t *widget, F func) {
		static_assert(std::derived_from<Widget_t, Widgets::Widget>, "Widget_t must be derived from the widget class");
		static_assert(std::derived_from<extractSecondArgument_t<F>, AbstractEvent>, "Event type of handler must be derived from the event class");

		auto wrapper = [=] (Widgets::Widget *widget, const std::shared_ptr<AbstractEvent> &event) {
			func(dynamic_cast<Widget_t*>(widget), std::dynamic_pointer_cast<extractSecondArgument_t<F>>(event));
		};
		AbstractEvent::eventHandlers[(Widgets::Widget*) widget].insert({std::type_index(typeid(extractSecondArgument_t<F>)), wrapper});
	}

	template<typename Widget_t, typename Event_t>
	void connect(Widget_t *widget, void (Widget_t::*func)(const std::shared_ptr<Event_t> &)) {
		static_assert(std::derived_from<Widget_t, Widgets::Widget>, "Widget_t must be derived from the widget class");
		static_assert(std::derived_from<Event_t, AbstractEvent>, "Event_t must be derived from the event class");

		auto wrapper = [=] (Widgets::Widget *widget, const std::shared_ptr<AbstractEvent> &event) {
			(reinterpret_cast<Widget_t*>(widget)->*func)(std::dynamic_pointer_cast<Event_t>(event));
		};
		AbstractEvent::eventHandlers[(Widgets::Widget*) widget].insert({std::type_index(typeid(Event_t)), wrapper});
	}

	void disconnect(Widgets::Widget *widget);

}
