// === TODO SECTION ===
// #1: Cookie header value may be a list.The headers_t should be changed to json like object
// #2: Consider the implicit header mode implementation
// #3: Think about the UTF-8 stuff
// ====================

#pragma once

#include <event.h>
#include <typedef.h>
#include <sockpp/stream_socket.h>
#include <iostream>

#define number int

using namespace events;

namespace http
{
	class server_response
	{
	private:
		using socket_t = sockpp::stream_socket;

	public:
		server_response(socket_t& socket, std::string const& httpVersion)
			: socket(socket)
			, finished(false)
			, headersSent(false)
			, sendDate(false)
			, statusCode(200)
			, statusMessage("OK")
			, writableEnded(false)
			, writableFinished(false)
			, httpVersion(httpVersion)
		{}

		// response.addTrailers(headers)
		void addTrailers(headers_t const& headers);

		// TODO: https://nodejs.org/api/http.html#http_response_cork
		// response.cork()
		// void cork();

		// response.end([data[, encoding][, callback])
		server_response& end()
		{
			writableEnded = true;
			// TODO: After flushing
			writableFinished = true;
			on_finish();
			return *this;
		}

		server_response& end(std::function<void()> callback)
		{
			callback();
			return end();
		}

		server_response& end(std::string const& data)
		{
			write(data);
			return end();
		}

		server_response& end(std::string const& data, std::string const& encoding)
		{
			write(data, encoding);
			return end();
		}

		server_response& end(std::string const& data, std::function<void()> callback)
		{
			write(data);
			return end(callback);
		}

		server_response& end(std::string const& data, std::string const& encoding, std::function<void()> callback)
		{
			write(data, encoding);
			return end(callback);
		}

		// response.flushHeaders()
		void flushHeaders();

		// TODO: Return type must be a variant https://nodejs.org/api/http.html#http_response_getheader_name
		// response.getHeader(name)
		std::string getHeader(std::string const& name);

		// response.getHeaderNames()
		std::unordered_set<std::string> getHeaderNames();

		// TODO #1
		// response.getHeaders()
		headers_t getHeaders();

		// response.hasHeader(name)
		bool hasHeader(std::string const& name)
		{
			return headers.find(name) != headers.end();
		}

		// response.removeHeader(name)
		void removeHeader(std::string const& name);

		// TODO #1
		// response.setHeader(name, value)
		void setHeader(std::string const& name, std::string const& value)
		{
			headers[name] = value;
		}

		// response.setTimeout(msecs[, callback])
		server_response& setTimeout(int msecs, std::function<void()> callback = {});

		// TODO: https://nodejs.org/api/http.html#http_response_uncork
		// response.uncork()
		// void uncork();

		// TODO #2
		// TODO #3
		// response.write(chunk[, encoding][, callback])
		bool write(std::string const& chunk)
		{
			return write(chunk, utf8, {});
		}

		bool write(std::string const& chunk, std::string const& unicode)
		{
			return write(chunk, unicode, {});
		}

		bool write(std::string const& chunk, std::function<void()> callback)
		{
			return write(chunk, utf8, callback);
		}

		bool write(std::string const& chunk, std::string const& unicode, std::function<void()> callback)
		{
			UNUSED(unicode);
			if (!hasHeader("Content-Length"))
			{
				setHeader("Content-Length", std::to_string(chunk.size()));
			}

			if (!headersSent)
				writeHead(200);

			int written = socket.write(chunk);

			if (callback) callback();

			return written == chunk.size();
		}

		// response.writeContinue()
		void writeContinue();

		// response.writeHead(statusCode[, statusMessage][, headers])
		server_response& writeHead(number statusCode)
		{
			return writeHead(statusCode, headers_t());
		}

		server_response& writeHead(number statusCode, std::string const& statusMessage)
		{
			return writeHead(statusCode, statusMessage, headers_t());
		}

		server_response& writeHead(number statusCode, headers_t const& headers)
		{
			return writeHead(statusCode, std::string(), headers);
		}

		server_response& writeHead(number statusCode, std::string const& statusMessage, headers_t const& headers)
		{
			socket.write(httpVersion + ' ' + std::to_string(statusCode) + ' ' + statusMessage + '\n');
			writeHeaders(headers);
			writeHeaders(this->headers);
			socket.write("\n");
			headersSent = true;
			return *this;
		}

		// response.writeProcessing()
		void writeProcessing();

		socket_t& socket;
		bool finished;
		bool headersSent;
		bool sendDate;
		number statusCode;
		std::string statusMessage;
		bool writableEnded;
		bool writableFinished;
		std::string httpVersion;

		event<> on_close;
		event<> on_finish;

	private:
		void writeHeaders(headers_t const& headers)
		{
			for (header_t h : headers)
				socket.write(h.first + ": " + h.second + '\n');
		}

		headers_t headers;
	};
}