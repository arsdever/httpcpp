#pragma once

#define EVENTS

namespace events
{
	template <typename ... A>
	class event
	{
	public:
		using event_handler = std::function<void(A...)>;

		//event(std::string const& name);

		event& operator +(event_handler h) { return operator += (h); }
		event& operator +(std::initializer_list<event_handler> hlist) { return operator += (hlist); }

		event& operator +=(event_handler h)
		{
			m_handlers.push_back(h);
			return *this;
		}

		event& operator +=(std::initializer_list<event_handler> hlist)
		{
			m_handlers.insert(m_handlers.end(), hlist.begin(), hlist.end());
			return *this;
		}

		event& operator = (event const&) = delete;
		event& operator = (event&&) = delete;

		void operator()(A ... args)
		{
			for (event_handler h : m_handlers)
				h(std::forward<A>(args)...);
		}

		std::vector<event_handler> m_handlers;
	};

	template <>
	class event<void>
	{
	public:
		using event_handler = std::function<void()>;

		//event(std::string const& name);

		event& operator +(event_handler h) { return operator += (h); }
		event& operator +(std::initializer_list<event_handler> hlist) { return operator += (hlist); }

		event& operator +=(event_handler h)
		{
			m_handlers.push_back(h);
			return *this;
		}

		event& operator +=(std::initializer_list<event_handler> hlist)
		{
			m_handlers.insert(m_handlers.end(), hlist.begin(), hlist.end());
			return *this;
		}

		void operator()()
		{
			for (event_handler h : m_handlers)
				h();
		}

		std::vector<event_handler> m_handlers;
	};
}