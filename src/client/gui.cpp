#include "gui.h"
#include "http_client.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Add this method to the `ClientGUI` class to clear the text
void ClientGUI::clear_text()
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, "", -1);
}

static void on_clear_button(GtkWidget *widget, gpointer data)
{
    reinterpret_cast<ClientGUI *>(data)->clear_text();
}

static void on_send_button(GtkWidget *widget, gpointer data)
{
    reinterpret_cast<ClientGUI *>(data)->on_send_clicked();
}

// Função para aplicar CSS customizado
void apply_custom_css()
{
    const char *css_data = R"(
        /* Definindo as cores da paleta */
        @define-color light_gray #F1F7F7;
        @define-color dark_teal #021A1A;
        @define-color bright_green #00DF82;
        @define-color medium_green #2CC295;
        @define-color black #000000;

        /* Estilo da janela principal */
        window {
            background-color: @black;
        }

        /* Estilo dos botões */
        button {
            background: linear-gradient(to bottom, @medium_green, @bright_green);
            border: 2px solid @dark_teal;
            border-radius: 8px;
            color: @dark_teal;
            font-weight: bold;
            // padding: 8px 16px;
            // margin: 2px;
        }

        button:hover {
            background: linear-gradient(to bottom, @bright_green, @medium_green);
            box-shadow: 0 2px 4px rgba(2, 26, 26, 0.3);
        }

        button:active {
            background-color: @dark_teal;
            color: @black;
        }

        /* Estilo dos campos de entrada */
        entry {
            background-color: @black;
            border: 2px solid @medium_green;
            border-radius: 6px;
            color: @light_gray;
            // padding: 8px;
            // margin: 2px;
        }

        entry:focus {
            border-color: @light_gray;
            box-shadow: 0 0 6px rgba(0, 223, 130, 0.3);
        }

        /* Estilo do file chooser */
        filechooserbutton button {
            background-color: @black;
            border: 2px solid @medium_green;
            color: @dark_teal;
        }

        /* Estilo das labels */
        label {
            color: @dark_teal;
            font-weight: bold;
        }

        /* Estilo da área de texto */
        textview {
            background-color: @black;
            color: @dark_teal;
            border: 2px solid @light_gray;
        }

        textview text {
            background-color: @black;
            color: @light_gray;
        }

        /* Estilo da scrollbar */
        scrollbar {
            background-color: @black;
        }

        scrollbar slider {
            background-color: @medium_green;
            border-radius: 10px;
        }

        scrollbar slider:hover {
            background-color: @bright_green;
        }

        /* Estilo do scrolled window */
        scrolledwindow {
            border: 2px solid @medium_green;
            border-radius: 6px;
        }
    )";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    g_object_unref(provider);
}

ClientGUI::ClientGUI() {}

void ClientGUI::run(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    apply_custom_css();

    // Janela principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cliente Mestre-Escravo");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Caixa horizontal (esquerda/direita)
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // ================== COLUNA ESQUERDA ==================
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox), left_box, FALSE, FALSE, 10);

    // Espaço expansivo acima dos botões
    GtkWidget *vspace_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(left_box), vspace_top, TRUE, TRUE, 0);

    // Container vertical para os botões
    GtkWidget *btn_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(left_box), btn_vbox, FALSE, FALSE, 0);

    // Host + porta
    entry_host = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_host), "Host (ex: 127.0.0.1)");
    gtk_box_pack_start(GTK_BOX(btn_vbox), entry_host, FALSE, FALSE, 0);

    entry_port = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_port), "Porta (ex: 8080)");
    gtk_box_pack_start(GTK_BOX(btn_vbox), entry_port, FALSE, FALSE, 0);

    // File chooser
    file_chooser = gtk_file_chooser_button_new("Selecione um arquivo TXT", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_box_pack_start(GTK_BOX(btn_vbox), file_chooser, FALSE, FALSE, 0);

    // Botão enviar
    GtkWidget *btn_send = gtk_button_new_with_label("Enviar");
    gtk_box_pack_start(GTK_BOX(btn_vbox), btn_send, FALSE, FALSE, 0);
    g_signal_connect(btn_send, "clicked", G_CALLBACK(on_send_button), this);

    // Botão limpar
    GtkWidget *btn_clear = gtk_button_new_with_label("Limpar");
    gtk_box_pack_start(GTK_BOX(btn_vbox), btn_clear, FALSE, FALSE, 0);
    g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_clear_button), this);

    // ================== LOGO ABAIXO DOS BOTÕES ==================
    // Espaço pequeno entre botões e logo
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(spacer, -1, 20); // 20px de espaço
    gtk_box_pack_start(GTK_BOX(left_box), spacer, FALSE, FALSE, 0);

    // Widget da imagem (logo)
    GtkWidget *logo_image = gtk_image_new_from_file("Alien3.png"); // Substitua pelo caminho do seu logo
    
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("Alien3.png", 100, 100, TRUE, NULL);
    if (pixbuf) {
        logo_image = gtk_image_new_from_pixbuf(pixbuf);
        g_object_unref(pixbuf);
    } else {
        logo_image = gtk_label_new("Logo não encontrado");
    }

    // Centralizar o logo
    GtkWidget *logo_alignment = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(logo_alignment), gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(logo_alignment), logo_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(logo_alignment), gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0), TRUE, TRUE, 0);
    
    gtk_box_pack_start(GTK_BOX(left_box), logo_alignment, FALSE, FALSE, 0);

    // Espaço expansivo abaixo do logo
    GtkWidget *vspace_bottom = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(left_box), vspace_bottom, TRUE, TRUE, 0);

    // ================== COLUNA DIREITA ==================
    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox), right_box, TRUE, TRUE, 10);

    GtkWidget *label_resp = gtk_label_new("Resposta do servidor (JSON):");
    gtk_box_pack_start(GTK_BOX(right_box), label_resp, FALSE, FALSE, 5);

    text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);
    gtk_box_pack_start(GTK_BOX(right_box), scroll, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    gtk_main();
}

void ClientGUI::on_send_clicked()
{
    const char *host_c = gtk_entry_get_text(GTK_ENTRY(entry_host));
    const char *port_c = gtk_entry_get_text(GTK_ENTRY(entry_port));
    char *filepath_c = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));

    if (!host_c || !port_c || !filepath_c)
    {
        append_text("Preencha host, porta e selecione um arquivo.\n");
        return;
    }

    std::string host = host_c;
    int port = std::stoi(port_c);
    std::string filepath = filepath_c;

    // Lê arquivo
    std::ifstream ifs(filepath);
    if (!ifs)
    {
        append_text("Erro ao abrir arquivo: " + filepath + "\n");
        return;
    }
    std::ostringstream ss;
    ss << ifs.rdbuf();
    std::string content = ss.str();

    // Faz requisição
    try
    {
        std::string response = post_text_and_get_json(host, port, "/info", content);
        append_text("Resposta do servidor:\n" + response + "\n");
    }
    catch (const std::exception &ex)
    {
        append_text(std::string("Erro: ") + ex.what() + "\n");
    }

    g_free(filepath_c);
}

void ClientGUI::append_text(const std::string &text)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, text.c_str(), -1);
}