/*
 * soli-app.c
 * This file is part of soli
 *
 * Copyright (C) 2005-2006 - Paolo Maggi
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

#include "soli-app.h"
#include "soli-app-private.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <glib/gi18n.h>
#include <gio/gio.h>
#include <libpeas/peas-extension-set.h>
#include <gtksourceview/gtksource.h>

#ifdef ENABLE_INTROSPECTION
#include <girepository.h>
#endif

#include "soli-commands-private.h"
#include "soli-notebook.h"
#include "soli-debug.h"
#include "soli-utils.h"
#include "soli-enum-types.h"
#include "soli-dirs.h"
#include "soli-settings.h"
#include "soli-app-activatable.h"
#include "soli-plugins-engine.h"
#include "soli-commands.h"
#include "soli-preferences-dialog.h"
#include "soli-tab.h"
#include "soli-tab-private.h"

#ifndef ENABLE_GVFS_METADATA
#include "soli-metadata-manager.h"
#endif

#define SOLI_PAGE_SETUP_FILE		"soli-page-setup"
#define SOLI_PRINT_SETTINGS_FILE	"soli-print-settings"

typedef struct
{
	SoliPluginsEngine *engine;

	GtkCssProvider     *theme_provider;

	SoliLockdownMask  lockdown;

	GtkPageSetup      *page_setup;
	GtkPrintSettings  *print_settings;

	SoliSettings     *settings;
	GSettings         *editor_settings;
	GSettings         *window_settings;

	GMenuModel        *hamburger_menu;
	GMenuModel        *notebook_menu;
	GMenuModel        *tab_width_menu;
	GMenuModel        *line_col_menu;

	PeasExtensionSet  *extensions;
	GNetworkMonitor   *monitor;

	/* command line parsing */
	gboolean new_window;
	gboolean new_document;
	gchar *geometry;
	const GtkSourceEncoding *encoding;
	GInputStream *stdin_stream;
	GSList *file_list;
	gint line_position;
	gint column_position;
	GApplicationCommandLine *command_line;
} SoliAppPrivate;

enum
{
	PROP_0,
	PROP_LOCKDOWN,
	LAST_PROP
};

static GParamSpec *properties[LAST_PROP];

static const GOptionEntry options[] =
{
	/* Version */
	{
		"version", 'V', 0, G_OPTION_ARG_NONE, NULL,
		N_("Show the application's version"), NULL
	},

	/* List available encodings */
	{
		"list-encodings", '\0', 0, G_OPTION_ARG_NONE, NULL,
		N_("Display list of possible values for the encoding option"),
		NULL
	},

	/* Encoding */
	{
		"encoding", '\0', 0, G_OPTION_ARG_STRING, NULL,
		N_("Set the character encoding to be used to open the files listed on the command line"),
		N_("ENCODING")
	},

	/* Open a new window */
	{
		"new-window", '\0', 0, G_OPTION_ARG_NONE, NULL,
		N_("Create a new top-level window in an existing instance of soli"),
		NULL
	},

	/* Create a new empty document */
	{
		"new-document", '\0', 0, G_OPTION_ARG_NONE, NULL,
		N_("Create a new document in an existing instance of soli"),
		NULL
	},

	/* Window geometry */
	{
		"geometry", 'g', 0, G_OPTION_ARG_STRING, NULL,
		N_("Set the size and position of the window (WIDTHxHEIGHT+X+Y)"),
		N_("GEOMETRY")
	},

	/* Wait for closing documents */
	{
		"wait", 'w', 0, G_OPTION_ARG_NONE, NULL,
		N_("Open files and block process until files are closed"),
		NULL
	},

	/* New instance */
	{
		"standalone", 's', 0, G_OPTION_ARG_NONE, NULL,
		N_("Run soli in standalone mode"),
		NULL
	},

	/* collects file arguments */
	{
		G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, NULL, NULL,
		N_("[FILE...] [+LINE[:COLUMN]]")
	},

	{NULL}
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (SoliApp, soli_app, GTK_TYPE_APPLICATION)

static void
soli_app_dispose (GObject *object)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (SOLI_APP (object));

	g_clear_object (&priv->editor_settings);
	g_clear_object (&priv->window_settings);
	g_clear_object (&priv->settings);

	g_clear_object (&priv->page_setup);
	g_clear_object (&priv->print_settings);

	/* Note that unreffing the extensions will automatically remove
	 * all extensions which in turn will deactivate the extension
	 */
	g_clear_object (&priv->extensions);

	g_clear_object (&priv->engine);

	if (priv->theme_provider != NULL)
	{
		gtk_style_context_remove_provider_for_screen (gdk_screen_get_default (),
		                                              GTK_STYLE_PROVIDER (priv->theme_provider));
		g_clear_object (&priv->theme_provider);
	}

	g_clear_object (&priv->hamburger_menu);
	g_clear_object (&priv->notebook_menu);
	g_clear_object (&priv->tab_width_menu);
	g_clear_object (&priv->line_col_menu);

	G_OBJECT_CLASS (soli_app_parent_class)->dispose (object);
}

static void
soli_app_get_property (GObject    *object,
			guint       prop_id,
			GValue     *value,
			GParamSpec *pspec)
{
	SoliApp *app = SOLI_APP (object);

	switch (prop_id)
	{
		case PROP_LOCKDOWN:
			g_value_set_flags (value, soli_app_get_lockdown (app));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static gchar *
soli_app_help_link_id_impl (SoliApp    *app,
                             const gchar *name,
                             const gchar *link_id)
{
	if (link_id)
	{
		return g_strdup_printf ("help:%s/%s", name, link_id);
	}
	else
	{
		return g_strdup_printf ("help:%s", name);
	}
}

static gboolean
soli_app_show_help_impl (SoliApp    *app,
                          GtkWindow   *parent,
                          const gchar *name,
                          const gchar *link_id)
{
	GError *error = NULL;
	gboolean ret;
	gchar *link;

	if (name == NULL)
	{
		name = "soli";
	}
	else if (strcmp (name, "soli.xml") == 0)
	{
		g_warning ("%s: Using \"soli.xml\" for the help name is deprecated, use \"soli\" or simply NULL instead", G_STRFUNC);
		name = "soli";
	}

	link = SOLI_APP_GET_CLASS (app)->help_link_id (app, name, link_id);

	ret = gtk_show_uri (gtk_widget_get_screen (GTK_WIDGET (parent)),
	                    link,
	                    GDK_CURRENT_TIME,
	                    &error);

	g_free (link);

	if (error != NULL)
	{
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new (parent,
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_ERROR,
						 GTK_BUTTONS_CLOSE,
						 _("There was an error displaying the help."));

		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
							  "%s", error->message);

		g_signal_connect (G_OBJECT (dialog),
				  "response",
				  G_CALLBACK (gtk_widget_destroy),
				  NULL);

		gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);

		gtk_widget_show (dialog);

		g_error_free (error);
	}

	return ret;
}

