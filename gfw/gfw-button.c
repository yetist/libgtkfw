/* vi: set sw=4 ts=4 wrap ai: */
/*
 * gfw-button.c: This file is part of ____
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

#include "gfw-button.h"

enum {
	CLICKED,
	LAST_SIGNAL
};

enum {
	PROP_0,
	PROP_NORMAL_PIXBUF,
	PROP_ACTIVE_PIXBUF,
	PROP_PRELIGHT_PIXBUF
};

#define GFW_BUTTON_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), GFW_TYPE_BUTTON, GfwButtonPrivate))

typedef struct _GfwButtonPrivate        GfwButtonPrivate;

struct _GfwButtonPrivate
{
	guint in_button:1;
	GdkPixbuf *pixbuf[3];
};

static void gfw_button_set_property  (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gfw_button_get_property  (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static gboolean gfw_button_press (GtkWidget *widget, GdkEventButton *event);
static gboolean gfw_button_release (GtkWidget *widget, GdkEventButton *event);
static gboolean gfw_button_enter_notify (GtkWidget *widget, GdkEventCrossing *event);
static gboolean gfw_button_leave_notify (GtkWidget *widget, GdkEventCrossing *event);

static void gfw_button_state_changed   (GtkWidget *widget, GtkStateType previous_state);
void gfw_button_clicked (GfwButton *button);

#if GTK_CHECK_VERSION(3,0,0)
static void gfw_button_get_preferred_width (GtkWidget *widget, gint *minimal_width, gint *natural_width);
static void gfw_button_get_preferred_height (GtkWidget *widget, gint *minimal_height, gint *natural_height);
static gboolean gfw_button_draw (GtkWidget *widget, cairo_t *cr);
#else
static void gfw_button_size_request (GtkWidget *widget, GtkRequisition *requisition);
static gboolean gfw_button_expose (GtkWidget *widget, GdkEventExpose *event);
#endif

static guint button_signals[LAST_SIGNAL] = { 0 };
 
G_DEFINE_TYPE (GfwButton, gfw_button, GTK_TYPE_EVENT_BOX);

static void gfw_button_class_init (GfwButtonClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	gobject_class->set_property = gfw_button_set_property;
	gobject_class->get_property = gfw_button_get_property;

#if GTK_CHECK_VERSION(3,0,0)
		widget_class->get_preferred_width = gfw_button_get_preferred_width;
		widget_class->get_preferred_height = gfw_button_get_preferred_height;
		widget_class->draw = gfw_button_draw;
#else
		widget_class->size_request = gfw_button_size_request;
		widget_class->expose_event = gfw_button_expose;
#endif

	widget_class->state_changed = gfw_button_state_changed;
	widget_class->button_press_event = gfw_button_press;
	widget_class->button_release_event = gfw_button_release;
	widget_class->enter_notify_event = gfw_button_enter_notify;
	widget_class->leave_notify_event = gfw_button_leave_notify;
	class->clicked = NULL;
	/**
	 * GtkButton::clicked:
	 * @button: the object that received the signal
	 *
	 * Emitted when the button has been activated (pressed and released).
	 */ 
	button_signals[CLICKED] =
		g_signal_new ("clicked",
				G_OBJECT_CLASS_TYPE (gobject_class),
				G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
				G_STRUCT_OFFSET (GfwButtonClass, clicked),
				NULL, NULL,
				g_cclosure_marshal_VOID__VOID,
				G_TYPE_NONE, 0);

	g_object_class_install_property (gobject_class,
			PROP_NORMAL_PIXBUF,
			g_param_spec_object ("normal-pixbuf",
				"Normal Image",
				"A GdkImage to display",
				GDK_TYPE_PIXBUF,
				G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));

	g_object_class_install_property (gobject_class,
			PROP_ACTIVE_PIXBUF,
			g_param_spec_object ("active-pixbuf",
				"Normal Image",
				"A GdkImage to display",
				GDK_TYPE_PIXBUF,
				G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));

	g_object_class_install_property (gobject_class,
			PROP_PRELIGHT_PIXBUF,
			g_param_spec_object ("prelight-pixbuf",
				"Normal Image",
				"A GdkImage to display",
				GDK_TYPE_PIXBUF,
				G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));

	g_type_class_add_private (class, sizeof (GfwButtonPrivate));
}

