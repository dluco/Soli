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

typedef struct _SoliWindowPrivate SoliWindowPrivate;

struct _SoliWindowPrivate
{
	GtkWidget *notebook;
};

G_DEFINE_TYPE_WITH_PRIVATE (SoliWindow, soli_window, GTK_TYPE_APPLICATION_WINDOW);

static void
soli_window_init (SoliWindow *soli_window)
{
	gtk_widget_init_template (GTK_WIDGET (soli_window));
}

static void
soli_window_class_init (SoliWindowClass *klass)
{
	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
	                                             "/org/gnome/soli/window.ui");

	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass),
	                                              SoliWindow, notebook);
}

SoliWindow *
soli_window_new (SoliApp *app)
{
	return g_object_new (SOLI_WINDOW_TYPE, "application", app, NULL);
}

void
soli_window_open (SoliWindow *win, GFile *file)
{
	SoliWindowPrivate *priv;
	GtkWidget *scrolled, *view, *label;
	gchar *basename, *contents;
	gsize length;

	priv = soli_window_get_instance_private (win);
	basename = g_file_get_basename (file);

	scrolled = gtk_scrolled_window_new (NULL, NULL);
	view = gtk_text_view_new ();
	label = gtk_label_new (basename);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
	                                GTK_POLICY_AUTOMATIC,
	                                GTK_POLICY_AUTOMATIC);
	
	gtk_text_view_set_editable (GTK_TEXT_VIEW (view), FALSE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), FALSE);
	
	gtk_container_add (GTK_CONTAINER (scrolled), view);

	gtk_notebook_append_page (GTK_NOTEBOOK (priv->notebook),
	                          scrolled, label);

	if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
	{
		GtkTextBuffer *buffer;

		buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
		gtk_text_buffer_set_text (buffer, contents, length);
		g_free (contents);
	}

	gtk_widget_show_all (scrolled);

	g_free (basename);
}