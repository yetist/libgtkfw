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
 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glib.h>
#include <glib-object.h>
#include <gtk/gtkwidget.h>
#include <cairo.h>
#include "gfw-pie-chart.h"
 
#define UNUSED __attribute__ ((unused))
 
// Properties
enum {
  PROP_0,
  PROP_START_ANGLE,
};
 
static GtkWidgetClass *parent_class = NULL;
 
// object signals
static void gfw_pie_chart_class_init( GfwPieChartClass *class );
static void gfw_pie_chart_init( GfwPieChart *pie );
static void gfw_pie_chart_finalize( GObject *pie );
static void gfw_pie_chart_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec );
static void gfw_pie_chart_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec );
 
// widget signals
static void gfw_pie_chart_realize( GtkWidget *pie );
static void gfw_pie_chart_size_allocate( GtkWidget *pie, GtkAllocation *allocation );
static gboolean gfw_pie_chart_expose( GtkWidget *pie, GdkEventExpose *event );
 
// internals
 
/**********************************************\
 
         Section: GfwPieChart Methods
 
\**********************************************/
 
/* ===========================================
 
      Group: GfwPieChart Core Object Methods
 
   =========================================== */
 
GtkType gfw_pie_chart_get_type() {
  static unsigned int gfw_pie_chart_type = 0;
 
  if( !gfw_pie_chart_type ) {
    GtkTypeInfo gfw_pie_chart_info = {
      "GfwPieChart",
      sizeof(GfwPieChart),
      sizeof(GfwPieChartClass),
      (GtkClassInitFunc)gfw_pie_chart_class_init,
      (GtkObjectInitFunc)gfw_pie_chart_init,
      NULL, NULL, NULL
    };
    gfw_pie_chart_type = gtk_type_unique( GTK_TYPE_WIDGET, &gfw_pie_chart_info );
  }
  return( gfw_pie_chart_type );
}
 
GtkWidget *gfw_pie_chart_new() {
  GfwPieChart *pie;
  GtkWidget *widget;
 
  pie = gtk_type_new( gfw_pie_chart_get_type() );
  widget = GTK_WIDGET(pie);
  widget->style = gtk_style_new();
 
  return widget;
}
 
static void gfw_pie_chart_class_init( GfwPieChartClass *class ) {
  GObjectClass *obj_class;
  GtkWidgetClass *wid_class;
 
  obj_class = (GObjectClass *)class;
  wid_class = (GtkWidgetClass *)class;
  parent_class = gtk_type_class( gtk_widget_get_type() );
 
  obj_class->finalize = gfw_pie_chart_finalize;
  obj_class->set_property = gfw_pie_chart_set_property;
  obj_class->get_property = gfw_pie_chart_get_property;
 
  wid_class->realize = gfw_pie_chart_realize;
  wid_class->size_allocate = gfw_pie_chart_size_allocate;
  wid_class->expose_event = gfw_pie_chart_expose;
 
  g_object_class_install_property( obj_class, PROP_START_ANGLE,
                                  g_param_spec_double( "start_angle", "Start Angle",
						       "The angle of the leading edge of the first segment.",
						       -M_PI, M_PI, 0, G_PARAM_READWRITE ) );
 
}
 
static void gfw_pie_chart_init( GfwPieChart *pie ) {
 
}
 
static void gfw_pie_chart_finalize( GObject *pie ) {
 
}
 
// GObject Methods
 
static void gfw_pie_chart_set_property( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec UNUSED ) {
  GfwPieChart *pie;
 
  pie = GTK_PIE_CHART(object);
 
  switch( prop_id ) {
  case PROP_START_ANGLE: pie->start_angle = g_value_get_double( value ); break;
  default:
    break;
  }
}
 
static void gfw_pie_chart_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec ) {
  GfwPieChart *pie;
 
  pie = GTK_PIE_CHART(object);
 
  switch( prop_id ) {
  case PROP_START_ANGLE: g_value_set_int( value, pie->start_angle ); break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
    break;
  }
}
 
