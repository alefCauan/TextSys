#include "http_client.h"
#include <stdexcept>
#include <sstream>

// Dependência: single-header `httplib.h` (cpp-httplib). Coloque em client/include/httplib.h
#include "../bibs/httplib.h"

std::string post_text_and_get_json( 
    const std::string &host, 
    int port, 
    const std::string &path, 
    const std::string &body
) {
    // construímos o client com host e porta
    httplib::Client cli(host.c_str(), port);
    cli.set_read_timeout(5, 0); // 5s
    cli.set_write_timeout(5, 0);

    auto res = cli.Post(path.c_str(), body, "text/plain");
    if (!res)
    {
        std::ostringstream oss;
        oss << "no response (network error or connection refused) to " << host << ":" << port;
        throw std::runtime_error(oss.str());
    }

    if (res->status != 200)
    {
        std::ostringstream oss;
        oss << "unexpected status " << res->status << ": " << res->body;
        throw std::runtime_error(oss.str());
    }

    return res->body;
}