static void
soli_app_set_window_title_impl (SoliApp    *app,
                                 SoliWindow *window,
                                 const gchar *title)
{
	gtk_window_set_title (GTK_WINDOW (window), title);
}

static gboolean
is_in_viewport (GtkWindow    *window,
		GdkScreen    *screen,
		gint          workspace,
		gint          viewport_x,
		gint          viewport_y)
{
	GdkScreen *s;
	GdkDisplay *display;
	GdkWindow *gdkwindow;
	const gchar *cur_name;
	const gchar *name;
	gint cur_n;
	gint n;
	gint ws;
	gint sc_width, sc_height;
	gint x, y, width, height;
	gint vp_x, vp_y;

	/* Check for screen and display match */
	display = gdk_screen_get_display (screen);
	cur_name = gdk_display_get_name (display);
	cur_n = gdk_screen_get_number (screen);

	s = gtk_window_get_screen (window);
	display = gdk_screen_get_display (s);
	name = gdk_display_get_name (display);
	n = gdk_screen_get_number (s);

	if (strcmp (cur_name, name) != 0 || cur_n != n)
	{
		return FALSE;
	}

	/* Check for workspace match */
	ws = soli_utils_get_window_workspace (window);
	if (ws != workspace && ws != SOLI_ALL_WORKSPACES)
	{
		return FALSE;
	}

	/* Check for viewport match */
	gdkwindow = gtk_widget_get_window (GTK_WIDGET (window));
	gdk_window_get_position (gdkwindow, &x, &y);
	width = gdk_window_get_width (gdkwindow);
	height = gdk_window_get_height (gdkwindow);
	soli_utils_get_current_viewport (screen, &vp_x, &vp_y);
	x += vp_x;
	y += vp_y;

	sc_width = gdk_screen_get_width (screen);
	sc_height = gdk_screen_get_height (screen);

	return x + width * .25 >= viewport_x &&
	       x + width * .75 <= viewport_x + sc_width &&
	       y >= viewport_y &&
	       y + height <= viewport_y + sc_height;
}

static SoliWindow *
get_active_window (GtkApplication *app)
{
	GdkScreen *screen;
	guint workspace;
	gint viewport_x, viewport_y;
	GList *windows, *l;

	screen = gdk_screen_get_default ();

	workspace = soli_utils_get_current_workspace (screen);
	soli_utils_get_current_viewport (screen, &viewport_x, &viewport_y);

	/* Gtk documentation says the window list is always in MRU order */
	windows = gtk_application_get_windows (app);
	for (l = windows; l != NULL; l = l->next)
	{
		GtkWindow *window = l->data;

		if (SOLI_IS_WINDOW (window) && is_in_viewport (window, screen, workspace, viewport_x, viewport_y))
		{
			return SOLI_WINDOW (window);
		}
	}

	return NULL;
}

static void
set_command_line_wait (SoliApp *app,
		       SoliTab *tab)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (app);

	g_object_set_data_full (G_OBJECT (tab),
	                        "SoliTabCommandLineWait",
	                        g_object_ref (priv->command_line),
	                        (GDestroyNotify)g_object_unref);
}

static void
set_command_line_wait_doc (SoliDocument *doc,
			   SoliApp      *app)
{
	SoliTab *tab = soli_tab_get_from_document (doc);

	set_command_line_wait (app, tab);
}

static void
open_files (GApplication            *application,
	    gboolean                 new_window,
	    gboolean                 new_document,
	    gchar                   *geometry,
	    gint                     line_position,
	    gint                     column_position,
	    const GtkSourceEncoding *encoding,
	    GInputStream            *stdin_stream,
	    GSList                  *file_list,
	    GApplicationCommandLine *command_line)
{
	SoliWindow *window = NULL;
	SoliTab *tab;
	gboolean doc_created = FALSE;

	if (!new_window)
	{
		window = get_active_window (GTK_APPLICATION (application));
	}

	if (window == NULL)
	{
		soli_debug_message (DEBUG_APP, "Create main window");
		window = soli_app_create_window (SOLI_APP (application), NULL);

		soli_debug_message (DEBUG_APP, "Show window");
		gtk_widget_show (GTK_WIDGET (window));
	}

	if (geometry)
	{
		gtk_window_parse_geometry (GTK_WINDOW (window), geometry);
	}

	if (stdin_stream)
	{
		soli_debug_message (DEBUG_APP, "Load stdin");

		tab = soli_window_create_tab_from_stream (window,
		                                           stdin_stream,
		                                           encoding,
		                                           line_position,
		                                           column_position,
		                                           TRUE);
		doc_created = tab != NULL;

		if (doc_created && command_line)
		{
			set_command_line_wait (SOLI_APP (application),
					       tab);
		}
		g_input_stream_close (stdin_stream, NULL, NULL);
	}

	if (file_list != NULL)
	{
		GSList *loaded;

		soli_debug_message (DEBUG_APP, "Load files");
		loaded = _soli_cmd_load_files_from_prompt (window,
		                                            file_list,
		                                            encoding,
		                                            line_position,
		                                            column_position);

		doc_created = doc_created || loaded != NULL;

		if (command_line)
		{
			g_slist_foreach (loaded, (GFunc)set_command_line_wait_doc, SOLI_APP (application));
		}
		g_slist_free (loaded);
	}

	if (!doc_created || new_document)
	{
		soli_debug_message (DEBUG_APP, "Create tab");
		tab = soli_window_create_tab (window, TRUE);

		if (command_line)
		{
			set_command_line_wait (SOLI_APP (application),
					       tab);
		}
	}

	gtk_window_present (GTK_WINDOW (window));
}

