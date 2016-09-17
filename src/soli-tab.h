/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-tab.h
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 *
 * soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SOLI_TAB_H_
#define _SOLI_TAB_H_

#include <gtk/gtk.h>
#include "soli-view.h"
#include "soli-document.h"

G_BEGIN_DECLS

typedef enum
{
	SOLI_TAB_STATE_NORMAL = 0,
	SOLI_TAB_STATE_LOADING,
	SOLI_TAB_STATE_SAVING,
	SOLI_TAB_STATE_LOADING_ERROR,
	SOLI_TAB_STATE_SAVING_ERROR,
	SOLI_TAB_STATE_GENERIC_ERROR,
	SOLI_TAB_STATE_CLOSING,
	SOLI_TAB_NUM_STATES
} SoliTabState;

#define SOLI_TYPE_TAB             (soli_tab_get_type ())

G_DECLARE_FINAL_TYPE (SoliTab, soli_tab, SOLI, TAB, GtkBox)

SoliTab *soli_tab_new (void);

SoliTabState soli_tab_get_state (SoliTab *tab);

SoliTab *soli_tab_get_from_document (SoliDocument *doc);

void soli_tab_load (SoliTab *tab,
					GFile *location,
					const GtkSourceEncoding *encoding);

gboolean soli_tab_save_finish (SoliTab *tab, GAsyncResult *result);

void
soli_tab_save_async (SoliTab *tab,
					GCancellable *cancellable,
					GAsyncReadyCallback callback,
					gpointer user_data);

void
soli_tab_save_as_async (SoliTab *tab,
						GFile *location,
						GCancellable *cancellable,
						GAsyncReadyCallback callback,
						gpointer user_data);

SoliView *
soli_tab_get_view (SoliTab *tab);
SoliDocument *
soli_tab_get_document (SoliTab *tab);

gboolean
soli_tab_can_close (SoliTab *tab);

G_END_DECLS

#endif /* _SOLI_TAB_H_ */

