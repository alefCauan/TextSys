#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include "http_client.h"

void printUsage(const char* programName) {
    std::cout << "\n=== Cliente HTTP - Processamento de Arquivos ===\n\n";
    std::cout << "Uso:\n";
    std::cout << "  " << programName << " <arquivo.txt>\n";
    std::cout << "  " << programName << " <host_mestre> <porta_mestre> <arquivo.txt>\n\n";
    std::cout << "Exemplos:\n";
    std::cout << "  " << programName << " dados.txt\n";
    std::cout << "  " << programName << " 192.168.1.100 8080 dados.txt\n\n";
    std::cout << "Descrição:\n";
    std::cout << "  Envia um arquivo .txt contendo letras e números para o servidor mestre\n";
    std::cout << "  e exibe a resposta JSON processada.\n\n";
}

void printFileInfo(const std::string& filepath, const std::string& content) {
    namespace fs = std::filesystem;
    
    std::cout << "\n=== Informações do Arquivo ===\n";
    std::cout << "Arquivo: " << filepath << "\n";
    
    if (fs::exists(filepath)) {
        auto fileSize = fs::file_size(filepath);
        std::cout << "Tamanho: " << fileSize << " bytes\n";
    }
    
    std::cout << "Conteúdo (" << content.length() << " caracteres):\n";
    std::cout << std::string(40, '-') << "\n";
    
    // Mostra as primeiras linhas do arquivo para preview
    std::istringstream iss(content);
    std::string line;
    int lineCount = 0;
    const int maxPreviewLines = 10;
    
    while (std::getline(iss, line) && lineCount < maxPreviewLines) {
        std::cout << std::setw(3) << (lineCount + 1) << ": " << line << "\n";
        lineCount++;
    }
    
    if (lineCount == maxPreviewLines) {
        std::istringstream iss2(content);
        int totalLines = 0;
        while (std::getline(iss2, line)) {
            totalLines++;
        }
        if (totalLines > maxPreviewLines) {
            std::cout << "... (mais " << (totalLines - maxPreviewLines) << " linhas)\n";
        }
    }
    
    std::cout << std::string(40, '-') << "\n";
}

void printServerInfo(const std::string& host, int port) {
    std::cout << "\n=== Configuração do Servidor ===\n";
    std::cout << "Host: " << host << "\n";
    std::cout << "Porta: " << port << "\n";
    std::cout << "Endpoint: /info\n";
    std::cout << "URL: http://" << host << ":" << port << "/info\n";
}

std::string formatJsonResponse(const std::string& jsonResponse) {
    // Simples formatação para melhor legibilidade
    std::string formatted = jsonResponse;
    
    // Adiciona quebras de linha após { e ,
    for (size_t i = 0; i < formatted.length(); ++i) {
        if (formatted[i] == '{' || formatted[i] == ',') {
            formatted.insert(i + 1, "\n  ");
            i += 3; // Ajusta o índice devido à inserção
        } else if (formatted[i] == '}') {
            formatted.insert(i, "\n");
            i += 1;
        }
    }
    
    return formatted;
}

int main(int argc, char **argv)
{
    std::string host = "localhost";
    int port = 8080;
    std::string filepath;

    // Parse command line arguments
    if (argc == 2) {
        filepath = argv[1];
    }
    else if (argc == 4) {
        host = argv[1];
        port = std::stoi(argv[2]);
        filepath = argv[3];
    }
    else {
        printUsage(argv[0]);
        return 1;
    }

    // Validate file extension
    if (filepath.length() < 4 || filepath.substr(filepath.length() - 4) != ".txt") {
        std::cout << "Aviso: O arquivo não possui extensão .txt\n";
    }

    // Check if file exists
    if (!std::filesystem::exists(filepath)) {
        std::cerr << "Erro: Arquivo '" << filepath << "' não encontrado.\n";
        return 2;
    }

    // Read file content
    std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
    if (!ifs) {
        std::cerr << "Erro: Não foi possível abrir o arquivo '" << filepath << "'.\n";
        return 2;
    }

    std::ostringstream ss;
    ss << ifs.rdbuf();
    std::string content = ss.str();
    
    if (content.empty()) {
        std::cerr << "Aviso: O arquivo está vazio.\n";
    }

    // Display information
    printFileInfo(filepath, content);
    printServerInfo(host, port);

    std::cout << "\n=== Enviando Requisição ===\n";
    std::cout << "Conectando ao servidor mestre...\n";

    // Measure request time
    auto start = std::chrono::high_resolution_clock::now();

    try {
        std::string response = post_text_and_get_json(host, port, "/info", content);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "✓ Requisição enviada com sucesso!\n";
        std::cout << "Tempo de resposta: " << duration.count() << "ms\n";
        
        std::cout << "\n=== Resposta do Mestre ===\n";
        std::cout << std::string(50, '=') << "\n";
        std::cout << formatJsonResponse(response) << "\n";
        std::cout << std::string(50, '=') << "\n";
        
    }
    catch (const std::exception &ex) {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cerr << "\n✗ Falha na requisição após " << duration.count() << "ms\n";
        std::cerr << "Erro: " << ex.what() << "\n\n";
        std::cerr << "Possíveis soluções:\n";
        std::cerr << "- Verificar se o servidor mestre está rodando\n";
        std::cerr << "- Confirmar host e porta corretos\n";
        std::cerr << "- Verificar conectividade de rede\n";
        std::cerr << "- Verificar se o endpoint /info está disponível\n";
        return 3;
    }

    std::cout << "\n✓ Operação concluída com sucesso!\n";
    return 0;
}