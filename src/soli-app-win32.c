/*
 * soli-app-win32.c
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

#include "soli-app-win32.h"

#define SAVE_DATADIR DATADIR
#undef DATADIR
#include <io.h>
#include <conio.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include <windows.h>
#define DATADIR SAVE_DATADIR
#undef SAVE_DATADIR

struct _SoliAppWin32
{
	SoliApp parent_instance;
};

G_DEFINE_TYPE (SoliAppWin32, soli_app_win32, SOLI_TYPE_APP)

static void
soli_app_win32_finalize (GObject *object)
{
	G_OBJECT_CLASS (soli_app_win32_parent_class)->finalize (object);
}

static gchar *
soli_app_win32_help_link_id_impl (SoliApp    *app,
                                   const gchar *name,
                                   const gchar *link_id)
{
	if (link_id)
	{
		return g_strdup_printf ("http://library.gnome.org/users/soli/stable/%s",
		                        link_id);
	}
	else
	{
		return g_strdup ("http://library.gnome.org/users/soli/stable/");
	}
}

static void
setup_path (void)
{
	gchar *path;
	gchar *installdir;
	gchar *bin;

	installdir = g_win32_get_package_installation_directory_of_module (NULL);

	bin = g_build_filename (installdir, "bin", NULL);
	g_free (installdir);

	/* Set PATH to include the soli executable's folder */
	path = g_build_path (";", bin, g_getenv ("PATH"), NULL);
	g_free (bin);

	if (!g_setenv ("PATH", path, TRUE))
	{
		g_warning ("Could not set PATH for soli");
	}

	g_free (path);
}

static void
prep_console (void)
{
	/* If we open soli from a console get the stdout printing */
	if (fileno (stdout) != -1 &&
		_get_osfhandle (fileno (stdout)) != -1)
	{
		/* stdout is fine, presumably redirected to a file or pipe */
	}
	else
	{
		typedef BOOL (* WINAPI AttachConsole_t) (DWORD);

		AttachConsole_t p_AttachConsole =
			(AttachConsole_t) GetProcAddress (GetModuleHandle ("kernel32.dll"),
							  "AttachConsole");

		if (p_AttachConsole != NULL && p_AttachConsole (ATTACH_PARENT_PROCESS))
		{
			freopen ("CONOUT$", "w", stdout);
			dup2 (fileno (stdout), 1);
			freopen ("CONOUT$", "w", stderr);
			dup2 (fileno (stderr), 2);
		}
	}
}

static void
soli_app_win32_startup (GApplication *application)
{
	G_APPLICATION_CLASS (soli_app_win32_parent_class)->startup (application);

	setup_path ();
	prep_console ();
}

static void
soli_app_win32_class_init (SoliAppWin32Class *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GApplicationClass *gapp_class = G_APPLICATION_CLASS (klass);
	SoliAppClass *app_class = SOLI_APP_CLASS (klass);

	object_class->finalize = soli_app_win32_finalize;

	gapp_class->startup = soli_app_win32_startup;

	app_class->help_link_id = soli_app_win32_help_link_id_impl;
}

static void
soli_app_win32_init (SoliAppWin32 *self)
{
}

/* ex:set ts=8 noet: */