static void
new_window_activated (GSimpleAction *action,
                      GVariant      *parameter,
                      gpointer       user_data)
{
	SoliApp *app;
	SoliWindow *window;

	app = SOLI_APP (user_data);
	window = soli_app_create_window (app, NULL);

	soli_debug_message (DEBUG_APP, "Show window");
	gtk_widget_show (GTK_WIDGET (window));

	soli_debug_message (DEBUG_APP, "Create tab");
	soli_window_create_tab (window, TRUE);

	gtk_window_present (GTK_WINDOW (window));
}

static void
new_document_activated (GSimpleAction *action,
                        GVariant      *parameter,
                        gpointer       user_data)
{
	GApplication *application = G_APPLICATION (user_data);

	open_files (application,
	            FALSE,
	            TRUE,
	            NULL,
	            0,
	            0,
	            NULL,
	            NULL,
	            NULL,
	            NULL);
}

static void
preferences_activated (GSimpleAction  *action,
                       GVariant       *parameter,
                       gpointer        user_data)
{
	GtkApplication *app;
	SoliWindow *window;

	app = GTK_APPLICATION (user_data);
	window = SOLI_WINDOW (gtk_application_get_active_window (app));

	soli_show_preferences_dialog (window);
}

static void
keyboard_shortcuts_activated (GSimpleAction *action,
                              GVariant      *parameter,
                              gpointer       user_data)
{
	GtkApplication *app;
	SoliWindow *window;

	app = GTK_APPLICATION (user_data);
	window = SOLI_WINDOW (gtk_application_get_active_window (app));

	_soli_cmd_help_keyboard_shortcuts (window);
}

static void
help_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       user_data)
{
	GtkApplication *app;
	SoliWindow *window;

	app = GTK_APPLICATION (user_data);
	window = SOLI_WINDOW (gtk_application_get_active_window (app));

	_soli_cmd_help_contents (window);
}

static void
about_activated (GSimpleAction  *action,
                 GVariant       *parameter,
                 gpointer        user_data)
{
	GtkApplication *app;
	SoliWindow *window;

	app = GTK_APPLICATION (user_data);
	window = SOLI_WINDOW (gtk_application_get_active_window (app));

	_soli_cmd_help_about (window);
}

static void
quit_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       user_data)
{
	_soli_cmd_file_quit (NULL, NULL, NULL);
}

static GActionEntry app_entries[] = {
	{ "new-window", new_window_activated, NULL, NULL, NULL },
	{ "new-document", new_document_activated, NULL, NULL, NULL },
	{ "preferences", preferences_activated, NULL, NULL, NULL },
	{ "shortcuts", keyboard_shortcuts_activated, NULL, NULL, NULL },
	{ "help", help_activated, NULL, NULL, NULL },
	{ "about", about_activated, NULL, NULL, NULL },
	{ "quit", quit_activated, NULL, NULL, NULL }
};

static void
extension_added (PeasExtensionSet *extensions,
		 PeasPluginInfo   *info,
		 PeasExtension    *exten,
		 SoliApp         *app)
{
	soli_app_activatable_activate (SOLI_APP_ACTIVATABLE (exten));
}

static void
extension_removed (PeasExtensionSet *extensions,
		   PeasPluginInfo   *info,
		   PeasExtension    *exten,
		   SoliApp         *app)
{
	soli_app_activatable_deactivate (SOLI_APP_ACTIVATABLE (exten));
}

static void
load_accels (void)
{
	gchar *filename;

	filename = g_build_filename (soli_dirs_get_user_config_dir (),
				     "accels",
				     NULL);
	if (filename != NULL)
	{
		soli_debug_message (DEBUG_APP, "Loading keybindings from %s\n", filename);
		gtk_accel_map_load (filename);
		g_free (filename);
	}
}

static GtkCssProvider *
load_css_from_resource (const gchar *filename,
                        gboolean     required)
{
	GError *error = NULL;
	GFile *css_file;
	GtkCssProvider *provider;
	gchar *resource_name;

	resource_name = g_strdup_printf ("resource:///ca/dluco/soli/css/%s", filename);
	css_file = g_file_new_for_uri (resource_name);
	g_free (resource_name);

	if (!required && !g_file_query_exists (css_file, NULL))
	{
		g_object_unref (css_file);
		return NULL;
	}

	provider = gtk_css_provider_new ();

	if (gtk_css_provider_load_from_file (provider, css_file, &error))
	{
		gtk_style_context_add_provider_for_screen (gdk_screen_get_default (),
		                                           GTK_STYLE_PROVIDER (provider),
		                                           GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	}
	else
	{
		g_warning ("Could not load css provider: %s", error->message);
		g_error_free (error);
	}

	g_object_unref (css_file);
	return provider;
}

static void
theme_changed (GtkSettings *settings,
	       GParamSpec  *pspec,
	       SoliApp    *app)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (app);

	gchar *theme, *lc_theme, *theme_css;

	g_object_get (settings, "gtk-theme-name", &theme, NULL);
	lc_theme = g_ascii_strdown (theme, -1);
	g_free (theme);

	theme_css = g_strdup_printf ("soli.%s.css", lc_theme);
	g_free (lc_theme);

	if (priv->theme_provider != NULL)
	{
		gtk_style_context_remove_provider_for_screen (gdk_screen_get_default (),
		                                              GTK_STYLE_PROVIDER (priv->theme_provider));
		g_clear_object (&priv->theme_provider);
	}

	priv->theme_provider = load_css_from_resource (theme_css, FALSE);

	g_free (theme_css);
}

static void
setup_theme_extensions (SoliApp *app)
{
	GtkSettings *settings;

	settings = gtk_settings_get_default ();
	g_signal_connect (settings, "notify::gtk-theme-name",
	                  G_CALLBACK (theme_changed), app);
	theme_changed (settings, NULL, app);
}

static GMenuModel *
get_menu_model (SoliApp   *app,
                const char *id)
{
	GMenu *menu;

	menu = gtk_application_get_menu_by_id (GTK_APPLICATION (app), id);

	return menu ? G_MENU_MODEL (g_object_ref_sink (menu)) : NULL;
}

static void
add_accelerator (GtkApplication *app,
                 const gchar    *action_name,
                 const gchar    *accel)
{
	const gchar *vaccels[] = {
		accel,
		NULL
	};

	gtk_application_set_accels_for_action (app, action_name, vaccels);
}

