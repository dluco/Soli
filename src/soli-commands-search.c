/*
 * soli-commands-search.c
 * This file is part of soli
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2006 Paolo Maggi
 * Copyright (C) 2013 Sébastien Wilmet
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "soli-commands.h"
#include "soli-commands-private.h"

#include <string.h>
#include <glib/gi18n.h>
#include <gdk/gdkkeysyms.h>

#include "soli-debug.h"
#include "soli-statusbar.h"
#include "soli-tab.h"
#include "soli-tab-private.h"
#include "soli-view-frame.h"
#include "soli-window.h"
#include "soli-window-private.h"
#include "soli-utils.h"
#include "soli-replace-dialog.h"

#define SOLI_REPLACE_DIALOG_KEY	"soli-replace-dialog-key"
#define SOLI_LAST_SEARCH_DATA_KEY	"soli-last-search-data-key"

typedef struct _LastSearchData LastSearchData;
struct _LastSearchData
{
	gint x;
	gint y;
};

static void
last_search_data_free (LastSearchData *data)
{
	g_slice_free (LastSearchData, data);
}

static void
last_search_data_restore_position (SoliReplaceDialog *dlg)
{
	LastSearchData *data;

	data = g_object_get_data (G_OBJECT (dlg), SOLI_LAST_SEARCH_DATA_KEY);

	if (data != NULL)
	{
		gtk_window_move (GTK_WINDOW (dlg),
				 data->x,
				 data->y);
	}
}

static void
last_search_data_store_position (SoliReplaceDialog *dlg)
{
	LastSearchData *data;

	data = g_object_get_data (G_OBJECT (dlg), SOLI_LAST_SEARCH_DATA_KEY);

	if (data == NULL)
	{
		data = g_slice_new (LastSearchData);

		g_object_set_data_full (G_OBJECT (dlg),
					SOLI_LAST_SEARCH_DATA_KEY,
					data,
					(GDestroyNotify) last_search_data_free);
	}

	gtk_window_get_position (GTK_WINDOW (dlg),
				 &data->x,
				 &data->y);
}

/* Use occurences only for Replace All */
static void
text_found (SoliWindow *window,
	    gint         occurrences)
{
	if (occurrences > 1)
	{
		soli_statusbar_flash_message (SOLI_STATUSBAR (window->priv->statusbar),
					       window->priv->generic_message_cid,
					       ngettext("Found and replaced %d occurrence",
					     	        "Found and replaced %d occurrences",
					     	        occurrences),
					       occurrences);
	}
	else if (occurrences == 1)
	{
		soli_statusbar_flash_message (SOLI_STATUSBAR (window->priv->statusbar),
					       window->priv->generic_message_cid,
					       _("Found and replaced one occurrence"));
	}
	else
	{
		soli_statusbar_flash_message (SOLI_STATUSBAR (window->priv->statusbar),
					       window->priv->generic_message_cid,
					       " ");
	}
}

#define MAX_MSG_LENGTH 40

static void
text_not_found (SoliWindow        *window,
		SoliReplaceDialog *replace_dialog)
{
	const gchar *search_text;
	gchar *truncated_text;

	search_text = soli_replace_dialog_get_search_text (replace_dialog);
	truncated_text = soli_utils_str_end_truncate (search_text, MAX_MSG_LENGTH);

	soli_statusbar_flash_message (SOLI_STATUSBAR (window->priv->statusbar),
				       window->priv->generic_message_cid,
				       /* Translators: %s is replaced by the text
				          entered by the user in the search box */
				       _("\"%s\" not found"), truncated_text);

	g_free (truncated_text);
}

static void
finish_search_from_dialog (SoliWindow *window,
			   gboolean     found)
{
	SoliReplaceDialog *replace_dialog;

	replace_dialog = g_object_get_data (G_OBJECT (window), SOLI_REPLACE_DIALOG_KEY);

	g_return_if_fail (replace_dialog != NULL);

	if (found)
	{
		text_found (window, 0);
	}
	else
	{
		text_not_found (window, replace_dialog);
	}
}

