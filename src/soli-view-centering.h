/*
 * soli-view-centering.h
 * This file is part of soli
 *
 * Copyright (C) 2014 - Sébastien Lafargue
 * Copyright (C) 2015 - Sébastien Wilmet
 *
 * Soli is free software; you can redistribute this file and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Soli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SOLI_VIEW_CENTERING_H
#define SOLI_VIEW_CENTERING_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_VIEW_CENTERING		(soli_view_centering_get_type())
#define SOLI_VIEW_CENTERING(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_VIEW_CENTERING, SoliViewCentering))
#define SOLI_VIEW_CENTERING_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_VIEW_CENTERING, SoliViewCentering const))
#define SOLI_VIEW_CENTERING_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_VIEW_CENTERING, SoliViewCenteringClass))
#define SOLI_IS_VIEW_CENTERING(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_VIEW_CENTERING))
#define SOLI_IS_VIEW_CENTERING_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_VIEW_CENTERING))
#define SOLI_VIEW_CENTERING_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_VIEW_CENTERING, SoliViewCenteringClass))

typedef struct _SoliViewCentering		SoliViewCentering;
typedef struct _SoliViewCenteringClass		SoliViewCenteringClass;
typedef struct _SoliViewCenteringPrivate	SoliViewCenteringPrivate;

struct _SoliViewCentering
{
	GtkBin parent;

	SoliViewCenteringPrivate *priv;
};

struct _SoliViewCenteringClass
{
	GtkBinClass parent_class;
};

GType			soli_view_centering_get_type			(void) G_GNUC_CONST;

SoliViewCentering *	soli_view_centering_new			(void);

void			soli_view_centering_set_centered		(SoliViewCentering *container,
									 gboolean            centered);

gboolean		soli_view_centering_get_centered		(SoliViewCentering *container);

G_END_DECLS

#endif /* SOLI_VIEW_CENTERING_H */

/* ex:set ts=8 noet: */
