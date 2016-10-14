/*
 * soli-modeline-plugin.c
 * Emacs, Kate and Vim-style modelines support for soli.
 *
 * Copyright (C) 2005-2010 - Steve Frécinaux <code@istique.net>
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
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//#include <glib/gi18n-lib.h>
#include <gmodule.h>
#include "soli-modeline-plugin.h"
#include "modeline-parser.h"

#include <soli-debug.h>
#include <soli-view-activatable.h>
#include <soli-view.h>

struct _SoliModelinePluginPrivate
{
	SoliView *view;

	gulong document_loaded_handler_id;
	gulong document_saved_handler_id;
};

enum
{
	PROP_0,
	PROP_VIEW
};

static void	soli_view_activatable_iface_init (SoliViewActivatableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (SoliModelinePlugin,
				soli_modeline_plugin,
				PEAS_TYPE_EXTENSION_BASE,
				0,
				G_IMPLEMENT_INTERFACE_DYNAMIC (SOLI_TYPE_VIEW_ACTIVATABLE,
							       soli_view_activatable_iface_init)
				G_ADD_PRIVATE_DYNAMIC (SoliModelinePlugin))

static void
soli_modeline_plugin_constructed (GObject *object)
{
	gchar *data_dir;

	data_dir = peas_extension_base_get_data_dir (PEAS_EXTENSION_BASE (object));

	modeline_parser_init (data_dir);

	g_free (data_dir);

	G_OBJECT_CLASS (soli_modeline_plugin_parent_class)->constructed (object);
}

static void
soli_modeline_plugin_init (SoliModelinePlugin *plugin)
{
	soli_debug_message (DEBUG_PLUGINS, "SoliModelinePlugin initializing");

	plugin->priv = soli_modeline_plugin_get_instance_private (plugin);

}

static void
soli_modeline_plugin_dispose (GObject *object)
{
	SoliModelinePlugin *plugin = SOLI_MODELINE_PLUGIN (object);

	soli_debug_message (DEBUG_PLUGINS, "SoliModelinePlugin disposing");

	g_clear_object (&plugin->priv->view);

	G_OBJECT_CLASS (soli_modeline_plugin_parent_class)->dispose (object);
}

static void
soli_modeline_plugin_finalize (GObject *object)
{
	soli_debug_message (DEBUG_PLUGINS, "SoliModelinePlugin finalizing");

	modeline_parser_shutdown ();

	G_OBJECT_CLASS (soli_modeline_plugin_parent_class)->finalize (object);
}

static void
soli_modeline_plugin_set_property (GObject      *object,
                                    guint         prop_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
	SoliModelinePlugin *plugin = SOLI_MODELINE_PLUGIN (object);

	switch (prop_id)
	{
		case PROP_VIEW:
			plugin->priv->view = SOLI_VIEW (g_value_dup_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
soli_modeline_plugin_get_property (GObject    *object,
                                    guint       prop_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
	SoliModelinePlugin *plugin = SOLI_MODELINE_PLUGIN (object);

	switch (prop_id)
	{
		case PROP_VIEW:
			g_value_set_object (value, plugin->priv->view);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
on_document_loaded_or_saved (SoliDocument *document,
			     GtkSourceView *view)
{
	modeline_parser_apply_modeline (view);
}

static void
soli_modeline_plugin_activate (SoliViewActivatable *activatable)
{
	SoliModelinePlugin *plugin;
	GtkTextBuffer *doc;

	soli_debug (DEBUG_PLUGINS);

	plugin = SOLI_MODELINE_PLUGIN (activatable);

	modeline_parser_apply_modeline (GTK_SOURCE_VIEW (plugin->priv->view));

	doc = gtk_text_view_get_buffer (GTK_TEXT_VIEW (plugin->priv->view));

	plugin->priv->document_loaded_handler_id =
		g_signal_connect (doc, "loaded",
				  G_CALLBACK (on_document_loaded_or_saved),
				  plugin->priv->view);
	plugin->priv->document_saved_handler_id =
		g_signal_connect (doc, "saved",
				  G_CALLBACK (on_document_loaded_or_saved),
				  plugin->priv->view);
}

static void
soli_modeline_plugin_deactivate (SoliViewActivatable *activatable)
{
	SoliModelinePlugin *plugin;
	GtkTextBuffer *doc;

	soli_debug (DEBUG_PLUGINS);

	plugin = SOLI_MODELINE_PLUGIN (activatable);

	doc = gtk_text_view_get_buffer (GTK_TEXT_VIEW (plugin->priv->view));

	g_signal_handler_disconnect (doc, plugin->priv->document_loaded_handler_id);
	g_signal_handler_disconnect (doc, plugin->priv->document_saved_handler_id);
}

static void
soli_modeline_plugin_class_init (SoliModelinePluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->constructed = soli_modeline_plugin_constructed;
	object_class->dispose = soli_modeline_plugin_dispose;
	object_class->finalize = soli_modeline_plugin_finalize;
	object_class->set_property = soli_modeline_plugin_set_property;
	object_class->get_property = soli_modeline_plugin_get_property;

	g_object_class_override_property (object_class, PROP_VIEW, "view");
}

static void
soli_view_activatable_iface_init (SoliViewActivatableInterface *iface)
{
	iface->activate = soli_modeline_plugin_activate;
	iface->deactivate = soli_modeline_plugin_deactivate;
}

static void
soli_modeline_plugin_class_finalize (SoliModelinePluginClass *klass)
{
}


G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
	soli_modeline_plugin_register_type (G_TYPE_MODULE (module));

	peas_object_module_register_extension_type (module,
						    SOLI_TYPE_VIEW_ACTIVATABLE,
						    SOLI_TYPE_MODELINE_PLUGIN);
}

/* ex:set ts=8 noet: */
