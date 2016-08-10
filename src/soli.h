/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli.h
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

#ifndef _SOLI_
#define _SOLI_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_APPLICATION             (soli_get_type ())
#define SOLI_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_APPLICATION, Soli))
#define SOLI_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_APPLICATION, SoliClass))
#define SOLI_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_APPLICATION))
#define SOLI_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_APPLICATION))
#define SOLI_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_APPLICATION, SoliClass))

typedef struct _SoliClass SoliClass;
typedef struct _Soli Soli;
typedef struct _SoliPrivate SoliPrivate;

struct _SoliClass
{
	GtkApplicationClass parent_class;
};

struct _Soli
{
	GtkApplication parent;

	SoliPrivate *priv;

};

GType soli_get_type (void) G_GNUC_CONST;
Soli *soli_new (void);

/* Callbacks */

G_END_DECLS

#endif /* _APPLICATION_H_ */

