/*
 * soli-tab.h
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

#ifndef SOLI_TAB_H
#define SOLI_TAB_H

#include <gtksourceview/gtksource.h>
#include "soli-view.h"
#include "soli-document.h"

G_BEGIN_DECLS

typedef enum
{
	SOLI_TAB_STATE_NORMAL = 0,
	SOLI_TAB_STATE_LOADING,
	SOLI_TAB_STATE_REVERTING,
	SOLI_TAB_STATE_SAVING,
	SOLI_TAB_STATE_PRINTING,
	SOLI_TAB_STATE_PRINT_PREVIEWING, /* unused, deprecated */
	SOLI_TAB_STATE_SHOWING_PRINT_PREVIEW,
	SOLI_TAB_STATE_GENERIC_NOT_EDITABLE, /* unused, deprecated */
	SOLI_TAB_STATE_LOADING_ERROR,
	SOLI_TAB_STATE_REVERTING_ERROR,
	SOLI_TAB_STATE_SAVING_ERROR,
	SOLI_TAB_STATE_GENERIC_ERROR,
	SOLI_TAB_STATE_CLOSING,
	SOLI_TAB_STATE_EXTERNALLY_MODIFIED_NOTIFICATION,
	SOLI_TAB_NUM_OF_STATES /* This is not a valid state */
} SoliTabState;

#define SOLI_TYPE_TAB (soli_tab_get_type())

G_DECLARE_FINAL_TYPE (SoliTab, soli_tab, SOLI, TAB, GtkBox)

SoliView	*soli_tab_get_view			(SoliTab            *tab);

/* This is only an helper function */
SoliDocument	*soli_tab_get_document			(SoliTab            *tab);

SoliTab	*soli_tab_get_from_document		(SoliDocument       *doc);

SoliTabState	 soli_tab_get_state			(SoliTab            *tab);

gboolean	 soli_tab_get_auto_save_enabled	(SoliTab            *tab);

void		 soli_tab_set_auto_save_enabled	(SoliTab            *tab,
							 gboolean            enable);

gint		 soli_tab_get_auto_save_interval	(SoliTab            *tab);

void		 soli_tab_set_auto_save_interval	(SoliTab            *tab,
							 gint                interval);

void		 soli_tab_set_info_bar			(SoliTab            *tab,
							 GtkWidget           *info_bar);

G_END_DECLS

#endif  /* SOLI_TAB_H  */

/* ex:set ts=8 noet: */
