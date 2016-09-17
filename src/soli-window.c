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

#include "soli-window.h"

#include <libpeas/peas.h>

#include "soli-window-activatable.h"
#include "soli-app.h"
#include "soli-notebook.h"
#include "soli-tab.h"
#include "soli-view.h"
#include "soli-commands.h"
#include "soli-settings.h"
#include "soli-plugins-engine.h"

struct _SoliWindowPrivate
{
	GSettings *settings;

	SoliNotebook *notebook;

	PeasExtensionSet *extensions;

	guint dispose_has_run : 1;
};

G_DEFINE_TYPE_WITH_PRIVATE (SoliWindow, soli_window, GTK_TYPE_APPLICATION_WINDOW);

static GActionEntry win_entries[] = {
	{ "open", soli_cmd_open },
	{ "save", soli_cmd_save },
	{ "close", soli_cmd_close },
	
	{ "cut", soli_cmd_cut },
	{ "copy", soli_cmd_copy },
	{ "paste", soli_cmd_paste },
};

static void
on_extension_added (PeasExtensionSet *extensions,
					PeasPluginInfo *info,
					PeasExtension *extension,
					SoliWindow *window)
{
	soli_window_activatable_activate (SOLI_WINDOW_ACTIVATABLE (extension));
}

static void
on_extension_removed (PeasExtensionSet *extensions,
					PeasPluginInfo *info,
					PeasExtension *extension,
					SoliWindow *window)
{
	soli_window_activatable_deactivate (SOLI_WINDOW_ACTIVATABLE (extension));
}

static void
on_extension_update_state (PeasExtensionSet *extensions,
					PeasPluginInfo *info,
					PeasExtension *extension,
					SoliWindow *window)
{
	soli_window_activatable_update_state (SOLI_WINDOW_ACTIVATABLE (extension));
}

static void
soli_window_dispose (GObject *object)
{
	SoliWindow *window;

	window = SOLI_WINDOW (object);

	g_clear_object (&window->priv->settings);

	/* First, force collection so that plugins really drop
	 * some of the references.
	 */
	peas_engine_garbage_collect (PEAS_ENGINE (soli_plugins_engine_get_default ()));

	if (!window->priv->dispose_has_run)
	{
		/* Note that unreffing the extensions will automatically remove
		 * all extensions, which will in turn deactivate the extension.
		 */
		g_object_unref (window->priv->extensions);

		peas_engine_garbage_collect (PEAS_ENGINE (soli_plugins_engine_get_default ()));

		window->priv->dispose_has_run = TRUE;
	}

	/* Force collection again to clean up broken reference loops. */
	peas_engine_garbage_collect (peas_engine_get_default ());

	g_action_map_remove_action (G_ACTION_MAP (window), "show-line-numbers");
	g_action_map_remove_action (G_ACTION_MAP (window), "highlight-current-line");
	g_action_map_remove_action (G_ACTION_MAP (window), "wrap-mode");

	G_OBJECT_CLASS (soli_window_parent_class)->dispose (object);
}

static void
soli_window_init (SoliWindow *window)
{
	GAction *action;

	window->priv = soli_window_get_instance_private (window);

	window->priv->dispose_has_run = FALSE;
	window->priv->settings = g_settings_new ("ca.dluco.soli.preferences");

	gtk_widget_init_template (GTK_WIDGET (window));

	g_action_map_add_action_entries (G_ACTION_MAP (window),
									win_entries,
									G_N_ELEMENTS (win_entries),
									window);

	/* Map settings keys to their corresponding menu item's state. */
	action = g_settings_create_action (window->priv->settings, "show-line-numbers");
	g_action_map_add_action (G_ACTION_MAP (window), action);
	g_object_unref (action);

	action = g_settings_create_action (window->priv->settings, "highlight-current-line");
	g_action_map_add_action (G_ACTION_MAP (window), action);
	g_object_unref (action);

	action = g_settings_create_action (window->priv->settings, "wrap-mode");
	g_action_map_add_action (G_ACTION_MAP (window), action);
	g_object_unref (action);

	window->priv->extensions = peas_extension_set_new (peas_engine_get_default (),
														SOLI_TYPE_WINDOW_ACTIVATABLE,
														"window", window,
														NULL);

	peas_extension_set_foreach (window->priv->extensions,
								(PeasExtensionSetForeachFunc) on_extension_added,
								window);

	g_signal_connect (window->priv->extensions,
					"extension-added",
					G_CALLBACK (on_extension_added),
					window);

	g_signal_connect (window->priv->extensions,
					"extension-removed",
					G_CALLBACK (on_extension_removed),
					window);
}

static void
soli_window_class_init (SoliWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	object_class->dispose = soli_window_dispose;

	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/ca/dluco/soli/soli-window.ui");

	gtk_widget_class_bind_template_child_private (widget_class,
	                                              SoliWindow, notebook);
}

SoliNotebook *
soli_window_get_notebook (SoliWindow *window)
{
	g_return_val_if_fail (SOLI_IS_WINDOW (window), NULL);

	return window->priv->notebook;
}

SoliTab *
soli_window_get_active_tab (SoliWindow *window)
{
	g_return_val_if_fail (SOLI_IS_WINDOW (window), NULL);

	return soli_notebook_get_active_tab (window->priv->notebook);
}

SoliView *
soli_window_get_active_view (SoliWindow *window)
{
	SoliTab *tab;

	g_return_val_if_fail (SOLI_IS_WINDOW (window), NULL);

	tab = soli_window_get_active_tab (window);

	g_return_val_if_fail (tab != NULL, NULL);

	return soli_tab_get_view (tab);
}

GList *
soli_window_get_unsaved_docs (SoliWindow *window)
{
	GList *res = NULL, *tabs, *t;

	g_return_val_if_fail (SOLI_IS_WINDOW (window), NULL);

	tabs = gtk_container_get_children (GTK_CONTAINER (window->priv->notebook));
	for (t = g_list_last (tabs); t != NULL; t = t->prev)
	{
		SoliTab *tab = t->data;

		if (!soli_tab_can_close (tab))
		{
			res = g_list_prepend (res, soli_tab_get_document (tab));
		}
	}

	return g_list_reverse (res);
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
	SoliNotebook *notebook;
	
	g_return_val_if_fail (SOLI_IS_WINDOW (window), NULL);
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	
	tab = soli_tab_new ();
	
	soli_tab_load (tab, location, gtk_source_encoding_get_current ());
	
	notebook = soli_window_get_notebook (window);
	
	return process_new_tab (window, notebook, tab);
}

void
soli_window_open (SoliWindow *window, GFile *file)
{
	g_return_if_fail (SOLI_IS_WINDOW (window));
	g_return_if_fail (G_IS_FILE (file));
	
	soli_window_new_tab_from_location(window, file);
}

void
soli_window_close_tab (SoliWindow *window,
						SoliTab *tab)
{
	SoliNotebook *notebook;
	
	g_return_if_fail (SOLI_IS_WINDOW (window));
	g_return_if_fail (SOLI_IS_TAB (tab));
	
	notebook = soli_window_get_notebook (window);
	
	soli_notebook_close_tab (notebook, tab);
}

void
soli_window_close_all_tabs (SoliWindow *window)
{
	g_return_if_fail (SOLI_IS_WINDOW (window));
	// TODO: check window state

	soli_notebook_remove_all_tabs (window->priv->notebook);
}