static void gfw_button_init (GfwButton *button)
{
	GfwButtonPrivate *priv;
	priv = GFW_BUTTON_GET_PRIVATE (button);
	priv->in_button = FALSE;
	priv->pixbuf[GTK_STATE_NORMAL] = NULL;
	priv->pixbuf[GTK_STATE_ACTIVE] = NULL;
	priv->pixbuf[GTK_STATE_PRELIGHT] = NULL;
}

void gfw_button_set_pixbuf(GfwButton *button, GtkStateType state, GdkPixbuf *pixbuf)
{
	g_return_if_fail (GFW_IS_BUTTON(button));
	g_return_if_fail (GDK_IS_PIXBUF(pixbuf));
	g_return_if_fail (state >= GTK_STATE_NORMAL && state <= GTK_STATE_PRELIGHT);

	GfwButtonPrivate *priv;
	priv = GFW_BUTTON_GET_PRIVATE (button);

	if (priv->pixbuf[state] != NULL)
		g_object_unref(priv->pixbuf[state]);
	priv->pixbuf[state] = pixbuf;
}

static void gfw_button_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	GfwButton *button;
	GfwButtonPrivate *priv;
	GtkStateType state;

	button = GFW_BUTTON (object);
	priv = GFW_BUTTON_GET_PRIVATE (button);

	switch (prop_id)
	{
		case PROP_NORMAL_PIXBUF:
			gfw_button_set_pixbuf(button, GTK_STATE_NORMAL, g_value_get_object(value));
			break;
		case PROP_ACTIVE_PIXBUF:
			gfw_button_set_pixbuf(button, GTK_STATE_ACTIVE, g_value_get_object(value));
			break;
		case PROP_PRELIGHT_PIXBUF:
			gfw_button_set_pixbuf(button, GTK_STATE_PRELIGHT, g_value_get_object(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void gfw_button_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	GfwButton *button;
	GfwButtonPrivate *priv;

	button = GFW_BUTTON (object);
	priv = GFW_BUTTON_GET_PRIVATE (button);

	switch (prop_id)
	{
		case PROP_NORMAL_PIXBUF:
			g_value_set_object (value, priv->pixbuf[GTK_STATE_NORMAL]);
			break;
		case PROP_ACTIVE_PIXBUF:
			g_value_set_object (value, priv->pixbuf[GTK_STATE_ACTIVE]);
			break;
		case PROP_PRELIGHT_PIXBUF:
			g_value_set_object (value, priv->pixbuf[GTK_STATE_PRELIGHT]);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

/* public functions below */

GtkWidget* gfw_button_new (GdkPixbuf *pixbuf)
{
	GtkWidget *widget;
	widget = g_object_new (GFW_TYPE_BUTTON, "normal-pixbuf", pixbuf, NULL);
	gtk_widget_set_events(widget, GDK_MOTION_NOTIFY | GDK_BUTTON_PRESS | GDK_BUTTON_RELEASE | GDK_ENTER_NOTIFY | GDK_LEAVE_NOTIFY);
	gtk_widget_set_state(widget, GTK_STATE_NORMAL);
	return widget;
}

GtkWidget* gfw_button_new_with_pixbufs (GdkPixbuf *pixbuf, const gchar *first_property_name, ...)
{
	GtkWidget *widget;
	va_list var_args;

	widget = g_object_new (GFW_TYPE_BUTTON, "normal-pixbuf", pixbuf, NULL);
	gtk_widget_set_events(widget, GDK_MOTION_NOTIFY | GDK_BUTTON_PRESS | GDK_BUTTON_RELEASE | GDK_ENTER_NOTIFY | GDK_LEAVE_NOTIFY);

	/* short circuit for calls supplying no properties */
	if (!first_property_name)
		return widget;

	va_start (var_args, first_property_name);
	g_object_set_valist (G_OBJECT (widget), first_property_name, var_args);
	va_end (var_args);
	gtk_widget_set_state(widget, GTK_STATE_NORMAL);
	return widget;
}

static gboolean gfw_button_press (GtkWidget *widget, GdkEventButton *event)
{
	GfwButton *button;
	GfwButtonPrivate *priv;

	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);

	g_printf("call button_press\n");
	if (event->button == 1)
	{
		if (priv->in_button && priv->pixbuf[GTK_STATE_ACTIVE] != NULL)
		{
			gtk_widget_set_state(widget, GTK_STATE_ACTIVE);
		}
	}
	return TRUE;
}

static gboolean gfw_button_release (GtkWidget *widget, GdkEventButton *event)
{
	GfwButton *button;
	GfwButtonPrivate *priv;

	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);

	g_printf("call button_release\n");
	if (event->button == 1)
	{
		if (priv->in_button)
		{
			if (priv->pixbuf[GTK_STATE_PRELIGHT] != NULL)
			{
				gtk_widget_set_state(widget, GTK_STATE_PRELIGHT);
			}
			else
			{
				gtk_widget_set_state(widget, GTK_STATE_NORMAL);
			}
			gfw_button_clicked(button);
		}
		else
		{
			gtk_widget_set_state(widget, GTK_STATE_NORMAL);
		}
	}
	return TRUE;
}

void gfw_button_clicked (GfwButton *button)
{
  g_return_if_fail (GFW_IS_BUTTON (button));
  g_printf("call button_clicked\n");

  g_signal_emit (button, button_signals[CLICKED], 0);
}

static gboolean gfw_button_enter_notify (GtkWidget *widget, GdkEventCrossing *event)
{
	GfwButton *button;
	GfwButtonPrivate *priv;
	GtkWidget *event_widget;

	g_printf("call enter_notify\n");
	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);

	event_widget = gtk_get_event_widget ((GdkEvent*) event);

	if ((event_widget == widget) && (event->detail != GDK_NOTIFY_INFERIOR))
	{
		priv->in_button = TRUE;
	}
	if (priv->in_button && priv->pixbuf[GTK_STATE_PRELIGHT] != NULL)
	{
		gtk_widget_set_state(widget, GTK_STATE_PRELIGHT);
	}
	return FALSE;
}

static gboolean gfw_button_leave_notify (GtkWidget *widget, GdkEventCrossing *event)
{
	GfwButton *button;
	GfwButtonPrivate *priv;
	GtkWidget *event_widget;

	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);
	g_printf("call leave_notify\n");

	event_widget = gtk_get_event_widget ((GdkEvent*) event);

	if ((event_widget == widget) && (event->detail != GDK_NOTIFY_INFERIOR) && (gtk_widget_get_sensitive(event_widget)))
	{
			priv->in_button = FALSE;
	}
	if (!priv->in_button)
	{
		gtk_widget_set_state(widget, GTK_STATE_NORMAL);
	}
	return FALSE;
}

