/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-button.h: This file is part of ____
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

#ifndef __GFW_BUTTON_H__ 
#define __GFW_BUTTON_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GFW_TYPE_BUTTON                    (gfw_button_get_type ())
#define GFW_BUTTON(obj)                    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFW_TYPE_BUTTON, GfwButton))
#define GFW_BUTTON_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST ((klass), GFW_TYPE_UTTON, GfwButtonClass))
#define GFW_IS_BUTTON(obj)                 (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFW_TYPE_BUTTON))
#define GFW_IS_BUTTON_CLASS(klass)         (G_TYPE_CHECK_CLASS_TYPE ((klass), GFW_TYPE_BUTTON))
#define GFW_BUTTON_GET_CLASS(obj)          (G_TYPE_INSTANCE_GET_CLASS ((obj), GFW_TYPE_BUTTON, GfwButtonClass))

typedef struct _GfwButton             GfwButton;
typedef struct _GfwButtonClass        GfwButtonClass;

typedef enum
{
  GFW_STATE_NORMAL,
  GFW_STATE_ACTIVE,
  GFW_STATE_PRELIGHT,
  GFW_STATE_INSENSITIVE
} GfwStateType;

struct _GfwButton
{
  GtkEventBox      eventbox;
};

struct _GfwButtonClass
{
  GtkEventBoxClass  parent_class;
  void (* clicked)  (GfwButton *button);
};

GType               gfw_button_get_type           (void) G_GNUC_CONST;
GtkWidget*          gfw_button_new                (GdkPixbuf *pixbuf);
GtkWidget*          gfw_button_new_with_pixbufs   (GdkPixbuf *pixbuf, const gchar *first_property_name, ...);
void                gfw_button_set_pixbuf         (GfwButton *button, GfwStateType state, GdkPixbuf *pixbuf);

G_END_DECLS

#endif /* __GFW_BUTTON_H__ */
