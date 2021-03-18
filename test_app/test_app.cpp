#include <boost_http_server.h>
#include <error.h>
#include <iostream>
#include <thread>

int main()
{
	boost::asio::io_context		  context;
	boost::asio::io_service::work work(context);
	httpcpp::boost_http_server	  server(context);

	server.close([](std::optional<httpcpp::error> const& e) {
		if (e)
			{
				// has error
				std::cout << "Error" << std::endl;
			}
	});
	server.listen(8080);
	server.close([](std::optional<httpcpp::error> const& e) {
		if (e)
			{
				// has error
				std::cout << "Error" << std::endl;
			}
	});

	context.run();
	return 0;
}