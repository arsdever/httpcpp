#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <optional>

#include <boost/asio.hpp>

#include <error.h>

namespace httpcpp
{
    class boost_http_server_impl
    {
    public:
        using callback_t = std::function<void(std::optional<error> const&)>;
        using port_t = unsigned short;
        using host_t = std::string;
        using backlog_t = int;
        using context_t = boost::asio::io_context;

        static constexpr port_t default_port_value = 0;
        static constexpr char const* default_host_value = "0.0.0.0";
        static constexpr backlog_t default_backlog_value = 511;

    public:
        boost_http_server_impl(context_t& context)
            : m_serverAcceptor(context)
        {}

        void listen(port_t port, host_t host, backlog_t backlog, callback_t callback = callback_t())
        {
            boost::system::error_code make_address_error_code;
            boost::asio::ip::address address = boost::asio::ip::make_address(host, make_address_error_code);
            if (make_address_error_code)
            {
                std::cerr << make_address_error_code.message() << std::endl;
            }

            boost::asio::ip::tcp::endpoint ep{address, port};
            m_serverAcceptor.open(ep.protocol());
            m_serverAcceptor.bind(ep);
            m_serverAcceptor.listen();
            start_accepting();
        }

        void close(callback_t callback = callback_t())
        {
            if (!m_serverAcceptor.is_open())
            {
                if (callback)
                    callback(error());
                return;
            }

            m_serverAcceptor.close();
        }

    private:
        void start_accepting()
        {
            m_serverAcceptor.async_accept(
                std::bind(
                    &boost_http_server_impl::handle_accept, this,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
        }

        void handle_accept(boost::system::error_code const& ec, boost::asio::ip::tcp::socket sock)
        {
            if (!ec)
            {
                // Session established
                start_accepting();
            }
            else
            {
                std::cerr << ec.message() << std::endl;
                // Session was not established
            }
        }

    private:
        boost::asio::ip::tcp::acceptor m_serverAcceptor;
    };
}