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

#include <gtk/gtk.h>
#include <libpeas-gtk/peas-gtk.h>

#include "soli-app.h"
#include "soli-window.h"
#include "soli-tab.h"
#include "soli-view.h"
#include "soli-document.h"

#define SOLI_TAB_TO_CLOSE "soli-tab-to-close"

void
soli_cmd_open (GSimpleAction *action,
				GVariant *parameter,
				gpointer window)
{
	GtkWidget *dialog;
	gint result;

	dialog = gtk_file_chooser_dialog_new ("Open File",
										GTK_WINDOW (window),
										GTK_FILE_CHOOSER_ACTION_OPEN,
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

	gtk_widget_destroy (GTK_WIDGET (dialog));
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
tab_save_as_ready_cb (SoliTab *tab,
					GAsyncResult *result,
					GTask *task)
{
	gboolean success = soli_tab_save_finish (tab, result);
	g_task_return_boolean (task, success);
	g_object_unref (task);
}

static gboolean
save_as_tab_finish (SoliTab *tab,
					GAsyncResult *result)
{
	g_return_val_if_fail (g_task_is_valid (result, tab), FALSE);

	return g_task_propagate_boolean (G_TASK (result), NULL);
}

static void
save_as_tab_ready_cb (SoliTab *tab,
						GAsyncResult *result,
						GTask *task)
{
	gboolean success = save_as_tab_finish (tab, result);

	g_task_return_boolean (task, success);
	g_object_unref (task);
}

static void
save_dialog_response_cb (GtkDialog *dialog,
						gint response_id,
						GTask *task)
{
	SoliTab *tab;
	GFile *location;

	tab = g_task_get_source_object (task);

	if (response_id != GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_destroy (GTK_WIDGET (dialog));
		g_task_return_boolean (task, FALSE);
		g_object_unref (task);
		return;
	}

	location = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));
	g_return_if_fail (location != NULL);

	gtk_widget_destroy (GTK_WIDGET (dialog));

	soli_tab_save_as_async (tab,
							location,
							g_task_get_cancellable (task),
							(GAsyncReadyCallback) tab_save_as_ready_cb,
							task);

	g_object_unref (location);
}

static void
save_as_tab_async (SoliTab *tab,
					SoliWindow *window,
					GCancellable *cancellable,
					GAsyncReadyCallback callback,
					gpointer user_data)
{
	GtkWidget *dialog;
	GTask *task;
	SoliDocument *doc;
	GtkSourceFile *file;
	GFile *location;

	g_return_if_fail (SOLI_IS_TAB (tab));
	g_return_if_fail (SOLI_IS_WINDOW (window));

	task = g_task_new (tab, cancellable, callback, user_data);
	g_task_set_task_data (task, g_object_ref (window), g_object_unref);

	dialog = gtk_file_chooser_dialog_new ("Save As",
										GTK_WINDOW (window),
										GTK_FILE_CHOOSER_ACTION_SAVE,
										"_Cancel",
										GTK_RESPONSE_CANCEL,
										"_Save",
										GTK_RESPONSE_ACCEPT,
										NULL);

	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

	gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

	doc = soli_tab_get_document (tab);
	file = soli_document_get_file (doc);
	location = gtk_source_file_get_location (file);

	if (location != NULL)
	{
		gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog),
										g_file_get_basename (location));
	}
	else
	{
		gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog),
										"Untitled Document");
	}

	g_signal_connect (dialog,
					"response",
					G_CALLBACK (save_dialog_response_cb),
					task);

	gtk_widget_show_all (dialog);
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

	if (soli_document_is_untitled (doc) ||
			gtk_source_file_is_readonly (file))
	{
		/* Document must be saved via "Save As..." */
		save_as_tab_async (tab,
							window,
							cancellable,
							(GAsyncReadyCallback) save_as_tab_ready_cb,
							task);

		return;
	}

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

static void
save_and_close_tab (SoliTab *tab,
					SoliWindow *window)
{
	/* Track save state, close when finished */
	// TODO: need to implement tab states
//	g_signal_connect (tab,
//						"notify::state",
//						G_CALLBACK (tab_save_state_changed_cb),
//						window);

	save_tab (tab, window);
}

