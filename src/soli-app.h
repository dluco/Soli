/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-app.h
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

#ifndef _SOLI_APP_
#define _SOLI_APP_

#include <gtk/gtk.h>
#include "soli-window.h"

G_BEGIN_DECLS

#define SOLI_TYPE_APP             (soli_app_get_type ())

G_DECLARE_DERIVABLE_TYPE (SoliApp, soli_app, SOLI, APP, GtkApplication)

struct _SoliAppClass
{
	GtkApplicationClass parent_class;

	SoliWindow *(*create_window)	(SoliApp *app);
};

SoliApp *soli_app_new (void);

SoliWindow *soli_app_get_active_window (SoliApp *app);

SoliWindow *soli_app_create_window (SoliApp *app, GdkScreen *screen);

/* Callbacks */

G_END_DECLS

#endif /* _SOLI_APP_ */

