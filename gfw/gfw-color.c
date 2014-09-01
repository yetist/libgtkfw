/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-color.c: This file is part of ____
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

#include "gfw-color.h"

enum {
	ACTIVATED,
	LAST_SIGNAL
};

enum {
	PROP_0,
};

static guint gfw_color_signals[LAST_SIGNAL] = {0};

#define GFW_COLOR_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), GFW_TYPE_COLOR, GfwColorPrivate))
#define ROUND(x) ((int) ((x) + 0.5))

typedef struct _GfwColorPrivate        GfwColorPrivate;

struct _GfwColorPrivate
{
	GdkRGBA rgba;
	GdkRGBA text_rgba;
};

static void gfw_color_set_property  (GObject          *object,
		guint             prop_id,
		const GValue     *value,
		GParamSpec       *pspec);
static void gfw_color_get_property  (GObject          *object,
		guint             prop_id,
		GValue           *value,
		GParamSpec       *pspec);
static gboolean gfw_color_draw(GtkWidget *widget, cairo_t *cr);
static gboolean gfw_color_button_release(GtkWidget *widget, GdkEventButton *event);
static gboolean gfw_color_button_press(GtkWidget *widget, GdkEventButton *event);
static void gfw_color_get_preferred_width (GtkWidget *widget, gint *minimum, gint *natural);
static void gfw_color_get_preferred_height(GtkWidget *widget, gint *minimum, gint *natural);

G_DEFINE_TYPE (GfwColor, gfw_color, GTK_TYPE_DRAWING_AREA);

static void gfw_color_class_init (GfwColorClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	gobject_class->set_property = gfw_color_set_property;
	gobject_class->get_property = gfw_color_get_property;

	widget_class->draw = gfw_color_draw;
	widget_class->button_release_event = gfw_color_button_release;
	widget_class->button_press_event = gfw_color_button_press;
	widget_class->get_preferred_width =  gfw_color_get_preferred_width;
	widget_class->get_preferred_height = gfw_color_get_preferred_height;

	gfw_color_signals[ACTIVATED] = g_signal_new("activated",
			G_OBJECT_CLASS_TYPE(gobject_class),
			G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(GfwColorClass, activated),
			NULL, NULL,
			g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

	g_type_class_add_private (class, sizeof (GfwColorPrivate));
}

static void gfw_color_init (GfwColor *color)
{
	GfwColorPrivate *priv;

	priv = GFW_COLOR_GET_PRIVATE (color);

	priv->rgba = (GdkRGBA){0.0, 0.0, 0.0, 0.0};
	priv->text_rgba = (GdkRGBA){1.0, 1.0, 1.0, 1.0};

	gtk_widget_set_can_focus(GTK_WIDGET(color), TRUE);
	gtk_widget_add_events (GTK_WIDGET (color), 
			GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_FOCUS_CHANGE_MASK | GDK_2BUTTON_PRESS | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

}

static void gfw_color_set_property (GObject      *object,
		guint         prop_id,
		const GValue *value,
		GParamSpec   *pspec)
{
	GfwColor *color;

	color = GFW_COLOR (object);

	switch (prop_id)
	{

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void gfw_color_get_property (GObject      *object,
		guint         prop_id,
		GValue       *value,
		GParamSpec   *pspec)
{
	GfwColor *color;

	color = GFW_COLOR (object);

	switch (prop_id)
	{

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void gfw_color_get_preferred_width (GtkWidget *widget,
		gint      *minimum,
		gint      *natural)
{
	*minimum = *natural = 150;
}

static void gfw_color_get_preferred_height (GtkWidget *widget,
		gint      *minimum,
		gint      *natural)
{
	*minimum = *natural = 30;
}

static gboolean gfw_color_draw(GtkWidget *widget, cairo_t *cr)
{

	GfwColorPrivate *priv;
	gchar *text;
	gint width, height;

	priv = GFW_COLOR_GET_PRIVATE (widget);

	cairo_set_source_rgba(cr, priv->rgba.red, priv->rgba.green, priv->rgba.blue, priv->rgba.alpha);
	cairo_rectangle(cr, 0, 0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));
	cairo_fill(cr);

	text = gdk_rgba_to_string(&priv->rgba);
	PangoLayout *layout;
	PangoFontDescription *font_description;
	font_description = pango_font_description_new();
	layout = pango_cairo_create_layout(cr);

	pango_font_description_set_family(font_description, "monospace");
	pango_font_description_set_weight(font_description, PANGO_WEIGHT_NORMAL);
	pango_font_description_set_absolute_size(font_description, 14 * PANGO_SCALE);
	pango_layout_set_font_description(layout, font_description);
	pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);

	cairo_set_source_rgba(cr, priv->text_rgba.red, priv->text_rgba.green, priv->text_rgba.blue, priv->text_rgba.alpha);

	pango_layout_set_markup(layout, text, -1);
	pango_layout_set_width(layout, gtk_widget_get_allocated_width(widget) * PANGO_SCALE);
	pango_layout_set_height(layout, gtk_widget_get_allocated_height(widget) * PANGO_SCALE);

	pango_layout_get_pixel_size(layout, &width, &height);
	cairo_move_to(cr, 0, (gtk_widget_get_allocated_height(widget) - height) / 2);

	pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

	pango_cairo_update_layout(cr, layout);
	pango_cairo_show_layout(cr, layout);

	g_object_unref (layout);
	pango_font_description_free (font_description);
	g_free(text);

	return FALSE;
}

static gboolean gfw_color_button_release(GtkWidget *widget, GdkEventButton *event)
{
	gtk_widget_grab_focus(widget);
	return FALSE;
}

static gboolean gfw_color_button_press(GtkWidget *widget, GdkEventButton *event)
{
	gtk_widget_grab_focus(widget);

	if ((event->type == GDK_2BUTTON_PRESS) && (event->button == 1)) {
		g_signal_emit(widget, gfw_color_signals[ACTIVATED], 0);
	}

	return FALSE;
}

static gdouble value_to_revert(gdouble value)
{
	gdouble v;
	v = 1.0 - value;
	if (ABS(v - value) < 0.1) {
		if (value > v) 
			return v - 0.3;
		else
			return v + 0.3;
	} else {
		return v;
	}
}

static void color_update_text_rgba(GfwColor *color)
{
	GfwColorPrivate *priv;
	priv = GFW_COLOR_GET_PRIVATE (color);

	priv->text_rgba.red = value_to_revert(priv->rgba.red);
	priv->text_rgba.green = value_to_revert(priv->rgba.green);
	priv->text_rgba.blue = value_to_revert(priv->rgba.blue);
	priv->text_rgba.alpha = 1.0;
}

//Public function
GtkWidget* gfw_color_new (void)
{
	return g_object_new (GFW_TYPE_COLOR, NULL);
}

void gfw_color_set_rgba (GfwColor* color, GdkRGBA rgba)
{
	GfwColorPrivate *priv;

	priv = GFW_COLOR_GET_PRIVATE (color);

	priv->rgba = rgba;
	color_update_text_rgba(color);
	gtk_widget_queue_draw(GTK_WIDGET(color));
}

GdkRGBA* gfw_color_get_rgba (GfwColor* color)
{
	GfwColorPrivate *priv;

	priv = GFW_COLOR_GET_PRIVATE (color);

	return gdk_rgba_copy (&priv->rgba);
}
