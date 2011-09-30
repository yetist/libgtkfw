/* vi: set sw=4 ts=4 wrap ai: */
/*
 * magic-image-button.h: This file is part of ____
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

#ifndef __MAGIC_BUTTON_H__ 
#define __MAGIC_BUTTON_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MAGIC_TYPE_BUTTON                    (magic_button_get_type ())
#define MAGIC_BUTTON(obj)                    (G_TYPE_CHECK_INSTANCE_CAST ((obj), MAGIC_TYPE_BUTTON, MagicButton))
#define MAGIC_BUTTON_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST ((klass), MAGIC_TYPE_UTTON, MagicButtonClass))
#define MAGIC_IS_BUTTON(obj)                 (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MAGIC_TYPE_BUTTON))
#define MAGIC_IS_BUTTON_CLASS(klass)         (G_TYPE_CHECK_CLASS_TYPE ((klass), MAGIC_TYPE_BUTTON))
#define MAGIC_BUTTON_GET_CLASS(obj)          (G_TYPE_INSTANCE_GET_CLASS ((obj), MAGIC_TYPE_BUTTON, MagicButtonClass))

typedef struct _MagicButton             MagicButton;
typedef struct _MagicButtonClass        MagicButtonClass;

struct _MagicButton
{
  GtkEventBox      eventbox;
};

struct _MagicButtonClass
{
  GtkEventBoxClass  parent_class;
  void (* clicked)  (MagicButton *button);
};

GType               magic_button_get_type           (void) G_GNUC_CONST;
GtkWidget*          magic_button_new                (GdkPixbuf *pixbuf);
GtkWidget*          magic_button_new_with_pixbufs   (GdkPixbuf *pixbuf, const gchar *first_property_name, ...);
void                magic_button_set_pixbuf         (MagicButton *button, GtkStateType state, GdkPixbuf *pixbuf);

G_END_DECLS

#endif /* __MAGIC_BUTTON_H__ */
