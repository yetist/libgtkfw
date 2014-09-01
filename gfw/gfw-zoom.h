/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-zoom.h: This file is part of ____
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

#ifndef __GFW_ZOOM_H__ 
#define __GFW_ZOOM_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GFW_TYPE_ZOOM              (gfw_zoom_get_type ())
#define GFW_ZOOM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFW_TYPE_ZOOM, GfwZoom))
#define GFW_ZOOM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GFW_TYPE_ZOOM, GfwZoomClass))
#define GFW_IS_ZOOM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFW_TYPE_ZOOM))
#define GFW_IS_ZOOM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GFW_TYPE_ZOOM))
#define GFW_ZOOM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GFW_TYPE_ZOOM, GfwZoomClass))

typedef struct _GfwZoom             GfwZoom;
typedef struct _GfwZoomClass        GfwZoomClass;

struct _GfwZoom
{
	GtkDrawingArea object;
};

struct _GfwZoomClass
{
	GtkDrawingAreaClass     parent_class;
	void  (* color_changed) (GtkWidget* widget, GdkColor* c, gpointer userdata);
	void  (* activated)     (GtkWidget* widget, gpointer userdata);
};

GType          gfw_zoom_get_type                (void) G_GNUC_CONST;
GtkWidget*     gfw_zoom_new                     (void);
void           gfw_zoom_set_scale               (GfwZoom* zoom, gfloat scale);
gfloat         gfw_zoom_get_scale               (GfwZoom* zoom);
void           gfw_zoom_set_fade                (GfwZoom* zoom, gboolean fade);
gint32         gfw_zoom_get_size                (GfwZoom* zoom);
void           gfw_zoom_set_size                (GfwZoom* zoom, gint32 size);
void           gfw_zoom_set_mark                (GfwZoom* zoom, int index, GdkPoint position);
void           gfw_zoom_clear_mark              (GfwZoom* zoom, int index);
void           gfw_zoom_update                  (GfwZoom* zoom, GdkPoint pointer, GdkPoint screen_size, GdkPixbuf* pixbuf);

GdkRectangle   gfw_zoom_get_current_screen_rect (GfwZoom *zoom);
void           gfw_zoom_set_current_screen_size (GfwZoom* zoom, GdkPoint screen_size);
void           gfw_zoom_set_current_pointer     (GfwZoom* zoom, GdkPoint pointer);
void           gfw_zoom_set_current_pixbuf      (GfwZoom* zoom, GdkPixbuf* pixbuf);

G_END_DECLS

#endif /* __GFW_ZOOM_H__ */
