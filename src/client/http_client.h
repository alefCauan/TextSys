#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H


#include <string>


// Faz POST do texto em `body` para http://host:port/path
// Retorna o corpo da resposta (espera JSON retornado pelo Mestre).
// Lança std::runtime_error em caso de erro de conexão ou status != 200.
std::string post_text_and_get_json(const std::string& host, int port,
const std::string& path, const std::string& body);


#endif // HTTP_CLIENT_H