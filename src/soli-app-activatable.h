/*
 * soli-app-activatable.h
 * This file is part of soli
 *
 * Copyright (C) 2010 - Steve Frécinaux
 * Copyright (C) 2010 - Jesse van den Kieboom
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

#ifndef SOLI_APP_ACTIVATABLE_H
#define SOLI_APP_ACTIVATABLE_H

#include <glib-object.h>
#include "soli-menu-extension.h"

G_BEGIN_DECLS

#define SOLI_TYPE_APP_ACTIVATABLE (soli_app_activatable_get_type ())

G_DECLARE_INTERFACE (SoliAppActivatable, soli_app_activatable, SOLI, APP_ACTIVATABLE, GObject)

struct _SoliAppActivatableInterface
{
	GTypeInterface g_iface;

	/* Virtual public methods */
	void	(*activate)		(SoliAppActivatable *activatable);
	void	(*deactivate)		(SoliAppActivatable *activatable);
};

void	 soli_app_activatable_activate			(SoliAppActivatable *activatable);
void	 soli_app_activatable_deactivate		(SoliAppActivatable *activatable);

/**
 * soli_app_activatable_extend_menu:
 * @activatable: A #SoliAppActivatable.
 * @extension_point: the extension point section of the menu to get.
 *
 * Gets the #SoliMenuExtension for the menu @extension_point. Note that
 * the extension point could be in different menus (gear menu, app menu, etc)
 * depending on the platform.
 *
 * Returns: (transfer full): a #SoliMenuExtension for the specific section
 * or %NULL if not found.
 */
SoliMenuExtension	*soli_app_activatable_extend_menu	(SoliAppActivatable *activatable,
								 const gchar *extension_point);

G_END_DECLS

#endif /* SOLI_APP_ACTIVATABLE_H */
/* ex:set ts=8 noet: */