static gboolean
forward_search_finished (GtkSourceSearchContext *search_context,
			 GAsyncResult           *result,
			 SoliView              *view)
{
	gboolean found;
	GtkSourceBuffer *buffer;
	GtkTextIter match_start;
	GtkTextIter match_end;

	found = gtk_source_search_context_forward_finish (search_context,
							  result,
							  &match_start,
							  &match_end,
							  NULL);

	buffer = gtk_source_search_context_get_buffer (search_context);

	if (found)
	{
		gtk_text_buffer_select_range (GTK_TEXT_BUFFER (buffer),
					      &match_start,
					      &match_end);

		soli_view_scroll_to_cursor (view);
	}
	else
	{
		GtkTextIter end_selection;

		gtk_text_buffer_get_selection_bounds (GTK_TEXT_BUFFER (buffer),
						      NULL,
						      &end_selection);

		gtk_text_buffer_select_range (GTK_TEXT_BUFFER (buffer),
					      &end_selection,
					      &end_selection);
	}

	return found;
}

static void
forward_search_from_dialog_finished (GtkSourceSearchContext *search_context,
				     GAsyncResult           *result,
				     SoliWindow            *window)
{
	SoliView *view = soli_window_get_active_view (window);
	gboolean found;

	if (view == NULL)
	{
		return;
	}

	found = forward_search_finished (search_context, result, view);

	finish_search_from_dialog (window, found);
}

static void
run_forward_search (SoliWindow *window,
		    gboolean     from_dialog)
{
	SoliView *view;
	GtkTextBuffer *buffer;
	GtkTextIter start_at;
	GtkSourceSearchContext *search_context;

	view = soli_window_get_active_view (window);

	if (view == NULL)
	{
		return;
	}

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	search_context = soli_document_get_search_context (SOLI_DOCUMENT (buffer));

	if (search_context == NULL)
	{
		return;
	}

	gtk_text_buffer_get_selection_bounds (buffer, NULL, &start_at);

	if (from_dialog)
	{
		gtk_source_search_context_forward_async (search_context,
							 &start_at,
							 NULL,
							 (GAsyncReadyCallback)forward_search_from_dialog_finished,
							 window);
	}
	else
	{
		gtk_source_search_context_forward_async (search_context,
							 &start_at,
							 NULL,
							 (GAsyncReadyCallback)forward_search_finished,
							 view);
	}
}

static gboolean
backward_search_finished (GtkSourceSearchContext *search_context,
			  GAsyncResult           *result,
			  SoliView              *view)
{
	gboolean found;
	GtkTextIter match_start;
	GtkTextIter match_end;
	GtkSourceBuffer *buffer;

	found = gtk_source_search_context_backward_finish (search_context,
							   result,
							   &match_start,
							   &match_end,
							   NULL);

	buffer = gtk_source_search_context_get_buffer (search_context);

	if (found)
	{
		gtk_text_buffer_select_range (GTK_TEXT_BUFFER (buffer),
					      &match_start,
					      &match_end);

		soli_view_scroll_to_cursor (view);
	}
	else
	{
		GtkTextIter start_selection;

		gtk_text_buffer_get_selection_bounds (GTK_TEXT_BUFFER (buffer),
						      &start_selection,
						      NULL);

		gtk_text_buffer_select_range (GTK_TEXT_BUFFER (buffer),
					      &start_selection,
					      &start_selection);
	}

	return found;
}

static void
backward_search_from_dialog_finished (GtkSourceSearchContext *search_context,
				      GAsyncResult           *result,
				      SoliWindow            *window)
{
	SoliView *view = soli_window_get_active_view (window);
	gboolean found;

	if (view == NULL)
	{
		return;
	}

	found = backward_search_finished (search_context, result, view);

	finish_search_from_dialog (window, found);
}