static gboolean
show_menubar (void)
{
	GtkSettings *settings = gtk_settings_get_default ();
	gboolean result;

	g_object_get (settings,
	              "gtk-shell-shows-menubar", &result,
	              NULL);

	result = TRUE;

	g_print ("Show menubar: %s\n", (result) ? "true" : "false");

	return result;
}

static void
soli_app_startup (GApplication *application)
{
	SoliAppPrivate *priv;
	GtkCssProvider *css_provider;
	GtkSourceStyleSchemeManager *manager;
#ifndef ENABLE_GVFS_METADATA
	const gchar *cache_dir;
	gchar *metadata_filename;
#endif

	priv = soli_app_get_instance_private (SOLI_APP (application));

	G_APPLICATION_CLASS (soli_app_parent_class)->startup (application);

	/* Setup debugging */
	soli_debug_init ();
	soli_debug_message (DEBUG_APP, "Startup");

	setup_theme_extensions (SOLI_APP (application));

#ifndef ENABLE_GVFS_METADATA
	cache_dir = soli_dirs_get_user_cache_dir ();
	metadata_filename = g_build_filename (cache_dir, "soli-metadata.xml", NULL);
	soli_metadata_manager_init (metadata_filename);
	g_free (metadata_filename);
#endif

	/* Load settings */
	priv->settings = soli_settings_new ();
	priv->editor_settings = g_settings_new ("ca.dluco.soli.preferences.editor");
	priv->window_settings = g_settings_new ("ca.dluco.soli.state.window");

	/* initial lockdown state */
	priv->lockdown = soli_settings_get_lockdown (priv->settings);

	g_action_map_add_action_entries (G_ACTION_MAP (application),
	                                 app_entries,
	                                 G_N_ELEMENTS (app_entries),
	                                 application);

	/* menus */
	if (!show_menubar ())
	{
		gtk_application_set_menubar (GTK_APPLICATION (application), NULL);
		priv->hamburger_menu = get_menu_model (SOLI_APP (application),
		                                       "hamburger-menu");
	}

	priv->notebook_menu = get_menu_model (SOLI_APP (application), "notebook-menu");
	priv->tab_width_menu = get_menu_model (SOLI_APP (application), "tab-width-menu");
	priv->line_col_menu = get_menu_model (SOLI_APP (application), "line-col-menu");

	/* Accelerators */
	add_accelerator (GTK_APPLICATION (application), "app.new-window", "<Primary>N");
	add_accelerator (GTK_APPLICATION (application), "app.quit", "<Primary>Q");
	add_accelerator (GTK_APPLICATION (application), "app.help", "F1");

	add_accelerator (GTK_APPLICATION (application), "win.hamburger-menu", "F10");
	add_accelerator (GTK_APPLICATION (application), "win.open", "<Primary>O");
	add_accelerator (GTK_APPLICATION (application), "win.save", "<Primary>S");
	add_accelerator (GTK_APPLICATION (application), "win.save-as", "<Primary><Shift>S");
	add_accelerator (GTK_APPLICATION (application), "win.save-all", "<Primary><Shift>L");
	add_accelerator (GTK_APPLICATION (application), "win.new-tab", "<Primary>T");
	add_accelerator (GTK_APPLICATION (application), "win.reopen-closed-tab", "<Primary><Shift>T");
	add_accelerator (GTK_APPLICATION (application), "win.close", "<Primary>W");
	add_accelerator (GTK_APPLICATION (application), "win.close-all", "<Primary><Shift>W");
	add_accelerator (GTK_APPLICATION (application), "win.print", "<Primary>P");
	add_accelerator (GTK_APPLICATION (application), "win.find", "<Primary>F");
	add_accelerator (GTK_APPLICATION (application), "win.find-next", "<Primary>G");
	add_accelerator (GTK_APPLICATION (application), "win.find-prev", "<Primary><Shift>G");
	add_accelerator (GTK_APPLICATION (application), "win.replace", "<Primary>H");
	add_accelerator (GTK_APPLICATION (application), "win.clear-highlight", "<Primary><Shift>K");
	add_accelerator (GTK_APPLICATION (application), "win.goto-line", "<Primary>I");
	add_accelerator (GTK_APPLICATION (application), "win.focus-active-view", "Escape");
	add_accelerator (GTK_APPLICATION (application), "win.side-panel", "F9");
	add_accelerator (GTK_APPLICATION (application), "win.bottom-panel", "<Primary>F9");
	add_accelerator (GTK_APPLICATION (application), "win.fullscreen", "F11");
	add_accelerator (GTK_APPLICATION (application), "win.new-tab-group", "<Primary><Alt>N");
	add_accelerator (GTK_APPLICATION (application), "win.previous-tab-group", "<Primary><Shift><Alt>Page_Up");
	add_accelerator (GTK_APPLICATION (application), "win.next-tab-group", "<Primary><Shift><Alt>Page_Down");
	add_accelerator (GTK_APPLICATION (application), "win.previous-document", "<Primary><Alt>Page_Up");
	add_accelerator (GTK_APPLICATION (application), "win.next-document", "<Primary><Alt>Page_Down");

	load_accels ();

	/* Load custom css */
	g_object_unref (load_css_from_resource ("soli-style.css", TRUE));
	css_provider = load_css_from_resource ("soli-style-os.css", FALSE);
	g_clear_object (&css_provider);

	/*
	 * We use the default gtksourceview style scheme manager so that plugins
	 * can obtain it easily without a soli specific api, but we need to
	 * add our search path at startup before the manager is actually used.
	 */
	manager = gtk_source_style_scheme_manager_get_default ();
	gtk_source_style_scheme_manager_append_search_path (manager,
	                                                    soli_dirs_get_user_styles_dir ());

	priv->engine = soli_plugins_engine_get_default ();
	priv->extensions = peas_extension_set_new (PEAS_ENGINE (priv->engine),
	                                           SOLI_TYPE_APP_ACTIVATABLE,
	                                           "app", SOLI_APP (application),
	                                           NULL);

	g_signal_connect (priv->extensions,
	                  "extension-added",
	                  G_CALLBACK (extension_added),
	                  application);

	g_signal_connect (priv->extensions,
	                  "extension-removed",
	                  G_CALLBACK (extension_removed),
	                  application);

	peas_extension_set_foreach (priv->extensions,
	                            (PeasExtensionSetForeachFunc) extension_added,
	                            application);
}

