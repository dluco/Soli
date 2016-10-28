/*
 * Copyright (C) 2009 Ignacio Casal Quinteiro <icq@gnome.org>
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
 *
 */

#ifndef __SOLI_WORD_COMPLETION_PLUGIN_H__
#define __SOLI_WORD_COMPLETION_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <libpeas/peas-extension-base.h>
#include <libpeas/peas-object-module.h>

G_BEGIN_DECLS

#define SOLI_TYPE_WORD_COMPLETION_PLUGIN		(soli_word_completion_plugin_get_type ())
#define SOLI_WORD_COMPLETION_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), SOLI_TYPE_WORD_COMPLETION_PLUGIN, SoliWordCompletionPlugin))
#define SOLI_WORD_COMPLETION_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), SOLI_TYPE_WORD_COMPLETION_PLUGIN, SoliWordCompletionPluginClass))
#define SOLI_IS_WORD_COMPLETION_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SOLI_TYPE_WORD_COMPLETION_PLUGIN))
#define SOLI_IS_WORD_COMPLETION_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SOLI_TYPE_WORD_COMPLETION_PLUGIN))
#define SOLI_WORD_COMPLETION_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SOLI_TYPE_WORD_COMPLETION_PLUGIN, SoliWordCompletionPluginClass))

typedef struct _SoliWordCompletionPlugin		SoliWordCompletionPlugin;
typedef struct _SoliWordCompletionPluginPrivate	SoliWordCompletionPluginPrivate;
typedef struct _SoliWordCompletionPluginClass		SoliWordCompletionPluginClass;

struct _SoliWordCompletionPlugin
{
	PeasExtensionBase parent_instance;

	SoliWordCompletionPluginPrivate *priv;
};

struct _SoliWordCompletionPluginClass
{
	PeasExtensionBaseClass parent_class;
};

GType			soli_word_completion_plugin_get_type	(void) G_GNUC_CONST;

G_MODULE_EXPORT void	peas_register_types			(PeasObjectModule *module);

G_END_DECLS

#endif /* __SOLI_WORD_COMPLETION_PLUGIN_H__ */
