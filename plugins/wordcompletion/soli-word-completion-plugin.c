/*
 * Copyright (C) 2009 Ignacio Casal Quinteiro <icq@gnome.org>
 *               2009 Jesse van den Kieboom <jesse@gnome.org>
 *               2013 Sébastien Wilmet <swilmet@gnome.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "soli-word-completion-plugin.h"

#include <soli-debug.h>
#include <soli-window.h>
#include <soli-window-activatable.h>
#include <soli-view.h>
#include <soli-view-activatable.h>
#include <libpeas-gtk/peas-gtk-configurable.h>
#include <gtksourceview/gtksource.h>
#include <gtksourceview/completion-providers/words/gtksourcecompletionwords.h>

#define WINDOW_PROVIDER "SoliWordCompletionPluginProvider"

#define WORD_COMPLETION_SETTINGS_BASE "ca.dluco.soli.plugins.wordcompletion"
#define SETTINGS_KEY_INTERACTIVE_COMPLETION "interactive-completion"
#define SETTINGS_KEY_MINIMUM_WORD_SIZE "minimum-word-size"

static void soli_window_activatable_iface_init (SoliWindowActivatableInterface *iface);
static void soli_view_activatable_iface_init (SoliViewActivatableInterface *iface);
static void peas_gtk_configurable_iface_init (PeasGtkConfigurableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (SoliWordCompletionPlugin,
                                soli_word_completion_plugin,
                                PEAS_TYPE_EXTENSION_BASE,
                                0,
                                G_IMPLEMENT_INTERFACE_DYNAMIC (SOLI_TYPE_WINDOW_ACTIVATABLE,
                                                               soli_window_activatable_iface_init)
                                G_IMPLEMENT_INTERFACE_DYNAMIC (SOLI_TYPE_VIEW_ACTIVATABLE,
                                                               soli_view_activatable_iface_init)
                                G_IMPLEMENT_INTERFACE_DYNAMIC (PEAS_GTK_TYPE_CONFIGURABLE,
                                                               peas_gtk_configurable_iface_init))

struct _SoliWordCompletionPluginPrivate
{
	GtkWidget *window;
	SoliView *view;
	GtkSourceCompletionProvider *provider;
};

enum
{
	PROP_0,
	PROP_WINDOW,
	PROP_VIEW
};

static void
soli_word_completion_plugin_init (SoliWordCompletionPlugin *plugin)
{
	soli_debug_message (DEBUG_PLUGINS, "SoliWordCompletionPlugin initializing");

	plugin->priv = G_TYPE_INSTANCE_GET_PRIVATE (plugin,
	                                            SOLI_TYPE_WORD_COMPLETION_PLUGIN,
	                                            SoliWordCompletionPluginPrivate);
}

static void
soli_word_completion_plugin_dispose (GObject *object)
{
	SoliWordCompletionPlugin *plugin = SOLI_WORD_COMPLETION_PLUGIN (object);

	if (plugin->priv->window != NULL)
	{
		g_object_unref (plugin->priv->window);
		plugin->priv->window = NULL;
	}

	if (plugin->priv->view != NULL)
	{
		g_object_unref (plugin->priv->view);
		plugin->priv->view = NULL;
	}

	if (plugin->priv->provider != NULL)
	{
		g_object_unref (plugin->priv->provider);
		plugin->priv->provider = NULL;
	}

	G_OBJECT_CLASS (soli_word_completion_plugin_parent_class)->dispose (object);
}

static void
soli_word_completion_plugin_set_property (GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
	SoliWordCompletionPlugin *plugin = SOLI_WORD_COMPLETION_PLUGIN (object);

	switch (prop_id)
	{
		case PROP_WINDOW:
			plugin->priv->window = g_value_dup_object (value);
			break;
		case PROP_VIEW:
			plugin->priv->view = SOLI_VIEW (g_value_dup_object (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
soli_word_completion_plugin_get_property (GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
	SoliWordCompletionPlugin *plugin = SOLI_WORD_COMPLETION_PLUGIN (object);

	switch (prop_id)
	{
		case PROP_WINDOW:
			g_value_set_object (value, plugin->priv->window);
			break;
		case PROP_VIEW:
			g_value_set_object (value, plugin->priv->view);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
update_activation (GtkSourceCompletionWords *provider,
		   GSettings                *settings)
{
	GtkSourceCompletionActivation activation;

	g_object_get (provider, "activation", &activation, NULL);

	if (g_settings_get_boolean (settings, SETTINGS_KEY_INTERACTIVE_COMPLETION))
	{
		activation |= GTK_SOURCE_COMPLETION_ACTIVATION_INTERACTIVE;
	}
	else
	{
		activation &= ~GTK_SOURCE_COMPLETION_ACTIVATION_INTERACTIVE;
	}

	g_object_set (provider, "activation", activation, NULL);
}

static void
on_interactive_completion_changed_cb (GSettings                *settings,
				      gchar                    *key,
				      GtkSourceCompletionWords *provider)
{
	update_activation (provider, settings);
}

static GtkSourceCompletionWords *
create_provider (void)
{
	GtkSourceCompletionWords *provider;
	GSettings *settings;

	provider = gtk_source_completion_words_new ("Document Words", NULL);

	settings = g_settings_new (WORD_COMPLETION_SETTINGS_BASE);

	g_settings_bind (settings, SETTINGS_KEY_MINIMUM_WORD_SIZE,
			 provider, "minimum-word-size",
			 G_SETTINGS_BIND_GET);

	update_activation (provider, settings);

	g_signal_connect_object (settings,
				 "changed::" SETTINGS_KEY_INTERACTIVE_COMPLETION,
				 G_CALLBACK (on_interactive_completion_changed_cb),
				 provider,
				 0);

	g_object_unref (settings);

	return provider;
}

static void
soli_word_completion_window_activate (SoliWindowActivatable *activatable)
{
	SoliWordCompletionPluginPrivate *priv;
	GtkSourceCompletionWords *provider;

	soli_debug (DEBUG_PLUGINS);

	priv = SOLI_WORD_COMPLETION_PLUGIN (activatable)->priv;

	provider = create_provider ();

	g_object_set_data_full (G_OBJECT (priv->window),
	                        WINDOW_PROVIDER,
	                        provider,
	                        (GDestroyNotify)g_object_unref);
}

static void
soli_word_completion_window_deactivate (SoliWindowActivatable *activatable)
{
	SoliWordCompletionPluginPrivate *priv;

	soli_debug (DEBUG_PLUGINS);

	priv = SOLI_WORD_COMPLETION_PLUGIN (activatable)->priv;

	g_object_set_data (G_OBJECT (priv->window), WINDOW_PROVIDER, NULL);
}

static void
soli_word_completion_view_activate (SoliViewActivatable *activatable)
{
	SoliWordCompletionPluginPrivate *priv;
	GtkSourceCompletion *completion;
	GtkSourceCompletionProvider *provider;
	GtkTextBuffer *buf;

	soli_debug (DEBUG_PLUGINS);

	priv = SOLI_WORD_COMPLETION_PLUGIN (activatable)->priv;

	priv->window = gtk_widget_get_toplevel (GTK_WIDGET (priv->view));

	/* We are disposing the window */
	g_object_ref (priv->window);

	completion = gtk_source_view_get_completion (GTK_SOURCE_VIEW (priv->view));
	buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->view));

	provider = g_object_get_data (G_OBJECT (priv->window), WINDOW_PROVIDER);

	if (provider == NULL)
	{
		/* Standalone provider */
		provider = GTK_SOURCE_COMPLETION_PROVIDER (create_provider ());
	}

	priv->provider = g_object_ref (provider);

	gtk_source_completion_add_provider (completion, provider, NULL);
	gtk_source_completion_words_register (GTK_SOURCE_COMPLETION_WORDS (provider),
	                                      buf);
}