static void gfw_pie_chart_set_size( GfwPieChart *pie, int width, int height ) {
  pie->width = width;
  pie->height = height;
 
  pie->radius = fmin( width, height )*0.33;
}
 
static void gfw_pie_chart_update_segments( GfwPieChart *pie ) {
  float total_sweep = 0.0;
  gboolean divide_remaining = FALSE;
  GList *list;
 
  g_return_if_fail( GTK_IS_PIE_CHART(pie) );
 
  list = pie->segments;
  while( list ) {
    if( ((GfwPieSegment *)list->data)->sweep > 0.0 )
      total_sweep += ((GfwPieSegment *)list->data)->sweep;
    else
      divide_remaining = TRUE;
    list = list->next;
  }
 
  if( total_sweep > 0.0 )
    pie->sweep_factor = 100.0/total_sweep;
  else
    pie->sweep_factor = 1.0;
}
 
guint gfw_pie_chart_add_segment( GfwPieChart *pie, float sweep, const gchar *name, float *rgba ) {
  GfwPieSegment *segment;
 
  g_return_val_if_fail( GTK_IS_PIE_CHART(pie), 0 );
 
  segment = g_new0( GfwPieSegment, 1 );
  g_return_val_if_fail( segment, 0 );
  segment->sweep = sweep;
  segment->name = g_strdup( name );
  if( rgba )
    memcpy( segment->rgba, rgba, sizeof(float)*4 );
  pie->segments = g_list_append( pie->segments, segment );
 
  gfw_pie_chart_update_segments( pie );
 
  gtk_widget_queue_draw( (GtkWidget *)pie );
 
  return( g_list_length( pie->segments ) );
}
 
void gfw_pie_chart_remove_segment( GfwPieChart *pie, guint id ) {
  GfwPieSegment *segment;
  GList *segment_item;
 
  g_return_if_fail( GTK_IS_PIE_CHART(pie) );
 
  segment_item = g_list_nth( pie->segments, id );
  g_return_if_fail( segment_item && segment_item->data );
  segment = (GfwPieSegment *)segment_item->data;
 
  if( segment->name )
    g_free( segment->name );
 
  pie->segments = g_list_delete_link( pie->segments, segment_item );
  g_free( segment );
 
  gtk_widget_queue_draw( (GtkWidget *)pie );
}
 
static void gfw_pie_chart_realize( GtkWidget *widget ) {
  GfwPieChart *pie;
  GdkWindowAttr attributes;
  int attributes_mask = 0;
 
  g_return_if_fail( widget );
  g_return_if_fail( GTK_IS_PIE_CHART(widget) );
 
  pie = GTK_PIE_CHART(widget);
  GTK_WIDGET_SET_FLAGS( widget, GTK_REALIZED );
 
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
 
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = gtk_widget_get_events( widget ) | GDK_EXPOSURE_MASK;
  attributes.visual = gtk_widget_get_visual( widget );
  attributes.colormap = gtk_widget_get_colormap( widget );
 
  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window = gdk_window_new( widget->parent->window, &attributes, attributes_mask );
 
  widget->style = gtk_style_attach( widget->style, widget->window );
 
  gdk_window_set_user_data( widget->window, widget );
  gdk_window_set_background( widget->window, widget->style->bg );
 
  gfw_pie_chart_set_size( pie, widget->allocation.width, widget->allocation.height );
}
 
static void gfw_pie_chart_size_allocate( GtkWidget *widget, GtkAllocation *allocation ) {
  GfwPieChart *pie;
 
  g_return_if_fail( widget );
  g_return_if_fail( GTK_IS_PIE_CHART(widget) );
  g_return_if_fail( allocation );
 
  pie = GTK_PIE_CHART(widget);
 
  widget->allocation = *allocation;
  if( GTK_WIDGET_REALIZED(widget) )
    gdk_window_move_resize( widget->window, allocation->x, allocation->y, allocation->width, allocation->height );
 
  gfw_pie_chart_set_size( pie, widget->allocation.width, widget->allocation.height );
}
 
