/*
 * soli-menu-extension.h
 * This file is part of soli
 *
 * Copyright (C) 2014 - Ignacio Casal Quinteiro
 *
 * soli is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * soli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with soli. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLI_MENU_EXTENSION_H
#define SOLI_MENU_EXTENSION_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define SOLI_TYPE_MENU_EXTENSION (soli_menu_extension_get_type ())

G_DECLARE_FINAL_TYPE (SoliMenuExtension, soli_menu_extension, SOLI, MENU_EXTENSION, GObject)

SoliMenuExtension       *soli_menu_extension_new                 (GMenu                *menu);

void                      soli_menu_extension_append_menu_item    (SoliMenuExtension   *menu,
                                                                    GMenuItem            *item);

void                      soli_menu_extension_prepend_menu_item   (SoliMenuExtension   *menu,
                                                                    GMenuItem            *item);

void                      soli_menu_extension_remove_items        (SoliMenuExtension   *menu);

G_END_DECLS

#endif /* SOLI_MENU_EXTENSION_H */

/* ex:set ts=8 noet: */
