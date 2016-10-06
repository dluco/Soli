/*
 * soli-progress-info-bar.h
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Maggi
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

#ifndef SOLI_PROGRESS_INFO_BAR_H
#define SOLI_PROGRESS_INFO_BAR_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_PROGRESS_INFO_BAR (soli_progress_info_bar_get_type ())
G_DECLARE_FINAL_TYPE (SoliProgressInfoBar, soli_progress_info_bar, SOLI, PROGRESS_INFO_BAR, GtkInfoBar)

GtkWidget	*soli_progress_info_bar_new			(const gchar          *icon_name,
								 const gchar          *markup,
								 gboolean              has_cancel);

void		 soli_progress_info_bar_set_icon_name		(SoliProgressInfoBar *bar,
								 const gchar          *icon_name);

void		 soli_progress_info_bar_set_markup		(SoliProgressInfoBar *bar,
								 const gchar          *markup);

void		 soli_progress_info_bar_set_text		(SoliProgressInfoBar *bar,
								 const gchar          *text);

void		 soli_progress_info_bar_set_fraction		(SoliProgressInfoBar *bar,
								 gdouble               fraction);

void		 soli_progress_info_bar_pulse			(SoliProgressInfoBar *bar);

G_END_DECLS

#endif  /* SOLI_PROGRESS_INFO_BAR_H  */

/* ex:set ts=8 noet: */
