#include <gtk/gtk.h>
#include <gfw/gtkfw.h>

/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below. */
static void hello( GtkWidget *widget,
                   gpointer   data )
{
    g_print (">>>>>>Hello World\n");
}

static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    /* If you return FALSE in the "delete_event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    g_print ("delete event occurred\n");

    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event". */

    return FALSE;
}

/* Another callback */
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}

#if GTK_CHECK_VERSION(3,0,0)
static gboolean draw (GtkWidget *widget, cairo_t *cr)
{
	GdkPixbuf   *pixbuf = gdk_pixbuf_new_from_file("./background.png",NULL);
	gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
	//gdk_cairo_set_source_window(cr, widget->window, 0, 0);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint(cr);
	cairo_destroy(cr);

	return FALSE;
}
#else
static gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer userdata)
{
	cairo_t *cr = gdk_cairo_create(widget->window);
	GdkPixbuf   *pixbuf = gdk_pixbuf_new_from_file("./background.png",NULL);
	gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
	//gdk_cairo_set_source_window(cr, widget->window, 0, 0);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint(cr);
	cairo_destroy(cr);

    return FALSE;
}
#endif

int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *vbox;
    GdkPixbuf *normal, *active, *prelight;
    GdkPixbuf *bn, *ba;

    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_app_paintable(window,TRUE); // --> important,if comment, the bg will be flushed again in gtk_widget_show(window);
    gtk_widget_realize(window);
#if GTK_CHECK_VERSION(3,0,0)
    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw), NULL);
#else
    g_signal_connect(G_OBJECT(window), "expose-event", G_CALLBACK(expose), NULL);
#endif

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    normal = gdk_pixbuf_new_from_file("leave.png", NULL);
    active = gdk_pixbuf_new_from_file("press.png", NULL);
    prelight = gdk_pixbuf_new_from_file("enter.png", NULL);
    bn = gdk_pixbuf_new_from_file("login-button-normal.png", NULL);
    ba = gdk_pixbuf_new_from_file("login-button-active.png", NULL);

    /* first button */
    button = gfw_button_new_with_images(normal, "active-pixbuf", active, "prelight-pixbuf", prelight, NULL);
    gtk_widget_set_tooltip_text (button, "gfwbutton with 3 states: normal, active and prelight");
    g_signal_connect (button, "clicked", G_CALLBACK (hello), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 5);

    /* 2nd button */
    button = gfw_button_new_with_images(bn, "active-pixbuf", ba, NULL);
    gtk_widget_set_tooltip_text (button, "gfwbutton with 2 states: normal and active");
    g_signal_connect (button, "clicked", G_CALLBACK (hello), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);

    /* 3rd button */
    button = gfw_button_new(ba);
    gtk_widget_set_tooltip_text (button, "gfwbutton with 1 state: normal");
    g_signal_connect (button, "clicked", G_CALLBACK (hello), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);

    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}
