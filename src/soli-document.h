/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-document.h
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 *
 * soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SOLI_DOCUMENT_H_
#define _SOLI_DOCUMENT_H_

//#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define SOLI_TYPE_DOCUMENT             (soli_document_get_type ())
#define SOLI_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_DOCUMENT, SoliDocument))
#define SOLI_DOCUMENT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_DOCUMENT, SoliDocumentClass))
#define SOLI_IS_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_DOCUMENT))
#define SOLI_IS_DOCUMENT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_DOCUMENT))
#define SOLI_DOCUMENT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_DOCUMENT, SoliDocumentClass))

typedef struct _SoliDocument SoliDocument;
typedef struct _SoliDocumentClass SoliDocumentClass;
typedef struct _SoliDocumentPrivate SoliDocumentPrivate;

struct _SoliDocument
{
	GtkSourceBuffer parent_instance;

	SoliDocumentPrivate *priv;
};

struct _SoliDocumentClass
{
	GtkSourceBufferClass parent_class;
};

GType soli_document_get_type (void) G_GNUC_CONST;
SoliDocument *soli_document_new (void);

GtkSourceFile *soli_document_get_file (SoliDocument *doc);
GFile *soli_document_get_location (SoliDocument *doc);

G_END_DECLS

#endif /* _SOLI_DOCUMENT_H_ */

