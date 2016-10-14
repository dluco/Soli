/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-plugins-engine.c
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

#include "soli-plugins-engine.h"

#include <girepository.h>
#include "soli-dirs.h"
#include "soli-settings.h"

struct _SoliPluginsEngine
{
	PeasEngine parent_instance;

	GSettings *plugin_settings;
};

G_DEFINE_TYPE (SoliPluginsEngine, soli_plugins_engine, PEAS_TYPE_ENGINE)

static SoliPluginsEngine *default_engine = NULL;

static void
soli_plugins_engine_init (SoliPluginsEngine *engine)
{
	gchar *typelib_dir;
	GError *error = NULL;

	peas_engine_enable_loader (PEAS_ENGINE (engine), "python3");

	engine->plugin_settings = g_settings_new ("ca.dluco.soli.plugins");

	/* Require soli's typelib. */
	typelib_dir = g_build_filename (soli_dirs_get_soli_lib_dir (),
									"girepository-1.0",
									NULL);

	if (!g_irepository_require_private (g_irepository_get_default (),
										typelib_dir, "Soli", "1.0", 0, &error))
	{
		g_warning ("Could not load Soli repository: %s", error->message);
		g_error_free (error);
		error = NULL;
	}

	g_free (typelib_dir);

	/* This should be moved to libpeas */
	if (!g_irepository_require (g_irepository_get_default (),
								"Peas", "1.0", 0, &error))
	{
		g_warning ("Could not load Peas repository: %s", error->message);
		g_error_free (error);
		error = NULL;
	}

	if (!g_irepository_require (g_irepository_get_default (),
								"PeasGtk", "1.0", 0, &error))
	{
		g_warning ("Could not load PeasGtk repository: %s", error->message);
		g_error_free (error);
		error = NULL;
	}

	peas_engine_add_search_path (PEAS_ENGINE (engine),
								soli_dirs_get_user_plugins_dir (),
								soli_dirs_get_user_plugins_dir ());

	peas_engine_add_search_path (PEAS_ENGINE (engine),
								soli_dirs_get_soli_plugins_dir (),
								soli_dirs_get_soli_plugins_data_dir ());

	g_settings_bind (engine->plugin_settings,
					SOLI_SETTINGS_ACTIVE_PLUGINS,
					engine,
					"loaded-plugins",
					G_SETTINGS_BIND_DEFAULT);
}

static void
soli_plugins_engine_dispose (GObject *object)
{
	SoliPluginsEngine *engine = SOLI_PLUGINS_ENGINE (object);

	g_clear_object (&engine->plugin_settings);

	G_OBJECT_CLASS (soli_plugins_engine_parent_class)->dispose (object);
}

static void
soli_plugins_engine_class_init (SoliPluginsEngineClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = soli_plugins_engine_dispose;
}

SoliPluginsEngine *
soli_plugins_engine_get_default (void)
{
	if (default_engine == NULL)
	{
		default_engine = SOLI_PLUGINS_ENGINE (g_object_new (SOLI_TYPE_PLUGINS_ENGINE,
															NULL));

		g_object_add_weak_pointer (G_OBJECT (default_engine),
									(gpointer) &default_engine);
	}

	return default_engine;
}
