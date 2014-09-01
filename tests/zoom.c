/* vi: set sw=4 ts=4 wrap ai: */
/*
 * a.c: This file is part of ____
 *
 * Copyright (C) 2011 yetist <xiaotian.wu@i-soft.com.cn>
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */

#include <gfw.h>

static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    g_print ("delete event occurred\n");
    return FALSE;
}

static GdkPixbuf * get_root_window_pixbuf(gint *width, gint *height)
{
	GdkPixbuf *pixbuf;

	pixbuf = gdk_pixbuf_get_from_window (
			gdk_get_default_root_window(),
			0, 0,
			*width, *height);
	return pixbuf;
}

gboolean on_mouse_move (GtkWidget *widget, GdkEventMotion *event, gpointer   user_data)
{
	struct GdkEventMotion {
		GdkEventType type;
		GdkWindow *window;
		gint8 send_event;
		guint32 time;
		gdouble x;
		gdouble y;
		gdouble *axes;
		guint state;
		gint16 is_hint;
		GdkDevice *device;
		gdouble x_root, y_root;
	};
	g_print("move, %f, %f\n", event->x, event->y);
	return TRUE;
}

int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *zoom;
    GtkWidget *vbox;

    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

    g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (window, "motion-notify-event", G_CALLBACK (on_mouse_move), NULL);

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add (GTK_CONTAINER (window), vbox);

	zoom = gfw_zoom_new();
    gtk_box_pack_start(GTK_BOX(vbox), zoom, TRUE, TRUE, 5);

	gint width, height;
	GdkPoint pos = {0, 0};
	GdkPixbuf *pixbuf = get_root_window_pixbuf(&width, &height);
	GdkPoint screen_size = {width, height};
	gfw_zoom_set_current_screen_size(GFW_ZOOM(zoom), screen_size);
	gfw_zoom_set_current_pointer (GFW_ZOOM(zoom), pos);
	gfw_zoom_set_current_pixbuf (GFW_ZOOM(zoom), pixbuf);

    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}
