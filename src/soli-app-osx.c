/*
 * soli-app-osx.c
 * This file is part of soli
 *
 * Copyright (C) 2010 - Jesse van den Kieboom
 *
 * soli is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * soli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with soli; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "soli-app-osx.h"

#include <gdk/gdkquartz.h>
#include <string.h>
#include <glib/gi18n.h>

#include "soli-app-private.h"
#include "soli-dirs.h"
#include "soli-debug.h"
#include "soli-commands.h"
#include "soli-commands-private.h"
#include "soli-recent.h"

static SoliWindow *
ensure_window (SoliAppOSX *app,
               gboolean     with_empty_document)
{
	GList *windows;
	SoliWindow *ret = NULL;

	windows = gtk_application_get_windows (GTK_APPLICATION (app));

	while (windows)
	{
		GtkWindow *window;
		GdkWindow *win;
		NSWindow *nswin;

		window = windows->data;
		windows = g_list_next (windows);

		if (!gtk_widget_get_realized (GTK_WIDGET (window)))
		{
			continue;
		}

		if (!SOLI_IS_WINDOW (window))
		{
			continue;
		}

		win = gtk_widget_get_window (GTK_WIDGET (window));
		nswin = gdk_quartz_window_get_nswindow (win);

		if ([nswin isOnActiveSpace])
		{
			ret = SOLI_WINDOW (window);
			break;
		}
	}

	if (!ret)
	{
		ret = soli_app_create_window (SOLI_APP (app), NULL);
		gtk_widget_show (GTK_WIDGET (ret));
	}

	if (with_empty_document && soli_window_get_active_document (ret) == NULL)
	{
		soli_window_create_tab (ret, TRUE);
	}

	gtk_window_present (GTK_WINDOW (ret));
	return ret;
}

@interface SoliAppOSXDelegate : NSObject
{
	SoliAppOSX *app;
	id<NSApplicationDelegate> orig;
}

- (id)initWithApp:(SoliAppOSX *)theApp;
- (void)release;

- (id)forwardingTargetForSelector:(SEL)aSelector;
- (BOOL)respondsToSelector:(SEL)aSelector;

- (BOOL)applicationShouldHandleReopen:(NSApplication *)theApplication hasVisibleWindows:(BOOL)flag;
- (void)applicationWillBecomeActive:(NSNotification *)aNotification;
- (void)application:(NSApplication *)sender openFiles:(NSArray *)filenames;

@end

@implementation SoliAppOSXDelegate
- (id)initWithApp:(SoliAppOSX *)theApp
{
	[super init];
	app = theApp;

	orig = [NSApp delegate];
	[NSApp setDelegate:self];

	return self;
}

- (void)release
{
	[NSApp setDelegate:orig];
	[super release];
}

- (id)forwardingTargetForSelector:(SEL)aSelector
{
	return orig;
}

- (BOOL)respondsToSelector:(SEL)aSelector
{
    return [super respondsToSelector:aSelector] || [orig respondsToSelector:aSelector];
}

- (BOOL)applicationShouldHandleReopen:(NSApplication *)theApplication hasVisibleWindows:(BOOL)flag
{
	ensure_window (app, TRUE);
	return NO;
}

- (void)applicationWillBecomeActive:(NSNotification *)aNotification
{
	ensure_window (app, TRUE);
}

- (void)application:(NSApplication *)sender openFiles:(NSArray *)filenames
{
	ensure_window (app, FALSE);
	[orig application:sender openFiles:filenames];
}

@end

struct _SoliAppOSX
{
	SoliApp parent_instance;

	SoliMenuExtension *recent_files_menu;
	gulong recent_manager_changed_id;

	SoliAppOSXDelegate *app_delegate;

	GList *recent_actions;
	SoliRecentConfiguration recent_config;
};

G_DEFINE_TYPE (SoliAppOSX, soli_app_osx, SOLI_TYPE_APP)

static void
remove_recent_actions (SoliAppOSX *app)
{
	while (app->recent_actions)
	{
		gchar *action_name = app->recent_actions->data;

		g_action_map_remove_action (G_ACTION_MAP (app), action_name);
		g_free (action_name);

		app->recent_actions = g_list_delete_link (app->recent_actions,
		                                          app->recent_actions);
	}
}

static void
soli_app_osx_finalize (GObject *object)
{
	SoliAppOSX *app = SOLI_APP_OSX (object);

	g_object_unref (app->recent_files_menu);

	remove_recent_actions (app);

	g_signal_handler_disconnect (app->recent_config.manager,
	                             app->recent_manager_changed_id);

	soli_recent_configuration_destroy (&app->recent_config);

	[app->app_delegate release];

	G_OBJECT_CLASS (soli_app_osx_parent_class)->finalize (object);
}

gboolean
soli_app_osx_show_url (SoliAppOSX *app,
                        const gchar *url)
{
	return [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}

static gboolean
soli_app_osx_show_help_impl (SoliApp    *app,
                              GtkWindow   *parent,
                              const gchar *name,
                              const gchar *link_id)
{
	gboolean ret = FALSE;

	if (name == NULL || g_strcmp0 (name, "soli.xml") == 0 || g_strcmp0 (name, "soli") == 0)
	{
		gchar *link;

		if (link_id)
		{
			link = g_strdup_printf ("http://library.gnome.org/users/soli/stable/%s",
						link_id);
		}
		else
		{
			link = g_strdup ("http://library.gnome.org/users/soli/stable/");
		}

		ret = soli_app_osx_show_url (SOLI_APP_OSX (app), link);
		g_free (link);
	}

	return ret;
}

static void
soli_app_osx_set_window_title_impl (SoliApp    *app,
                                     SoliWindow *window,
                                     const gchar *title)
{
	NSWindow *native;
	SoliDocument *document;
	GdkWindow *wnd;

	g_return_if_fail (SOLI_IS_WINDOW (window));

	wnd = gtk_widget_get_window (GTK_WIDGET (window));

	if (wnd == NULL)
	{
		return;
	}

	native = gdk_quartz_window_get_nswindow (wnd);
	document = soli_window_get_active_document (window);

	if (document)
	{
		bool ismodified;

		if (soli_document_is_untitled (document))
		{
			[native setRepresentedURL:nil];
		}
		else
		{
			GtkSourceFile *file;
			GFile *location;
			gchar *uri;

			file = soli_document_get_file (document);
			location = gtk_source_file_get_location (file);

			uri = g_file_get_uri (location);

			NSURL *nsurl = [NSURL URLWithString:[NSString stringWithUTF8String:uri]];

			[native setRepresentedURL:nsurl];
			g_free (uri);
		}

		ismodified = !soli_document_is_untouched (document);
		[native setDocumentEdited:ismodified];
	}
	else
	{
		[native setRepresentedURL:nil];
		[native setDocumentEdited:false];
	}

	SOLI_APP_CLASS (soli_app_osx_parent_class)->set_window_title (app, window, title);
}

typedef struct
{
	SoliAppOSX   *app;
	GtkRecentInfo *info;
} RecentFileInfo;

static void
recent_file_info_free (gpointer  data,
                       GClosure *closure)
{
	RecentFileInfo *info = data;

	g_object_unref (info->app);
	gtk_recent_info_unref (info->info);

	g_slice_free (RecentFileInfo, data);
}

static void
recent_file_activated (GAction        *action,
                       GVariant       *parameter,
                       RecentFileInfo *info)
{
	SoliWindow *window;
	const gchar *uri;
	GFile *file;

	uri = gtk_recent_info_get_uri (info->info);
	file = g_file_new_for_uri (uri);

	window = ensure_window (info->app, FALSE);

	soli_commands_load_location (SOLI_WINDOW (window), file, NULL, 0, 0);
	g_object_unref (file);
}

static void
recent_files_menu_populate (SoliAppOSX *app)
{
	GList *items;
	gint i = 0;

	soli_menu_extension_remove_items (app->recent_files_menu);
	remove_recent_actions (app);

	items = soli_recent_get_items (&app->recent_config);

	while (items)
	{
		GtkRecentInfo *info = items->data;
		GMenuItem *mitem;
		const gchar *name;
		gchar *acname;
		gchar *acfullname;
		GSimpleAction *action;
		RecentFileInfo *finfo;

		name = gtk_recent_info_get_display_name (info);

		acname = g_strdup_printf ("recent-file-action-%d", ++i);
		action = g_simple_action_new (acname, NULL);

		finfo = g_slice_new (RecentFileInfo);
		finfo->app = g_object_ref (app);
		finfo->info = gtk_recent_info_ref (info);

		g_signal_connect_data (action,
		                       "activate",
		                       G_CALLBACK (recent_file_activated),
		                       finfo,
		                       recent_file_info_free,
		                       0);

		g_action_map_add_action (G_ACTION_MAP (app), G_ACTION (action));
		g_object_unref (action);

		acfullname = g_strdup_printf ("app.%s", acname);

		app->recent_actions = g_list_prepend (app->recent_actions, acname);

		mitem = g_menu_item_new (name, acfullname);
		soli_menu_extension_append_menu_item (app->recent_files_menu, mitem);

		g_free (acfullname);

		g_object_unref (mitem);
		gtk_recent_info_unref (info);

		items = g_list_delete_link (items, items);
	}
}

static void
recent_manager_changed (GtkRecentManager *manager,
                        SoliAppOSX      *app)
{
	recent_files_menu_populate (app);
}

static void
open_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       userdata)
{
	_soli_cmd_file_open (NULL, NULL, NULL);
}

static GActionEntry app_entries[] = {
	{ "open", open_activated, NULL, NULL, NULL }
};

static void
update_open_sensitivity (SoliAppOSX *app)
{
	GAction *action;
	gboolean has_windows;

	has_windows = (gtk_application_get_windows (GTK_APPLICATION (app)) != NULL);

	action = g_action_map_lookup_action (G_ACTION_MAP (app), "open");
	g_simple_action_set_enabled (G_SIMPLE_ACTION (action), !has_windows);
}

static void
soli_app_osx_startup (GApplication *application)
{
	const gchar *replace_accels[] = {
		"<Primary><Alt>F",
		NULL
	};

	const gchar *open_accels[] = {
		"<Primary>O",
		NULL
	};

	const gchar *fullscreen_accels[] = {
		"<Primary><Control>F",
		NULL
	};

	SoliAppOSX *app = SOLI_APP_OSX (application);

	G_APPLICATION_CLASS (soli_app_osx_parent_class)->startup (application);

	app->app_delegate = [[[SoliAppOSXDelegate alloc] initWithApp:app] retain];

	g_action_map_add_action_entries (G_ACTION_MAP (application),
	                                 app_entries,
	                                 G_N_ELEMENTS (app_entries),
	                                 application);

	gtk_application_set_accels_for_action (GTK_APPLICATION (application),
	                                       "win.replace",
	                                       replace_accels);

	gtk_application_set_accels_for_action (GTK_APPLICATION (application),
	                                       "app.open",
	                                       open_accels);

	gtk_application_set_accels_for_action (GTK_APPLICATION (application),
	                                       "win.fullscreen",
	                                       fullscreen_accels);

	soli_recent_configuration_init_default (&app->recent_config);

	app->recent_files_menu = _soli_app_extend_menu (SOLI_APP (application),
	                                                 "recent-files-section");

	app->recent_manager_changed_id = g_signal_connect (app->recent_config.manager,
	                                                   "changed",
	                                                   G_CALLBACK (recent_manager_changed),
	                                                   app);

	recent_files_menu_populate (app);

	g_application_hold (application);
	update_open_sensitivity (app);
}

static void
set_window_allow_fullscreen (SoliWindow *window)
{
	GdkWindow *wnd;
	NSWindow *native;

	wnd = gtk_widget_get_window (GTK_WIDGET (window));

	if (wnd != NULL)
	{
		native = gdk_quartz_window_get_nswindow (wnd);
		[native setCollectionBehavior: [native collectionBehavior] | NSWindowCollectionBehaviorFullScreenPrimary];
	}
}

static void
on_window_realized (GtkWidget *widget)
{
	set_window_allow_fullscreen (SOLI_WINDOW (widget));
}

static SoliWindow *
soli_app_osx_create_window_impl (SoliApp *app)
{
	SoliWindow *window;

	window = SOLI_APP_CLASS (soli_app_osx_parent_class)->create_window (app);

	gtk_window_set_titlebar (GTK_WINDOW (window), NULL);

	if (gtk_widget_get_realized (GTK_WIDGET (window)))
	{
		set_window_allow_fullscreen (window);
	}
	else
	{
		g_signal_connect (window, "realize", G_CALLBACK (on_window_realized), NULL);
	}

	return window;
}

static gboolean
soli_app_osx_process_window_event_impl (SoliApp    *app,
                                         SoliWindow *window,
                                         GdkEvent    *event)
{
	NSEvent *nsevent;

	/* For OS X we will propagate the event to NSApp, which handles some OS X
	* specific keybindings and the accelerators for the menu
	*/
	nsevent = gdk_quartz_event_get_nsevent (event);
	[NSApp sendEvent:nsevent];

	/* It does not really matter what we return here since it's the last thing
	* in the chain. Also we can't get from sendEvent whether the event was
	* actually handled by NSApp anyway
	*/
	return TRUE;
}

