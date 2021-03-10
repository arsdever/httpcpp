#include <boost_http_server.h>
#include <thread>

int main()
{
	boost::asio::io_context context;
	boost::asio::io_service::work work(context);
	httpcpp::boost_http_server server(context);

	server.listen(8080);

	context.run();
	return 0;
}