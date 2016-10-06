/*
 * soli-file-chooser-dialog-gtk.h
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Maggi
 * Copyright (C) 2014 - Jesse van den Kieboom
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

#ifndef SOLI_FILE_CHOOSER_DIALOG_OSX_H
#define SOLI_FILE_CHOOSER_DIALOG_OSX_H

#include <gtk/gtk.h>
#include "soli-file-chooser-dialog.h"

G_BEGIN_DECLS

#define SOLI_TYPE_FILE_CHOOSER_DIALOG_OSX (soli_file_chooser_dialog_osx_get_type ())

G_DECLARE_FINAL_TYPE (SoliFileChooserDialogOSX, soli_file_chooser_dialog_osx, SOLI, FILE_CHOOSER_DIALOG_OSX, GObject)

SoliFileChooserDialog	*soli_file_chooser_dialog_osx_create		(const gchar              *title,
									 GtkWindow                *parent,
									 SoliFileChooserFlags     flags,
									 const GtkSourceEncoding  *encoding,
									 const gchar              *cancel_label,
									 GtkResponseType           cancel_response,
									 const gchar              *accept_label,
									 GtkResponseType           accept_response);

G_END_DECLS

#endif /* SOLI_FILE_CHOOSER_DIALOG_OSX_H */

/* ex:set ts=8 noet: */