static void
soli_app_activate (GApplication *application)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (SOLI_APP (application));

	open_files (application,
	            priv->new_window,
	            priv->new_document,
	            priv->geometry,
	            priv->line_position,
	            priv->column_position,
	            priv->encoding,
	            priv->stdin_stream,
	            priv->file_list,
	            priv->command_line);
}

static void
clear_options (SoliApp *app)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (app);

	g_free (priv->geometry);
	g_clear_object (&priv->stdin_stream);
	g_slist_free_full (priv->file_list, g_object_unref);

	priv->new_window = FALSE;
	priv->new_document = FALSE;
	priv->geometry = NULL;
	priv->encoding = NULL;
	priv->file_list = NULL;
	priv->line_position = 0;
	priv->column_position = 0;
	priv->command_line = NULL;
}

static void
get_line_column_position (const gchar *arg,
                          gint        *line,
                          gint        *column)
{
	gchar **split;

	split = g_strsplit (arg, ":", 2);

	if (split != NULL)
	{
		if (split[0] != NULL)
		{
			*line = atoi (split[0]);
		}

		if (split[1] != NULL)
		{
			*column = atoi (split[1]);
		}
	}

	g_strfreev (split);
}

static gint
soli_app_command_line (GApplication            *application,
                        GApplicationCommandLine *cl)
{
	SoliAppPrivate *priv;
	GVariantDict *options;
	const gchar *encoding_charset;
	const gchar **remaining_args;

	priv = soli_app_get_instance_private (SOLI_APP (application));

	options = g_application_command_line_get_options_dict (cl);

	g_variant_dict_lookup (options, "new-window", "b", &priv->new_window);
	g_variant_dict_lookup (options, "new-document", "b", &priv->new_document);
	g_variant_dict_lookup (options, "geometry", "s", &priv->geometry);

	if (g_variant_dict_contains (options, "wait"))
	{
		priv->command_line = cl;
	}

	if (g_variant_dict_lookup (options, "encoding", "&s", &encoding_charset))
	{
		priv->encoding = gtk_source_encoding_get_from_charset (encoding_charset);

		if (priv->encoding == NULL)
		{
			g_application_command_line_printerr (cl,
							     _("%s: invalid encoding."),
							     encoding_charset);
		}
	}

	/* Parse filenames */
	if (g_variant_dict_lookup (options, G_OPTION_REMAINING, "^a&ay", &remaining_args))
	{
		gint i;

		for (i = 0; remaining_args[i]; i++)
		{
			if (*remaining_args[i] == '+')
			{
				if (*(remaining_args[i] + 1) == '\0')
				{
					/* goto the last line of the document */
					priv->line_position = G_MAXINT;
					priv->column_position = 0;
				}
				else
				{
					get_line_column_position (remaining_args[i] + 1,
								  &priv->line_position,
								  &priv->column_position);
				}
			}
			else if (*remaining_args[i] == '-' && *(remaining_args[i] + 1) == '\0')
			{
				priv->stdin_stream = g_application_command_line_get_stdin (cl);
			}
			else
			{
				GFile *file;

				file = g_application_command_line_create_file_for_arg (cl, remaining_args[i]);
				priv->file_list = g_slist_prepend (priv->file_list, file);
			}
		}

		priv->file_list = g_slist_reverse (priv->file_list);
		g_free (remaining_args);
	}

	g_application_activate (application);
	clear_options (SOLI_APP (application));

	return 0;
}

static void
print_all_encodings (void)
{
	GSList *all_encodings;
	GSList *l;

	all_encodings = gtk_source_encoding_get_all ();

	for (l = all_encodings; l != NULL; l = l->next)
	{
		const GtkSourceEncoding *encoding = l->data;
		g_print ("%s\n", gtk_source_encoding_get_charset (encoding));
	}

	g_slist_free (all_encodings);
}

static gint
soli_app_handle_local_options (GApplication *application,
                                GVariantDict *options)
{
	if (g_variant_dict_contains (options, "version"))
	{
		g_print ("%s - Version %s\n", g_get_application_name (), VERSION);
		return 0;
	}

	if (g_variant_dict_contains (options, "list-encodings"))
	{
		print_all_encodings ();
		return 0;
	}

	if (g_variant_dict_contains (options, "standalone"))
	{
		GApplicationFlags old_flags;

		old_flags = g_application_get_flags (application);
		g_application_set_flags (application, old_flags | G_APPLICATION_NON_UNIQUE);
	}

	if (g_variant_dict_contains (options, "wait"))
	{
		GApplicationFlags old_flags;

		old_flags = g_application_get_flags (application);
		g_application_set_flags (application, old_flags | G_APPLICATION_IS_LAUNCHER);
	}

	return -1;
}

/* Note: when launched from command line we do not reach this method
 * since we manually handle the command line parameters in order to
 * parse +LINE:COL, stdin, etc.
 * However this method is called when open() is called via dbus, for
 * instance when double clicking on a file in nautilus
 */
static void
soli_app_open (GApplication  *application,
                GFile        **files,
                gint           n_files,
                const gchar   *hint)
{
	gint i;
	GSList *file_list = NULL;

	for (i = 0; i < n_files; i++)
	{
		file_list = g_slist_prepend (file_list, files[i]);
	}

	file_list = g_slist_reverse (file_list);

	open_files (application,
	            FALSE,
	            FALSE,
	            NULL,
	            0,
	            0,
	            NULL,
	            NULL,
	            file_list,
	            NULL);

	g_slist_free (file_list);
}

static gboolean
ensure_user_config_dir (void)
{
	const gchar *config_dir;
	gboolean ret = TRUE;
	gint res;

	config_dir = soli_dirs_get_user_config_dir ();
	if (config_dir == NULL)
	{
		g_warning ("Could not get config directory\n");
		return FALSE;
	}

	res = g_mkdir_with_parents (config_dir, 0755);
	if (res < 0)
	{
		g_warning ("Could not create config directory\n");
		ret = FALSE;
	}

	return ret;
}