static float gfw_pie_chart_draw_segment( GfwPieChart *pie, cairo_t *cr, GfwPieSegment *segment, float start ) {
  float end = start + /* pie->sweep_factor* */ segment->sweep*(2.0*M_PI);
  float mid = (start + end)*0.5;
 
  cairo_save( cr );
  //cairo_move_to( cr, pie->radius*cos( start*2*pi ), pie->radius*sin( start*(2.0*M_PI) ) )
  cairo_move_to( cr, pie->width/2, pie->height/2 );
  cairo_line_to( cr, pie->width/2 + pie->radius*cos( start ), pie->height/2 + pie->radius*sin( start ) );
  cairo_arc( cr, pie->width/2, pie->height/2, pie->radius, start, end );
  //cairo_arc_negative( cr, pie->width/2, pie->height/2, 0, end*(2.0*M_PI), start*(2.0*M_PI) );
  cairo_close_path( cr );
 
  cairo_set_line_width( cr, 1.0 );
  cairo_set_source_rgba( cr, segment->rgba[0], segment->rgba[1], segment->rgba[2], segment->rgba[3]  );
  cairo_fill_preserve( cr );
  cairo_set_source_rgba( cr, 0, 0, 0, 1.0 );
  cairo_stroke( cr );
 
  cairo_move_to( cr, pie->width/2 + 0.5*pie->radius*cos( mid ), pie->height/2 + 0.5*pie->radius*sin( mid ) );
  cairo_line_to( cr, pie->width/2 + 1.3*pie->radius*cos( mid ), pie->height/2 + 1.3*pie->radius*sin( mid ) );
  if( mid > M_PI_2 && mid < (M_PI_2 + M_PI) )
    cairo_line_to( cr, pie->width/6, pie->height/2 + 1.3*pie->radius*sin( mid ) );
  else
    cairo_line_to( cr, pie->width*5/6, pie->height/2 + 1.3*pie->radius*sin( mid ) );
  cairo_stroke( cr );
  cairo_restore( cr );
 
  cairo_save( cr );
  if( mid > M_PI_2 && mid < (M_PI_2 + M_PI) ) {
    cairo_text_extents_t text_extents = { 0, 0, 0, 0, 0, 0 };
    cairo_text_extents( cr, segment->name ? segment->name : "", &text_extents );
    cairo_move_to( cr, pie->width/6 - text_extents.width, pie->height/2 + 1.3*pie->radius*sin( mid ) + 4.0 );
  } else {
    cairo_move_to( cr, pie->width*5/6, pie->height/2 + 1.3*pie->radius*sin( mid ) + 4.0 );
  }
  cairo_set_source_rgba( cr, 0, 0, 0, 1.0 );
  cairo_select_font_face( cr, "sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
  cairo_set_font_size( cr, 10.0 );
  cairo_show_text( cr, segment->name ? segment->name : "" );
  cairo_restore( cr );
 
  return( end );
}
 
static int gfw_pie_chart_expose( GtkWidget *widget, GdkEventExpose *event ) {
  GfwPieChart *pie;
  cairo_t *cr;
  //GdkRectangle rect = { 0, 0, 0, 0 };
  GList *list;
  float angle;
 
  g_return_val_if_fail( widget, FALSE);
  g_return_val_if_fail( GTK_IS_PIE_CHART(widget), FALSE );
  pie = GTK_PIE_CHART(widget);
 
  cr = gdk_cairo_create( widget->window );
 
  cairo_save( cr ); // expose area
 
  cairo_rectangle( cr, event->area.x, event->area.y, event->area.width, event->area.height );
  cairo_clip( cr );
 
  angle = pie->start_angle;
  list = pie->segments;
  while( list ) {
    GfwPieSegment *segment = (GfwPieSegment *)list->data;
    angle = gfw_pie_chart_draw_segment( pie, cr, segment, angle );
    list = list->next;
  }
 
  cairo_restore( cr ); // expose area
 
  cairo_destroy( cr );
 
  return FALSE;
}
