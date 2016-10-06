/*
 * soli-statusbar.h
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Borelli
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

#ifndef SOLI_STATUSBAR_H
#define SOLI_STATUSBAR_H

#include <gtk/gtk.h>
#include "soli-window.h"

G_BEGIN_DECLS

#define SOLI_TYPE_STATUSBAR (soli_statusbar_get_type ())

G_DECLARE_FINAL_TYPE (SoliStatusbar, soli_statusbar, SOLI, STATUSBAR, GtkStatusbar)

GtkWidget	*soli_statusbar_new			(void);

void		 soli_statusbar_set_window_state	(SoliStatusbar   *statusbar,
							 SoliWindowState  state,
							 gint              num_of_errors);

void		 soli_statusbar_set_overwrite		(SoliStatusbar   *statusbar,
							 gboolean          overwrite);

void		 soli_statusbar_clear_overwrite 	(SoliStatusbar   *statusbar);

void		 soli_statusbar_flash_message		(SoliStatusbar   *statusbar,
							 guint             context_id,
							 const gchar      *format,
							 ...) G_GNUC_PRINTF(3, 4);

G_END_DECLS

#endif

/* ex:set ts=8 noet: */
