/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-view.c
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

#include "soli-view.h"
#include "soli-document.h"

struct _SoliViewPrivate
{
	gchar dummy;
};

G_DEFINE_TYPE_WITH_PRIVATE (SoliView, soli_view, GTK_SOURCE_TYPE_VIEW);

static GtkTextBuffer *soli_view_create_buffer (GtkTextView *text_view);

static void
soli_view_init (SoliView *view)
{
	view->priv = soli_view_get_instance_private (view);

	/* TODO: Add initialization code here */
}

static void
soli_view_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (soli_view_parent_class)->finalize (object);
}

static void
soli_view_class_init (SoliViewClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkTextViewClass *text_view_class = GTK_TEXT_VIEW_CLASS (klass);

	object_class->finalize = soli_view_finalize;
	
	text_view_class->create_buffer = soli_view_create_buffer;
}

GtkWidget *
soli_view_new (SoliDocument *doc)
{
	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), NULL);
	
	return GTK_WIDGET (g_object_new (SOLI_TYPE_VIEW, "buffer", doc, NULL));
}

static GtkTextBuffer *
soli_view_create_buffer (GtkTextView *text_view)
{
	return GTK_TEXT_BUFFER (soli_document_new ());
}


