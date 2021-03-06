/*
 * soli-tab-label.h
 * This file is part of soli
 *
 * Copyright (C) 2010 - Paolo Borelli
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

#ifndef SOLI_TAB_LABEL_H
#define SOLI_TAB_LABEL_H

#include <gtk/gtk.h>
#include "soli-tab.h"

G_BEGIN_DECLS

#define SOLI_TYPE_TAB_LABEL (soli_tab_label_get_type ())

G_DECLARE_FINAL_TYPE (SoliTabLabel, soli_tab_label, SOLI, TAB_LABEL, GtkBox)

GtkWidget 	*soli_tab_label_new				(SoliTab *tab);

SoliTab	*soli_tab_label_get_tab			(SoliTabLabel *tab_label);

G_END_DECLS

#endif /* SOLI_TAB_LABEL_H */

/* ex:set ts=8 noet: */
