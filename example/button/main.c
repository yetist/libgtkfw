
#include <gtk/gtk.h>
#include <gfw.h>

static GtkWidget *button;

static void clicked( GtkWidget *widget, gpointer   data )
{
    gboolean status;
    status = gtk_widget_get_sensitive(button);
    if (status) {
	    gtk_widget_set_sensitive(button, FALSE);
    } else {
	    gtk_widget_set_sensitive(button, TRUE);
    }
    g_print ("button clicked\n");
}

static gboolean delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    g_print ("delete event occurred\n");
    return FALSE;
}

static void destroy( GtkWidget *widget, gpointer   data )
{
    gtk_main_quit ();
}

int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *vbox;
    GdkPixbuf *normal, *active, *prelight, *insensitive;
    GdkPixbuf *bn, *ba;

    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

#if GTK_CHECK_VERSION(3,0,0)
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
#else
    vbox = gtk_vbox_new(FALSE, 5);
#endif
    gtk_container_add (GTK_CONTAINER (window), vbox);

    normal = gdk_pixbuf_new_from_file("01-normal.png", NULL);
    active = gdk_pixbuf_new_from_file("01-active.png", NULL);

    /* button */
    button = gfw_button_new_with_pixbufs(normal, "active-pixbuf", active, NULL);
    gtk_widget_set_tooltip_text (button, "gfwbutton with 2 states: normal and active");
    g_signal_connect (button, "clicked", G_CALLBACK (clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 5);

    normal = gdk_pixbuf_new_from_file("03-normal.png", NULL);
    active = gdk_pixbuf_new_from_file("03-active.png", NULL);
    prelight = gdk_pixbuf_new_from_file("03-prelight.png", NULL);
    insensitive = gdk_pixbuf_new_from_file("03-insensitive.png", NULL);

    /* button */
    button = gfw_button_new_with_pixbufs(normal, "active-pixbuf", active, "prelight-pixbuf", prelight, NULL);
    gtk_widget_set_tooltip_text (button, "gfwbutton with 3 states: normal, active and prelight");
    g_signal_connect (button, "clicked", G_CALLBACK (clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 5);

    /* insensitive button */
    button = gfw_button_new_with_pixbufs(normal, "active-pixbuf", active, "prelight-pixbuf", prelight, "insensitive-pixbuf", insensitive, NULL);
    gtk_widget_set_tooltip_text (button, "gfwbutton with 4 states: normal, active, prelight and insensitive.");
    g_signal_connect (button, "clicked", G_CALLBACK (clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 5);

    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}