static void
soli_app_osx_constructed (GObject *object)
{
	/* FIXME: should we do this on all platforms? */
	g_object_set (object, "register-session", TRUE, NULL);
	G_OBJECT_CLASS (soli_app_osx_parent_class)->constructed (object);
}

static void
soli_app_osx_window_added (GtkApplication *application,
                            GtkWindow      *window)
{
	GTK_APPLICATION_CLASS (soli_app_osx_parent_class)->window_added (application, window);

	update_open_sensitivity (SOLI_APP_OSX (application));
}

static void
soli_app_osx_window_removed (GtkApplication *application,
                              GtkWindow      *window)
{
	GTK_APPLICATION_CLASS (soli_app_osx_parent_class)->window_removed (application, window);

	update_open_sensitivity (SOLI_APP_OSX (application));
}

static void
soli_app_osx_class_init (SoliAppOSXClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	SoliAppClass *app_class = SOLI_APP_CLASS (klass);
	GApplicationClass *application_class = G_APPLICATION_CLASS (klass);
	GtkApplicationClass *gtkapplication_class = GTK_APPLICATION_CLASS (klass);

	object_class->finalize = soli_app_osx_finalize;
	object_class->constructed = soli_app_osx_constructed;

	application_class->startup = soli_app_osx_startup;

	gtkapplication_class->window_added = soli_app_osx_window_added;
	gtkapplication_class->window_removed = soli_app_osx_window_removed;

	app_class->show_help = soli_app_osx_show_help_impl;
	app_class->set_window_title = soli_app_osx_set_window_title_impl;
	app_class->create_window = soli_app_osx_create_window_impl;
	app_class->process_window_event = soli_app_osx_process_window_event_impl;
}

static void
soli_app_osx_init (SoliAppOSX *app)
{
	/* This is required so that Cocoa is not going to parse the
	   command line arguments by itself and generate OpenFile events.
	   We already parse the command line ourselves, so this is needed
	   to prevent opening files twice, etc. */
	[[NSUserDefaults standardUserDefaults] setObject:@"NO"
	                                       forKey:@"NSTreatUnknownArgumentsAsOpen"];
}

/* ex:set ts=8 noet: */
