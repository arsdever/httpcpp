#include <http.h>
#include <thread>

using namespace events;
using namespace http;

int main()
{
	server svr;
	bool finished = false;
	svr.on_request += [&finished](incoming_message& req, server_response& res) {
		res.end("Hello world");
		finished = true;
	};

	svr.listen();
	while (!finished)
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	svr.close();
	return 0;
}