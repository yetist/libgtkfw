/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-color.h: This file is part of ____
 *
 * Copyright (C) 2014 yetist <xiaotian.wu@i-soft.com.cn>
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

#ifndef __GFW_COLOR_H__ 
#define __GFW_COLOR_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GFW_TYPE_COLOR              (gfw_color_get_type ())
#define GFW_COLOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFW_TYPE_COLOR, GfwColor))
#define GFW_COLOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GFW_TYPE_COLOR, GfwColorClass))
#define GFW_IS_COLOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFW_TYPE_COLOR))
#define GFW_IS_COLOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GFW_TYPE_COLOR))
#define GFW_COLOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GFW_TYPE_COLOR, GfwColorClass))

typedef struct _GfwColor             GfwColor;
typedef struct _GfwColorClass        GfwColorClass;

struct _GfwColor
{
	GtkDrawingArea      drawing;
};

struct _GfwColorClass
{
	GtkDrawingAreaClass     parent_class;
	void  (*activated) (GtkWidget* widget, gpointer userdata);
};

GType          gfw_color_get_type                 (void) G_GNUC_CONST;
GtkWidget*     gfw_color_new                      (void);
void           gfw_color_set_rgba                 (GfwColor* color, GdkRGBA rgba);
GdkRGBA*       gfw_color_get_rgba                 (GfwColor* color);

G_END_DECLS

#endif /* __GFW_COLOR_H__ */
