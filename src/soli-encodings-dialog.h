/*
 * soli-encodings-dialog.h
 * This file is part of soli
 *
 * Copyright (C) 2003-2005 Paolo Maggi
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

#ifndef SOLI_ENCODINGS_DIALOG_H
#define SOLI_ENCODINGS_DIALOG_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_ENCODINGS_DIALOG (soli_encodings_dialog_get_type())

G_DECLARE_FINAL_TYPE (SoliEncodingsDialog, soli_encodings_dialog, SOLI, ENCODINGS_DIALOG, GtkDialog)

GtkWidget	*soli_encodings_dialog_new		(void);

G_END_DECLS

#endif /* SOLI_ENCODINGS_DIALOG_H */
/* ex:set ts=8 noet: */
