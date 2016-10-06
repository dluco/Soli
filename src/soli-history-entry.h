/*
 * soli-history-entry.h
 * This file is part of soli
 *
 * Copyright (C) 2006 - Paolo Borelli
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

#ifndef SOLI_HISTORY_ENTRY_H
#define SOLI_HISTORY_ENTRY_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_HISTORY_ENTRY (soli_history_entry_get_type ())

G_DECLARE_FINAL_TYPE (SoliHistoryEntry, soli_history_entry, SOLI, HISTORY_ENTRY, GtkComboBoxText)

GtkWidget	*soli_history_entry_new			(const gchar       *history_id,
								 gboolean           enable_completion);

void		 soli_history_entry_prepend_text		(SoliHistoryEntry *entry,
								 const gchar       *text);

void		 soli_history_entry_append_text		(SoliHistoryEntry *entry,
								 const gchar       *text);

void		 soli_history_entry_clear			(SoliHistoryEntry *entry);

void		 soli_history_entry_set_history_length		(SoliHistoryEntry *entry,
								 guint              max_saved);

guint		 soli_history_entry_get_history_length		(SoliHistoryEntry *gentry);

void		soli_history_entry_set_enable_completion	(SoliHistoryEntry *entry,
								 gboolean           enable);

gboolean	soli_history_entry_get_enable_completion	(SoliHistoryEntry *entry);

GtkWidget	*soli_history_entry_get_entry			(SoliHistoryEntry *entry);

G_END_DECLS

#endif /* SOLI_HISTORY_ENTRY_H */

/* ex:set ts=8 noet: */
