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

#include <string.h>

static gchar *get_default_content_type (void);
static void on_content_type_changed (SoliDocument *doc, GParamSpec *pspec, gpointer empty);
static void soli_document_loaded_real (SoliDocument *doc);

typedef struct _SoliDocumentPrivate
{
	GtkSourceFile *file;

	gchar *content_type;
} SoliDocumentPrivate;

enum
{
	PROP_0,
	PROP_CONTENT_TYPE,
	LAST_PROP
};

static GParamSpec *properties[LAST_PROP];

enum
{
	LOAD,
	LOADED,
	LAST_SIGNAL
};

static guint document_signals[LAST_SIGNAL];

G_DEFINE_TYPE_WITH_PRIVATE (SoliDocument, soli_document, GTK_SOURCE_TYPE_BUFFER);

static void
soli_document_constructed (GObject *object)
{
	SoliDocument *doc = SOLI_DOCUMENT (object);
	
	G_OBJECT_CLASS (soli_document_parent_class)->constructed (object);
}

static void
soli_document_dispose (GObject *object)
{
	SoliDocument *doc = SOLI_DOCUMENT (object);

	if (doc->priv->file != NULL)
	{
		g_object_unref (doc->priv->file);
		doc->priv->file = NULL;
	}
	
	G_OBJECT_CLASS (soli_document_parent_class)->dispose (object);
}

static void
soli_document_finalize (GObject *object)
{
	SoliDocumentPrivate *priv;

	priv = soli_document_get_instance_private (SOLI_DOCUMENT (object));

	g_free (priv->content_type);

	G_OBJECT_CLASS (soli_document_parent_class)->finalize (object);
}

static gchar *
get_default_content_type (void)
{
	return g_content_type_from_mime_type ("text/plain");
}

static gchar *
get_content_type_from_content (SoliDocument *doc)
{
	gchar *content_type;
	gchar *data;
	GtkTextBuffer *buffer;
	GtkTextIter start, end;

	buffer = GTK_TEXT_BUFFER (doc);

	/* Mark off a portion of text at the beginning of the buffer. */
	gtk_text_buffer_get_start_iter (buffer, &start);
	end = start;
	/* end iter may wrap around to beginning, if a short buffer. */
	gtk_text_iter_forward_chars (&end, 255);

	data = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);

	content_type = g_content_type_guess (NULL,
										(const guchar *)data,
										strlen (data),
										NULL);

	g_free (data);

	return content_type;
}

static void
set_content_type_no_guess (SoliDocument *doc,
							const gchar *content_type)
{
	SoliDocumentPrivate *priv;
	gchar *dupped_content_type;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));

	priv = soli_document_get_instance_private (doc);

	if (priv->content_type != NULL &&
			content_type != NULL &&
			g_str_equal (priv->content_type, content_type))
	{
		/* Nothing to change. */
		return;
	}

	g_free (priv->content_type);

	dupped_content_type = g_strdup (content_type);

	if (dupped_content_type == NULL ||
			g_content_type_is_unknown (dupped_content_type))
	{
		priv->content_type = get_default_content_type ();
		g_free (dupped_content_type);
	}
	else
	{
		priv->content_type = dupped_content_type;
	}

	g_object_notify_by_pspec (G_OBJECT (doc), properties[PROP_CONTENT_TYPE]);
}

static void
set_content_type (SoliDocument *doc,
					const gchar *content_type)
{
	SoliDocumentPrivate *priv;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));

	priv = soli_document_get_instance_private (doc);

	if (content_type == NULL)
	{
		GFile *location;
		gchar *guessed_type = NULL;

		location = gtk_source_file_get_location (priv->file);
		if (location != NULL)
		{
			gchar *basename;

			basename = g_file_get_basename (location);
			guessed_type = g_content_type_guess (basename, NULL, 0, NULL);

			g_free (basename);
		}

		set_content_type_no_guess (doc, guessed_type);
		g_free (guessed_type);
	}
	else
	{
		set_content_type_no_guess (doc, content_type);
	}
}

