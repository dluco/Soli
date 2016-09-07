/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-app-activatable.h
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

#ifndef _SOLI_APP_ACTIVATABLE_
#define _SOLI_APP_ACTIVATABLE_

#include <glib-object.h>

G_BEGIN_DECLS

#define SOLI_TYPE_APP_ACTIVATABLE (soli_app_activatable_get_type ())

G_DECLARE_INTERFACE (SoliAppActivatable, soli_app_activatable, SOLI, APP_ACTIVATABLE, GObject)

struct _SoliAppActivatableInterface
{
	GTypeInterface g_iface;

	/* Virtual public methods */
	void (*activate) (SoliAppActivatable *activatable);
	void (*deactivate) (SoliAppActivatable *activatable);
};

void soli_app_activatable_activate (SoliAppActivatable *activatable);
void soli_app_activatable_deactivate (SoliAppActivatable *activatable);

G_END_DECLS

#endif /* _SOLI_APP_ACTIVATABLE_ */
