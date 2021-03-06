/*
 * soli-highlight-mode-dialog.c
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

#include "soli-highlight-mode-dialog.h"

#include <gtk/gtk.h>

struct _SoliHighlightModeDialog
{
	GtkDialog parent_instance;

	SoliHighlightModeSelector *selector;
	gulong on_language_selected_id;
};

G_DEFINE_TYPE (SoliHighlightModeDialog, soli_highlight_mode_dialog, GTK_TYPE_DIALOG)

static void
soli_highlight_mode_dialog_response (GtkDialog *dialog,
                                      gint       response_id)
{
	SoliHighlightModeDialog *dlg = SOLI_HIGHLIGHT_MODE_DIALOG (dialog);

	if (response_id == GTK_RESPONSE_OK)
	{
		g_signal_handler_block (dlg->selector, dlg->on_language_selected_id);
		soli_highlight_mode_selector_activate_selected_language (dlg->selector);
		g_signal_handler_unblock (dlg->selector, dlg->on_language_selected_id);
	}

	gtk_widget_destroy (GTK_WIDGET (dialog));
}

static void
on_language_selected (SoliHighlightModeSelector *sel,
                      GtkSourceLanguage          *language,
                      SoliHighlightModeDialog   *dlg)
{
	g_signal_handler_block (dlg->selector, dlg->on_language_selected_id);
	soli_highlight_mode_selector_activate_selected_language (dlg->selector);
	g_signal_handler_unblock (dlg->selector, dlg->on_language_selected_id);

	gtk_widget_destroy (GTK_WIDGET (dlg));
}

static void
soli_highlight_mode_dialog_class_init (SoliHighlightModeDialogClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	GtkDialogClass *dialog_class = GTK_DIALOG_CLASS (klass);

	dialog_class->response = soli_highlight_mode_dialog_response;

	/* Bind class to template */
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/ca/dluco/soli/ui/soli-highlight-mode-dialog.ui");
	gtk_widget_class_bind_template_child (widget_class, SoliHighlightModeDialog, selector);
}

static void
soli_highlight_mode_dialog_init (SoliHighlightModeDialog *dlg)
{
	gtk_widget_init_template (GTK_WIDGET (dlg));
	gtk_dialog_set_default_response (GTK_DIALOG (dlg), GTK_RESPONSE_OK);

	dlg->on_language_selected_id = g_signal_connect (dlg->selector, "language-selected",
	                                                 G_CALLBACK (on_language_selected), dlg);
}

GtkWidget *
soli_highlight_mode_dialog_new (GtkWindow *parent)
{
	return GTK_WIDGET (g_object_new (SOLI_TYPE_HIGHLIGHT_MODE_DIALOG,
	                                 "transient-for", parent,
	                                 "use-header-bar", TRUE,
	                                 NULL));
}

SoliHighlightModeSelector *
soli_highlight_mode_dialog_get_selector (SoliHighlightModeDialog *dlg)
{
	g_return_val_if_fail (SOLI_IS_HIGHLIGHT_MODE_DIALOG (dlg), NULL);

	return dlg->selector;
}

/* ex:set ts=8 noet: */