static void
save_accels (void)
{
	gchar *filename;

	filename = g_build_filename (soli_dirs_get_user_config_dir (),
				     "accels",
				     NULL);
	if (filename != NULL)
	{
		soli_debug_message (DEBUG_APP, "Saving keybindings in %s\n", filename);
		gtk_accel_map_save (filename);
		g_free (filename);
	}
}

static gchar *
get_page_setup_file (void)
{
	const gchar *config_dir;
	gchar *setup = NULL;

	config_dir = soli_dirs_get_user_config_dir ();

	if (config_dir != NULL)
	{
		setup = g_build_filename (config_dir,
					  SOLI_PAGE_SETUP_FILE,
					  NULL);
	}

	return setup;
}

static void
save_page_setup (SoliApp *app)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (app);

	if (priv->page_setup != NULL)
	{
		gchar *filename;
		GError *error = NULL;

		filename = get_page_setup_file ();

		gtk_page_setup_to_file (priv->page_setup,
					filename,
					&error);
		if (error)
		{
			g_warning ("%s", error->message);
			g_error_free (error);
		}

		g_free (filename);
	}
}

static gchar *
get_print_settings_file (void)
{
	const gchar *config_dir;
	gchar *settings = NULL;

	config_dir = soli_dirs_get_user_config_dir ();

	if (config_dir != NULL)
	{
		settings = g_build_filename (config_dir,
					     SOLI_PRINT_SETTINGS_FILE,
					     NULL);
	}

	return settings;
}

static void
save_print_settings (SoliApp *app)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (app);

	if (priv->print_settings != NULL)
	{
		gchar *filename;
		GError *error = NULL;

		filename = get_print_settings_file ();

		gtk_print_settings_to_file (priv->print_settings,
					    filename,
					    &error);
		if (error)
		{
			g_warning ("%s", error->message);
			g_error_free (error);
		}

		g_free (filename);
	}
}

static void
soli_app_shutdown (GApplication *app)
{
	soli_debug_message (DEBUG_APP, "Quitting\n");

	/* Last window is gone... save some settings and exit */
	ensure_user_config_dir ();

	save_accels ();
	save_page_setup (SOLI_APP (app));
	save_print_settings (SOLI_APP (app));

	/* GTK+ can still hold references to some soli objects, for example
	 * SoliDocument for the clipboard. So the metadata-manager should be
	 * shutdown after.
	 */
	G_APPLICATION_CLASS (soli_app_parent_class)->shutdown (app);

#ifndef ENABLE_GVFS_METADATA
	soli_metadata_manager_shutdown ();
#endif

	soli_dirs_shutdown ();
}

static gboolean
window_delete_event (SoliWindow *window,
                     GdkEvent    *event,
                     SoliApp    *app)
{
	SoliWindowState ws;

	ws = soli_window_get_state (window);

	if (ws &
	    (SOLI_WINDOW_STATE_SAVING | SOLI_WINDOW_STATE_PRINTING))
	{
		return TRUE;
	}

	_soli_cmd_file_quit (NULL, NULL, window);

	/* Do not destroy the window */
	return TRUE;
}

static SoliWindow *
soli_app_create_window_impl (SoliApp *app)
{
	SoliWindow *window;

	window = g_object_new (SOLI_TYPE_WINDOW, "application", app, NULL);

	soli_debug_message (DEBUG_APP, "Window created");

	g_signal_connect (window,
			  "delete_event",
			  G_CALLBACK (window_delete_event),
			  app);

	return window;
}

static void
soli_app_class_init (SoliAppClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

	object_class->dispose = soli_app_dispose;
	object_class->get_property = soli_app_get_property;

	app_class->startup = soli_app_startup;
	app_class->activate = soli_app_activate;
	app_class->command_line = soli_app_command_line;
	app_class->handle_local_options = soli_app_handle_local_options;
	app_class->open = soli_app_open;
	app_class->shutdown = soli_app_shutdown;

	klass->show_help = soli_app_show_help_impl;
	klass->help_link_id = soli_app_help_link_id_impl;
	klass->set_window_title = soli_app_set_window_title_impl;
	klass->create_window = soli_app_create_window_impl;

	properties[PROP_LOCKDOWN] =
		g_param_spec_flags ("lockdown",
		                    "Lockdown",
		                    "The lockdown mask",
		                    SOLI_TYPE_LOCKDOWN_MASK,
		                    0,
		                    G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, LAST_PROP, properties);
}

static void
load_page_setup (SoliApp *app)
{
	SoliAppPrivate *priv;
	gchar *filename;
	GError *error = NULL;

	priv = soli_app_get_instance_private (app);

	g_return_if_fail (priv->page_setup == NULL);

	filename = get_page_setup_file ();

	priv->page_setup = gtk_page_setup_new_from_file (filename, &error);
	if (error)
	{
		/* Ignore file not found error */
		if (error->domain != G_FILE_ERROR ||
		    error->code != G_FILE_ERROR_NOENT)
		{
			g_warning ("%s", error->message);
		}

		g_error_free (error);
	}

	g_free (filename);

	/* fall back to default settings */
	if (priv->page_setup == NULL)
	{
		priv->page_setup = gtk_page_setup_new ();
	}
}

static void
load_print_settings (SoliApp *app)
{
	SoliAppPrivate *priv;
	gchar *filename;
	GError *error = NULL;

	priv = soli_app_get_instance_private (app);

	g_return_if_fail (priv->print_settings == NULL);

	filename = get_print_settings_file ();

	priv->print_settings = gtk_print_settings_new_from_file (filename, &error);
	if (error != NULL)
	{
		/* - Ignore file not found error.
		 * - Ignore empty file error, i.e. group not found. This happens
		 *   when we click on cancel in the print dialog, when using the
		 *   printing for the first time in soli.
		 */
		if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT) &&
		    !g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_GROUP_NOT_FOUND))
		{
			g_warning ("Load print settings error: %s", error->message);
		}

		g_error_free (error);
	}

	g_free (filename);

	/* fall back to default settings */
	if (priv->print_settings == NULL)
	{
		priv->print_settings = gtk_print_settings_new ();
	}
}

