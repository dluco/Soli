/*
 * soli-modeline-plugin.h
 * Emacs, Kate and Vim-style modelines support for soli.
 *
 * Copyright (C) 2005-2007 - Steve Frécinaux <code@istique.net>
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

#ifndef SOLI_MODELINE_PLUGIN_H
#define SOLI_MODELINE_PLUGIN_H

#include <glib.h>
#include <glib-object.h>
#include <libpeas/peas-extension-base.h>
#include <libpeas/peas-object-module.h>

G_BEGIN_DECLS

#define SOLI_TYPE_MODELINE_PLUGIN		(soli_modeline_plugin_get_type ())
#define SOLI_MODELINE_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SOLI_TYPE_MODELINE_PLUGIN, SoliModelinePlugin))
#define SOLI_MODELINE_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), SOLI_TYPE_MODELINE_PLUGIN, SoliModelinePluginClass))
#define SOLI_IS_MODELINE_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SOLI_TYPE_MODELINE_PLUGIN))
#define SOLI_IS_MODELINE_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SOLI_TYPE_MODELINE_PLUGIN))
#define SOLI_MODELINE_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SOLI_TYPE_MODELINE_PLUGIN, SoliModelinePluginClass))

typedef struct _SoliModelinePlugin		SoliModelinePlugin;
typedef struct _SoliModelinePluginPrivate	SoliModelinePluginPrivate;
typedef struct _SoliModelinePluginClass	SoliModelinePluginClass;

struct _SoliModelinePlugin {
	PeasExtensionBase parent;

	/*< private >*/
	SoliModelinePluginPrivate *priv;
};

struct _SoliModelinePluginClass {
	PeasExtensionBaseClass parent_class;
};

GType	soli_modeline_plugin_get_type		(void) G_GNUC_CONST;

G_MODULE_EXPORT void peas_register_types (PeasObjectModule *module);

G_END_DECLS

#endif /* SOLI_MODELINE_PLUGIN_H */
/* ex:set ts=8 noet: */
