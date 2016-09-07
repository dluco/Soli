/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-app-activatable.c
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
, * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "soli-app-activatable.h"
#include "soli-app.h"

G_DEFINE_INTERFACE(SoliAppActivatable, soli_app_activatable, G_TYPE_OBJECT)

static void
soli_app_activatable_default_init (SoliAppActivatableInterface *iface)
{
	g_object_interface_install_property (iface,
										g_param_spec_object ("app",
															"App",
															"The soli app",
															SOLI_TYPE_APP,
															G_PARAM_READWRITE |
															G_PARAM_CONSTRUCT_ONLY |
															G_PARAM_STATIC_STRINGS));
}

void
soli_app_activatable_activate (SoliAppActivatable *activatable)
{
	SoliAppActivatableInterface *iface;

	g_return_if_fail (SOLI_IS_APP_ACTIVATABLE (activatable));

	iface = SOLI_APP_ACTIVATABLE_GET_IFACE (activatable);

	if (iface->activate != NULL)
	{
		iface->activate (activatable);
	}
}

void
soli_app_activatable_deactivate (SoliAppActivatable *activatable)
{
	SoliAppActivatableInterface *iface;

	g_return_if_fail (SOLI_IS_APP_ACTIVATABLE (activatable));

	iface = SOLI_APP_ACTIVATABLE_GET_IFACE (activatable);

	if (iface->deactivate != NULL)
	{
		iface->deactivate (activatable);
	}
}
