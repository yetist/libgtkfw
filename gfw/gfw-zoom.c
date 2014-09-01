/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-zoom.c: This file is part of ____
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

#include <math.h>
#include "gfw-zoom.h"

enum {
	COLOR_CHANGED,
	ACTIVATED,
	LAST_SIGNAL
};

enum {
	PROP_0,
};

static guint gfw_zoom_signals[LAST_SIGNAL] = { 0 };

#define GFW_ZOOM_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), GFW_TYPE_ZOOM, GfwZoomPrivate))

typedef struct _GfwZoomPrivate        GfwZoomPrivate;

struct _GfwZoomPrivate
{
	GdkColor color;
	gfloat scale;

	GdkPixbuf *pixbuf;

	GdkPoint point;
	GdkPoint point_size;
	gint32 width_height;

	struct{
		gboolean valid;
		GdkPoint position;
	}marks[2];

	GdkPoint pointer;
	GdkPoint screen_size;
	gboolean fade;
};

static void gfw_zoom_set_property  (GObject          *object,
		guint             prop_id,
		const GValue     *value,
		GParamSpec       *pspec);
static void gfw_zoom_get_property  (GObject          *object,
		guint             prop_id,
		GValue           *value,
		GParamSpec       *pspec);
static void gfw_zoom_finalize(GObject *zoom);
static gboolean gfw_zoom_draw (GtkWidget *widget, cairo_t *cr);
static gboolean gfw_zoom_button_press(GtkWidget *widget, GdkEventButton *event);

G_DEFINE_TYPE (GfwZoom, gfw_zoom, GTK_TYPE_DRAWING_AREA);

static void gfw_zoom_class_init (GfwZoomClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	gobject_class->set_property = gfw_zoom_set_property;
	gobject_class->get_property = gfw_zoom_get_property;
	gobject_class->finalize = gfw_zoom_finalize;

	widget_class->draw = gfw_zoom_draw;
	widget_class->button_press_event = gfw_zoom_button_press;

	gfw_zoom_signals[ACTIVATED] = g_signal_new(
			"activated",
			G_OBJECT_CLASS_TYPE(gobject_class),
			G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(GfwZoomClass, activated),
			NULL, NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE, 0);

	gfw_zoom_signals[COLOR_CHANGED] = g_signal_new (
			"color-changed",
			G_OBJECT_CLASS_TYPE(gobject_class),
			G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(GfwZoomClass, color_changed),
			NULL, NULL,
			g_cclosure_marshal_VOID__POINTER,
			G_TYPE_NONE, 1,
			G_TYPE_POINTER);

	g_type_class_add_private (class, sizeof (GfwZoomPrivate));
}

static void gfw_zoom_init (GfwZoom *zoom)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	gtk_widget_add_events (GTK_WIDGET (zoom),
			GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_BUTTON_MOTION_MASK | GDK_2BUTTON_PRESS);

}

