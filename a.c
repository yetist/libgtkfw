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

#include "magic-window.h"
#include "magic-button.h"

typedef struct _cursoroffset {gint x,y;} CursorOffset;

static void hello( GtkWidget *widget,
                   gpointer   data )
{
    g_print (">>>>>>Hello World\n");
}

static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    g_print ("delete event occurred\n");
    return FALSE;
}

static void
shape_pressed (GtkWidget *widget, GdkEventButton *event)
{
  CursorOffset *p;

  /* ignore double and triple click */
  if (event->type != GDK_BUTTON_PRESS)
    return;

  p = g_object_get_data (G_OBJECT (widget), "cursor_offset");
  p->x = (int) event->x;
  p->y = (int) event->y;

  gtk_grab_add (widget);
  gdk_pointer_grab (widget->window, TRUE,
		    GDK_BUTTON_RELEASE_MASK |
		    GDK_BUTTON_MOTION_MASK |
		    GDK_POINTER_MOTION_HINT_MASK,
		    NULL, NULL, 0);
}

static void
shape_released (GtkWidget *widget)
{
  gtk_grab_remove (widget);
  gdk_display_pointer_ungrab (gtk_widget_get_display (widget),
			      GDK_CURRENT_TIME);
}

static void
shape_motion (GtkWidget      *widget, 
	      GdkEventMotion *event)
{
  gint xp, yp;
  CursorOffset * p;
  GdkModifierType mask;

  p = g_object_get_data (G_OBJECT (widget), "cursor_offset");

  /*
   * Can't use event->x / event->y here 
   * because I need absolute coordinates.
   */
  gdk_window_get_pointer (NULL, &xp, &yp, &mask);
  gtk_widget_set_uposition (widget, xp  - p->x, yp  - p->y);
}

gboolean window_drag(GtkWidget *widget, GdkEventButton *event, GdkWindowEdge edge)  
{  
	if (event->button == 1)  
	{  
		gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)), event->button, event->  
				x_root, event->y_root, event->time);  
	}  

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
	CursorOffset* icon_pos;

	gtk_init (&argc, &argv);
	//window = magic_window_new (GTK_WINDOW_TOPLEVEL);
	window = magic_window_new (GTK_WINDOW_POPUP);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	bg = gdk_pixbuf_new_from_file("background.png", NULL);

	gtk_widget_set_events (window, 
			gtk_widget_get_events (window) |
			GDK_BUTTON_MOTION_MASK |
			GDK_POINTER_MOTION_HINT_MASK |
			GDK_BUTTON_PRESS_MASK);
	magic_window_set_background(MAGIC_WINDOW(window), bg);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	magic_window_set_transparent (MAGIC_WINDOW(window), TRUE);
	magic_window_set_size_fit_pixbuf (MAGIC_WINDOW(window), TRUE);

	gtk_widget_realize (window);
	icon_pos = g_new (CursorOffset, 1);
	g_object_set_data (G_OBJECT (window), "cursor_offset", icon_pos);

	//gtk_widget_set_uposition (window, x, y);
	//	g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(window_drag), NULL);
	g_signal_connect (window, "button_press_event",
			G_CALLBACK (shape_pressed), NULL);
	g_signal_connect (window, "button_release_event",
			G_CALLBACK (shape_released), NULL);
	g_signal_connect (window, "motion_notify_event",
			G_CALLBACK (shape_motion), NULL);
	g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	//gtk_container_set_border_width (GTK_CONTAINER (window), 10);


#if 0
	fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed);

	normal = gdk_pixbuf_new_from_file("leave.png", NULL);
	active = gdk_pixbuf_new_from_file("press.png", NULL);
	prelight = gdk_pixbuf_new_from_file("enter.png", NULL);

	/* first button */
	button = magic_button_new_with_pixbufs(normal, "active-pixbuf", active, "prelight-pixbuf", prelight, NULL);
	gtk_widget_set_tooltip_text (button, "magicbutton with 3 states: normal, active and prelight");
	g_signal_connect (button, "clicked", G_CALLBACK (hello), NULL);

	gtk_fixed_put(fixed, button, 80, 50);


#else
	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add (GTK_CONTAINER (window), vbox);

	normal = gdk_pixbuf_new_from_file("leave.png", NULL);
	active = gdk_pixbuf_new_from_file("press.png", NULL);
	prelight = gdk_pixbuf_new_from_file("enter.png", NULL);

	/* first button */
	button = magic_button_new_with_pixbufs(normal, "active-pixbuf", active, "prelight-pixbuf", prelight, NULL);
	gtk_widget_set_tooltip_text (button, "magicbutton with 3 states: normal, active and prelight");
	g_signal_connect (button, "clicked", G_CALLBACK (hello), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 5);

	/* 2nd button */
	bn = gdk_pixbuf_new_from_file("login-button-normal.png", NULL);
	ba = gdk_pixbuf_new_from_file("login-button-active.png", NULL);
	button = magic_button_new_with_pixbufs(bn, "active-pixbuf", ba, NULL);
	gtk_widget_set_tooltip_text (button, "magicbutton with 2 states: normal and active");
	g_signal_connect (button, "clicked", G_CALLBACK (hello), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
#endif

	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}
