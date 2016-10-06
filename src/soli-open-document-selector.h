/*
 * soli-open-document-selector.h
 * This file is part of soli
 *
 * Copyright (C) 2014 - Sébastien Lafargue
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

#ifndef SOLI_OPEN_DOCUMENT_SELECTOR_H
#define SOLI_OPEN_DOCUMENT_SELECTOR_H

#include <glib-object.h>
#include "soli-window.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_OPEN_DOCUMENT_SELECTOR (soli_open_document_selector_get_type ())

G_DECLARE_FINAL_TYPE (SoliOpenDocumentSelector, soli_open_document_selector, SOLI, OPEN_DOCUMENT_SELECTOR, GtkBox)

SoliOpenDocumentSelector	*soli_open_document_selector_new		(SoliWindow               *window);

SoliWindow			*soli_open_document_selector_get_window	(SoliOpenDocumentSelector *selector);

GtkWidget			*soli_open_document_selector_get_search_entry	(SoliOpenDocumentSelector *selector);

G_END_DECLS

#endif /* SOLI_OPEN_DOCUMENT_SELECTOR_H */
/* ex:set ts=8 noet: */