static void
run_backward_search (SoliWindow *window,
		     gboolean     from_dialog)
{
	SoliView *view;
	GtkTextBuffer *buffer;
	GtkTextIter start_at;
	GtkSourceSearchContext *search_context;

	view = soli_window_get_active_view (window);

	if (view == NULL)
	{
		return;
	}

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	search_context = soli_document_get_search_context (SOLI_DOCUMENT (buffer));

	if (search_context == NULL)
	{
		return;
	}

	gtk_text_buffer_get_selection_bounds (buffer, &start_at, NULL);

	if (from_dialog)
	{
		gtk_source_search_context_backward_async (search_context,
							  &start_at,
							  NULL,
							  (GAsyncReadyCallback)backward_search_from_dialog_finished,
							  window);
	}
	else
	{
		gtk_source_search_context_backward_async (search_context,
							  &start_at,
							  NULL,
							  (GAsyncReadyCallback)backward_search_finished,
							  view);
	}
}

static void
do_find (SoliReplaceDialog *dialog,
	 SoliWindow        *window)
{
	if (soli_replace_dialog_get_backwards (dialog))
	{
		run_backward_search (window, TRUE);
	}
	else
	{
		run_forward_search (window, TRUE);
	}
}

static void
do_replace (SoliReplaceDialog *dialog,
	    SoliWindow        *window)
{
	SoliDocument *doc;
	GtkSourceSearchContext *search_context;
	const gchar *replace_entry_text;
	gchar *unescaped_replace_text;
	GtkTextIter start;
	GtkTextIter end;
	GError *error = NULL;

	doc = soli_window_get_active_document (window);

	if (doc == NULL)
	{
		return;
	}

	search_context = soli_document_get_search_context (doc);

	if (search_context == NULL)
	{
		return;
	}

	/* replace text may be "", we just delete */
	replace_entry_text = soli_replace_dialog_get_replace_text (dialog);
	g_return_if_fail (replace_entry_text != NULL);

	unescaped_replace_text = gtk_source_utils_unescape_search_text (replace_entry_text);

	gtk_text_buffer_get_selection_bounds (GTK_TEXT_BUFFER (doc), &start, &end);

	gtk_source_search_context_replace (search_context,
					   &start,
					   &end,
					   unescaped_replace_text,
					   -1,
					   &error);

	g_free (unescaped_replace_text);

	if (error != NULL)
	{
		soli_replace_dialog_set_replace_error (dialog, error->message);
		g_error_free (error);
	}

	do_find (dialog, window);
}

static void
do_replace_all (SoliReplaceDialog *dialog,
		SoliWindow        *window)
{
	SoliView *view;
	GtkSourceSearchContext *search_context;
	GtkTextBuffer *buffer;
	GtkSourceCompletion *completion;
	const gchar *replace_entry_text;
	gchar *unescaped_replace_text;
	gint count;
	GError *error = NULL;

	view = soli_window_get_active_view (window);

	if (view == NULL)
	{
		return;
	}

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	search_context = soli_document_get_search_context (SOLI_DOCUMENT (buffer));

	if (search_context == NULL)
	{
		return;
	}

	/* FIXME: this should really be done automatically in gtksoureview, but
	 * it is an important performance fix, so let's do it here for now.
	 */
	completion = gtk_source_view_get_completion (GTK_SOURCE_VIEW (view));
	gtk_source_completion_block_interactive (completion);

	/* replace text may be "", we just delete all occurrences */
	replace_entry_text = soli_replace_dialog_get_replace_text (dialog);
	g_return_if_fail (replace_entry_text != NULL);

	unescaped_replace_text = gtk_source_utils_unescape_search_text (replace_entry_text);

	count = gtk_source_search_context_replace_all (search_context,
						       unescaped_replace_text,
						       -1,
						       &error);

	g_free (unescaped_replace_text);

	gtk_source_completion_unblock_interactive (completion);

	if (count > 0)
	{
		text_found (window, count);
	}
	else if (error == NULL)
	{
		text_not_found (window, dialog);
	}

	if (error != NULL)
	{
		soli_replace_dialog_set_replace_error (dialog, error->message);
		g_error_free (error);
	}
}

static void
replace_dialog_response_cb (SoliReplaceDialog *dialog,
			    gint                response_id,
			    SoliWindow        *window)
{
	soli_debug (DEBUG_COMMANDS);

	switch (response_id)
	{
		case SOLI_REPLACE_DIALOG_FIND_RESPONSE:
			do_find (dialog, window);
			break;

		case SOLI_REPLACE_DIALOG_REPLACE_RESPONSE:
			do_replace (dialog, window);
			break;

		case SOLI_REPLACE_DIALOG_REPLACE_ALL_RESPONSE:
			do_replace_all (dialog, window);
			break;

		default:
			last_search_data_store_position (dialog);
			gtk_widget_hide (GTK_WIDGET (dialog));
	}
}

