/*
 * soli-sort-plugin.h
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

#ifndef SOLI_SORT_PLUGIN_H
#define SOLI_SORT_PLUGIN_H

#include <glib.h>
#include <glib-object.h>
#include <libpeas/peas-extension-base.h>
#include <libpeas/peas-object-module.h>

G_BEGIN_DECLS

#define SOLI_TYPE_SORT_PLUGIN		(soli_sort_plugin_get_type ())
#define SOLI_SORT_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SOLI_TYPE_SORT_PLUGIN, SoliSortPlugin))
#define SOLI_SORT_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), SOLI_TYPE_SORT_PLUGIN, SoliSortPluginClass))
#define SOLI_IS_SORT_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SOLI_TYPE_SORT_PLUGIN))
#define SOLI_IS_SORT_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SOLI_TYPE_SORT_PLUGIN))
#define SOLI_SORT_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SOLI_TYPE_SORT_PLUGIN, SoliSortPluginClass))

typedef struct _SoliSortPlugin		SoliSortPlugin;
typedef struct _SoliSortPluginPrivate	SoliSortPluginPrivate;
typedef struct _SoliSortPluginClass	SoliSortPluginClass;

struct _SoliSortPlugin
{
	PeasExtensionBase parent;

	/*< private >*/
	SoliSortPluginPrivate *priv;
};

struct _SoliSortPluginClass
{
	PeasExtensionBaseClass parent_class;
};

GType			soli_sort_plugin_get_type	(void) G_GNUC_CONST;

G_MODULE_EXPORT void	peas_register_types		(PeasObjectModule *module);

G_END_DECLS

#endif /* SOLI_SORT_PLUGIN_H */
/* ex:set ts=8 noet: */
