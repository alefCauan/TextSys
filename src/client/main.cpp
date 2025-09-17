#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "http_client.h"

int main(int argc, char **argv)
{
    std::string host = "localhost";
    int port = 8080;
    std::string filepath;

    if (argc == 2)
    {
        filepath = argv[1];
    }
    else if (argc == 4)
    {
        host = argv[1];
        port = std::stoi(argv[2]);
        filepath = argv[3];
    }
    else
    {
        std::cout << "Usage:\n " << argv[0] << " <file.txt>\n or\n " << argv[0] << " <master_host> <master_port> <file.txt>\n";
        return 1;
    }

    std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
    if (!ifs)
    {
        std::cerr << "Error: could not open file '" << filepath << "'\n";
        return 2;
    }

    std::ostringstream ss;
    ss << ifs.rdbuf();
    std::string content = ss.str();

    try
    {
        std::string response = post_text_and_get_json(host, port, "/info", content);
        std::cout << "Response from master:\n"
                  << response << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Request failed: " << ex.what() << std::endl;
        return 3;
    }

    return 0;
}