static void
replace_dialog_destroyed (SoliWindow        *window,
			  SoliReplaceDialog *dialog)
{
	soli_debug (DEBUG_COMMANDS);

	g_object_set_data (G_OBJECT (window),
			   SOLI_REPLACE_DIALOG_KEY,
			   NULL);
	g_object_set_data (G_OBJECT (dialog),
			   SOLI_LAST_SEARCH_DATA_KEY,
			   NULL);
}

static GtkWidget *
create_dialog (SoliWindow *window)
{
	GtkWidget *dialog = soli_replace_dialog_new (window);

	g_signal_connect (dialog,
			  "response",
			  G_CALLBACK (replace_dialog_response_cb),
			  window);

	g_object_set_data (G_OBJECT (window),
			   SOLI_REPLACE_DIALOG_KEY,
			   dialog);

	g_object_weak_ref (G_OBJECT (dialog),
			   (GWeakNotify) replace_dialog_destroyed,
			   window);

	return dialog;
}

void
_soli_cmd_search_find (GSimpleAction *action,
                        GVariant      *parameter,
                        gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliTab *active_tab;
	SoliViewFrame *frame;

	soli_debug (DEBUG_COMMANDS);

	active_tab = soli_window_get_active_tab (window);

	if (active_tab == NULL)
	{
		return;
	}

	frame = _soli_tab_get_view_frame (active_tab);
	soli_view_frame_popup_search (frame);
}

void
_soli_cmd_search_replace (GSimpleAction *action,
                           GVariant      *parameter,
                           gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	gpointer data;
	GtkWidget *replace_dialog;

	soli_debug (DEBUG_COMMANDS);

	data = g_object_get_data (G_OBJECT (window), SOLI_REPLACE_DIALOG_KEY);

	if (data == NULL)
	{
		replace_dialog = create_dialog (window);
	}
	else
	{
		g_return_if_fail (SOLI_IS_REPLACE_DIALOG (data));

		replace_dialog = GTK_WIDGET (data);
	}

	gtk_widget_show (replace_dialog);
	last_search_data_restore_position (SOLI_REPLACE_DIALOG (replace_dialog));
	soli_replace_dialog_present_with_time (SOLI_REPLACE_DIALOG (replace_dialog),
					        GDK_CURRENT_TIME);
}

void
_soli_cmd_search_find_next (GSimpleAction *action,
                             GVariant      *parameter,
                             gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);

	soli_debug (DEBUG_COMMANDS);

	run_forward_search (window, FALSE);
}

void
_soli_cmd_search_find_prev (GSimpleAction *action,
                             GVariant      *parameter,
                             gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);

	soli_debug (DEBUG_COMMANDS);

	run_backward_search (window, FALSE);
}

void
_soli_cmd_search_clear_highlight (GSimpleAction *action,
                                   GVariant      *parameter,
                                   gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliTab *active_tab;
	SoliViewFrame *frame;
	SoliDocument *doc;

	soli_debug (DEBUG_COMMANDS);

	active_tab = soli_window_get_active_tab (window);

	if (active_tab == NULL)
	{
		return;
	}

	frame = _soli_tab_get_view_frame (active_tab);
	soli_view_frame_clear_search (frame);

	doc = soli_tab_get_document (active_tab);
	soli_document_set_search_context (doc, NULL);
}

void
_soli_cmd_search_goto_line (GSimpleAction *action,
                             GVariant      *parameter,
                             gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliTab *active_tab;
	SoliViewFrame *frame;

	soli_debug (DEBUG_COMMANDS);

	active_tab = soli_window_get_active_tab (window);

	if (active_tab == NULL)
	{
		return;
	}

	frame = _soli_tab_get_view_frame (active_tab);
	soli_view_frame_popup_goto_line (frame);
}

/* ex:set ts=8 noet: */
