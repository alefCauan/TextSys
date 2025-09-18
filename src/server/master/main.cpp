#include <iostream>
#include <thread>
#include <future>
#include <cstdlib>
#include "../../bibs/httplib.h"
#include "../../bibs/json.hpp"

using json = nlohmann::json;

const int PORTA_SERVIDOR = 8080;

json consultarEscravo(const std::string &host, int port, const std::string &body)
{
    std::cerr << "Chamando escravo em " << host << ":" << port << std::endl;

    httplib::Client cli(host, port);

    // Verifica se está vivo
    if (auto res = cli.Get("/health"))
    {
        if (res->status != 200)
        {
            return {{"erro", "Escravo indisponível"}};
        }
    }
    else
    {
        return {{"erro", "Falha ao conectar"}};
    }

    // Envia dados
    if (auto res = cli.Post("/processar", body, "application/json"))
    {
        if (res->status == 200)
        {
            try
            {
                return json::parse(res->body);
            }
            catch (...)
            {
                return {{"erro", "Resposta inválida do escravo"}};
            }
        }
        else
        {
            return {{"erro", "Falha no processamento"}};
        }
    }

    return {{"erro", "Sem resposta do escravo"}};
}

int main()
{
    httplib::Server svr;

    svr.Post("/info", [](const httplib::Request &req, httplib::Response &res)
             {
    std::cerr << "Recebido do cliente: \n" << req.body << std::endl;

    // Executa em paralelo
    auto f1 = std::async(std::launch::async, consultarEscravo, "escravo1", 8081, req.body);
    auto f2 = std::async(std::launch::async, consultarEscravo, "escravo2", 8082, req.body);

    // Espera os dois terminarem
    json resposta1 = f1.get();
    json resposta2 = f2.get();

    // Combina resultados
    json combinado;
    combinado["escravo1"] = resposta1;
    combinado["escravo2"] = resposta2;

    // Retorna consolidado
    res.set_content(combinado.dump(), "application/json"); });

    const char *hostIP = std::getenv("HOST_IP");
    if (!hostIP)
        hostIP = "localhost";

    std::cerr << "Servidor rodando em http://" << hostIP << ":" << PORTA_SERVIDOR << std::endl;
    svr.listen("0.0.0.0", PORTA_SERVIDOR);
}
