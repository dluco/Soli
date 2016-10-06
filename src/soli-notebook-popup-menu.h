/*
 * soli-notebook-popup-menu.h
 * This file is part of soli
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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


#ifndef SOLI_NOTEBOOK_POPUP_MENU_H
#define SOLI_NOTEBOOK_POPUP_MENU_H

#include <gtk/gtk.h>
#include "soli-window.h"
#include "soli-tab.h"

G_BEGIN_DECLS

#define SOLI_TYPE_NOTEBOOK_POPUP_MENU			(soli_notebook_popup_menu_get_type ())
G_DECLARE_FINAL_TYPE (SoliNotebookPopupMenu, soli_notebook_popup_menu, SOLI, NOTEBOOK_POPUP_MENU, GtkMenu)

GtkWidget           *soli_notebook_popup_menu_new          (SoliWindow *window,
                                                             SoliTab    *tab);

G_END_DECLS

#endif /* SOLI_NOTEBOOK_POPUP_MENU_H */
