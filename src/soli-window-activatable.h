/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-window-activatable.h
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 *
 * soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SOLI_WINDOW_ACTIVATABLE_
#define _SOLI_WINDOW_ACTIVATABLE_

#include <glib-object.h>

G_BEGIN_DECLS

#define SOLI_TYPE_WINDOW_ACTIVATABLE (soli_window_activatable_get_type ())

G_DECLARE_INTERFACE (SoliWindowActivatable, soli_window_activatable, SOLI, WINDOW_ACTIVATABLE, GObject)

struct _SoliWindowActivatableInterface
{
	GTypeInterface g_iface;
	
	/* Virtual public methods */
	void (*activate) (SoliWindowActivatable *activatable);
	void (*deactivate) (SoliWindowActivatable *activatable);
	void (*update_state) (SoliWindowActivatable *activatable);
};

void soli_window_activatable_activate (SoliWindowActivatable *activatable);

void soli_window_activatable_deactivate (SoliWindowActivatable *activatable);

void soli_window_activatable_update_state (SoliWindowActivatable *activatable);

G_END_DECLS

#endif /* _SOLI_WINDOW_ACTIVATABLE_ */

