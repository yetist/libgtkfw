/* vi: set sw=4 ts=4 wrap ai: */
/*
 * ipaddress.c: This file is part of ____
 *
 * Copyright (C) 2012 yetist <xiaotian.wu@i-soft.com.cn>
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

#include <gfw.h>

static void ip_address_changed(GfwIpEntry* entry)
{
}

int main (int argc, char *argv[])
{
	GtkWidget *window, *ipaddress;
	gint address[4] = { 123, 220, 135, 255 };
	gtk_init (&argc, &argv);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "MyIPAddress");
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	ipaddress = gfw_ip_entry_new ();
	gfw_ip_entry_set_n_value(GFW_IP_ENTRY(ipaddress), 0, 25);
	gfw_ip_entry_set_n_value(GFW_IP_ENTRY(ipaddress), 1, 202);
	gfw_ip_entry_set_n_value(GFW_IP_ENTRY(ipaddress), 2, 235);
	gfw_ip_entry_set_n_value(GFW_IP_ENTRY(ipaddress), 3, 255);
	g_signal_connect (G_OBJECT (ipaddress), "ip-changed", G_CALLBACK(ip_address_changed), NULL);
	gtk_container_add (GTK_CONTAINER (window), ipaddress);
	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}
