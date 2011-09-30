/* vi: set sw=4 ts=4: */
/*
 * gfw-ip-entry.c: This file is part of ____
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
#include <gfw/gfw-ip-entry.h>

enum {
    CHANGED_SIGNAL,
    LAST_SIGNAL
};

enum {
    PROP_0,
    PROP_IP1,
    PROP_IP2,
    PROP_IP3,
    PROP_IP4
};

#define GFW_IP_ENTRY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE((obj), GFW_TYPE_IP_ENTRY, GfwIpEntryPrivate))

typedef struct _GfwIpEntryPrivate        GfwIpEntryPrivate;

struct _GfwIpEntryPrivate
{
    guint8 address[4];
};

static void gfw_ip_entry_set_property  (GObject          *object,
                                         guint             prop_id,
                                         const GValue     *value,
                                         GParamSpec       *pspec);
static void gfw_ip_entry_get_property  (GObject          *object,
                                         guint             prop_id,
                                         GValue           *value,
                                         GParamSpec       *pspec);

static guint ip_entry_signals[LAST_SIGNAL] = {0};

G_DEFINE_TYPE (GfwIpEntry, gfw_ip_entry, GTK_TYPE_ENTRY);

static void
gfw_ip_entry_class_init (GfwIpEntryClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (class);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

    gobject_class->set_property = gfw_ip_entry_set_property;
    gobject_class->get_property = gfw_ip_entry_get_property;

    ip_entry_signals[CHANGED_SIGNAL] = 
		g_signal_new ("ip-changed",
				G_OBJECT_CLASS_TYPE (gobject_class),
				G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
				G_STRUCT_OFFSET (GfwIpEntryClass, ip_changed),
				NULL, NULL,
				g_cclosure_marshal_VOID__VOID,
				G_TYPE_NONE, 0);

    g_type_class_add_private (class, sizeof (GfwIpEntryPrivate));
}

static void
gfw_ip_entry_init (GfwIpEntry *ip_entry)
{
    GfwIpEntryPrivate *priv;

    priv = GFW_IP_ENTRY_GET_PRIVATE (ip_entry);

}

GfwIpEntry*
gfw_ip_entry_new (void)
{
    return g_object_new (GFW_TYPE_IP_ENTRY, NULL);
}

static void
gfw_ip_entry_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    GfwIpEntry *ip_entry;

    ip_entry = GFW_IP_ENTRY (object);

    switch (prop_id)
    {

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
gfw_ip_entry_get_property (GObject      *object,
                            guint         prop_id,
                            GValue       *value,
                            GParamSpec   *pspec)
{
    GfwIpEntry *ip_entry;

    ip_entry = GFW_IP_ENTRY (object);

    switch (prop_id)
    {

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

/*
vi:ts=4:wrap:ai:expandtab
*/
