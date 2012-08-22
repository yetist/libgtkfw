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
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <math.h>

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
    guint8 range[4][2];
};

static void gfw_ip_entry_set_property  (GObject          *object,
                                         guint             prop_id,
                                         const GValue     *value,
                                         GParamSpec       *pspec);
static void gfw_ip_entry_get_property  (GObject          *object,
                                         guint             prop_id,
                                         GValue           *value,
                                         GParamSpec       *pspec);
static void ip_entry_render (GfwIpEntry *ipentry);
static gboolean ip_entry_key_pressed (GtkEntry *entry, GdkEventKey *event);
static void ip_entry_move_cursor (GObject *entry, GParamSpec *spec);

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

    /* Register four GObject properties, one for each ip address number. */
    g_object_class_install_property (gobject_class, PROP_IP1,
            g_param_spec_int ("ip-number-1",
                "IP Address Number 1",
                "The first IP address number",
                0, 255, 0,
                G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_IP2,
            g_param_spec_int ("ip-number-2",
                "IP Address Number 2",
                "The second IP address number",
                0, 255, 0,
                G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_IP3,
            g_param_spec_int ("ip-number-3",
                "IP Address Number 3",
                "The third IP address number",
                0, 255, 0,
                G_PARAM_READWRITE));
    g_object_class_install_property (gobject_class, PROP_IP4,
            g_param_spec_int ("ip-number-4",
                "IP Address Number 1",
                "The fourth IP address number",
                0, 255, 0,
                G_PARAM_READWRITE));
    g_type_class_add_private (class, sizeof (GfwIpEntryPrivate));
}

static void
gfw_ip_entry_init (GfwIpEntry *ip_entry)
{
    GfwIpEntryPrivate *priv;

    priv = GFW_IP_ENTRY_GET_PRIVATE (ip_entry);

    PangoFontDescription *fd;
    guint i;

    for (i = 0; i < 4; i++)
    {
        priv->address[i] = 0;
        priv->range[i][0] = 0;
        priv->range[i][1] = 255;
    }


    fd = pango_font_description_from_string ("Monospace");
    gtk_widget_modify_font (GTK_WIDGET (ip_entry), fd);
    ip_entry_render (ip_entry);
    pango_font_description_free (fd);

    /* The key-press-event signal will be used to filter out certain keys. We will
     * also monitor the cursor-position property so it can be moved correctly. */
    g_signal_connect (G_OBJECT (ip_entry), "key-press-event",
            G_CALLBACK (ip_entry_key_pressed), NULL);
    g_signal_connect (G_OBJECT (ip_entry), "notify::cursor-position",
            G_CALLBACK (ip_entry_move_cursor), NULL);
}

GtkWidget* gfw_ip_entry_new (void)
{
    return g_object_new (GFW_TYPE_IP_ENTRY, NULL);
}

static void
gfw_ip_entry_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    GfwIpEntry *ipentry;

    ipentry = GFW_IP_ENTRY (object);
    //gint address[4] = { -1, -1, -1, -1 };

    switch (prop_id)
    {
        case PROP_IP1:
            //address[0] = g_value_get_int (value);
            //my_ip_address_set_address (ipaddress, address);
            gfw_ip_entry_set_n_value(ipentry, 0, g_value_get_int(value));
            break;
        case PROP_IP2:
            //address[1] = g_value_get_int (value);
            //my_ip_address_set_address (ipaddress, address);
            gfw_ip_entry_set_n_value(ipentry, 1, g_value_get_int(value));
            break;
        case PROP_IP3:
            //address[2] = g_value_get_int (value);
            //my_ip_address_set_address (ipaddress, address);
            gfw_ip_entry_set_n_value(ipentry, 2, g_value_get_int(value));
            break;
        case PROP_IP4:
            //address[3] = g_value_get_int (value);
            //my_ip_address_set_address (ipaddress, address);
            gfw_ip_entry_set_n_value(ipentry, 3, g_value_get_int(value));
            break;
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
    GfwIpEntryPrivate *priv;

    ip_entry = GFW_IP_ENTRY (object);
    priv = GFW_IP_ENTRY_GET_PRIVATE (ip_entry);

    switch (prop_id)
    {
        case PROP_IP1:
            g_value_set_int (value, priv->address[0]);
            break;
        case PROP_IP2:
            g_value_set_int (value, priv->address[1]);
            break;
        case PROP_IP3:
            g_value_set_int (value, priv->address[2]);
            break;
        case PROP_IP4:
            g_value_set_int (value, priv->address[3]);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

/* Render the current content of the IP address in the GtkEntry widget. */
static void ip_entry_render (GfwIpEntry *ipentry)
{
    GfwIpEntryPrivate *priv;
    priv = GFW_IP_ENTRY_GET_PRIVATE (ipentry);

    GString *text;
    guint i;
    /* Create a string that displays the IP address content, adding spaces if a
     * number cannot fill three characters. */
    text = g_string_new (NULL);
    for (i = 0; i < 4; i++)
    {
        gchar *temp;
        if (priv->address[i] > 99)
            temp = g_strdup_printf ("%i .", priv->address[i]);
        else if (priv->address[i] > 9)
            temp = g_strdup_printf (" %i .", priv->address[i]);
        else
            temp = g_strdup_printf (" %i  .", priv->address[i]);
        text = g_string_append (text, temp);
        g_free (temp);
    }
    /* Remove the trailing decimal place and add the string to the GtkEntry. */
    text = g_string_truncate (text, 18);
    gtk_entry_set_text (GTK_ENTRY (ipentry), text->str);
    g_string_free (text, TRUE);
}

/* Force the cursor to always be at the end of one of the four numbers. */
static void ip_entry_move_cursor (GObject *entry, GParamSpec *spec)
{
    gint cursor = gtk_editable_get_position (GTK_EDITABLE (entry));
    if (cursor <= 5)
        //gtk_editable_select_region(GTK_EDITABLE (entry), 0, 3);
        gtk_editable_set_position(GTK_EDITABLE (entry), 3);
    else if (cursor <= 10)
        //gtk_editable_select_region(GTK_EDITABLE (entry), 4, 7);
        gtk_editable_set_position(GTK_EDITABLE (entry), 8);
    else if (cursor <= 15)
        //gtk_editable_select_region(GTK_EDITABLE (entry), 8, 11);
        gtk_editable_set_position(GTK_EDITABLE (entry), 13);
    else
        //gtk_editable_select_region(GTK_EDITABLE (entry), 12, 15);
        gtk_editable_set_position(GTK_EDITABLE (entry), 18);
}

int get_next_number(GfwIpEntry *entry, int i, int pos, int value)
{
    /*
     * 01234567890123456789
     * |0123.5678.0123.5678|
     * | 1  . 1  . 1  . 1  |
     * | 12 . 12 . 12 . 12 |
     * |123 .123 .123 .123 |
     * 1, 2
     * 2, 3
     * 3, 3
     *
     * i=0; cursor=0-4
     * i=1; cursor=5-9
     * i=2; cursor=10-14
     * i=3; cursor=15-19
     */
    const gchar *text;
    GfwIpEntryPrivate *priv;
    text = gtk_entry_get_text(GTK_ENTRY(entry));
    priv = GFW_IP_ENTRY_GET_PRIVATE (entry);
}

static int get_number_bit(int num)
{
    if (num > 99)
        return 3;
    else if (num > 9)
        return 2;
    else
        return 1;
}

/* Handle key presses of numbers, tabs, backspaces and returns. */
static gboolean ip_entry_key_pressed (GtkEntry *entry, GdkEventKey *event)
{
    GfwIpEntryPrivate *priv;
    priv = GFW_IP_ENTRY_GET_PRIVATE (entry);

    guint k = event->keyval;
    gint cursor,i, value;
    if ((k >= GDK_0 && k <= GDK_9) || (k >= GDK_KP_0 && k <= GDK_KP_9))
    {
        cursor = floor(gtk_editable_get_position (GTK_EDITABLE (entry)) / 5);
        i = gtk_editable_get_position (GTK_EDITABLE (entry));
        g_printf("i=%d, cursor=%d\n", i, cursor);
        return TRUE;
    }
    /* Move to the next number or wrap around to the first. */
    else if (k == GDK_Tab)
    {
        cursor = (floor (gtk_editable_get_position (GTK_EDITABLE (entry)) / 5) + 1);
        gtk_editable_set_position (GTK_EDITABLE (entry), (5 * (cursor % 5)) + 4);
    }
    /* Delete the last digit of the current number. This just divides the number by
     * 10, relying on the fact that any remainder will be ignored. */
    else if (k == GDK_BackSpace)
    {
        gint bit;
        cursor = floor (gtk_editable_get_position (GTK_EDITABLE (entry)) / 5);
        priv->address[cursor] /= 10;
        bit = get_number_bit(priv->address[cursor]);
        g_printf("bit=%d, pos=%d\n", bit, 5 * cursor + bit);
        ip_entry_render (GFW_IP_ENTRY(entry));
        gtk_editable_set_position (GTK_EDITABLE (entry), (5 * cursor) +  bit);
        g_signal_emit_by_name ((gpointer) entry, "ip-changed");
    }
    /* Activate the GtkEntry widget, which corresponds to the activate signal. */
    else if ((k == GDK_Return) || (k == GDK_KP_Enter))
        gtk_widget_activate (GTK_WIDGET (entry));
    return TRUE;
}

static gboolean _ip_entry_key_pressed (GtkEntry *entry, GdkEventKey *event)
{
    GfwIpEntryPrivate *priv;
    priv = GFW_IP_ENTRY_GET_PRIVATE (entry);

    guint k = event->keyval;
    gint cursor, value;
    /* If the key is an integer, append the new number to the address. This is only
     * done if the resulting number will be less than 255. */
    if ((k >= GDK_0 && k <= GDK_9) || (k >= GDK_KP_0 && k <= GDK_KP_9))
    {
        cursor = floor(gtk_editable_get_position (GTK_EDITABLE (entry)) / 4);
        value = g_ascii_digit_value (event->string[0]);
        if ((priv->address[cursor] == 25) && (value > 5))
        {
            g_printf("cursor=%d, current=%d, press=%d\n", cursor, priv->address[cursor], value);
            priv->address[cursor] = 255;
            ip_entry_render (GFW_IP_ENTRY(entry));
            g_signal_emit_by_name ((gpointer) entry, "ip-changed");
            GtkWidget *dialog;
            GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(entry));
            if (gtk_widget_is_toplevel (toplevel))
                dialog = gtk_message_dialog_new (toplevel,
                        GTK_DIALOG_DESTROY_WITH_PARENT |
                        GTK_DIALOG_MODAL,
                        GTK_MESSAGE_ERROR,
                        GTK_BUTTONS_CLOSE,
                        "%d is not a valid number. please set a number between %d and %d", value, priv->range[cursor][0], priv->range[cursor][1]);
            else
                dialog = gtk_message_dialog_new (NULL,
                        GTK_DIALOG_DESTROY_WITH_PARENT |
                        GTK_DIALOG_MODAL,
                        GTK_MESSAGE_ERROR,
                        GTK_BUTTONS_CLOSE,
                        "%d is not a valid number. please set a number between %d and %d", value, priv->range[cursor][0], priv->range[cursor][1]);
            g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
            gtk_window_present (GTK_WINDOW (dialog));
            return TRUE;
        }
        if (priv->address[cursor] < 26)
        {
            priv->address[cursor] *= 10;
            priv->address[cursor] += value;
            g_printf("cur=%d, press=%d\n", priv->address[cursor], value);
            ip_entry_render (GFW_IP_ENTRY(entry));
            //gtk_editable_set_position (GTK_EDITABLE (entry), (4 * cursor) + 3);
            gtk_editable_select_region(GTK_EDITABLE (entry), (4 * cursor), (4 * cursor) + 3);
            g_signal_emit_by_name ((gpointer) entry, "ip-changed");
        }
    }
    /* Move to the next number or wrap around to the first. */
    else if (k == GDK_Tab)
    {
        cursor = (floor (gtk_editable_get_position (GTK_EDITABLE (entry)) / 4) + 1);
        gtk_editable_set_position (GTK_EDITABLE (entry), (4 * (cursor % 4)) + 3);
    }
    /* Delete the last digit of the current number. This just divides the number by
     * 10, relying on the fact that any remainder will be ignored. */
    else if (k == GDK_BackSpace)
    {
        cursor = floor (gtk_editable_get_position (GTK_EDITABLE (entry)) / 4);
        priv->address[cursor] /= 10;
        ip_entry_render (GFW_IP_ENTRY(entry));
        gtk_editable_set_position (GTK_EDITABLE (entry), (4 * cursor) + 3);
        g_signal_emit_by_name ((gpointer) entry, "ip-changed");
    }
    /* Activate the GtkEntry widget, which corresponds to the activate signal. */
    else if ((k == GDK_Return) || (k == GDK_KP_Enter))
        gtk_widget_activate (GTK_WIDGET (entry));
    return TRUE;
}


void  gfw_ip_entry_set_address (GfwIpEntry *ipentry, guint8 address[4])
{
    GfwIpEntryPrivate *priv;
    guint i;

    priv = GFW_IP_ENTRY_GET_PRIVATE (ipentry);

    for (i = 0; i < 4; i++)
    {
        if (address[i] >= priv->range[i][0] && address[i] <= priv->range[i][1])
        {
            priv->address[i] = address[i];
        }
    }
    ip_entry_render (ipentry);
    g_signal_emit_by_name ((gpointer) ipentry, "ip-changed");
}

void gfw_ip_entry_set_inet_address   (GfwIpEntry *ipentry, GInetAddress *address)
{
    const guint8 *addr;
    addr = g_inet_address_to_bytes(address);
    //guint8 address[4] = {addr.S_un.S_un_b.s_b1, addr.S_un.S_un_b.s_b2, addr.S_un.S_un_b.s_b3, addr.S_un.S_un_b.s_b4};
    //gfw_ip_entry_set_address (ipentry, address);
}

void gfw_ip_entry_set_n_value (GfwIpEntry *ipentry, gint n, guint8 value)
{
    GfwIpEntryPrivate *priv;

    priv = GFW_IP_ENTRY_GET_PRIVATE (ipentry);
    if ((n >=0 && n <= 3) && (value >= priv->range[n][0] && value <= priv->range[n][1]))
    {
        priv->address[n] = value;
    }
    ip_entry_render (ipentry);
    g_signal_emit_by_name ((gpointer) ipentry, "ip-changed");
}

void  gfw_ip_entry_set_values (GfwIpEntry *ipentry, guint8 v0, guint8 v1, guint8 v2, guint8 v3);

void  gfw_ip_entry_set_n_range (GfwIpEntry *ipentry, gint n, guint8 lower, guint8 upper)
{
    GfwIpEntryPrivate *priv;
    priv = GFW_IP_ENTRY_GET_PRIVATE (ipentry);

    if ( n < 0 || n > 255)
        return;

    if (lower >=0 && lower <=255)
        priv->range[n][0]=lower;
    if (upper >=0 && upper <=255)
        priv->range[n][1]=upper;
}

void            gfw_ip_entry_set_n_focus        (GfwIpEntry *ipentry, gint n);



gchar* gfw_ip_entry_get_string         (GfwIpEntry *ipentry)
{
    GfwIpEntryPrivate *priv;
    priv = GFW_IP_ENTRY_GET_PRIVATE (ipentry);
    return g_strdup_printf ("%d.%d.%d.%d", priv->address[0], priv->address[1], priv->address[2], priv->address[3]);
}

GInetAddress* gfw_ip_entry_get_inet_address  (GfwIpEntry *ipentry)
{
    GfwIpEntryPrivate *priv;
    GInetAddress *inetaddr;

    priv = GFW_IP_ENTRY_GET_PRIVATE (ipentry);
    inetaddr = g_inet_address_new_from_bytes (priv->address, G_SOCKET_FAMILY_IPV4);

    return inetaddr;
}

void            gfw_ip_entry_get_values         (GfwIpEntry *ipentry, guint8 *v0, guint8 *v1, guint8 *v2, guint8 *v3);
guint8          gfw_ip_entry_get_n_value        (GfwIpEntry *ipentry, gint n);


gboolean        gfw_ip_entry_is_blank           (GfwIpEntry *ipentry)
{
}

void            gfw_ip_entry_clear              (GfwIpEntry *ipentry);

/*
vi:ts=4:wrap:ai:expandtab
*/
