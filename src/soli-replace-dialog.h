/*
 * soli-replace-dialog.h
 * This file is part of soli
 *
 * Copyright (C) 2005 Paolo Maggi
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

#ifndef SOLI_REPLACE_DIALOG_H
#define SOLI_REPLACE_DIALOG_H

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include "soli-window.h"

G_BEGIN_DECLS

#define SOLI_TYPE_REPLACE_DIALOG (soli_replace_dialog_get_type ())
G_DECLARE_FINAL_TYPE (SoliReplaceDialog, soli_replace_dialog, SOLI, REPLACE_DIALOG, GtkDialog)

enum
{
	SOLI_REPLACE_DIALOG_FIND_RESPONSE = 100,
	SOLI_REPLACE_DIALOG_REPLACE_RESPONSE,
	SOLI_REPLACE_DIALOG_REPLACE_ALL_RESPONSE
};

GtkWidget		*soli_replace_dialog_new			(SoliWindow        *window);

void			 soli_replace_dialog_present_with_time		(SoliReplaceDialog *dialog,
									 guint32             timestamp);

const gchar		*soli_replace_dialog_get_search_text		(SoliReplaceDialog *dialog);

const gchar		*soli_replace_dialog_get_replace_text		(SoliReplaceDialog *dialog);

gboolean		 soli_replace_dialog_get_backwards		(SoliReplaceDialog *dialog);

void			 soli_replace_dialog_set_replace_error		(SoliReplaceDialog *dialog,
									 const gchar        *error_msg);

G_END_DECLS

#endif  /* SOLI_REPLACE_DIALOG_H  */

/* ex:set ts=8 noet: */
