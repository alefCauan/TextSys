#include "gui.h"

//  COMANDO PARA RODAR DENTRO DA PASTA output

// g++ -Wall -Wextra -g3     /home/alef/programas/TextSys/src/client/main.cpp     /home/alef/programas/TextSys/src/client/gui.cpp /home/alef/programas/TextSys/src/client/http_client.cpp   -o /home/alef/programas/TextSys/src/client/output/main_gtk     $(pkg-config --cflags --libs gtk+-3.0)     && LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 ./main_gtk  ./main_gtk

int main(int argc, char **argv)
{
    ClientGUI app;
    app.run(argc, argv);
    return 0;
}
