/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-window.h: This file is part of ____
 *
 * Copyright (C) 2011 yetist <yetist@gmail.com>
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

#ifndef __GFW_WINDOW_H__ 
#define __GFW_WINDOW_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS


#define GFW_TYPE_WINDOW              (gfw_window_get_type ())
#define GFW_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFW_TYPE_WINDOW, GfwWindow))
#define GFW_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GFW_TYPE_WINDOW, GfwWindowClass))
#define GFW_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFW_TYPE_WINDOW))
#define GFW_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GFW_TYPE_WINDOW))
#define GFW_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GFW_TYPE_WINDOW, GfwWindowClass))

typedef struct _GfwWindow             GfwWindow;
typedef struct _GfwWindowClass        GfwWindowClass;

struct _GfwWindow
{
  GtkWindow      window;
};

struct _GfwWindowClass
{
  GtkWindowClass     parent_class;
};

GType          gfw_window_get_type           (void) G_GNUC_CONST;
GtkWidget*     gfw_window_new (GtkWindowType type);
void           gfw_window_set_background (GfwWindow *window, GdkPixbuf *pixbuf);
void           gfw_window_set_transparent (GfwWindow *window, gboolean transparent);
gboolean       gfw_window_get_transparent (GfwWindow *window);
void           gfw_window_set_size_fit_pixbuf (GfwWindow *window, gboolean is_fit);
gboolean       gfw_window_get_size_fit_pixbuf (GfwWindow *window);

G_END_DECLS

#endif /* __GFW_WINDOW_H__ */
