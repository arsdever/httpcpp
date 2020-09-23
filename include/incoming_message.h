#pragma once

#include <event.h>
#include <typedef.h>
#include <sockpp/stream_socket.h>
#include <sstream>
#include <iostream>

#define CB void(*cb)() = nullptr
#define number int

namespace http
{
	class incoming_message
	{
	private:
		incoming_message(incoming_message const&) = delete;
		incoming_message& operator =(incoming_message const&) = delete;

	public:
		incoming_message()
			: aborted(false)
			, complete(false)
		{}

		incoming_message(incoming_message&& o)
			: aborted(o.aborted)
			, complete(o.complete)
			, headers(std::move(o.headers))
			, http_version(std::move(o.http_version))
			, method(std::move(o.method))
			, rawHeaders(std::move(o.rawHeaders))
			, rawTailers(std::move(o.rawTailers))
			, socket(std::move(o.socket))
			, statusCode(std::move(o.statusCode))
			, statusMessage(std::move(o.statusMessage))
			, tailers(std::move(o.tailers))
			, url(std::move(o.url))
		{}

		void destroy();
		void setTimeout(number msec, CB);

		bool aborted;
		bool complete;
		headers_t headers;
		std::string http_version;
		std::string method;
		std::vector<std::string> rawHeaders;
		std::vector<std::string> rawTailers;
		sockpp::stream_socket socket;
		number statusCode;
		std::string statusMessage;
		headers_t tailers;
		std::string url;

		static incoming_message parse_from_raw(std::string const& buffer)
		{
			std::regex rgx("(\\r?\\n+)");
			std::sregex_token_iterator iter(buffer.begin(),
				buffer.end(),
				rgx,
				-1);
			std::sregex_token_iterator end;
			incoming_message msg;
			if (iter != end)
			{
				std::stringstream firstline(*iter);
				firstline >> msg.method >> msg.url >> msg.http_version;
			}

			for (++iter; iter != end; ++iter)
			{
				std::string line = std::string(*iter);
				if (line.empty())
					break;

				std::string header_key = line.substr(0, line.find(':'));
				std::string header_value = line.substr(line.find(' '));
				msg.headers.insert({ header_key, header_value });
				msg.rawHeaders.push_back(line);
			}

			/*std::string line;
			data_stream.getline()
			while (!line.empty())
			{
				std::string header_key = line.substr(0, line.size() - 1);
				std::string raw_header_key = line;
				data_stream >> line;
				std::string header_value = line;
				std::string raw_header_value = line;
				data_stream >> line;
				msg.headers.insert({ header_key, header_value });
				msg.rawHeaders.push_back(raw_header_key + " " + raw_header_value);
			}*/
			return std::move(msg);
		}

	public EVENTS:
		events::event<void> on_aborted;
		events::event<void> close;
	};
}