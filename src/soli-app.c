/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-app.c
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

#include "soli-app.h"
#include "soli-window.h"
#include "soli-commands.h"

G_DEFINE_TYPE (SoliApp, soli_app, GTK_TYPE_APPLICATION);

static void
soli_app_init (SoliApp *object)
{
}

static GActionEntry app_entries[] = {
	{ "quit", soli_cmd_quit },
	{ "preferences", soli_cmd_preferences },
	{ "about", soli_cmd_about }
};

static void
open_files (GApplication *app,
			GSList *file_list)
{
	SoliWindow *window;
	GSList *l;

	window = soli_app_get_active_window (SOLI_APP (app));
	if (window == NULL)
	{
		window = soli_app_create_window (SOLI_APP (app), NULL);

		gtk_widget_show (GTK_WIDGET (window));
	}

	for (l = file_list; l != NULL; l = l->next)
	{
		GFile *file = l->data;

		if (G_IS_FILE (file))
		{
			soli_window_open (window, file);
		}
	}

	gtk_window_present (GTK_WINDOW (window));
}

static void
soli_app_startup (GApplication *app)
{
	GtkBuilder *builder;
	GMenuModel *menu_bar;

	G_APPLICATION_CLASS (soli_app_parent_class)->startup (app);

	g_action_map_add_action_entries (G_ACTION_MAP (app),
	                                 app_entries, G_N_ELEMENTS (app_entries),
	                                 app);

	builder = gtk_builder_new_from_resource ("/org/gnome/soli/menu-bar.ui");
	menu_bar = G_MENU_MODEL (gtk_builder_get_object (builder, "menu-bar"));
	gtk_application_set_menubar (GTK_APPLICATION (app), menu_bar);

	g_object_unref (builder);
}

/* GApplication implementation */
static void
soli_app_activate (GApplication *app)
{
	open_files (app, NULL);
}

static void
soli_app_open (GApplication  *app,
		         GFile        **files,
		         gint           n_files,
		         const gchar   *hint)
{
	GSList *file_list = NULL;
	gint i;

	for (i = 0; i < n_files; i++)
	{
		file_list = g_slist_prepend (file_list, files[i]);
	}

	file_list = g_slist_reverse (file_list);

	open_files (app, file_list);

	g_slist_free (file_list);
}

static gboolean
window_delete_event (SoliWindow *window,
					GdkEvent *event,
					SoliApp *app)
{
	// TODO: check window state before closing

	soli_cmd_quit (NULL, NULL, window);

	/* Do not destroy the window */
	return TRUE;
}

static SoliWindow *
soli_app_create_window_impl (SoliApp *app)
{
	SoliWindow *window;

	window = g_object_new (SOLI_TYPE_WINDOW, "application", app, NULL);

	g_signal_connect (window,
						"delete_event",
						G_CALLBACK (window_delete_event),
						app);

	return window;
}

static void
soli_app_class_init (SoliAppClass *klass)
{
	G_APPLICATION_CLASS (klass)->startup = soli_app_startup;
	G_APPLICATION_CLASS (klass)->activate = soli_app_activate;
	G_APPLICATION_CLASS (klass)->open = soli_app_open;

	klass->create_window = soli_app_create_window_impl;
}

SoliApp *
soli_app_new (void)
{
	return g_object_new (soli_app_get_type (),
	                     "application-id", "org.gnome.soli",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
}

SoliWindow *
soli_app_get_active_window (SoliApp *app)
{
	GList *windows, *l;

	windows = gtk_application_get_windows (GTK_APPLICATION (app));
	for (l = windows; l != NULL; l = l->next)
	{
		GtkWindow *window = l->data;

		if (SOLI_IS_WINDOW (window))
		{
			return SOLI_WINDOW (window);
		}

	}

	return NULL;
}

static gchar *
gen_role (void)
{
	GTimeVal result;
	static gint serial;

	g_get_current_time (&result);

	return g_strdup_printf ("soli-window-%ld-%ld-%d-%s",
							result.tv_sec,
							result.tv_usec,
							serial++,
							g_get_host_name());
}

SoliWindow *
soli_app_create_window (SoliApp *app,
						GdkScreen *screen)
{
	SoliWindow *window;
	gchar *role;

	window = SOLI_APP_GET_CLASS (app)->create_window (app);

	if (screen != NULL)
	{
		gtk_window_set_screen (GTK_WINDOW (window), screen);
	}

	role = gen_role();
	gtk_window_set_role (GTK_WINDOW (window), role);
	g_free (role);

	// TODO: set window state from settings

	return window;
}