static void gfw_zoom_set_property (GObject      *object,
		guint         prop_id,
		const GValue *value,
		GParamSpec   *pspec)
{
	GfwZoom *zoom;

	zoom = GFW_ZOOM (object);

	switch (prop_id)
	{

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void gfw_zoom_get_property (GObject      *object,
		guint         prop_id,
		GValue       *value,
		GParamSpec   *pspec)
{
	GfwZoom *zoom;

	zoom = GFW_ZOOM (object);

	switch (prop_id)
	{

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void gfw_zoom_finalize(GObject *zoom)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	if (priv->pixbuf){
		g_object_unref (priv->pixbuf);
		priv->pixbuf = 0;
	}

	G_OBJECT_CLASS(gfw_zoom_parent_class)->finalize (zoom);
}

static gdouble zoom_transformation(gdouble value)
{
	return (1 - log(1 + value * 0.01 * 3) / log((double)(1 + 3))) / 2;
}

static GdkPoint gfw_zoom_get_screen_position(GfwZoom *zoom, const GdkPoint position)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	gint x, y, width, height;

	x = priv->pointer.x;
	y = priv->pointer.y;
	width = priv->screen_size.x;
	height = priv->screen_size.y;

	gint32 left, right, top, bottom;

	gint32 area_width = (guint32)(priv->width_height * zoom_transformation(priv->scale));
	if (!area_width) area_width = 1;

	left	= x - area_width / 2;
	top		= y - area_width / 2;
	right	= x + (area_width - area_width / 2);
	bottom	= y + (area_width - area_width / 2);

	if (left < 0){
		right += -left;
		left=0;
	}
	if (right > width){
		left -= right - width;
		right = width;
	}
	if (top < 0){
		bottom += -top;
		top = 0;
	}
	if (bottom > height){
		top -= bottom - height;
		bottom = height;
	}

	gint32 xl = ((position.x - left) * priv->width_height) / area_width;
	gint32 xh = (((position.x + 1) - left) * priv->width_height) / area_width;
	gint32 yl = ((position.y - top) * priv->width_height) / area_width;
	gint32 yh = (((position.y + 1) - top) * priv->width_height) / area_width;

	GdkPoint result;
	result = (GdkPoint) {(xl + xh) / 2.0, (yl + yh) / 2.0};
	return result;
}

static GdkRectangle gfw_zoom_get_screen_rect(GfwZoom* zoom, GdkPoint pointer, GdkPoint screen_size)
{
	GfwZoomPrivate *priv;
	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	gint x, y, width, height;
	gint32 left, right, top, bottom;
	gint32 area_width;

	x = pointer.x;
	y = pointer.y;
	width = screen_size.x;
	height = screen_size.y;


	area_width = priv->width_height * zoom_transformation(priv->scale);

	if (!area_width) area_width = 1;

	left	= x - area_width / 2;
	top		= y - area_width / 2;
	right	= x + (area_width - area_width / 2);
	bottom	= y + (area_width - area_width / 2);

	if (left < 0){
		right += -left;
		left = 0;
	}
	if (right > width){
		left -= right - width;
		right = width;
	}
	if (top < 0){
		bottom += -top;
		top = 0;
	}
	if (bottom > height){
		top -= bottom - height;
		bottom = height;
	}

	width	= right - left;
	height	= bottom - top;

	return (GdkRectangle){left, top, width, height};
}

static gdouble math_distance(const GdkPoint a, const GdkPoint b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

static gboolean rect_is_inside(const GdkRectangle source, const GdkRectangle target)
{
	if (target.width == 0 && target.height == 0) return FALSE;
	if (source.x < target.x || source.x + source.width > target.x + target.width) return FALSE;
	if (source.y < target.y || source.y + source.height > target.y + target.height) return FALSE;
	return TRUE;
}

static GdkRectangle get_position_inside(const GdkRectangle out, const GdkRectangle in)
{	
	GdkRectangle r;
	if (in.x < out.x){
		r.x = out.x;
		r.width = in.width;
	}else if (in.x + in.width > out.x + out.width){
		r.x = out.x + out.width - in.width;
		r.width = in.width;
	}else{
		r.x = in.x;
		r.width = in.width;
	}

	if (in.y + in.height < out.y + out.height){
		r.y = out.y;
		r.height = in.height;
	}else if (in.y + in.height > out.y + out.height){
		r.y = out.y + out.height - in.height;
		r.height = in.height;
	}else{
		r.y = in.y;
		r.height = in.height;
	}
	return r;
}

static gboolean gfw_zoom_draw (GtkWidget *widget, cairo_t *cr)
{
	GfwZoomPrivate *priv;
	GtkStyleContext *context;

	priv = GFW_ZOOM_GET_PRIVATE (widget);
	context = gtk_widget_get_style_context (widget);

	cairo_translate(cr, 0, 0);

	if (priv->pixbuf){
		gdk_cairo_set_source_pixbuf(cr, priv->pixbuf, 0, 0);
		if (priv->fade){
			cairo_paint_with_alpha(cr, 0.2);
		}else{
			cairo_paint(cr);
		}
	}

	if (!priv->fade){
		float radius;
		float size = sqrt(priv->point_size.x * priv->point_size.x  + priv->point_size.y * priv->point_size.y);
		if (size < 5){
			radius = 5;
		}else if (size < 25){
			radius = 7;
		}else if (size < 50){
			radius = 10;
		}else{
			radius = 15;
		}

		cairo_set_source_rgba(cr, 0,0,0,0.75);
		cairo_arc(cr, priv->point.x, priv->point.y, radius + 0.5, -G_PI, G_PI);
		cairo_stroke(cr);

		cairo_set_source_rgba(cr, 1,1,1,0.75);
		cairo_arc(cr, priv->point.x, priv->point.y, radius, -G_PI, G_PI);
		cairo_stroke(cr);
	}

	PangoLayout *layout;
	PangoFontDescription *font_description;
	font_description = pango_font_description_new();
	layout = pango_cairo_create_layout(cr);

	pango_font_description_set_family(font_description, "sans");
	pango_font_description_set_absolute_size(font_description, 12 * PANGO_SCALE);
	pango_layout_set_font_description(layout, font_description);

	GdkRectangle widget_rect;
	widget_rect = (GdkRectangle) { 5, 5, priv->width_height, priv->width_height};

	cairo_rectangle(cr, 0, 0, priv->width_height, priv->width_height);
	cairo_clip(cr);

	GdkPoint relative_positions[2];

	gboolean draw_distance = TRUE;
	int i;
	for (i = 0; i < 2; i++){
		if (priv->marks[i].valid){
			relative_positions[i] = gfw_zoom_get_screen_position(GFW_ZOOM(widget), priv->marks[i].position);
		}else{
			draw_distance = FALSE;
		}
	}

	int layer;
	for (layer = 0; layer != 2; layer++){
		if (draw_distance){
			cairo_move_to(cr, relative_positions[0].x, relative_positions[0].y);
			for (i = 1; i < 2; i++){
				cairo_line_to(cr, relative_positions[i].x, relative_positions[i].y);
			}
			if (layer == 0){
				cairo_set_source_rgba(cr, 0, 0, 0, 1);
				cairo_set_line_width(cr, 3);
			}else{
				cairo_set_source_rgba(cr, 1, 1, 1, 1);
				cairo_set_line_width(cr, 1);
			}
			cairo_stroke(cr);
		}
		for (i = 0; i < 2; i++){
			if (priv->marks[i].valid){
				cairo_arc(cr, relative_positions[i].x, relative_positions[i].y, 2, -G_PI, G_PI);

				if (layer == 0){
					cairo_set_source_rgba(cr, 0, 0, 0, 1);
					cairo_set_line_width(cr, 2);
					cairo_stroke(cr);
				}else{
					cairo_set_source_rgba(cr, 1, 1, 1, 1);
					cairo_fill(cr);
				}

				gchar *text;
				text = g_strdup_printf("%dx%d", priv->marks[i].position.x, priv->marks[i].position.y);
				pango_layout_set_text(layout, text, -1);
				pango_cairo_update_layout(cr, layout);
				g_free(text);

				cairo_move_to(cr, relative_positions[i].x + 5, relative_positions[i].y);
				if (layer == 0){
					cairo_set_source_rgba(cr, 0, 0, 0, 1);
					pango_cairo_layout_path(cr, layout);
					cairo_set_line_width(cr, 1.5);
					cairo_stroke(cr);
				}else{
					cairo_set_source_rgba(cr, 1, 1, 1, 1);
					pango_cairo_show_layout(cr, layout);
				}
			}
		}
	}
	for (layer = 0; layer != 2; layer++){
		if (draw_distance){
			gdouble distance = math_distance(priv->marks[0].position, priv->marks[1].position);
			GdkPoint center = {(priv->marks[0].position.x + priv->marks[1].position.x) * 0.5, (priv->marks[0].position.y + priv->marks[1].position.y) * 0.5};
			gchar *text;
			text = g_strdup_printf("%.1f\n%dx%d", distance, 1 + ABS(priv->marks[0].position.x - priv->marks[1].position.x), 1 + ABS(priv->marks[0].position.y - priv->marks[1].position.y));
			pango_layout_set_text(layout, text, -1);
			pango_cairo_update_layout(cr, layout);
			g_free(text);


			GdkPoint relative_position = gfw_zoom_get_screen_position(GFW_ZOOM(widget), center);

			PangoRectangle rect;
			pango_layout_get_pixel_extents(layout, NULL, &rect);
			int text_width = rect.width;
			int text_height = rect.height;

			GdkRectangle text_rect = { relative_position.x + 10, relative_position.y, text_width, text_height};

			if (! rect_is_inside(text_rect, widget_rect))
				text_rect = get_position_inside(widget_rect, text_rect);

			cairo_move_to(cr, text_rect.x, text_rect.y);

			if (layer == 0){
				cairo_set_source_rgba(cr, 0, 0, 0, 1);
				pango_cairo_layout_path(cr, layout);
				cairo_set_line_width(cr, 1.5);
				cairo_stroke(cr);
			}else{
				cairo_set_source_rgba(cr, 1, 1, 1, 1);
				pango_cairo_show_layout(cr, layout);
			}
		}
	}

	g_object_unref(layout);
	pango_font_description_free(font_description);

	gtk_render_frame(context, cr, 0, 0, priv->width_height, priv->width_height);

	return TRUE;
}

static gboolean gfw_zoom_button_press(GtkWidget *widget, GdkEventButton *event)
{
	gtk_widget_grab_focus(widget);

	if ((event->type == GDK_2BUTTON_PRESS) && (event->button == 1)) {
		g_signal_emit(widget, gfw_zoom_signals[ACTIVATED], 0);
	}
	return FALSE;
}

//Public functions
GtkWidget* gfw_zoom_new (void)
{
	GtkWidget *widget;
	GfwZoomPrivate *priv;
	int rowstride;
	guchar *p, *pixels;
	int x, y;

	widget = g_object_new (GFW_TYPE_ZOOM, NULL);
	priv = GFW_ZOOM_GET_PRIVATE (widget);

	priv->fade = FALSE;
	priv->scale = 20;
	priv->point.x = 0;
	priv->point.y = 0;
	priv->width_height = 150;
	priv->pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, priv->width_height, priv->width_height);

	rowstride = gdk_pixbuf_get_rowstride(priv->pixbuf);
	pixels = gdk_pixbuf_get_pixels(priv->pixbuf);
	for (y = 0; y < priv->width_height; y++) {
		p = pixels + y * rowstride;
		for (x = 0; x < priv->width_height * 3; x++){
			p[x] = 0x80;
		}
	}

	gtk_widget_set_size_request(GTK_WIDGET(widget),
			priv->width_height,
			priv->width_height);

	return widget;
}


void gfw_zoom_set_scale (GfwZoom* zoom, gfloat scale)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	if (scale < 0){
		priv->scale = 0;
	}else if (scale > 100){
		priv->scale = 100;
	}else{
		priv->scale = scale;
	}
	gtk_widget_queue_draw(GTK_WIDGET(zoom));
}

gfloat gfw_zoom_get_scale (GfwZoom* zoom)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	return priv->scale;
}

