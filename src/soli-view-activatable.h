/*
 * soli-view-activatable.h
 * This file is part of soli
 *
 * Copyright (C) 2010 - Steve Frécinaux
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLI_VIEW_ACTIVATABLE_H
#define SOLI_VIEW_ACTIVATABLE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define SOLI_TYPE_VIEW_ACTIVATABLE (soli_view_activatable_get_type ())

G_DECLARE_INTERFACE (SoliViewActivatable, soli_view_activatable, SOLI, VIEW_ACTIVATABLE, GObject)

struct _SoliViewActivatableInterface
{
	GTypeInterface g_iface;

	/* Virtual public methods */
	void	(*activate)		(SoliViewActivatable *activatable);
	void	(*deactivate)		(SoliViewActivatable *activatable);
};

void	 soli_view_activatable_activate	(SoliViewActivatable *activatable);
void	 soli_view_activatable_deactivate	(SoliViewActivatable *activatable);

G_END_DECLS

#endif /* SOLI_VIEW_ACTIVATABLE_H */
/* ex:set ts=8 noet: */
