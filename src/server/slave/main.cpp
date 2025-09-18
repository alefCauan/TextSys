#include "server.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Uso: " << argv[0] << " [letras|numeros]\n";
        return 1;
    }
    run_slave_server(argv[1]);
    return 0;
}