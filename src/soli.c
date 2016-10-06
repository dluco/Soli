/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
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

#include <gtk/gtk.h>
#include <girepository.h>

#include "soli-app-x11.h"
#include "soli-dirs.h"

int
main (int argc, char *argv[])
{
	SoliApp *app;
	GOptionContext *context;
	GError *error = NULL;
	gint status;

	context = g_option_context_new (NULL);
	g_option_context_add_group (context, g_irepository_get_option_group ());

	g_option_context_parse (context, &argc, &argv, &error);
	if (error != NULL)
	{
		g_print ("%s: %s\n", PACKAGE, error->message);
		g_error_free (error);
		error = NULL;
	}
	g_option_context_free (context);

	soli_dirs_init ();
	
	app = g_object_new (SOLI_TYPE_APP_X11,
						"application-id", "ca.dluco.soli",
						"flags", G_APPLICATION_HANDLES_COMMAND_LINE | G_APPLICATION_HANDLES_OPEN,
						NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_run_dispose (G_OBJECT (app));

	g_object_unref (app);

	return status;
}
