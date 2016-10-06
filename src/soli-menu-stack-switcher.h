/*
 * soli-menu-stack-switcher.h
 * This file is part of soli
 *
 * Copyright (C) 2014 - Steve Frécinaux
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

#ifndef SOLI_MENU_STACK_SWITCHER_H
#define SOLI_MENU_STACK_SWITCHER_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_MENU_STACK_SWITCHER (soli_menu_stack_switcher_get_type())

G_DECLARE_FINAL_TYPE (SoliMenuStackSwitcher, soli_menu_stack_switcher, SOLI, MENU_STACK_SWITCHER, GtkMenuButton)

GtkWidget *  soli_menu_stack_switcher_new 	      (void);

void         soli_menu_stack_switcher_set_stack  (SoliMenuStackSwitcher *switcher,
                                                   GtkStack               *stack);

GtkStack *   soli_menu_stack_switcher_get_stack  (SoliMenuStackSwitcher *switcher);

G_END_DECLS

#endif  /* SOLI_MENU_STACK_SWITCHER_H  */

/* ex:set ts=2 sw=2 et: */
