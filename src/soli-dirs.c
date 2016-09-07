/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-dirs.c
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

#include "soli-dirs.h"

static gchar *user_config_dir       = NULL;
static gchar *user_cache_dir        = NULL;
static gchar *user_styles_dir       = NULL;
static gchar *user_plugins_dir      = NULL;
static gchar *soli_locale_dir       = NULL;
static gchar *soli_lib_dir          = NULL;
static gchar *soli_plugins_dir      = NULL;
static gchar *soli_plugins_data_dir = NULL;

void
soli_dirs_init (void)
{
	soli_locale_dir = g_build_filename (DATADIR,
										"locale",
										NULL);				
	soli_lib_dir = g_build_filename (LIBDIR,
									"soli",
									NULL);
	soli_plugins_data_dir = g_build_filename (DATADIR,
											"soli",
											"plugins",
											NULL);
											
	user_cache_dir = g_build_filename (g_get_user_cache_dir (),
									   "soli",
									   NULL);
	user_config_dir = g_build_filename (g_get_user_config_dir (),
										"soli",
										NULL);
	user_styles_dir = g_build_filename (g_get_user_data_dir (),
										"soli",
										"styles",
										NULL);
	user_plugins_dir = g_build_filename (g_get_user_data_dir (),
										 "soli",
										 "plugins",
										 NULL);
	soli_plugins_dir = g_build_filename (soli_lib_dir,
					      				"plugins",
					      				NULL);
}

void
soli_dirs_shutdown ()
{
	g_free (user_config_dir);
	g_free (user_cache_dir);
	g_free (user_styles_dir);
	g_free (user_plugins_dir);
	g_free (soli_locale_dir);
	g_free (soli_lib_dir);
	g_free (soli_plugins_dir);
	g_free (soli_plugins_data_dir);
}

const gchar *
soli_dirs_get_user_config_dir (void)
{
	return user_config_dir;
}

const gchar *
soli_dirs_get_user_cache_dir (void)
{
	return user_cache_dir;
}

const gchar *
soli_dirs_get_user_styles_dir (void)
{
	return user_styles_dir;
}

const gchar *
soli_dirs_get_user_plugins_dir (void)
{
	return user_plugins_dir;
}

const gchar *
soli_dirs_get_soli_locale_dir (void)
{
	return soli_locale_dir;
}

const gchar *
soli_dirs_get_soli_lib_dir (void)
{
	return soli_lib_dir;
}

const gchar *
soli_dirs_get_soli_plugins_dir (void)
{
	return soli_plugins_dir;
}

const gchar *
soli_dirs_get_soli_plugins_data_dir (void)
{
	return soli_plugins_data_dir;
}
