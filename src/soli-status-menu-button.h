/*
 * soli-status-menu-button.h
 * This file is part of soli
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLI_STATUS_MENU_BUTTON_H
#define SOLI_STATUS_MENU_BUTTON_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_STATUS_MENU_BUTTON (soli_status_menu_button_get_type ())

G_DECLARE_FINAL_TYPE (SoliStatusMenuButton, soli_status_menu_button, SOLI, STATUS_MENU_BUTTON, GtkMenuButton)

GtkWidget *soli_status_menu_button_new		(void);

void soli_status_menu_button_set_label		(SoliStatusMenuButton *button,
						 const gchar           *label);

const gchar *soli_status_menu_button_get_label (SoliStatusMenuButton *button);

G_END_DECLS

#endif /* SOLI_STATUS_MENU_BUTTON_H */

/* ex:set ts=8 noet: */
