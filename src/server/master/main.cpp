#include <iostream>
#include "../../bibs/httplib.h"
#include "../../bibs/json.hpp"

using json = nlohmann::json;

void processarInfo(const std::string &body)
{
    // Parse JSON se quiser
    try
    {
        auto j = json::parse(body);
        std::cout << "Campo 'nome': " << j["nome"] << std::endl;
    }
    catch (...)
    {
        std::cout << "Não é JSON válido" << std::endl;
    }
}

int main()
{
    httplib::Server svr;

    // Define um Endpoint REST: POST /info
    svr.Post("/info", [](const httplib::Request &req, httplib::Response &res)
             {
        // req.body contém o corpo da requisição (ex.: JSON)
        std::cout << "Recebido do cliente: " << req.body << std::endl;

        processarInfo(req.body);

        res.set_content("Dados recebidos com sucesso", "text/plain"); });

    std::cout << "Servidor rodando em http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}
