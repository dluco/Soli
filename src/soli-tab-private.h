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

#ifndef SOLI_TAB_PRIVATE_H
#define SOLI_TAB_PRIVATE_H

#include "soli-tab.h"
#include "soli-view-frame.h"

G_BEGIN_DECLS

SoliTab	*_soli_tab_new				(void);

gchar 		*_soli_tab_get_name			(SoliTab                *tab);

gchar 		*_soli_tab_get_tooltip			(SoliTab                *tab);

GdkPixbuf 	*_soli_tab_get_icon			(SoliTab                *tab);

void		 _soli_tab_load			(SoliTab                *tab,
							 GFile                   *location,
							 const GtkSourceEncoding *encoding,
							 gint                     line_pos,
							 gint                     column_pos,
							 gboolean                 create);

void		 _soli_tab_load_stream			(SoliTab                *tab,
							 GInputStream            *location,
							 const GtkSourceEncoding *encoding,
							 gint                     line_pos,
							 gint                     column_pos);

void		 _soli_tab_revert			(SoliTab                *tab);

void		 _soli_tab_save_async			(SoliTab                *tab,
							 GCancellable            *cancellable,
							 GAsyncReadyCallback      callback,
							 gpointer                 user_data);

gboolean	 _soli_tab_save_finish			(SoliTab                *tab,
							 GAsyncResult            *result);

void		 _soli_tab_save_as_async		(SoliTab                 *tab,
							 GFile                    *location,
							 const GtkSourceEncoding  *encoding,
							 GtkSourceNewlineType      newline_type,
							 GtkSourceCompressionType  compression_type,
							 GCancellable             *cancellable,
							 GAsyncReadyCallback       callback,
							 gpointer                  user_data);

void		 _soli_tab_print			(SoliTab                 *tab);

void		 _soli_tab_mark_for_closing		(SoliTab                 *tab);

gboolean	 _soli_tab_get_can_close		(SoliTab                 *tab);

SoliViewFrame	*_soli_tab_get_view_frame		(SoliTab                 *tab);

void		 _soli_tab_set_network_available	(SoliTab	     *tab,
							 gboolean	     enable);

G_END_DECLS

#endif  /* SOLI_TAB_PRIVATE_H */

/* ex:set ts=8 noet: */
