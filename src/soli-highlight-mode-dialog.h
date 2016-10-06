/*
 * soli-highlight-mode-dialog.h
 * This file is part of soli
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
 *
 * soli is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * soli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with soli. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef SOLI_HIGHLIGHT_MODE_DIALOG_H
#define SOLI_HIGHLIGHT_MODE_DIALOG_H

#include <glib.h>
#include "soli-highlight-mode-selector.h"

G_BEGIN_DECLS

#define SOLI_TYPE_HIGHLIGHT_MODE_DIALOG (soli_highlight_mode_dialog_get_type ())

G_DECLARE_FINAL_TYPE (SoliHighlightModeDialog, soli_highlight_mode_dialog, SOLI, HIGHLIGHT_MODE_DIALOG, GtkDialog)

GtkWidget                  *soli_highlight_mode_dialog_new             (GtkWindow *parent);

SoliHighlightModeSelector *soli_highlight_mode_dialog_get_selector    (SoliHighlightModeDialog *dlg);

G_END_DECLS

#endif /* SOLI_HIGHLIGHT_MODE_DIALOG_H */

/* ex:set ts=8 noet: */
