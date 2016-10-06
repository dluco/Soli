/*
 * soli-window-activatable.h
 * This file is part of soli
 *
 * Copyright (C) 2010 Steve Frécinaux
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "soli-window-activatable.h"

#include <string.h>

#include "soli-window.h"

/**
 * SECTION:soli-window-activatable
 * @short_description: Interface for activatable extensions on windows
 * @see_also: #PeasExtensionSet
 *
 * #SoliWindowActivatable is an interface which should be implemented by
 * extensions that should be activated on a soli main window.
 **/

G_DEFINE_INTERFACE(SoliWindowActivatable, soli_window_activatable, G_TYPE_OBJECT)

static void
soli_window_activatable_default_init (SoliWindowActivatableInterface *iface)
{
	/**
	 * SoliWindowActivatable:window:
	 *
	 * The window property contains the soli window for this
	 * #SoliWindowActivatable instance.
	 */
	g_object_interface_install_property (iface,
	                                     g_param_spec_object ("window",
	                                                          "Window",
	                                                          "The soli window",
	                                                          SOLI_TYPE_WINDOW,
	                                                          G_PARAM_READWRITE |
	                                                          G_PARAM_CONSTRUCT_ONLY |
	                                                          G_PARAM_STATIC_STRINGS));
}

/**
 * soli_window_activatable_activate:
 * @activatable: A #SoliWindowActivatable.
 *
 * Activates the extension on the window property.
 */
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

/**
 * soli_window_activatable_deactivate:
 * @activatable: A #SoliWindowActivatable.
 *
 * Deactivates the extension on the window property.
 */
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

/**
 * soli_window_activatable_update_state:
 * @activatable: A #SoliWindowActivatable.
 *
 * Triggers an update of the extension internal state to take into account
 * state changes in the window, due to some event or user action.
 */
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

/* ex:set ts=8 noet: */
