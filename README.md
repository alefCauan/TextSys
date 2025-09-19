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

---

## 📂 Estrutura do Projeto