static void
soli_word_completion_view_deactivate (SoliViewActivatable *activatable)
{
	SoliWordCompletionPluginPrivate *priv;
	GtkSourceCompletion *completion;
	GtkSourceCompletionProvider *provider;
	GtkTextBuffer *buf;

	soli_debug (DEBUG_PLUGINS);

	priv = SOLI_WORD_COMPLETION_PLUGIN (activatable)->priv;

	completion = gtk_source_view_get_completion (GTK_SOURCE_VIEW (priv->view));
	buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->view));

	gtk_source_completion_remove_provider (completion,
	                                       priv->provider,
	                                       NULL);

	gtk_source_completion_words_unregister (GTK_SOURCE_COMPLETION_WORDS (priv->provider),
	                                        buf);
}

static GtkWidget *
soli_word_completion_create_configure_widget (PeasGtkConfigurable *configurable)
{
	GtkBuilder *builder;
	GtkWidget *content;
	GtkWidget *interactive_completion;
	GtkWidget *min_word_size;
	GSettings *settings;
	GError *error = NULL;

	builder = gtk_builder_new ();
	gtk_builder_add_from_resource (builder,
				       "/ca/dluco/soli/plugins/wordcompletion/ui/soli-word-completion-configure.ui",
				       &error);

	if (error != NULL)
	{
		g_warning ("Word Completion configuration widget: %s", error->message);
		g_error_free (error);
		error = NULL;
	}

	content = GTK_WIDGET (gtk_builder_get_object (builder, "content"));
	g_object_ref (content);

	interactive_completion = GTK_WIDGET (gtk_builder_get_object (builder, "check_button_interactive_completion"));
	min_word_size = GTK_WIDGET (gtk_builder_get_object (builder, "spin_button_min_word_size"));

	g_object_unref (builder);

	settings = g_settings_new (WORD_COMPLETION_SETTINGS_BASE);

	g_settings_bind (settings, SETTINGS_KEY_INTERACTIVE_COMPLETION,
			 interactive_completion, "active",
			 G_SETTINGS_BIND_DEFAULT | G_SETTINGS_BIND_GET_NO_CHANGES);

	g_settings_bind (settings, SETTINGS_KEY_MINIMUM_WORD_SIZE,
			 min_word_size, "value",
			 G_SETTINGS_BIND_DEFAULT | G_SETTINGS_BIND_GET_NO_CHANGES);

	g_object_unref (settings);

	return content;
}

