#include <gtk/gtk.h>
#include <gfw.h>

static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    g_print ("delete event occurred\n");
    return FALSE;
}

static void destroy( GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}

void toggled (GtkToggleButton *togglebutton, gpointer user_data)
{
	GfwWindow *window;
	window = GFW_WINDOW(user_data);
	gfw_window_set_size_fit_pixbuf(window, !gfw_window_get_size_fit_pixbuf(window));
}

int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    GdkPixbuf *background;

    gtk_init (&argc, &argv);
    window = gfw_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    background = gdk_pixbuf_new_from_file("background.png", NULL);
    gfw_window_set_background(GFW_WINDOW(window), background);

    button = gtk_toggle_button_new_with_label("Size fit background");
    //gtk_widget_set_opacity(GTK_WIDGET(button), 0.5);
    g_signal_connect (button, "toggled", G_CALLBACK (toggled), window);

    gtk_container_add(GTK_CONTAINER (window), button);

    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}
