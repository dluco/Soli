/*
 * soli-document.h
 * This file is part of soli
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2005 Paolo Maggi
 * Copyright (C) 2014 Sébastien Wilmet
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

#ifndef SOLI_DOCUMENT_H
#define SOLI_DOCUMENT_H

#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define SOLI_TYPE_DOCUMENT (soli_document_get_type())

G_DECLARE_DERIVABLE_TYPE (SoliDocument, soli_document, SOLI, DOCUMENT, GtkSourceBuffer)

struct _SoliDocumentClass
{
	GtkSourceBufferClass parent_class;

	/* Signals */
	void (* cursor_moved)		(SoliDocument *document);

	void (* load)			(SoliDocument *document);

	void (* loaded)			(SoliDocument *document);

	void (* save)			(SoliDocument *document);

	void (* saved)  		(SoliDocument *document);
};

SoliDocument   *soli_document_new				(void);

GtkSourceFile	*soli_document_get_file			(SoliDocument       *doc);

G_DEPRECATED_FOR (gtk_source_file_get_location)
GFile		*soli_document_get_location			(SoliDocument       *doc);

G_DEPRECATED_FOR (gtk_source_file_set_location)
void		 soli_document_set_location			(SoliDocument       *doc,
								 GFile               *location);

gchar		*soli_document_get_uri_for_display		(SoliDocument       *doc);

gchar		*soli_document_get_short_name_for_display	(SoliDocument       *doc);

G_DEPRECATED
void		 soli_document_set_short_name_for_display	(SoliDocument       *doc,
								 const gchar         *short_name);

gchar		*soli_document_get_content_type		(SoliDocument       *doc);

G_DEPRECATED
void		 soli_document_set_content_type		(SoliDocument       *doc,
								 const gchar         *content_type);

gchar		*soli_document_get_mime_type			(SoliDocument       *doc);

G_DEPRECATED_FOR (gtk_source_file_is_readonly)
gboolean	 soli_document_get_readonly			(SoliDocument       *doc);

gboolean	 soli_document_is_untouched			(SoliDocument       *doc);

gboolean	 soli_document_is_untitled			(SoliDocument       *doc);

G_DEPRECATED_FOR (gtk_source_file_is_local)
gboolean	 soli_document_is_local			(SoliDocument       *doc);

G_DEPRECATED
gboolean	 soli_document_get_deleted			(SoliDocument       *doc);

gboolean	 soli_document_goto_line			(SoliDocument       *doc,
								gint                 line);

gboolean	 soli_document_goto_line_offset		(SoliDocument       *doc,
								 gint                 line,
								 gint                 line_offset);

void 		 soli_document_set_language			(SoliDocument       *doc,
								 GtkSourceLanguage   *lang);
GtkSourceLanguage
		*soli_document_get_language			(SoliDocument       *doc);

G_DEPRECATED_FOR (gtk_source_file_get_encoding)
const GtkSourceEncoding
		*soli_document_get_encoding			(SoliDocument       *doc);

G_DEPRECATED_FOR (gtk_source_file_get_newline_type)
GtkSourceNewlineType
		 soli_document_get_newline_type		(SoliDocument       *doc);

G_DEPRECATED_FOR (gtk_source_file_get_compression_type)
GtkSourceCompressionType
		 soli_document_get_compression_type		(SoliDocument       *doc);

gchar		*soli_document_get_metadata			(SoliDocument       *doc,
								 const gchar         *key);

void		 soli_document_set_metadata			(SoliDocument       *doc,
								 const gchar         *first_key,
								 ...);

void		 soli_document_set_search_context		(SoliDocument          *doc,
								 GtkSourceSearchContext *search_context);

GtkSourceSearchContext *
		 soli_document_get_search_context		(SoliDocument       *doc);

G_END_DECLS

#endif /* SOLI_DOCUMENT_H */
/* ex:set ts=8 noet: */