static void
save_and_close_document (SoliDocument *doc,
						SoliWindow *window)
{
	SoliTab *tab;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));
	g_return_if_fail (SOLI_IS_WINDOW (window));

	tab = soli_tab_get_from_document (doc);
	g_return_if_fail (tab != NULL);

	save_and_close_tab (tab, window);
}

static void
close_document (SoliDocument *doc,
				SoliWindow *window)
{
	SoliTab *tab;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));
	g_return_if_fail (SOLI_IS_WINDOW (window));

	tab = soli_tab_get_from_document (doc);
	g_return_if_fail (tab != NULL);

	soli_window_close_tab (window, tab);
}

static void
close_confirmation_response_cb (GtkDialog *dialog,
								gint response_id,
								SoliWindow *window)
{
	SoliTab *tab;
	SoliDocument *doc;

	gtk_widget_hide (GTK_WIDGET (dialog));

	tab = g_object_get_data (G_OBJECT (dialog), SOLI_TAB_TO_CLOSE);
	doc = soli_tab_get_document (tab);

	switch (response_id)
	{
		case GTK_RESPONSE_NO:
			close_document (doc, window);
			break;

		case GTK_RESPONSE_YES:
			save_and_close_document (doc, window);
			break;

		default:
			/* Do nothing */
			break;
	}

	gtk_widget_destroy (GTK_WIDGET (dialog));
}

static gboolean
tab_can_close (SoliTab *tab,
				SoliWindow *window)
{
	if (!soli_tab_can_close (tab))
	{
		/* Display close confirmation dialog */
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new (GTK_WINDOW (window),
										GTK_DIALOG_MODAL,
										GTK_MESSAGE_QUESTION,
										GTK_BUTTONS_NONE,
										"Save changes to document before closing?");

		gtk_dialog_add_buttons (GTK_DIALOG (dialog),
								"Close without Saving",
								GTK_RESPONSE_NO,
								"Cancel",
								GTK_RESPONSE_CANCEL,
								"Save",
								GTK_RESPONSE_YES,
								NULL);

		g_signal_connect (dialog,
						"response",
						G_CALLBACK (close_confirmation_response_cb),
						window);

		g_object_set_data (G_OBJECT (dialog),
							SOLI_TAB_TO_CLOSE,
							tab);

		gtk_widget_show (dialog);
		
		return FALSE;
	}

	return TRUE;
}

void
soli_cmd_close_tab (SoliTab *tab,
					SoliWindow *window)
{
	/* Ensure window is the tab's top-level container */
	g_return_if_fail (GTK_WIDGET (window) == gtk_widget_get_toplevel (GTK_WIDGET (tab)));
	
	/* Check if tab has unsaved changes, etc. */
	if (tab_can_close (tab, window))
	{
		soli_window_close_tab (window, tab);
	}
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
                gpointer       user_data)
{
	GApplication *app;

	g_return_if_fail (G_IS_APPLICATION (user_data));

	app = G_APPLICATION (user_data);

	g_application_quit (app);
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
soli_cmd_plugins (GSimpleAction *action,
					GVariant *parameter,
					gpointer user_data)
{
	SoliApp *app;
	SoliWindow *window;
	GtkWidget *dialog;
	GtkWidget *content_area, *plugin_manager;

	app = SOLI_APP (user_data);
	window = soli_app_get_active_window (app);

	dialog = gtk_dialog_new_with_buttons ("Plugins",
										GTK_WINDOW (window),
										GTK_DIALOG_MODAL,
										"_OK",
										GTK_RESPONSE_NONE,
										NULL);

	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	plugin_manager = peas_gtk_plugin_manager_new (NULL);

	gtk_container_add (GTK_CONTAINER (content_area), plugin_manager);

	gtk_widget_show_all (content_area);

	gtk_dialog_run (GTK_DIALOG (dialog));

//	gtk_widget_show_all (dialog);
//	gtk_window_present (GTK_WINDOW (dialog));

	gtk_widget_destroy (dialog);
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
