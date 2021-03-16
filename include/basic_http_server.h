#pragma once

#include <event.h>

namespace httpcpp
{
    template <typename impl>
    class basic_http_server
    {
    public:
        using backlog_t = typename impl::backlog_t;
        using callback_t = typename impl::callback_t;
        using context_t = typename impl::context_t;
        using host_t = typename impl::host_t;
        using port_t = typename impl::port_t;

    public:
        basic_http_server(context_t& context)
            : m_impl(context)
            , headers_timeout(m_impl.headers_timeout)
            , listening(m_impl.listening)
            , max_headers_count(m_impl.max_headers_count)
            , request_timeout(m_impl.request_timeout)
            , timeout(m_impl.timeout)
            , keep_alive_timeout(m_impl.keep_alive_timeout)
            , evt_check_continue(m_impl.evt_check_continue)
            , evt_check_expectation(m_impl.evt_check_expectation)
            , evt_client_error(m_impl.evt_client_error)
            , evt_close(m_impl.evt_close)
            , evt_connection(m_impl.evt_connection)
            , evt_request(m_impl.evt_request)
            , evt_upgrade(m_impl.evt_upgrade)
        {}

        void listen(callback_t callback = callback_t())
        {
            listen(impl::default_port_value, callback);
        }

        void listen(port_t port, callback_t callback = callback_t())
        {
            listen(port, impl::default_host_value, callback);
        }

        void listen(port_t port, host_t host, callback_t callback = callback_t())
        {
            listen(port, host, impl::default_backlog_value, callback);
        }

        void listen(port_t port, host_t host, backlog_t backlog, callback_t callback = callback_t())
        {
            m_impl.listen(port, host, backlog, callback);
        }

        void close(callback_t callback = callback_t())
        {
            m_impl.close(callback);
        }

    public:
        long& headers_timeout;
        bool& listening;
        long& max_headers_count;
        long& request_timeout;
        long& timeout;
        long& keep_alive_timeout;

        event<>& evt_check_continue;
        event<>& evt_check_expectation;
        event<>& evt_client_error;
        event<>& evt_close;
        event<>& evt_connection;
        event<>& evt_request;
        event<>& evt_upgrade;

    private:
        impl m_impl;
    };
}