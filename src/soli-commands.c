/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-commands.c
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 * 
 * Soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "soli-commands.h"
#include "soli-window.h"
#include <gtk/gtk.h>

void
soli_cmd_open (GSimpleAction *action,
				GVariant *parameter,
				gpointer window)
{
	GtkWidget *dialog;
	GtkFileChooserAction open_action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint result;

	dialog = gtk_file_chooser_dialog_new ("Open File",
										GTK_WINDOW (window),
										open_action,
										"_Cancel",
										GTK_RESPONSE_CANCEL,
										"_Open",
										GTK_RESPONSE_ACCEPT,
										NULL);

	result = gtk_dialog_run (GTK_DIALOG (dialog));
	if (result == GTK_RESPONSE_ACCEPT)
	{
		GFile *file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));
		soli_window_open (SOLI_WINDOW (window), file);

		g_object_unref (file);
	}

	gtk_widget_destroy (dialog);
}

void
soli_cmd_preferences (GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer       app)
{
}

void
soli_cmd_quit (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       app)
{
	g_application_quit (G_APPLICATION (app));
}

void
soli_cmd_about (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       app)
{
	static const gchar *authors[] = { "David Luco", NULL };
	GtkWindow *win;

	win = gtk_application_get_active_window (GTK_APPLICATION (app));
	
	gtk_show_about_dialog (win,
	                       "program-name",  "Soli",
	                       "version", VERSION,
	                       "license-type", GTK_LICENSE_GPL_3_0,
	                       "authors", authors,
	                       "comments",
	                   			"Test GTK+ application",
	                       "website",
	                   			"https://github.com/dluco/soli",
	                       NULL);
}
