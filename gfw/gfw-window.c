/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-window.c: This file is part of ____
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

#include "gfw-window.h"

enum {
    LAST_SIGNAL
};

enum {
    PROP_0,
	PROP_BACKGROUND,
	PROP_TRANSPARENT,
	PROP_SIZE_FIT_PIXBUF,
};

#define GFW_WINDOW_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), GFW_TYPE_WINDOW, GfwWindowPrivate))

typedef struct _GfwWindowPrivate        GfwWindowPrivate;

struct _GfwWindowPrivate
{
	GdkPixbuf *background;
	gboolean transparent;
	gboolean size_fit_pixbuf;
};

static void gfw_window_set_property  (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gfw_window_get_property  (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void gfw_window_finalize (GObject *object);
#if GTK_CHECK_VERSION(3,0,0)
static void gfw_window_get_preferred_width (GtkWidget *widget, gint *minimal_width, gint *natural_width);
static void gfw_window_get_preferred_height (GtkWidget *widget, gint *minimal_height, gint *natural_height);
static gboolean gfw_window_draw (GtkWidget *widget, cairo_t *cr);
#else
static gboolean gfw_window_expose (GtkWidget *widget, GdkEventExpose *event);
static void gfw_window_size_request (GtkWidget *widget, GtkRequisition *requisition);
#endif
void gfw_window_set_background (GfwWindow *window, GdkPixbuf *pixbuf);

G_DEFINE_TYPE (GfwWindow, gfw_window, GTK_TYPE_WINDOW);

static void
gfw_window_class_init (GfwWindowClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS (class);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

    object_class->set_property = gfw_window_set_property;
    object_class->get_property = gfw_window_get_property;
	object_class->finalize = gfw_window_finalize;

#if GTK_CHECK_VERSION(3,0,0)
	widget_class->draw = gfw_window_draw;
	widget_class->get_preferred_width = gfw_window_get_preferred_width;
	widget_class->get_preferred_height = gfw_window_get_preferred_height;
#else
	widget_class->expose_event = gfw_window_expose;
	widget_class->size_request = gfw_window_size_request;
#endif

	g_object_class_install_property (object_class,
			PROP_BACKGROUND,
			g_param_spec_object ("background",
				"Normal Image",
				"A GdkPixbuf for window background",
				GDK_TYPE_PIXBUF,
				G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));
	g_object_class_install_property (object_class,
			PROP_TRANSPARENT,
			g_param_spec_boolean ("transparent",
				"Transparent Mode",
				"Weather window is transparent",
				FALSE,
				G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));
	g_object_class_install_property (object_class,
			PROP_SIZE_FIT_PIXBUF,
			g_param_spec_boolean ("size-fit-pixbuf",
				"Fit pixbuf size",
				"Weather window's size fit the pixbuf",
				FALSE,
				G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));

    g_type_class_add_private (class, sizeof (GfwWindowPrivate));
}

static void
gfw_window_init (GfwWindow *window)
{
    GfwWindowPrivate *priv;

    priv = GFW_WINDOW_GET_PRIVATE (window);
	priv->background = NULL;
	priv->transparent = FALSE;
	priv->size_fit_pixbuf = FALSE;
}

static void gfw_window_finalize (GObject *object)
{
	GfwWindow *window;
    GfwWindowPrivate *priv;

	window = GFW_WINDOW (object);
    priv = GFW_WINDOW_GET_PRIVATE (window);

	if (priv->background != NULL)
		g_object_unref(priv->background);
	G_OBJECT_CLASS (gfw_window_parent_class)->finalize (object);
}

GtkWidget* gfw_window_new (GtkWindowType type)
{
	GfwWindow *window;
	g_return_val_if_fail (type >= GTK_WINDOW_TOPLEVEL && type <= GTK_WINDOW_POPUP, NULL);

	window = g_object_new (GFW_TYPE_WINDOW, "type", type, NULL);
	return GTK_WIDGET(window);
}

static void
gfw_window_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    GfwWindow *window;
    GfwWindowPrivate *priv;

    window = GFW_WINDOW (object);
    priv = GFW_WINDOW_GET_PRIVATE (window);

    switch (prop_id)
    {
		case PROP_BACKGROUND:
			gfw_window_set_background (window, g_value_get_object(value));
			break;
		case PROP_TRANSPARENT:
			priv->transparent = g_value_get_boolean(value);
			break;
		case PROP_SIZE_FIT_PIXBUF:
			priv->size_fit_pixbuf = g_value_get_boolean(value);
			break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
gfw_window_get_property (GObject      *object,
                            guint         prop_id,
                            GValue       *value,
                            GParamSpec   *pspec)
{
    GfwWindow *window;
    GfwWindowPrivate *priv;

    window = GFW_WINDOW (object);
    priv = GFW_WINDOW_GET_PRIVATE (window);

    switch (prop_id)
    {
		case PROP_BACKGROUND:
			g_value_set_object (value, priv->background);
			break;
		case PROP_TRANSPARENT:
			g_value_set_boolean (value, priv->transparent);
			break;
		case PROP_SIZE_FIT_PIXBUF:
			g_value_set_boolean (value, priv->size_fit_pixbuf);
			break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

#if GTK_CHECK_VERSION(3,0,0)
static void gfw_window_get_preferred_width (GtkWidget *widget, gint *minimal_width, gint *natural_width)
{
	GtkRequisition requisition;
	GfwWindowPrivate *priv;
	GtkWidget *child_widget;

	priv = GFW_WINDOW_GET_PRIVATE (widget);

	*minimal_width = *natural_width = gtk_container_get_border_width(GTK_CONTAINER(widget)) * 2;
	child_widget = gtk_bin_get_child(GTK_BIN(widget));
	if (child_widget != NULL && gtk_widget_get_visible (child_widget))
	{
		gint min_width, nat_width;
		gtk_widget_get_preferred_width(child_widget, &min_width, &nat_width);
		*minimal_width += min_width;
		*natural_width += nat_width;
	}

	if (priv->background != NULL && (priv->size_fit_pixbuf || priv->transparent))
	{
		gint width;
		width = gdk_pixbuf_get_width(priv->background);
		if (*minimal_width < width)
			*minimal_width = width;
		if (*natural_width< width)
			*natural_width = width;
	}
}

static void gfw_window_get_preferred_height (GtkWidget *widget, gint *minimal_height, gint *natural_height)
{
	GtkRequisition requisition;
	GfwWindowPrivate *priv;
	GtkWidget *child_widget;

	priv = GFW_WINDOW_GET_PRIVATE (widget);

	*minimal_height = *natural_height = gtk_container_get_border_width(GTK_CONTAINER(widget)) * 2;
	child_widget = gtk_bin_get_child(GTK_BIN(widget));
	if (child_widget != NULL && gtk_widget_get_visible (child_widget))
	{
		gint min_height, nat_height;
		gtk_widget_get_preferred_height(child_widget, &min_height, &nat_height);
		*minimal_height += min_height;
		*natural_height += nat_height;
	}

	if (priv->background != NULL && (priv->size_fit_pixbuf || priv->transparent))
	{
		gint height;
		height = gdk_pixbuf_get_height(priv->background);
		if (*minimal_height < height)
			*minimal_height = height;
		if (*natural_height< height)
			*natural_height = height;
	}
}

static gboolean gfw_window_draw (GtkWidget *widget, cairo_t *cr)
{
	GfwWindow *window;
	GfwWindowPrivate *priv;

	window = GFW_WINDOW (widget);
	priv = GFW_WINDOW_GET_PRIVATE (window);

	if (priv->background != NULL)
	{
		cairo_surface_t *surface;
		surface = gdk_cairo_surface_create_from_pixbuf (priv->background, 1, NULL);
		//surface = gdk_cairo_surface_create_from_pixbuf (priv->background, 1, GDK_WINDOW(gtk_widget_get_window(widget)));
		if (priv->transparent)
		{
			cairo_region_t *region;
			region = gdk_cairo_region_create_from_surface (surface);
			gdk_window_shape_combine_region (GDK_WINDOW(gtk_widget_get_window(widget)), region, 0, 0);
			cairo_region_destroy(region);
		}
		cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
		//cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
		cairo_set_source_surface (cr, surface, 0, 0);
		cairo_paint(cr);
	}

	if (GTK_WIDGET_CLASS (gfw_window_parent_class)->draw)
		GTK_WIDGET_CLASS (gfw_window_parent_class)->draw(widget, cr);

	return FALSE;
}
#else
static void gfw_window_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	GfwWindow *window;
	GfwWindowPrivate *priv;
	GtkBin *bin;

	window = GFW_WINDOW (widget);
	priv = GFW_WINDOW_GET_PRIVATE (window);

	bin = GTK_BIN (window);

	requisition->width = GTK_CONTAINER (window)->border_width * 2;
	requisition->height = GTK_CONTAINER (window)->border_width * 2;

	if (bin->child && GTK_WIDGET_VISIBLE (bin->child))
	{
		GtkRequisition child_requisition;

		gtk_widget_size_request (bin->child, &child_requisition);

		requisition->width += child_requisition.width;
		requisition->height += child_requisition.height;
	}
	if (priv->background != NULL && priv->size_fit_pixbuf)
	{
		gint width, height;
		width = gdk_pixbuf_get_width(priv->background);
		height = gdk_pixbuf_get_height(priv->background);
		if (requisition->width < width)
			requisition->width = width;
		if (requisition->height < height)
			requisition->height = height;
	}
}

static gboolean gfw_window_expose (GtkWidget *widget, GdkEventExpose *event)
{
    GfwWindow *window;
    GfwWindowPrivate *priv;

    window = GFW_WINDOW (widget);
    priv = GFW_WINDOW_GET_PRIVATE (window);

	if (priv->background != NULL)
	{
		if (priv->transparent)
		{
			GdkBitmap *bitmap;
			gdk_pixbuf_render_pixmap_and_mask(priv->background, NULL, &bitmap, 128);
			gtk_widget_shape_combine_mask(widget, bitmap, 0, 0);
			if (bitmap != NULL)
				g_object_unref(bitmap);
		}

		cairo_t *cr = gdk_cairo_create(widget->window);
		gdk_cairo_set_source_pixbuf(cr, priv->background, 0, 0);
		cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
		cairo_paint(cr);
		cairo_destroy(cr);
	}

  if (GTK_WIDGET_CLASS (gfw_window_parent_class)->expose_event)
    return GTK_WIDGET_CLASS (gfw_window_parent_class)->expose_event (widget, event);

  return FALSE;
}
#endif

void gfw_window_set_background (GfwWindow *window, GdkPixbuf *pixbuf)
{
    GfwWindowPrivate *priv;

	g_return_if_fail (GFW_IS_WINDOW (window));
	g_return_if_fail (GDK_IS_PIXBUF (pixbuf));

    priv = GFW_WINDOW_GET_PRIVATE (window);

	if (priv->background != NULL)
		g_object_unref(priv->background);
	priv->background = pixbuf;

	if (!gtk_widget_get_realized (GTK_WIDGET(window)))
	{
		gtk_widget_set_app_paintable(GTK_WIDGET(window),TRUE);
		gtk_widget_realize (GTK_WIDGET(window));
	}

}

void gfw_window_set_transparent (GfwWindow *window, gboolean transparent)
{
    GfwWindowPrivate *priv;

	g_return_if_fail (GFW_IS_WINDOW (window));

    priv = GFW_WINDOW_GET_PRIVATE (window);

	transparent = transparent != FALSE;

	if (priv->transparent == transparent)
		return;
	priv->transparent = transparent;

	g_object_notify (G_OBJECT (window), "transparent");
}

void gfw_window_set_size_fit_pixbuf (GfwWindow *window, gboolean is_fit)
{
    GfwWindowPrivate *priv;

	g_return_if_fail (GFW_IS_WINDOW (window));

    priv = GFW_WINDOW_GET_PRIVATE (window);

	is_fit = is_fit != FALSE;

	if (priv->size_fit_pixbuf == is_fit)
		return;
	priv->size_fit_pixbuf = is_fit;
	g_object_notify (G_OBJECT (window), "size_fit_pixbuf");
	gtk_window_set_resizable(GTK_WINDOW(window), !is_fit);
}

gboolean gfw_window_get_transparent (GfwWindow *window)
{
    GfwWindowPrivate *priv;
	g_return_val_if_fail (GFW_IS_WINDOW (window), FALSE);

    priv = GFW_WINDOW_GET_PRIVATE (window);
	return  priv->transparent;
}

gboolean gfw_window_get_size_fit_pixbuf (GfwWindow *window)
{
    GfwWindowPrivate *priv;

	g_return_val_if_fail (GFW_IS_WINDOW (window), FALSE);

    priv = GFW_WINDOW_GET_PRIVATE (window);
	return  priv->size_fit_pixbuf;
}