void gfw_zoom_set_fade (GfwZoom* zoom, gboolean fade)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	priv->fade = fade;
	gtk_widget_queue_draw(GTK_WIDGET(zoom));
}

gint32 gfw_zoom_get_size (GfwZoom* zoom)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	return priv->width_height;
}

void gfw_zoom_set_size (GfwZoom* zoom, gint32 size)
{
	GfwZoomPrivate *priv;
	int rowstride;
	guchar *p, *pixels;
	int x, y;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	if (priv->width_height != size ){
		if (priv->pixbuf){
			g_object_unref (priv->pixbuf);
			priv->pixbuf = 0;
		}

		priv->width_height = size;
		priv->pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, priv->width_height, priv->width_height);

		rowstride = gdk_pixbuf_get_rowstride(priv->pixbuf);
		pixels = gdk_pixbuf_get_pixels(priv->pixbuf);
		for (y = 0; y < priv->width_height; y++){
			p = pixels + y * rowstride;
			for (x = 0; x < priv->width_height * 3; x++){
				p[x] = 0x80;
			}
		}

		gtk_widget_set_size_request(GTK_WIDGET(zoom),
				priv->width_height,
				priv->width_height);
	}
}

void gfw_zoom_set_mark (GfwZoom* zoom, int index, GdkPoint position)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	priv->marks[index].position = position;
	priv->marks[index].valid = TRUE;

	gtk_widget_queue_draw(GTK_WIDGET(zoom));
}

