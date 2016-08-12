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


G_DEFINE_TYPE (SoliApp, soli_app, GTK_TYPE_APPLICATION);

static void
soli_app_init (SoliApp *object)
{
}

static void
preferences_activated (GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer       app)
{
}

static void
quit_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       app)
{
	g_application_quit (G_APPLICATION (app));
}

static void
about_activated (GSimpleAction *action,
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

static GActionEntry app_entries[] =
{
	{ "preferences", preferences_activated, NULL, NULL, NULL },
	{ "quit", quit_activated, NULL, NULL, NULL },

	{ "about", about_activated, NULL, NULL, NULL }
};

static void
soli_app_startup (GApplication *app)
{
	GtkBuilder *builder;
	GMenuModel *menu_bar;
	const gchar *quit_accels[2] = { "<Ctrl>Q", NULL };

	G_APPLICATION_CLASS (soli_app_parent_class)->startup (app);

	g_action_map_add_action_entries (G_ACTION_MAP (app),
	                                 app_entries, G_N_ELEMENTS (app_entries),
	                                 app);
	gtk_application_set_accels_for_action (GTK_APPLICATION (app),
	                                       "app.quit",
	                                       quit_accels);

	builder = gtk_builder_new_from_resource ("/org/gnome/soli/menu-bar.ui");
	menu_bar = G_MENU_MODEL (gtk_builder_get_object (builder, "menu-bar"));
	gtk_application_set_menubar (GTK_APPLICATION (app), menu_bar);
	g_object_unref (builder);
}

/* GApplication implementation */
static void
soli_app_activate (GApplication *app)
{
	SoliWindow *win;

	win = soli_window_new (SOLI_APP (app));
	gtk_window_present (GTK_WINDOW (win));	
}

static void
soli_app_open (GApplication  *app,
		         GFile        **files,
		         gint           n_files,
		         const gchar   *hint)
{
	GList *windows;
	SoliWindow *win;
	gint i;

	windows = gtk_application_get_windows (GTK_APPLICATION (app));
	if (windows)
		win = SOLI_WINDOW (windows->data);
	else
		win = soli_window_new (SOLI_APP (app));

	for (i = 0; i < n_files; i++)
	{
		soli_window_open (win, files[i]);
	}

	gtk_window_present (GTK_WINDOW (win));
}

static void
soli_app_class_init (SoliAppClass *klass)
{
	G_APPLICATION_CLASS (klass)->startup = soli_app_startup;
	G_APPLICATION_CLASS (klass)->activate = soli_app_activate;
	G_APPLICATION_CLASS (klass)->open = soli_app_open;
}

SoliApp *
soli_app_new (void)
{
	return g_object_new (soli_app_get_type (),
	                     "application-id", "org.gnome.soli",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
}