static void
get_network_available (GNetworkMonitor *monitor,
		       gboolean         available,
		       SoliApp        *app)
{
	gboolean enable;
	GList *windows, *w;

	enable = g_network_monitor_get_network_available (monitor);

	windows = gtk_application_get_windows (GTK_APPLICATION (app));

	for (w = windows; w != NULL; w = w->next)
	{
		SoliWindow *window = SOLI_WINDOW (w->data);

		if (SOLI_IS_WINDOW (window))
		{
			GList *tabs, *t;

			tabs = _soli_window_get_all_tabs (window);

			for (t = tabs; t != NULL; t = t->next)
			{
				_soli_tab_set_network_available (SOLI_TAB (t->data),
					                          enable);
			}

			g_list_free (tabs);
		}
	}
}

static void
soli_app_init (SoliApp *app)
{
	SoliAppPrivate *priv;

	priv = soli_app_get_instance_private (app);

	g_set_application_name ("soli");
	gtk_window_set_default_icon_name ("soli");

	priv->monitor = g_network_monitor_get_default ();
	g_signal_connect (priv->monitor,
	                  "network-changed",
	                  G_CALLBACK (get_network_available),
	                  app);

	g_application_add_main_option_entries (G_APPLICATION (app), options);

#ifdef ENABLE_INTROSPECTION
	g_application_add_option_group (G_APPLICATION (app), g_irepository_get_option_group ());
#endif
}

/* Generates a unique string for a window role */
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
				g_get_host_name ());
}

/**
 * soli_app_create_window:
 * @app: the #SoliApp
 * @screen: (allow-none):
 *
 * Create a new #SoliWindow part of @app.
 *
 * Return value: (transfer none): the new #SoliWindow
 */
SoliWindow *
soli_app_create_window (SoliApp  *app,
			 GdkScreen *screen)
{
	SoliAppPrivate *priv;
	SoliWindow *window;
	gchar *role;
	GdkWindowState state;
	gint w, h;

	soli_debug (DEBUG_APP);

	priv = soli_app_get_instance_private (app);

	window = SOLI_APP_GET_CLASS (app)->create_window (app);

	if (screen != NULL)
	{
		gtk_window_set_screen (GTK_WINDOW (window), screen);
	}

	role = gen_role ();
	gtk_window_set_role (GTK_WINDOW (window), role);
	g_free (role);

	state = g_settings_get_int (priv->window_settings,
	                            SOLI_SETTINGS_WINDOW_STATE);

	g_settings_get (priv->window_settings,
	                SOLI_SETTINGS_WINDOW_SIZE,
	                "(ii)", &w, &h);

	gtk_window_set_default_size (GTK_WINDOW (window), w, h);

	if ((state & GDK_WINDOW_STATE_MAXIMIZED) != 0)
	{
		gtk_window_maximize (GTK_WINDOW (window));
	}
	else
	{
		gtk_window_unmaximize (GTK_WINDOW (window));
	}

	if ((state & GDK_WINDOW_STATE_STICKY ) != 0)
	{
		gtk_window_stick (GTK_WINDOW (window));
	}
	else
	{
		gtk_window_unstick (GTK_WINDOW (window));
	}

	return window;
}

/**
 * soli_app_get_main_windows:
 * @app: the #SoliApp
 *
 * Returns all #SoliWindows currently open in #SoliApp.
 * This differs from gtk_application_get_windows() since it does not
 * include the preferences dialog and other auxiliary windows.
 *
 * Return value: (element-type Soli.Window) (transfer container):
 * a newly allocated list of #SoliWindow objects
 */
GList *
soli_app_get_main_windows (SoliApp *app)
{
	GList *res = NULL;
	GList *windows, *l;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	windows = gtk_application_get_windows (GTK_APPLICATION (app));
	for (l = windows; l != NULL; l = g_list_next (l))
	{
		if (SOLI_IS_WINDOW (l->data))
		{
			res = g_list_prepend (res, l->data);
		}
	}

	return g_list_reverse (res);
}

/**
 * soli_app_get_documents:
 * @app: the #SoliApp
 *
 * Returns all the documents currently open in #SoliApp.
 *
 * Return value: (element-type Soli.Document) (transfer container):
 * a newly allocated list of #SoliDocument objects
 */
GList *
soli_app_get_documents	(SoliApp *app)
{
	GList *res = NULL;
	GList *windows, *l;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	windows = gtk_application_get_windows (GTK_APPLICATION (app));
	for (l = windows; l != NULL; l = g_list_next (l))
	{
		if (SOLI_IS_WINDOW (l->data))
		{
			res = g_list_concat (res,
			                     soli_window_get_documents (SOLI_WINDOW (l->data)));
		}
	}

	return res;
}

/**
 * soli_app_get_views:
 * @app: the #SoliApp
 *
 * Returns all the views currently present in #SoliApp.
 *
 * Return value: (element-type Soli.View) (transfer container):
 * a newly allocated list of #SoliView objects
 */
GList *
soli_app_get_views (SoliApp *app)
{
	GList *res = NULL;
	GList *windows, *l;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	windows = gtk_application_get_windows (GTK_APPLICATION (app));
	for (l = windows; l != NULL; l = g_list_next (l))
	{
		if (SOLI_IS_WINDOW (l->data))
		{
			res = g_list_concat (res,
			                     soli_window_get_views (SOLI_WINDOW (l->data)));
		}
	}

	return res;
}

/**
 * soli_app_get_lockdown:
 * @app: a #SoliApp
 *
 * Gets the lockdown mask (see #SoliLockdownMask) for the application.
 * The lockdown mask determines which functions are locked down using
 * the GNOME-wise lockdown GConf keys.
 **/
SoliLockdownMask
soli_app_get_lockdown (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), SOLI_LOCKDOWN_ALL);

	priv = soli_app_get_instance_private (app);

	return priv->lockdown;
}

gboolean
soli_app_show_help (SoliApp    *app,
                     GtkWindow   *parent,
                     const gchar *name,
                     const gchar *link_id)
{
	g_return_val_if_fail (SOLI_IS_APP (app), FALSE);
	g_return_val_if_fail (parent == NULL || GTK_IS_WINDOW (parent), FALSE);

	return SOLI_APP_GET_CLASS (app)->show_help (app, parent, name, link_id);
}

void
soli_app_set_window_title (SoliApp    *app,
                            SoliWindow *window,
                            const gchar *title)
{
	g_return_if_fail (SOLI_IS_APP (app));
	g_return_if_fail (SOLI_IS_WINDOW (window));

	SOLI_APP_GET_CLASS (app)->set_window_title (app, window, title);
}

