/*
 * soli-highlight-mode-selector.h
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

#ifndef SOLI_HIGHLIGHT_MODE_SELECTOR_H
#define SOLI_HIGHLIGHT_MODE_SELECTOR_H

#include <glib-object.h>
#include <gtksourceview/gtksource.h>
#include "soli-window.h"

G_BEGIN_DECLS

#define SOLI_TYPE_HIGHLIGHT_MODE_SELECTOR (soli_highlight_mode_selector_get_type ())

G_DECLARE_FINAL_TYPE (SoliHighlightModeSelector, soli_highlight_mode_selector, SOLI, HIGHLIGHT_MODE_SELECTOR, GtkGrid)

SoliHighlightModeSelector *soli_highlight_mode_selector_new             (void);

void                        soli_highlight_mode_selector_select_language (SoliHighlightModeSelector *selector,
                                                                           GtkSourceLanguage          *language);

void                        soli_highlight_mode_selector_activate_selected_language
                                                                          (SoliHighlightModeSelector *selector);

G_END_DECLS

#endif /* SOLI_HIGHLIGHT_MODE_SELECTOR_H */

/* ex:set ts=8 noet: */
