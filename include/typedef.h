#pragma once

#define UNUSED(var) ( ( void ) var )

namespace http
{
	class server_response;
	class client_request;

	using header_t = std::pair<std::string, std::string>;
	using headers_t = std::unordered_map<std::string, std::string>;
}