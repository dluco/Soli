/*
 * soli-help-commands.c
 * This file is part of soli
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2005 Paolo Maggi
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "soli-commands.h"
#include "soli-commands-private.h"

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "soli-debug.h"
#include "soli-app.h"
#include "soli-dirs.h"

void
_soli_cmd_help_keyboard_shortcuts (SoliWindow *window)
{
	static GtkWidget *shortcuts_window;

	soli_debug (DEBUG_COMMANDS);

	if (shortcuts_window == NULL)
	{
		GtkBuilder *builder;

		builder = gtk_builder_new_from_resource ("/ca/dluco/soli/ui/soli-shortcuts.ui");
		shortcuts_window = GTK_WIDGET (gtk_builder_get_object (builder, "shortcuts-soli"));

		g_signal_connect (shortcuts_window,
				  "destroy",
				  G_CALLBACK (gtk_widget_destroyed),
				  &shortcuts_window);

		g_object_unref (builder);
	}

	if (GTK_WINDOW (window) != gtk_window_get_transient_for (GTK_WINDOW (shortcuts_window)))
	{
		gtk_window_set_transient_for (GTK_WINDOW (shortcuts_window), GTK_WINDOW (window));
	}

	gtk_widget_show_all (shortcuts_window);
	gtk_window_present (GTK_WINDOW (shortcuts_window));
}

void
_soli_cmd_help_contents (SoliWindow *window)
{
	soli_debug (DEBUG_COMMANDS);

	soli_app_show_help (SOLI_APP (g_application_get_default ()),
	                     GTK_WINDOW (window),
	                     NULL,
	                     NULL);
}

void
_soli_cmd_help_about (SoliWindow *window)
{
	static const gchar * const authors[] = {
		"Alex Roberts",
		"Chema Celorio",
		"Evan Lawrence",
		"Federico Mena Quintero <federico@novell.com>",
		"Garrett Regier <garrettregier@gmail.com>",
		"Ignacio Casal Quinteiro <icq@gnome.org>",
		"James Willcox <jwillcox@gnome.org>",
		"Jesse van den Kieboom <jessevdk@gnome.org>",
		"Paolo Borelli <pborelli@gnome.org>",
		"Paolo Maggi <paolo@gnome.org>",
		"S\303\251bastien Lafargue <slafargue@gnome.org>",
		"S\303\251bastien Wilmet <swilmet@gnome.org>",
		"Steve Fr\303\251cinaux <steve@istique.net>",
		NULL
	};

	static const gchar * const documenters[] = {
		"Jim Campbell <jwcampbell@gmail.com>",
		"Daniel Neel <dneelyep@gmail.com>",
		"Sun GNOME Documentation Team <gdocteam@sun.com>",
		"Eric Baudais <baudais@okstate.edu>",
		NULL
	};

	static const gchar copyright[] = "Copyright \xc2\xa9 1998-2016 - the gedit team";

	static const gchar comments[] = \
		N_("soli is a fork of gedit, a small and lightweight text editor for the GNOME Desktop");

	GError *error = NULL;

	soli_debug (DEBUG_COMMANDS);

	gtk_show_about_dialog (GTK_WINDOW (window),
			       "program-name", "soli",
			       "authors", authors,
			       "comments", _(comments),
			       "copyright", copyright,
			       "license-type", GTK_LICENSE_GPL_2_0,
			       "documenters", documenters,
			       "logo-icon-name", "text-editor",
			       "translator-credits", _("translator-credits"),
			       "version", VERSION,
			       "website", "http://www.github.com/dluco/soli",
			       "website-label", "www.github.com/dluco/soli",
			       NULL);
}

/* ex:set ts=8 noet: */
