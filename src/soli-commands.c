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
#include "soli-tab.h"
#include "soli-view.h"
#include "soli-document.h"
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

gboolean
soli_commands_save_document_finish (SoliDocument *doc,
									GAsyncResult *result)
{
	g_return_val_if_fail (g_task_is_valid (result, doc), FALSE);

	return g_task_propagate_boolean (G_TASK (result), NULL);
}

static void
save_tab_ready_cb (SoliDocument *doc,
					GAsyncResult *result,
					gpointer user_data)
{
	soli_commands_save_document_finish (doc, result);
}

static void
tab_save_ready_cb (SoliTab *tab,
					GAsyncResult *result,
					GTask *task)
{
	g_task_return_boolean (task, soli_tab_save_finish (tab, result));
	g_object_unref (task);
}

static void
save_document_async (SoliDocument *doc,
					SoliWindow *window,
					GCancellable *cancellable,
					GAsyncReadyCallback callback,
					gpointer user_data)
{
	GTask *task;
	SoliTab *tab;
	GtkSourceFile *file;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));
	g_return_if_fail (SOLI_IS_WINDOW (window));
	g_return_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable));

	task = g_task_new (doc, cancellable, callback, user_data);

	tab = soli_tab_get_from_document (doc);
	file = soli_document_get_file (doc);

	// TODO: what if file is untitled?

	soli_tab_save_async (tab,
						cancellable,
						(GAsyncReadyCallback) tab_save_ready_cb,
						task);
}

static void
save_tab (SoliTab *tab, SoliWindow *window)
{
	SoliDocument *doc = soli_tab_get_document (tab);

	save_document_async (doc,
						window,
						NULL,
						(GAsyncReadyCallback) save_tab_ready_cb,
						NULL);
}

void
soli_cmd_save (GSimpleAction *action,
			   GVariant      *parameter,
			   gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliTab *tab;

	tab = soli_window_get_active_tab (window);
	if (tab != NULL)
	{
		g_print ("Saving file...\n");

		save_tab (tab, window);
	}
}

void
soli_cmd_close_tab (SoliTab *tab,
					SoliWindow *window)
{
	/* Ensure window is the tab's top-level container */
	g_return_if_fail (GTK_WIDGET (window) == gtk_widget_get_toplevel (GTK_WIDGET (tab)));
	
	// TODO: check if tab has unsaved changes
	
	soli_window_close_tab (window, tab);
}

void
soli_cmd_close (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliTab *active_tab;
	
	active_tab = soli_window_get_active_tab (window);
	if (active_tab == NULL)
	{
		gtk_widget_destroy (GTK_WIDGET (window));
		return;
	}
	soli_cmd_close_tab (active_tab, window);
}

void
soli_cmd_quit (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       app)
{
	g_application_quit (G_APPLICATION (app));
}

void
soli_cmd_cut (GSimpleAction *action,
			   GVariant      *parameter,
			   gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliView *view = soli_window_get_active_view (window);

	g_return_if_fail (view != NULL);

	soli_view_cut_clipboard (view);
	gtk_widget_grab_focus (GTK_WIDGET (view));
}

void
soli_cmd_copy (GSimpleAction *action,
			   GVariant      *parameter,
			   gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliView *view = soli_window_get_active_view (window);

	g_return_if_fail (view != NULL);

	soli_view_copy_clipboard (view);
	gtk_widget_grab_focus (GTK_WIDGET (view));
}

void
soli_cmd_paste (GSimpleAction *action,
			   GVariant      *parameter,
			   gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliView *view = soli_window_get_active_view (window);

	g_return_if_fail (view != NULL);

	soli_view_paste_clipboard (view);
	gtk_widget_grab_focus (GTK_WIDGET (view));
}

void
soli_cmd_preferences (GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer       app)
{
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
