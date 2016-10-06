/*
 * soli-view-activatable.h
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

#include "soli-view-activatable.h"

#include "soli-view.h"

/**
 * SECTION:soli-view-activatable
 * @short_description: Interface for activatable extensions on views
 * @see_also: #PeasExtensionSet
 *
 * #SoliViewActivatable is an interface which should be implemented by
 * extensions that should be activated on a soli view.
 **/

G_DEFINE_INTERFACE(SoliViewActivatable, soli_view_activatable, G_TYPE_OBJECT)

static void
soli_view_activatable_default_init (SoliViewActivatableInterface *iface)
{
	/**
	 * SoliViewActivatable:view:
	 *
	 * The window property contains the soli window for this
	 * #SoliViewActivatable instance.
	 */
	g_object_interface_install_property (iface,
	                                     g_param_spec_object ("view",
	                                                          "view",
	                                                          "A soli view",
	                                                          SOLI_TYPE_VIEW,
	                                                          G_PARAM_READWRITE |
	                                                          G_PARAM_CONSTRUCT_ONLY |
	                                                          G_PARAM_STATIC_STRINGS));
}

/**
 * soli_view_activatable_activate:
 * @activatable: A #SoliViewActivatable.
 *
 * Activates the extension on the window property.
 */
void
soli_view_activatable_activate (SoliViewActivatable *activatable)
{
	SoliViewActivatableInterface *iface;

	g_return_if_fail (SOLI_IS_VIEW_ACTIVATABLE (activatable));

	iface = SOLI_VIEW_ACTIVATABLE_GET_IFACE (activatable);
	if (iface->activate != NULL)
	{
		iface->activate (activatable);
	}
}

/**
 * soli_view_activatable_deactivate:
 * @activatable: A #SoliViewActivatable.
 *
 * Deactivates the extension on the window property.
 */
void
soli_view_activatable_deactivate (SoliViewActivatable *activatable)
{
	SoliViewActivatableInterface *iface;

	g_return_if_fail (SOLI_IS_VIEW_ACTIVATABLE (activatable));

	iface = SOLI_VIEW_ACTIVATABLE_GET_IFACE (activatable);
	if (iface->deactivate != NULL)
	{
		iface->deactivate (activatable);
	}
}

