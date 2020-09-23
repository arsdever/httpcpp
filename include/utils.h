#pragma once

namespace http
{
	const std::string http_method_names[] =
	{
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"DELETE",
		"CONNECT",
		"OPTIONS",
		"TRACE",
		"PATCH"
	};
	
	static const std::string utf8 = "utf-8";

	enum class http_method
	{
		http_get = 0,
		http_head = 1,
		http_post = 2,
		http_put = 3,
		http_delete = 4,
		http_connect = 5,
		http_options = 6,
		http_trace = 7,
		http_patch = 8,
		count
	};

	http_method method_from_name(std::string const& header)
	{
		for (int i = 0; i < (int)http_method::count; ++i)
		{
			if (http_method_names[i] == header)
				return (http_method)i;
		}
	}
}