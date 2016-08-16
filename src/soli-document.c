/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-document.c
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

#include "soli-document.h"

struct _SoliDocumentPrivate
{
};


G_DEFINE_TYPE_WITH_PRIVATE (SoliDocument, soli_document, GTK_SOURCE_TYPE_BUFFER);

static void
soli_document_init (SoliDocument *doc)
{
	doc->priv = soli_document_get_instance_private (doc);

	/* TODO: Add initialization code here */
}

static void
soli_document_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (soli_document_parent_class)->finalize (object);
}

static void
soli_document_class_init (SoliDocumentClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = soli_document_finalize;
}


