/*
 * Gtk Pie Chart Widget
 * Copyright (C) 2008 Aron Rubin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 */
 
#ifndef GTK_PIE_CHART_H_INCLUDED
#define GTK_PIE_CHART_H_INCLUDED
 
#include <gdk/gdk.h>
#include <gtk/gtk.h>
 
G_BEGIN_DECLS

#define GFW_TYPE_PIE_CHART              (gfw_pie_chart_get_type ())
#define GFW_PIE_CHART(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFW_TYPE_PIE_CHART, GfwPieChart))
#define GFW_PIE_CHART_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GFW_TYPE_PIE_CHART, GfwPieChartClass))
#define GFW_IS_PIE_CHART(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFW_TYPE_PIE_CHART))
#define GFW_IS_PIE_CHART_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GFW_TYPE_PIE_CHART))
#define GFW_PIE_CHART_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GFW_TYPE_PIE_CHART, GfwPieChartClass))

typedef struct _GfwPieChart             GfwPieChart;
typedef struct _GfwPieChartClass        GfwPieChartClass;

struct _GfwPieChart
{
  GtkWidget      widget;
};

struct _GfwPieChartClass
{
  GtkWidgetClass     parent_class;
};


GType            gfw_pie_chart_get_type           (void) G_GNUC_CONST;
GtkWidget*       gfw_pie_chart_new                (void);
guint            gfw_pie_chart_add_segment        (GfwPieChart *pie, float sweep, const gchar *name, float *rgba);
void             gfw_pie_chart_remove_segment     (GfwPieChart *pie, guint id );
 
G_END_DECLS
 
#endif
