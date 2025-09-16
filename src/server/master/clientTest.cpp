#include <iostream>
#include "../../bibs/httplib.h"

int main()
{
    httplib::Client cli("localhost", 8080);

    httplib::Params params;
    params.emplace("nome", "Malujoro");

    auto res = cli.Post("/info", R"({"nome":"Malujoro"})", "application/json");

    if (res)
    {
        std::cout << "Resposta do servidor: " << res->body << std::endl;
    }
    else
    {
        std::cout << "Falha na requisição" << std::endl;
    }
}
