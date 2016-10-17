/*
 * soli.c
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Maggi
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
#include "soli-app-x11.h"

#include <glib.h>
#include <locale.h>

#include "soli-dirs.h"
#include "soli-debug.h"

int
main (int argc, char *argv[])
{
	SoliApp *app;
	gint status;
	const gchar *dir;


	/* NOTE: we should not make any calls to the soli api before the
	 * private library is loaded */
	soli_dirs_init ();

	/* Setup locale */
	setlocale (LC_ALL, "");

	dir = soli_dirs_get_soli_locale_dir ();

	app = g_object_new (SOLI_TYPE_APP_X11,
	                    "application-id", "ca.dluco.soli",
	                    "flags", G_APPLICATION_HANDLES_COMMAND_LINE | G_APPLICATION_HANDLES_OPEN,
	                    NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);

	/* Break reference cycles caused by the PeasExtensionSet
	 * for soliAppActivatable which holds a ref on the soliApp
	 */
	g_object_run_dispose (G_OBJECT (app));

	g_object_add_weak_pointer (G_OBJECT (app), (gpointer *) &app);
	g_object_unref (app);

	if (app != NULL)
	{
		soli_debug_message (DEBUG_APP, "Leaking with %i refs",
		                     G_OBJECT (app)->ref_count);
	}

	return status;
}

/* ex:set ts=8 noet: */