#if GTK_CHECK_VERSION(3,0,0)
static void gfw_button_get_preferred_width (GtkWidget *widget, gint *minimal_width, gint *natural_width)
{
	GtkRequisition requisition;
	GfwButtonPrivate *priv;

	priv = GFW_BUTTON_GET_PRIVATE (widget);

	*minimal_width = *natural_width = gdk_pixbuf_get_width(priv->pixbuf[gtk_widget_get_state(widget)]);
}

static void gfw_button_get_preferred_height (GtkWidget *widget, gint *minimal_height, gint *natural_height)
{
	GtkRequisition requisition;
	GfwButtonPrivate *priv;

	priv = GFW_BUTTON_GET_PRIVATE (widget);

	*minimal_height = *natural_height = gdk_pixbuf_get_height(priv->pixbuf[gtk_widget_get_state(widget)]);
}
#else
static void gfw_button_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
	GfwButton *button;
	GfwButtonPrivate *priv;
	GdkPixbuf *pixbuf;

	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);
	pixbuf = priv->pixbuf[gtk_widget_get_state(widget)];

	requisition->width = gdk_pixbuf_get_width(pixbuf);
	requisition->height = gdk_pixbuf_get_height(pixbuf);
}
#endif

static void gfw_button_state_changed (GtkWidget *widget, GtkStateType  previous_state)
{
	GfwButton *button;
	GfwButtonPrivate *priv;
	GtkStateType state;

	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);

	state = gtk_widget_get_state(widget);

	g_printf("call state_changed, prev_stat=%d, current_stat=%d\n", previous_state, state);

	gtk_widget_queue_draw (GTK_WIDGET (widget));
}

