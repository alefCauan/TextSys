# ⚙️ TextSys - Sistema Distribuído Mestre–Escravo em C++

Este projeto implementa um **sistema distribuído** em **C++ puro**, utilizando **threads** e **containers Docker**, seguindo a arquitetura **mestre–escravo**.  


## 🎯 Objetivo

Criar um sistema cliente-servidor distribuído, onde:  
- O **Cliente** envia arquivos de texto contendo letras e números.  
- O **Servidor Mestre** coordena a execução paralela, distribuindo tarefas entre os **Servidores Escravos**.  
- Os **Servidores Escravos** processam partes específicas (contagem de letras e números).  
- O **Mestre** consolida os resultados e retorna em formato **JSON** ao Cliente.  

## 🖥️ Funcionalidades

### 🔹 Cliente
- Envia um arquivo `.txt` com letras e números.  
- Pode ser implementado como **linha de comando (CLI)** ou **GUI em Qt/GTK+**.  
- Apenas envia requisições **HTTP REST** ao Mestre.  
- Exibe os resultados no terminal ou interface gráfica.  

### 🔹 Servidor Mestre (Container 1)
- Recebe requisições do Cliente.  
- Dispara **duas threads** em paralelo:
  - Uma para comunicação com o Escravo 1.  
  - Outra para comunicação com o Escravo 2.  
- Antes de enviar dados, consulta se cada Escravo está disponível (`/health`).  
- Após receber as respostas, **combina os resultados** e devolve em **JSON** ao Cliente.  

### 🔹 Servidores Escravos (Containers 2 e 3)
- **Escravo 1** → expõe endpoint `/letras`, recebe texto e retorna a **quantidade de letras**.  
- **Escravo 2** → expõe endpoint `/numeros`, recebe texto e retorna a **quantidade de números**.  


## 🛠️ Requisitos Técnicos

- **Linguagem:** C++17 ou superior  
- **Concorrência:** `std::thread`, `std::async` ou equivalente  
- **Comunicação REST:** `cpp-httplib` ou `Boost.Beast`  
- **Contêineres:** Docker + docker-compose para orquestração (Mestre + Escravos)  
- **Execução do Cliente:** Pode rodar fora dos containers (ex.: em um notebook), comunicando-se via rede com o Mestre  

## Como executar os servidores escravos com Docker

1. **Construa a imagem Docker (apenas uma vez):**
   ```bash
   docker build -t slave_server .
   ```

2. **Execute o container para contar letras:**
   ```bash
   docker run -d --name slave_letras -p 8081:8081 slave_server letras
   ```

3. **Execute o container para contar números:**
   ```bash
   docker run -d --name slave_numeros -p 8082:8082 slave_server numeros
   ```

4. **Teste os endpoints:**
   ```bash
   curl -X POST http://localhost:8081/letras -d "abc123DEF"
   curl -X POST http://localhost:8082/numeros -d "abc123DEF"
   ```
## 📂 Estrutura do Projeto
.
├── LICENSE
├── README.md
├── build/                     # Arquivos de build/compilação
├── docker-compose.yml          # Orquestração dos containers
└── src/
    ├── bibs/                   # Bibliotecas externas (httplib, json.hpp)
    │   ├── httplib.h
    │   └── json.hpp
    ├── client/                 # Código do cliente
    │   ├── Dockerfile
    │   ├── http_client.cpp
    │   ├── http_client.h
    │   ├── main.cpp
    │   └── requirements.txt
    ├── input.txt               # Arquivo de entrada (exemplo)
    └── server/                 # Código do servidor
        ├── master/             # Servidor Mestre
        └── slave/              # Servidores Escravos (letras/números)

