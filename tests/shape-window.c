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

static void button_pressed(GtkWidget *widget, gpointer   data )
{
    g_print ("button pressed.\n");
}

static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    g_print ("delete event occurred\n");
    return FALSE;
}

int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *vbox;
    GdkPixbuf *normal, *active, *prelight;
    GdkPixbuf *bn, *ba;
	GdkPixbuf *bg;
	GtkWidget *fixed;

    gtk_init (&argc, &argv);
    window = gfw_window_new (GTK_WINDOW_POPUP);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

	bg = gdk_pixbuf_new_from_file("clock.png", NULL);

	gfw_window_set_background(GFW_WINDOW(window), bg);
	gfw_window_set_transparent (GFW_WINDOW(window), TRUE);

    g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed);

    normal = gdk_pixbuf_new_from_file("btn1.png", NULL);
    active = gdk_pixbuf_new_from_file("btn2.png", NULL);
    prelight = gdk_pixbuf_new_from_file("btn3.png", NULL);

    button = gfw_button_new_with_pixbufs(normal, "active-pixbuf", active, "prelight-pixbuf", prelight, NULL);
    gtk_widget_set_tooltip_text (button, "gfwbutton with 3 states: normal, active and prelight");
    g_signal_connect (button, "clicked", G_CALLBACK (button_pressed), NULL);

	gtk_fixed_put(GTK_FIXED(fixed), button, 110, 70);
	
    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}
