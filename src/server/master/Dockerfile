# Usar GCC completo (build + runtime)
FROM gcc:12

# Diretório de trabalho
WORKDIR /app

# Copiar headers e código-fonte
COPY src/bibs ./bibs
COPY src/server/master ./server/master

# Compilar o servidor
RUN g++ -std=c++17 -I ./bibs ./server/master/main.cpp -o /app/servidor_mestre

# Expor porta do servidor
EXPOSE 8080

# Rodar o servidor
CMD ["/app/servidor_mestre"]