gboolean
soli_app_process_window_event (SoliApp    *app,
                                SoliWindow *window,
                                GdkEvent    *event)
{
	g_return_val_if_fail (SOLI_IS_APP (app), FALSE);
	g_return_val_if_fail (SOLI_IS_WINDOW (window), FALSE);

	if (SOLI_APP_GET_CLASS (app)->process_window_event)
	{
		return SOLI_APP_GET_CLASS (app)->process_window_event (app, window, event);
	}

    return FALSE;
}

static GMenuModel *
find_extension_point_section (GMenuModel  *model,
                              const gchar *extension_point)
{
	gint i, n_items;
	GMenuModel *section = NULL;

	n_items = g_menu_model_get_n_items (model);

	for (i = 0; i < n_items && !section; i++)
	{
		gchar *id = NULL;

		if (g_menu_model_get_item_attribute (model, i, "id", "s", &id) &&
		    strcmp (id, extension_point) == 0)
		{
			section = g_menu_model_get_item_link (model, i, G_MENU_LINK_SECTION);
		}
		else
		{
			GMenuModel *subsection;
			GMenuModel *submenu;
			gint j, j_items;

			subsection = g_menu_model_get_item_link (model, i, G_MENU_LINK_SECTION);

			j_items = g_menu_model_get_n_items (subsection);

			for (j = 0; j < j_items && !section; j++)
			{
				submenu = g_menu_model_get_item_link (subsection, j, G_MENU_LINK_SUBMENU);
				if (submenu)
				{
					section = find_extension_point_section (submenu, extension_point);
				}
			}
		}

		g_free (id);
	}

	return section;
}

static void
app_lockdown_changed (SoliApp *app)
{
	SoliAppPrivate *priv;
	GList *windows, *l;

	priv = soli_app_get_instance_private (app);

	windows = gtk_application_get_windows (GTK_APPLICATION (app));
	for (l = windows; l != NULL; l = g_list_next (l))
	{
		if (SOLI_IS_WINDOW (l->data))
		{
			_soli_window_set_lockdown (SOLI_WINDOW (l->data),
			                            priv->lockdown);
		}
	}

	g_object_notify (G_OBJECT (app), "lockdown");
}

void
_soli_app_set_lockdown (SoliApp          *app,
			 SoliLockdownMask  lockdown)
{
	SoliAppPrivate *priv;

	g_return_if_fail (SOLI_IS_APP (app));

	priv = soli_app_get_instance_private (app);

	priv->lockdown = lockdown;
	app_lockdown_changed (app);
}

void
_soli_app_set_lockdown_bit (SoliApp          *app,
			     SoliLockdownMask  bit,
			     gboolean           value)
{
	SoliAppPrivate *priv;

	g_return_if_fail (SOLI_IS_APP (app));

	priv = soli_app_get_instance_private (app);

	if (value)
	{
		priv->lockdown |= bit;
	}
	else
	{
		priv->lockdown &= ~bit;
	}

	app_lockdown_changed (app);
}

/* Returns a copy */
GtkPageSetup *
_soli_app_get_default_page_setup (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	priv = soli_app_get_instance_private (app);

	if (priv->page_setup == NULL)
	{
		load_page_setup (app);
	}

	return gtk_page_setup_copy (priv->page_setup);
}

void
_soli_app_set_default_page_setup (SoliApp     *app,
				   GtkPageSetup *page_setup)
{
	SoliAppPrivate *priv;

	g_return_if_fail (SOLI_IS_APP (app));
	g_return_if_fail (GTK_IS_PAGE_SETUP (page_setup));

	priv = soli_app_get_instance_private (app);

	g_set_object (&priv->page_setup, page_setup);
}

/* Returns a copy */
GtkPrintSettings *
_soli_app_get_default_print_settings (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	priv = soli_app_get_instance_private (app);

	if (priv->print_settings == NULL)
	{
		load_print_settings (app);
	}

	return gtk_print_settings_copy (priv->print_settings);
}

void
_soli_app_set_default_print_settings (SoliApp         *app,
				       GtkPrintSettings *settings)
{
	SoliAppPrivate *priv;

	g_return_if_fail (SOLI_IS_APP (app));
	g_return_if_fail (GTK_IS_PRINT_SETTINGS (settings));

	priv = soli_app_get_instance_private (app);

	if (priv->print_settings != NULL)
	{
		g_object_unref (priv->print_settings);
	}

	priv->print_settings = g_object_ref (settings);
}

SoliSettings *
_soli_app_get_settings (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	priv = soli_app_get_instance_private (app);

	return priv->settings;
}

GMenuModel *
_soli_app_get_hamburger_menu (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	priv = soli_app_get_instance_private (app);

	return priv->hamburger_menu;
}

GMenuModel *
_soli_app_get_notebook_menu (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	priv = soli_app_get_instance_private (app);

	return priv->notebook_menu;
}

GMenuModel *
_soli_app_get_tab_width_menu (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	priv = soli_app_get_instance_private (app);

	return priv->tab_width_menu;
}

GMenuModel *
_soli_app_get_line_col_menu (SoliApp *app)
{
	SoliAppPrivate *priv;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);

	priv = soli_app_get_instance_private (app);

	return priv->line_col_menu;
}

SoliMenuExtension *
_soli_app_extend_menu (SoliApp    *app,
                        const gchar *extension_point)
{
	SoliAppPrivate *priv;
	GMenuModel *model;
	GMenuModel *section;

	g_return_val_if_fail (SOLI_IS_APP (app), NULL);
	g_return_val_if_fail (extension_point != NULL, NULL);

	priv = soli_app_get_instance_private (app);

	/* First look in the gear or window menu */
	if (priv->hamburger_menu)
	{
		model = priv->hamburger_menu;
	}
	else
	{
		model = gtk_application_get_menubar (GTK_APPLICATION (app));
	}

	section = find_extension_point_section (model, extension_point);

	/* otherwise look in the app menu */
	if (section == NULL)
	{
		model = gtk_application_get_app_menu (GTK_APPLICATION (app));

		if (model != NULL)
		{
			section = find_extension_point_section (model, extension_point);
		}
	}

	return section != NULL ? soli_menu_extension_new (G_MENU (section)) : NULL;
}

/* ex:set ts=8 noet: */
