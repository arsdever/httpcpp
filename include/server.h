#pragma once

#include <event.h>
#include <utils.h>
#include <sockpp/tcp_acceptor.h>
#include <incoming_message.h>
#include <server_response.h>

#define CB void(*cb)() = nullptr
#define number int

namespace http
{
	class server
	{
	public:
		server()
			: listening{ false }
		{
			on_connection += std::bind(&server::handle_connection, this, std::placeholders::_1);
		}

		void listen()
		{
			std::thread([&]() {
				while (1)
				{
					sockpp::acceptor acc = sockpp::tcp_acceptor(8080);
					if (!acc)
					{
#ifndef GLBOAL_NOTHROW
						throw std::logic_error("Unable to listen to the port");
#else
						return;
#endif
					}

					listening = true;
					sockpp::stream_socket sock = acc.accept(nullptr);
					on_connection(std::move(sock));
				}
				}).detach();
		}

		void close(CB) {}
		void setTimeout(number msec = 0, CB) {}

		number headersTimeout;
		number maxHeadersCount;
		number requestTimeout;
		number timeout;
		number keepAliveTimeout;
		bool listening;
		sockpp::socket_initializer sockInit;

	private:
		void handle_connection(sockpp::stream_socket&& socket)
		{
			std::async(std::launch::async, [&]() {
				ssize_t read_bytes;
				constexpr ssize_t max_read_bytes = 4096;
				std::array<char, max_read_bytes> buffer;
				std::string msg = "";
				while (read_bytes = socket.read(buffer.data(), max_read_bytes))
				{
					msg.append(buffer.data(), read_bytes);
					if (read_bytes < max_read_bytes)
					{
						incoming_message req{ incoming_message::parse_from_raw(msg) };
						server_response res(socket, req.http_version);
						on_request(req, res);
					}
				}
				});
		}

		void handle_request(incoming_message&& msg, sockpp::stream_socket&& socket)
		{
			http_method method = method_from_name(msg.method);
			switch (method)
			{
			case http_method::http_get:		on_get(std::move(msg), std::move(socket)); return;
			case http_method::http_post:	on_post(std::move(msg), std::move(socket)); return;
			case http_method::http_head:	on_head(std::move(msg), std::move(socket)); return;
			case http_method::http_put:		on_put(std::move(msg), std::move(socket)); return;
			case http_method::http_delete:	on_delete(std::move(msg), std::move(socket)); return;
			case http_method::http_connect: on_connect(std::move(msg), std::move(socket)); return;
			case http_method::http_options: on_options(std::move(msg), std::move(socket)); return;
			case http_method::http_trace:	on_trace(std::move(msg), std::move(socket)); return;
			case http_method::http_patch:	on_patch(std::move(msg), std::move(socket)); return;
			default:
#ifndef GLBOAL_NOTHROW
				throw std::logic_error("Unsupported method");
#else
				return;
#endif
			}
		}

	public:
		events::event<void> on_check_continue;
		events::event<void> on_check_exception;
		events::event<void> on_client_error;
		events::event<void> on_close;
		events::event<sockpp::stream_socket&&> on_connection;
		events::event<incoming_message&, server_response&> on_request;
		events::event<void> on_upgrade;

		events::event<incoming_message&&, sockpp::stream_socket&&> on_get;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_post;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_head;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_put;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_delete;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_connect;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_options;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_trace;
		events::event<incoming_message&&, sockpp::stream_socket&&> on_patch;
	};
}