#if GTK_CHECK_VERSION(3,0,0)
static gboolean gfw_button_draw (GtkWidget *widget, cairo_t *cr)
{
	GfwButton *button;
	GfwButtonPrivate *priv;
	//GtkStateType state;

	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);
	g_print("call expose\n");
	//state = gtk_widget_get_state(widget);
	
	GtkStyleContext *context;
	GtkStateFlags state;
	GtkBorder padding;

	context = gtk_widget_get_style_context (widget);
	state = gtk_widget_get_state_flags (widget);
	gtk_style_context_get_padding (context, state, &padding);

	cairo_save (cr);


	gint x, y;
	gint width, height;
	gint extent;

	context = gtk_widget_get_style_context (widget);

	width = gtk_widget_get_allocated_width (widget);
	height = gtk_widget_get_allocated_height (widget);

#define PAD 3
	extent = MIN (width - 2 * PAD, height - 2 * PAD);
	x = PAD;
	y = PAD;

	gtk_render_arrow (context, cr, G_PI / 2, x, y, extent);


//  if (gtk_widget_get_mapped (widget))
  {
	  GdkPixbuf *pixbuf;
	  cairo_surface_t *mask;
	  cairo_region_t *mask_region;

	 // cairo_save(cr);

	 // cairo_set_source_rgb (cr, 0, 0, 0);
	 // cairo_rectangle (cr, 0.25, 0.25, 0.5, 0.5);
	 // cairo_fill (cr);


//	  pixbuf = priv->pixbuf[state];
//	  GdkRectangle image_bound;
//	  image_bound.width = MIN(gtk_widget_get_allocated_width (widget), gdk_pixbuf_get_width(priv->pixbuf[state]));
//	  image_bound.height = MIN(gtk_widget_get_allocated_height (widget), gdk_pixbuf_get_height(priv->pixbuf[state]));
//
//	  pixbuf = gdk_pixbuf_new_subpixbuf(priv->pixbuf[state], 0, 0 , image_bound.width, image_bound.height);
//	  mask = cairo_image_surface_create (CAIRO_FORMAT_A1,
//			  gdk_pixbuf_get_width (pixbuf),
//			  gdk_pixbuf_get_height (pixbuf));
//	  mask_region = gdk_cairo_region_create_from_surface (mask);
//	  gtk_widget_shape_combine_region (widget, mask_region);
//	  cairo_surface_destroy (mask);
//	  cairo_region_destroy (mask_region);
//
//	  gdk_cairo_set_source_pixbuf(cr, pixbuf, 0,0);
//	  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
//	  cairo_paint(cr);

	 // cairo_restore(cr);
	  //cairo_destroy(cr);
  }
    gtk_style_context_restore (context);
	  cairo_restore (cr);

	return FALSE;
}
#else
static gboolean gfw_button_expose (GtkWidget *widget, GdkEventExpose *event)
{
	GfwButton *button;
	GfwButtonPrivate *priv;
	GdkRectangle area;
	GtkStateType state;

	g_return_val_if_fail (event != NULL, FALSE);

	button = GFW_BUTTON (widget);
	priv = GFW_BUTTON_GET_PRIVATE (button);
	g_print("call expose\n");

	area = event->area;

	state = gtk_widget_get_state(widget);

	if (gtk_widget_get_mapped (widget))
	{
		GdkPixbuf *pixbuf;
		GdkRectangle image_bound;
		GdkBitmap *bitmap;

		g_printf("!!! erea :x=%d,y=%d,w=%d,h=%d\n", area.x, area.y, area.width , area.height);

		image_bound.width = MIN(area.width, gdk_pixbuf_get_width(priv->pixbuf[state]));
		image_bound.height = MIN(area.height, gdk_pixbuf_get_height(priv->pixbuf[state]));

		pixbuf = gdk_pixbuf_new_subpixbuf(priv->pixbuf[state], area.x, area.y, image_bound.width, image_bound.height);

		gdk_pixbuf_render_pixmap_and_mask(priv->pixbuf[state], NULL, &bitmap, 128);
		gtk_widget_shape_combine_mask(widget, bitmap, 0, 0);
		g_object_unref(bitmap);

		cairo_t *ct = gdk_cairo_create (widget->window);
		gdk_cairo_set_source_pixbuf(ct, pixbuf, area.x, area.y);
		cairo_paint(ct);
		cairo_destroy(ct);

		if (pixbuf)
			g_object_unref (pixbuf);
	}

	return FALSE;
}
#endif
