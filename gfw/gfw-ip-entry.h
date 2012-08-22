/* vi: set sw=4 ts=4: */
/*
 * gfw-ip-entry.h: This file is part of ____
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

#ifndef __GFW_IP_ENTRY_H__ 
#define __GFW_IP_ENTRY_H__  1

#include <gtk/gtk.h>

#ifdef G_OS_UNIX
#  include <netinet/in.h>
#else /* !G_OS_UNIX */
#  ifdef G_OS_WIN32
#    include <winsock2.h>
#  endif /* !G_OS_WIN32 */
#endif /* !G_OS_UNIX */

G_BEGIN_DECLS

#define GFW_TYPE_IP_ENTRY              (gfw_ip_entry_get_type ())
#define GFW_IP_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFW_TYPE_IP_ENTRY, GfwIpEntry))
#define GFW_IP_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GFW_TYPE_IP_ENTRY, GfwIpEntryClass))
#define GFW_IS_IP_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFW_TYPE_IP_ENTRY))
#define GFW_IS_IP_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GFW_TYPE_IP_ENTRY))
#define GFW_IP_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GFW_TYPE_IP_ENTRY, GfwIpEntryClass))

typedef struct _GfwIpEntry             GfwIpEntry;
typedef struct _GfwIpEntryClass        GfwIpEntryClass;

struct _GfwIpEntry
{
    GtkEntry      entry;
};

struct _GfwIpEntryClass
{
    GtkEntryClass       parent_class;
    void (*ip_changed)  (GfwIpEntry *ipentry);
};

GType           gfw_ip_entry_get_type           (void) G_GNUC_CONST;
GtkWidget*      gfw_ip_entry_new                (void);

void            gfw_ip_entry_set_inet_address   (GfwIpEntry *ipentry, GInetAddress *address);
void            gfw_ip_entry_set_address        (GfwIpEntry *ipentry, guint8 address[4]);
void            gfw_ip_entry_set_n_value        (GfwIpEntry *ipentry, gint n, guint8 value);
void            gfw_ip_entry_set_values         (GfwIpEntry *ipentry, guint8 v0, guint8 v1, guint8 v2, guint8 v3);
void            gfw_ip_entry_set_n_range        (GfwIpEntry *ipentry, gint n, guint8 lower, guint8 upper);
void            gfw_ip_entry_set_n_focus        (GfwIpEntry *ipentry, gint n);

gchar*          gfw_ip_entry_get_string         (GfwIpEntry *ipentry);
GInetAddress*   gfw_ip_entry_get_inet_address   (GfwIpEntry *ipentry);
void            gfw_ip_entry_get_values         (GfwIpEntry *ipentry, guint8 *v0, guint8 *v1, guint8 *v2, guint8 *v3);
guint8          gfw_ip_entry_get_n_value        (GfwIpEntry *ipentry, gint n);

gboolean        gfw_ip_entry_is_blank           (GfwIpEntry *ipentry);
void            gfw_ip_entry_clear              (GfwIpEntry *ipentry);

G_END_DECLS

#endif /* __GFW_IP_ENTRY_H__ */

/*
vi:ts=4:wrap:ai:expandtab
*/
