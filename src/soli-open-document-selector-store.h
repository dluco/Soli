/*
 * soli-open-document-selector-store.h
 * This file is part of soli
 *
 * Copyright (C) 2015 - Sébastien Lafargue
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

#ifndef SOLI_OPEN_DOCUMENT_SELECTOR_STORE_H
#define SOLI_OPEN_DOCUMENT_SELECTOR_STORE_H

#include "soli-open-document-selector-helper.h"
#include "soli-open-document-selector.h"

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_OPEN_DOCUMENT_SELECTOR_STORE (soli_open_document_selector_store_get_type ())

G_DECLARE_FINAL_TYPE (SoliOpenDocumentSelectorStore, soli_open_document_selector_store, SOLI, OPEN_DOCUMENT_SELECTOR_STORE, GObject)

#define SOLI_OPEN_DOCUMENT_SELECTOR_STORE_ERROR soli_open_document_selector_store_error_quark ()

typedef enum
{
	TYPE_OUT_OF_RANGE
} SoliOpenDocumentSelectorStoreError;

GQuark				 soli_open_document_selector_store_error_quark				(void);

gint				 soli_open_document_selector_store_get_recent_limit			(SoliOpenDocumentSelectorStore *store);

void				 soli_open_document_selector_store_set_filter				(SoliOpenDocumentSelectorStore *store,
                                                                                                         const gchar                    *filter);

gchar				*soli_open_document_selector_store_get_filter				(SoliOpenDocumentSelectorStore *store);

GList				*soli_open_document_selector_store_update_list_finish			(SoliOpenDocumentSelectorStore  *open_document_selector_store,
                                                                                                         GAsyncResult                    *res,
                                                                                                         GError                         **error);

void				soli_open_document_selector_store_update_list_async			(SoliOpenDocumentSelectorStore *open_document_selector_store,
                                                                                                         SoliOpenDocumentSelector      *open_document_selector,
                                                                                                         GCancellable                   *cancellable,
                                                                                                         GAsyncReadyCallback             callback,
                                                                                                         ListType                        type,
                                                                                                         gpointer                        user_data);

SoliOpenDocumentSelectorStore	*soli_open_document_selector_store_get_default				(void);

G_END_DECLS

#endif /* SOLI_OPEN_DOCUMENT_SELECTOR_STORE_H */
/* ex:set ts=8 noet: */