void gfw_zoom_clear_mark (GfwZoom* zoom, int index)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	priv->marks[index].valid = FALSE;

	gtk_widget_queue_draw(GTK_WIDGET(zoom));
}

GdkRectangle gfw_zoom_get_current_screen_rect(GfwZoom *zoom)
{
	GfwZoomPrivate *priv;
	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	return gfw_zoom_get_screen_rect(zoom, priv->pointer, priv->screen_size);
}

void gfw_zoom_set_current_screen_size (GfwZoom* zoom, GdkPoint screen_size)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	priv->screen_size = screen_size;
}

void gfw_zoom_set_current_pointer(GfwZoom* zoom, GdkPoint pointer)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);
	priv->pointer = pointer;
}

void gfw_zoom_update (GfwZoom* zoom, GdkPoint pointer, GdkPoint screen_size, GdkPixbuf* pixbuf)
{
	GfwZoomPrivate *priv;
	gint x, y, width, height;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	priv->pointer = pointer;
	priv->screen_size = screen_size;

	x = pointer.x;
	y = pointer.y;
	width=screen_size.x;
	height=screen_size.y;

	gint left, right, top, bottom;

	gint32 area_width = priv->width_height * zoom_transformation(priv->scale);

	if (!area_width) area_width = 1;

	left	= x - area_width / 2;
	top		= y - area_width / 2;
	right	= x + (area_width - area_width / 2);
	bottom	= y + (area_width - area_width / 2);

	if (left < 0){
		right += -left;
		left=0;
	}
	if (right > width){
		left -= right - width;
		right = width;
	}
	if (top < 0){
		bottom += -top;
		top = 0;
	}
	if (bottom > height){
		top -= bottom - height;
		bottom = height;
	}

	gint32 xl = ((x - left) * priv->width_height) / area_width;
	gint32 xh = (((x + 1) - left) * priv->width_height) / area_width;
	gint32 yl = ((y - top) * priv->width_height) / area_width;
	gint32 yh = (((y + 1) - top) * priv->width_height) / area_width;

	priv->point.x = (xl + xh) / 2.0;
	priv->point.y = (yl + yh) / 2.0;
	priv->point_size.x = xh - xl;
	priv->point_size.y = yh - yl;

	width	= right - left;
	height	= bottom - top;

	GdkRectangle r = gfw_zoom_get_current_screen_rect(zoom);
	gdk_pixbuf_scale(pixbuf,
			priv->pixbuf,
			0, //dest_x
			0, //dest_y
			priv->width_height, //dest_width
			priv->width_height, //dest_height
			-r.x * priv->width_height / (double)width, //offset_x
			-r.y * priv->width_height / (double)height, //offset_y
			priv->width_height / (double)width, //scale_x
			priv->width_height / (double)height, //scale_y
			GDK_INTERP_NEAREST);
	gtk_widget_queue_draw(GTK_WIDGET(zoom));
}

void gfw_zoom_set_current_pixbuf (GfwZoom* zoom, GdkPixbuf* pixbuf)
{
	GfwZoomPrivate *priv;

	priv = GFW_ZOOM_GET_PRIVATE (zoom);

	gfw_zoom_update (zoom, priv->pointer, priv->screen_size, pixbuf);
}