void
soli_document_set_content_type (SoliDocument *doc,
								const gchar *content_type)
{
	g_return_if_fail (SOLI_IS_DOCUMENT (doc));

	set_content_type (doc, content_type);
}

gchar *
soli_document_get_content_type (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;

	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), NULL);
	
	priv = soli_document_get_instance_private (doc);

	return g_strdup (priv->content_type);
}

gchar *
soli_document_get_mimetype (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;

	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), g_strdup ("text/plain"));
	
	priv = soli_document_get_instance_private (doc);

	if (priv->content_type != NULL &&
		!g_content_type_is_unknown (priv->content_type))
	{
		return g_content_type_get_mime_type (priv->content_type);
	}

	return g_strdup ("text/plain");
}

static void
soli_document_set_property (GObject *object,
							guint prop_id,
							const GValue *value,
							GParamSpec *pspec)
{
	SoliDocument *doc = SOLI_DOCUMENT (object);
	SoliDocumentPrivate *priv = soli_document_get_instance_private (doc);

	switch (prop_id)
	{
		case PROP_CONTENT_TYPE:
			g_value_take_string ((GValue *)value, soli_document_get_content_type (doc));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
soli_document_get_property (GObject *object,
							guint prop_id,
							GValue *value,
							GParamSpec *pspec)
{
	SoliDocument *doc = SOLI_DOCUMENT (object);
	SoliDocumentPrivate *priv = soli_document_get_instance_private (doc);

	switch (prop_id)
	{
		case PROP_CONTENT_TYPE:
			set_content_type (doc, g_value_get_string (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
soli_document_class_init (SoliDocumentClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkTextBufferClass *buf_class = GTK_TEXT_BUFFER_CLASS (klass);

	object_class->constructed = soli_document_constructed;
	object_class->dispose = soli_document_dispose;
	object_class->finalize = soli_document_finalize;
	object_class->get_property = soli_document_get_property;
	object_class->set_property = soli_document_set_property;
	
	klass->loaded = soli_document_loaded_real;

	properties[PROP_CONTENT_TYPE] =
		g_param_spec_string ("content-type",
							"Content Type",
							"The document's content type",
							NULL,
							G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, LAST_PROP, properties);

	document_signals[LOAD] =
		g_signal_new ("load",
					G_OBJECT_CLASS_TYPE (object_class),
					G_SIGNAL_RUN_LAST,
					G_STRUCT_OFFSET (SoliDocumentClass, loaded),
					NULL, NULL, NULL,
					G_TYPE_NONE, 0);

	document_signals[LOAD] =
		g_signal_new ("loaded",
					G_OBJECT_CLASS_TYPE (object_class),
					G_SIGNAL_RUN_FIRST,
					G_STRUCT_OFFSET (SoliDocumentClass, loaded),
					NULL, NULL, NULL,
					G_TYPE_NONE, 0);
}

SoliDocument *
soli_document_new (void)
{
	return g_object_new (SOLI_TYPE_DOCUMENT, NULL);
}

GtkSourceFile *
soli_document_get_file (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;
	
	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), NULL);
	
	priv = soli_document_get_instance_private (doc);
	
	return priv->file;
}

GFile *
soli_document_get_location (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;
	
	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), NULL);
	
	priv = soli_document_get_instance_private (doc);
	
	return gtk_source_file_get_location (priv->file);
}

gboolean
soli_document_is_untouched (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;
	GFile *location;

	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), TRUE);
	
	priv = soli_document_get_instance_private (doc);
	location = gtk_source_file_get_location (priv->file);

	return location == NULL && !gtk_text_buffer_get_modified (GTK_TEXT_BUFFER (doc));
}

gboolean
soli_document_is_untitled (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;

	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), TRUE);
	
	priv = soli_document_get_instance_private (doc);

	return gtk_source_file_get_location (priv->file) == NULL;
}

