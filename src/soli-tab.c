/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-tab.c
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

#include "soli-tab.h"
#include "soli-document.h"
#include "soli-view-frame.h"
#include <gtksourceview/gtksource.h>

struct _SoliTabPrivate
{
	SoliViewFrame *frame;
};

typedef struct _LoaderData LoaderData;

struct _LoaderData
{
	GtkSourceFileLoader *loader;
	GTimer *timer;
	
	guint user_requested_encoding : 1;
};


G_DEFINE_TYPE_WITH_PRIVATE (SoliTab, soli_tab, GTK_TYPE_BOX);

static void
soli_tab_init (SoliTab *tab)
{
	tab->priv = soli_tab_get_instance_private (tab);

	tab->priv->frame = soli_view_frame_new ();
	
	gtk_box_pack_end (GTK_BOX (tab),
						GTK_WIDGET (tab->priv->frame),
						TRUE, TRUE, 0);
	gtk_widget_show (GTK_WIDGET (tab->priv->frame));
}

static void
soli_tab_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (soli_tab_parent_class)->finalize (object);
}

static void
soli_tab_class_init (SoliTabClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = soli_tab_finalize;
}

SoliTab *
soli_tab_new (void)
{
	return g_object_new (SOLI_TYPE_TAB, NULL);
}

static LoaderData *
loader_data_new (void)
{
	return g_slice_new0 (LoaderData);
}

static void
loader_data_free (LoaderData *data)
{
	if (data != NULL)
	{
		if (data->loader != NULL)
		{
			g_object_unref (data->loader);
		}

		if (data->timer != NULL)
		{
			g_timer_destroy (data->timer);
		}

		g_slice_free (LoaderData, data);
	}
}

static void
loader_progress_cb (goffset size,
					goffset total_size,
					GTask *load_task)
{
	//SoliTab *tab = g_task_get_source_object (load_task);
	//LoaderData *data = g_task_get_data (load_task);
	
	return;
}

static void
load_cb (GtkSourceFileLoader *loader,
		GAsyncResult *result,
		GTask *load_task)
{
	return;
}

static void
launch_loader (GTask *load_task,
				const GtkSourceEncoding *encoding)
{
	SoliTab *tab = g_task_get_source_object (load_task);
	LoaderData *data = g_task_get_task_data (load_task);
	GSList *candidate_encodings = NULL;
	SoliDocument *doc;
	
	if (encoding != NULL)
	{
		data->user_requested_encoding = TRUE;
		candidate_encodings = g_slist_append (NULL, (gpointer) encoding);
	}
	else
	{
		data->user_requested_encoding = FALSE;
		candidate_encodings = NULL; // TODO
	}
	
	gtk_source_file_loader_set_candidate_encodings (data->loader, candidate_encodings);
	g_slist_free (candidate_encodings);
	
	// TODO
	//doc = soli_tab_get_document (tab);
	//g_signal_emit_by_name (doc, "load");
	
	if (data->timer != NULL)
	{
		g_timer_destroy (data->timer);
	}
	data->timer = g_timer_new ();
	
	gtk_source_file_loader_load_async (data->loader,
										G_PRIORITY_DEFAULT,
										g_task_get_cancellable (load_task),
										(GFileProgressCallback) loader_progress_cb,
										load_task,
										NULL,
										(GAsyncReadyCallback) load_cb,
										load_task);
}

static void
load_async (SoliTab *tab,
			GFile *location,
			const GtkSourceEncoding *encoding,
			GCancellable *cancellable,
			GAsyncReadyCallback callback,
			gpointer user_data)
{
	SoliDocument *doc;
	GtkSourceFile *file;
	GTask *load_task;
	LoaderData *data;
	
	g_return_if_fail (SOLI_IS_TAB (tab));
	g_return_if_fail (G_IS_FILE (location));
	g_return_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable));
	
	doc = soli_tab_get_document (tab); // TODO
	file = soli_document_get_file (doc);
	gtk_source_file_set_location (file, location);
	
	load_task = g_task_new (tab, cancellable, callback, user_data);
	
	data = loader_data_new ();
	g_task_set_task_data (load_task, data, (GDestroyNotify) loader_data_free);
	data->loader = gtk_source_file_loader_new (GTK_SOURCE_BUFFER (doc), file);
	
	launch_loader (load_task, encoding);
}

static gboolean
load_finish (SoliTab *tab,
				GAsyncResult *result)
{
	g_return_val_if_fail (g_task_is_valid (result, tab), FALSE);
	
	return g_task_propagate_boolean (G_TASK (result), NULL);
}

void
soli_tab_load (SoliTab *tab,
				GFile *location,
				const GtkSourceEncoding *encoding)
{
	GCancellable *cancellable;
	
	cancellable = g_cancellable_new ();
	
	load_async (tab,
				location,
				encoding,
				cancellable,
				(GAsyncReadyCallback) load_finish,
				NULL);
	
	g_object_unref (cancellable);
}

SoliDocument *
soli_tab_get_document (SoliTab *tab)
{
	SoliView *view;
	GtkTextBuffer *buffer;
	
	g_return_val_if_fail (SOLI_IS_TAB (tab), NULL);
	
	view = soli_view_frame_get_view (tab->priv->frame);
	
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	
	return SOLI_IS_DOCUMENT (buffer) ? SOLI_DOCUMENT (buffer) : NULL;
}