static void
soli_word_completion_plugin_class_init (SoliWordCompletionPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = soli_word_completion_plugin_dispose;
	object_class->set_property = soli_word_completion_plugin_set_property;
	object_class->get_property = soli_word_completion_plugin_get_property;

	g_object_class_override_property (object_class, PROP_WINDOW, "window");
	g_object_class_override_property (object_class, PROP_VIEW, "view");

	g_type_class_add_private (klass, sizeof (SoliWordCompletionPluginPrivate));
}

static void
soli_word_completion_plugin_class_finalize (SoliWordCompletionPluginClass *klass)
{
}

static void
soli_window_activatable_iface_init (SoliWindowActivatableInterface *iface)
{
	iface->activate = soli_word_completion_window_activate;
	iface->deactivate = soli_word_completion_window_deactivate;
}

static void
soli_view_activatable_iface_init (SoliViewActivatableInterface *iface)
{
	iface->activate = soli_word_completion_view_activate;
	iface->deactivate = soli_word_completion_view_deactivate;
}

static void
peas_gtk_configurable_iface_init (PeasGtkConfigurableInterface *iface)
{
	iface->create_configure_widget = soli_word_completion_create_configure_widget;
}

G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
	soli_word_completion_plugin_register_type (G_TYPE_MODULE (module));

	peas_object_module_register_extension_type (module,
	                                            SOLI_TYPE_WINDOW_ACTIVATABLE,
	                                            SOLI_TYPE_WORD_COMPLETION_PLUGIN);

	peas_object_module_register_extension_type (module,
	                                            SOLI_TYPE_VIEW_ACTIVATABLE,
	                                            SOLI_TYPE_WORD_COMPLETION_PLUGIN);

	peas_object_module_register_extension_type (module,
	                                            PEAS_GTK_TYPE_CONFIGURABLE,
	                                            SOLI_TYPE_WORD_COMPLETION_PLUGIN);
}
