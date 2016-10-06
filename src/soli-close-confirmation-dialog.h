/*
 * soli-close-confirmation-dialog.h
 * This file is part of soli
 *
 * Copyright (C) 2004-2005 GNOME Foundation
 * Copyright (C) 2015 Sébastien Wilmet
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

#ifndef SOLI_CLOSE_CONFIRMATION_DIALOG_H
#define SOLI_CLOSE_CONFIRMATION_DIALOG_H

#include <glib.h>
#include <gtk/gtk.h>
#include "soli-document.h"

#define SOLI_TYPE_CLOSE_CONFIRMATION_DIALOG (soli_close_confirmation_dialog_get_type ())

G_DECLARE_FINAL_TYPE (SoliCloseConfirmationDialog, soli_close_confirmation_dialog,
		      SOLI, CLOSE_CONFIRMATION_DIALOG,
		      GtkMessageDialog)

GtkWidget	*soli_close_confirmation_dialog_new			(GtkWindow     *parent,
									 GList         *unsaved_documents);

GtkWidget 	*soli_close_confirmation_dialog_new_single 		(GtkWindow     *parent,
									 SoliDocument *doc);

const GList	*soli_close_confirmation_dialog_get_unsaved_documents  (SoliCloseConfirmationDialog *dlg);

GList		*soli_close_confirmation_dialog_get_selected_documents	(SoliCloseConfirmationDialog *dlg);

#endif /* SOLI_CLOSE_CONFIRMATION_DIALOG_H */
/* ex:set ts=8 noet: */