gboolean
soli_document_needs_saving (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;
	gboolean externally_modified = FALSE;
	gboolean deleted = FALSE;

	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), FALSE);

	priv = soli_document_get_instance_private (doc);

	if (gtk_text_buffer_get_modified (GTK_TEXT_BUFFER (doc)))
	{
		return TRUE;
	}

	if (gtk_source_file_is_local (priv->file))
	{
		gtk_source_file_check_file_on_disk (priv->file);
		externally_modified = gtk_source_file_is_externally_modified (priv->file);
		deleted = gtk_source_file_is_deleted (priv->file);
	}

	// TODO: check create flag
	return (externally_modified || deleted);
}

static void
set_language (SoliDocument *doc,
				GtkSourceLanguage *lang)
{
	SoliDocumentPrivate *priv;
	GtkSourceLanguage *old_lang;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));
	
	priv = soli_document_get_instance_private (doc);

	old_lang = gtk_source_buffer_get_language (GTK_SOURCE_BUFFER (doc));

	if (lang == old_lang)
	{
		/* Nothing to change. */
		return;
	}

	gtk_source_buffer_set_language (GTK_SOURCE_BUFFER (doc), lang);
}

static GtkSourceLanguage *
guess_language (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;
	GtkSourceLanguageManager *manager;
	GtkSourceLanguage *lang;
	GFile *location;
	gchar *basename;

	g_return_val_if_fail (SOLI_IS_DOCUMENT (doc), NULL);

	priv = soli_document_get_instance_private (doc);

	manager = gtk_source_language_manager_get_default ();
	lang = NULL;
	basename = NULL;

	location = gtk_source_file_get_location (priv->file);

	if (location != NULL)
	{
		basename = g_file_get_basename (location);
	}

	lang = gtk_source_language_manager_guess_language (manager,
					basename,
					priv->content_type);

	g_free (basename);

	return lang;
}

static void
on_content_type_changed (SoliDocument *doc,
						GParamSpec *pspec,
						gpointer empty)
{
	SoliDocumentPrivate *priv;
	GtkSourceLanguage *lang;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));

	priv = soli_document_get_instance_private (doc);

	lang = guess_language (doc);

	set_language (doc, lang);
}

static void
loaded_query_info_cb (GFile *location,
					GAsyncResult *result,
					SoliDocument *doc)
{
	GFileInfo *info;
	GError *error = NULL;

	info = g_file_query_info_finish (location, result, &error);

	if (error != NULL)
	{
		if (error->domain != G_IO_ERROR ||
				error->code != G_IO_ERROR_NOT_FOUND)
		{
			g_warning ("Document loading: query info error: %s", error->message);
		}
		g_error_free (error);
		error = NULL;
	}

	if (info != NULL &&
			g_file_info_has_attribute (info, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE))
	{
		const gchar *content_type;

		content_type = g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE);

		set_content_type (doc, content_type);
	}

	g_clear_object (&info);

	/* Async operation finished. */
	g_object_unref (doc);
}

static void
soli_document_loaded_real (SoliDocument *doc)
{
	SoliDocumentPrivate *priv;
	GtkSourceLanguage *lang;
	GFile *location;

	g_return_if_fail (SOLI_IS_DOCUMENT (doc));

	priv = soli_document_get_instance_private (doc);

	lang = guess_language (doc);
	set_language (doc, lang);

	set_content_type (doc, NULL);

	location = gtk_source_file_get_location (priv->file);
	if (location != NULL)
	{
		/* Keep the doc alive during the async operation.*/
		g_object_ref (doc);

		g_file_query_info_async (location,
								G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE ","
								G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE,
								G_FILE_QUERY_INFO_NONE,
								G_PRIORITY_DEFAULT,
								NULL,
								(GAsyncReadyCallback) loaded_query_info_cb,
								doc);
	}
}

static void
soli_document_init (SoliDocument *doc)
{
	doc->priv = soli_document_get_instance_private (doc);

	doc->priv->content_type = get_default_content_type ();

	doc->priv->file = gtk_source_file_new ();

	// TODO: handle with preferences/settings
	gtk_source_buffer_set_highlight_syntax (GTK_SOURCE_BUFFER (doc), TRUE);

	g_signal_connect (doc,
					"notify::content-type",
					G_CALLBACK (on_content_type_changed),
					NULL);
}
