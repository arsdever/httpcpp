# C++ HTTP library

## About
This library contains the implementation of HTTP in C++ inspired by
[NodeJS HTTP](https://nodejs.org/api/http.html) module. It is tempting bo be
easy in use (like NodeJS one does) and extensible. The current roadmap
only includes [expresscpp](https://github.com/arsdever/expresscpp) module implementation, which, as you
might have guessed, will be based on [NodeJS Express](https://expressjs.com/)
module

## Build
This library contains only headers. But an additional third-party library
[sockpp](https://github.com/fpagliughi/sockpp) is required when building
your application or library.

## Usage
You should just include `<http.h>` header and start using it. Here is
a very simple example of its usage:
```c++
#include <http.h>
#include <thread>

using namespace events;
using namespace http;

int main()
{
	server svr;
	bool finished;
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
```

## Notes

I try to keep the interface of the library very close to
the one existing on NodeJS. So, anybody with knowledge
in NodeJS HTTP can easily understand this library and
vice-versa.