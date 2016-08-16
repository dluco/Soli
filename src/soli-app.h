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

G_BEGIN_DECLS

#define SOLI_TYPE_APP             (soli_app_get_type ())
#define SOLI_APP(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_APP, SoliApp))

typedef struct _SoliApp SoliApp;
typedef struct _SoliAppClass SoliAppClass;

struct _SoliAppClass
{
	GtkApplicationClass parent_class;
};

struct _SoliApp
{
	GtkApplication parent;
};

GType soli_app_get_type (void) G_GNUC_CONST;
SoliApp *soli_app_new (void);

/* Callbacks */

G_END_DECLS

#endif /* _SOLI_APP_ */

