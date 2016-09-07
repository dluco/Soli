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

#include "soli-window-activatable.h"
#include "soli-window.h"

G_DEFINE_INTERFACE(SoliWindowActivatable, soli_window_activatable, G_TYPE_OBJECT)

static void
soli_window_activatable_default_init (SoliWindowActivatableInterface *iface)
{
	g_object_interface_install_property (iface,
										g_param_spec_object ("window",
															"Window",
															"The soli window",
															SOLI_TYPE_WINDOW,
															G_PARAM_READWRITE |
															G_PARAM_CONSTRUCT_ONLY |
															G_PARAM_STATIC_STRINGS));
}

void
soli_window_activatable_activate (SoliWindowActivatable *activatable)
{
	SoliWindowActivatableInterface *iface;

	g_return_if_fail (SOLI_IS_WINDOW_ACTIVATABLE (activatable));

	iface = SOLI_WINDOW_ACTIVATABLE_GET_IFACE (activatable);

	if (iface->activate != NULL)
	{
		iface->activate (activatable);
	}
}

void
soli_window_activatable_deactivate (SoliWindowActivatable *activatable)
{
	SoliWindowActivatableInterface *iface;

	g_return_if_fail (SOLI_IS_WINDOW_ACTIVATABLE (activatable));

	iface = SOLI_WINDOW_ACTIVATABLE_GET_IFACE (activatable);

	if (iface->deactivate != NULL)
	{
		iface->deactivate (activatable);
	}
}

void
soli_window_activatable_update_state (SoliWindowActivatable *activatable)
{
	SoliWindowActivatableInterface *iface;

	g_return_if_fail (SOLI_IS_WINDOW_ACTIVATABLE (activatable));

	iface = SOLI_WINDOW_ACTIVATABLE_GET_IFACE (activatable);

	if (iface->update_state != NULL)
	{
		iface->update_state (activatable);
	}
}
