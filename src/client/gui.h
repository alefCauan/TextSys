#pragma once
#include <gtk/gtk.h>
#include <string>

class ClientGUI
{
public:
    ClientGUI();
    void run(int argc, char **argv);
    void on_send_clicked();
    void clear_text();

private:
    GtkWidget *window;
    GtkWidget *entry_host;
    GtkWidget *entry_port;
    GtkWidget *file_chooser;
    GtkWidget *text_view;

    void append_text(const std::string &text);
};
