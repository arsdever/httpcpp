#pragma once

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

    private:
        impl m_impl;
    };
}