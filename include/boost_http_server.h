#include <basic_http_server.h>
#include <boost_http_server_impl.h>

namespace httpcpp
{
using boost_http_server = basic_http_server<boost_http_server_impl, boost_http_server_impl::context_t&>;
}