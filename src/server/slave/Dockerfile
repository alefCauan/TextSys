FROM gcc:12

WORKDIR /app

COPY src/bibs ./bibs
COPY src/server/slave ./server/slave

RUN g++ -std=c++17 -I ./bibs ./server/slave/main.cpp -o /app/slave

EXPOSE 8081 8082

CMD ["/app/slave"]
