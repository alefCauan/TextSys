#include <iostream>
#include "../../bibs/httplib.h"
#include <cctype>
#include <string>

// Função para contar letras
size_t count_letters(const std::string& text) {
    size_t count = 0;
    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) count++;
    }
    return count;
}

// Função para contar números
size_t count_numbers(const std::string& text) {
    size_t count = 0;
    for (char c : text) {
        if (std::isdigit(static_cast<unsigned char>(c))) count++;
    }
    return count;
}

void run_slave_server(const std::string& modo, int porta) {
    httplib::Server svr;

    if (modo == "letras") {
        svr.Post("/letras", [](const httplib::Request& req, httplib::Response& res) {
            size_t qtd = count_letters(req.body);
            res.set_content(std::to_string(qtd), "text/plain");
        });
    } else if (modo == "numeros") {
        svr.Post("/numeros", [](const httplib::Request& req, httplib::Response& res) {
            size_t qtd = count_numbers(req.body);
            res.set_content(std::to_string(qtd), "text/plain");
        });
    } else {
        std::cerr << "Modo inválido. Use 'letras' ou 'numeros'.\n";
        return;
    }
    svr.listen("0.0.0.0", porta);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " [letras|numeros] [porta]\n";
        return 1;
    }
    std::string modo = argv[1];
    int porta = std::stoi(argv[2]);
    run_slave_server(modo, porta);
    return 0;
}
