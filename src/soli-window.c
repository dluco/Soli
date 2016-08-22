/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-window.c
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 *
 * soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "soli-app.h"
#include "soli-window.h"
#include "soli-notebook.h"
#include "soli-tab.h"
#include "soli-commands.h"

struct _SoliWindowPrivate
{
	SoliNotebook *notebook;
};

G_DEFINE_TYPE_WITH_PRIVATE (SoliWindow, soli_window, GTK_TYPE_APPLICATION_WINDOW);

static GActionEntry win_entries[] = {
	{ "open", soli_cmd_open }
};

static void
soli_window_init (SoliWindow *window)
{
	window->priv = soli_window_get_instance_private (window);

	gtk_widget_init_template (GTK_WIDGET (window));

	g_action_map_add_action_entries (G_ACTION_MAP (window),
									win_entries,
									G_N_ELEMENTS (win_entries),
									window);
}

static void
soli_window_class_init (SoliWindowClass *klass)
{
	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
	                                             "/org/gnome/soli/soli-window.ui");

	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass),
	                                              SoliWindow, notebook);
}

SoliWindow *
soli_window_new (SoliApp *app)
{
	return g_object_new (SOLI_TYPE_WINDOW, "application", app, NULL);
}

GtkWidget *
soli_window_get_notebook (SoliWindow *window)
{
	g_return_val_if_fail (SOLI_IS_WINDOW (window), NULL);

	return GTK_WIDGET (window->priv->notebook);
}

static SoliTab *
process_new_tab (SoliWindow *window, SoliNotebook *notebook, SoliTab *tab)
{
	g_return_val_if_fail (SOLI_IS_TAB (tab), NULL);
	
	soli_notebook_add_tab (notebook, tab, -1);
	
	gtk_widget_show (GTK_WIDGET (tab));
	
	/* Show tab's window if not visible */
	if (!gtk_widget_get_visible (GTK_WIDGET (window)))
	{
		gtk_window_present (GTK_WINDOW (window));
	}
	
	return tab;
}

SoliTab *
soli_window_new_tab_from_location (SoliWindow *window,
										GFile *location)
{
	SoliTab *tab;
	GtkWidget *notebook;
	
	g_return_val_if_fail (SOLI_IS_WINDOW (window), NULL);
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	
	tab = soli_tab_new ();
	
	soli_tab_load (tab, location, gtk_source_encoding_get_current ());
	
	notebook = soli_window_get_notebook (window);
	
	return process_new_tab (window, SOLI_NOTEBOOK (notebook), tab);
}

void
soli_window_open (SoliWindow *window, GFile *file)
{
	soli_window_new_tab_from_location(window, file);
	
//	SoliWindowPrivate *priv;
//	GtkWidget *scrolled, *view, *label;
//	gchar *basename, *contents;
//	gsize length;
//	
//	g_return_if_fail (SOLI_IS_WINDOW (window));
//	g_return_if_fail (G_IS_FILE (file));
//
//	priv = soli_window_get_instance_private (window);
//	basename = g_file_get_basename (file);
//
//	scrolled = gtk_scrolled_window_new (NULL, NULL);
//	view = gtk_text_view_new ();
//	label = gtk_label_new (basename);
//
//	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
//	                                GTK_POLICY_AUTOMATIC,
//	                                GTK_POLICY_AUTOMATIC);
//	
//	gtk_text_view_set_editable (GTK_TEXT_VIEW (view), FALSE);
//	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), FALSE);
//	
//	gtk_container_add (GTK_CONTAINER (scrolled), view);
//
//	gtk_notebook_append_page (GTK_NOTEBOOK (priv->notebook),
//	                          scrolled, label);
//
//	if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
//	{
//		GtkTextBuffer *buffer;
//
//		buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
//		gtk_text_buffer_set_text (buffer, contents, length);
//		g_free (contents);
//	}
//
//	gtk_widget_show_all (scrolled);
//
//	g_free (basename);
}
