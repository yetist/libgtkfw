/* vi: set sw=4 ts=4 wrap ai: */
/*
 * magic-window.h: This file is part of ____
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

#ifndef __MAGIC_WINDOW_H__ 
#define __MAGIC_WINDOW_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS


#define MAGIC_TYPE_WINDOW              (magic_window_get_type ())
#define MAGIC_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), MAGIC_TYPE_WINDOW, MagicWindow))
#define MAGIC_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MAGIC_TYPE_WINDOW, MagicWindowClass))
#define MAGIC_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MAGIC_TYPE_WINDOW))
#define MAGIC_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MAGIC_TYPE_WINDOW))
#define MAGIC_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MAGIC_TYPE_WINDOW, MagicWindowClass))

typedef struct _MagicWindow             MagicWindow;
typedef struct _MagicWindowClass        MagicWindowClass;

struct _MagicWindow
{
  GtkWindow      window;
};

struct _MagicWindowClass
{
  GtkWindowClass     parent_class;
};

GType          magic_window_get_type           (void) G_GNUC_CONST;
GtkWidget*     magic_window_new (GtkWindowType type);
void           magic_window_set_background (MagicWindow *window, GdkPixbuf *pixbuf);
void           magic_window_set_transparent (MagicWindow *window, gboolean transparent);
void           magic_window_set_size_fit_pixbuf (MagicWindow *window, gboolean is_fit);

G_END_DECLS

#endif /* __MAGIC_WINDOW_H__ */
