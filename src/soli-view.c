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

#include <libpeas/peas.h>

#include "soli-view-activatable.h"
#include "soli-document.h"
#include "soli-settings.h"
#include "soli-plugins-engine.h"

struct _SoliViewPrivate
{
	GSettings *settings;
	PeasExtensionSet *extensions;
};

G_DEFINE_TYPE_WITH_PRIVATE (SoliView, soli_view, GTK_SOURCE_TYPE_VIEW);

static GtkTextBuffer *soli_view_create_buffer (GtkTextView *text_view);

static void
soli_view_init (SoliView *view)
{
	view->priv = soli_view_get_instance_private (view);

	view->priv->settings = g_settings_new ("ca.dluco.soli.preferences");

	view->priv->extensions = peas_extension_set_new (PEAS_ENGINE (soli_plugins_engine_get_default ()),
								SOLI_TYPE_VIEW_ACTIVATABLE,
								"view", view,
								NULL);
}

static void
soli_view_dispose (GObject *object)
{
	SoliView *view = SOLI_VIEW (object);

	g_clear_object (&view->priv->settings);
	g_clear_object (&view->priv->extensions);

	G_OBJECT_CLASS (soli_view_parent_class)->dispose (object);
}

static void
soli_view_constructed (GObject *object)
{
	SoliView *view = SOLI_VIEW (object);
	SoliViewPrivate *priv = view->priv;

	g_settings_bind (priv->settings,
					 SOLI_SETTINGS_SHOW_LINE_NUMBERS,
					 view,
					 "show-line-numbers",
					 G_SETTINGS_BIND_GET);

	g_settings_bind (priv->settings,
					 SOLI_SETTINGS_HIGHLIGHT_CURRENT_LINE,
					 view,
					 "highlight-current-line",
					 G_SETTINGS_BIND_GET);

	g_settings_bind (priv->settings,
					 SOLI_SETTINGS_WRAP_MODE,
					 view,
					 "wrap-mode",
					 G_SETTINGS_BIND_GET);

	G_OBJECT_CLASS (soli_view_parent_class)->constructed (object);
}

static void
on_extension_added (PeasExtensionSet *extensions,
					PeasPluginInfo *info,
					PeasExtension *extension,
					SoliView *view)
{
	soli_view_activatable_activate (SOLI_VIEW_ACTIVATABLE (extension));
}

static void
on_extension_removed (PeasExtensionSet *extensions,
					PeasPluginInfo *info,
					PeasExtension *extension,
					SoliView *view)
{
	soli_view_activatable_deactivate (SOLI_VIEW_ACTIVATABLE (extension));
}

static void
soli_view_realize (GtkWidget *widget)
{
	SoliView *view = SOLI_VIEW (widget);

	GTK_WIDGET_CLASS (soli_view_parent_class)->realize (widget);

	g_signal_connect (view->priv->extensions,
						"extension-added",
						G_CALLBACK (on_extension_added),
						view);

	g_signal_connect (view->priv->extensions,
						"extension-removed",
						G_CALLBACK (on_extension_removed),
						view);
	/* We only activate the extensions when the view is realized,
	 * because most plugins will expect this behaviour, and we won't
	 * change the buffer later anyways.
	 */
	peas_extension_set_foreach (view->priv->extensions,
								(PeasExtensionSetForeachFunc) on_extension_added,
								view);
}

static void
soli_view_unrealize (GtkWidget *widget)
{
	SoliView *view = SOLI_VIEW (widget);

	g_signal_handlers_disconnect_by_func (view->priv->extensions,
										on_extension_added,
										view);

	g_signal_handlers_disconnect_by_func (view->priv->extensions,
										on_extension_removed,
										view);

	/* We need to deactivate the extension on unrealize because it is not
	 * mandatory that a view has been realized when it is dispose, leading
	 * to deactivating the plugin without being first activated.
	 */
	peas_extension_set_foreach (view->priv->extensions,
								(PeasExtensionSetForeachFunc) on_extension_removed,
								view);

	GTK_WIDGET_CLASS (soli_view_parent_class)->unrealize (widget);
}

static void
soli_view_class_init (SoliViewClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	GtkTextViewClass *text_view_class = GTK_TEXT_VIEW_CLASS (klass);

	object_class->dispose = soli_view_dispose;
	object_class->constructed = soli_view_constructed;

	widget_class->realize = soli_view_realize;
	widget_class->unrealize = soli_view_unrealize;
	
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

void
soli_view_cut_clipboard (SoliView *view)
{
	GtkTextBuffer *buffer;
	GtkClipboard *clipboard;

	g_return_if_fail (SOLI_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	clipboard = gtk_widget_get_clipboard (GTK_WIDGET (view),
										GDK_SELECTION_CLIPBOARD);

	gtk_text_buffer_cut_clipboard (buffer,
								clipboard,
								gtk_text_view_get_editable (GTK_TEXT_VIEW (view)));

	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (view),
								gtk_text_buffer_get_insert (buffer),
								0.02, // FIXME
								FALSE,
								0.0,
								0.0);
}

void
soli_view_copy_clipboard (SoliView *view)
{
	GtkTextBuffer *buffer;
	GtkClipboard *clipboard;

	g_return_if_fail (SOLI_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	clipboard = gtk_widget_get_clipboard (GTK_WIDGET (view),
										GDK_SELECTION_CLIPBOARD);

	gtk_text_buffer_copy_clipboard (buffer, clipboard);
}

void
soli_view_paste_clipboard (SoliView *view)
{
	GtkTextBuffer *buffer;
	GtkClipboard *clipboard;

	g_return_if_fail (SOLI_IS_VIEW (view));

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	clipboard = gtk_widget_get_clipboard (GTK_WIDGET (view),
										GDK_SELECTION_CLIPBOARD);

	gtk_text_buffer_paste_clipboard (buffer,
								clipboard,
								NULL,
								gtk_text_view_get_editable (GTK_TEXT_VIEW (view)));

	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (view),
								gtk_text_buffer_get_insert (buffer),
								0.02, // FIXME
								FALSE,
								0.0,
								0.0);
}
