/*
 * soli-view.h
 * This file is part of soli
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2005 Paolo Maggi
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

#ifndef SOLI_VIEW_H
#define SOLI_VIEW_H

#include <gtk/gtk.h>

#include "soli-document.h"
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define SOLI_TYPE_VIEW            (soli_view_get_type ())
#define SOLI_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), SOLI_TYPE_VIEW, SoliView))
#define SOLI_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), SOLI_TYPE_VIEW, SoliViewClass))
#define SOLI_IS_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), SOLI_TYPE_VIEW))
#define SOLI_IS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_VIEW))
#define SOLI_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SOLI_TYPE_VIEW, SoliViewClass))

typedef struct _SoliView		SoliView;
typedef struct _SoliViewClass		SoliViewClass;
typedef struct _SoliViewPrivate	SoliViewPrivate;

struct _SoliView
{
	GtkSourceView view;

	/*< private >*/
	SoliViewPrivate *priv;
};

struct _SoliViewClass
{
	GtkSourceViewClass parent_class;

	void	 (* drop_uris)			(SoliView	 *view,
						 gchar          **uri_list);

	gpointer padding;
};

GType		 soli_view_get_type     	(void) G_GNUC_CONST;

GtkWidget	*soli_view_new			(SoliDocument   *doc);

void		 soli_view_cut_clipboard 	(SoliView       *view);
void		 soli_view_copy_clipboard 	(SoliView       *view);
void		 soli_view_paste_clipboard	(SoliView       *view);
void		 soli_view_delete_selection	(SoliView       *view);
void		 soli_view_select_all		(SoliView       *view);

void		 soli_view_scroll_to_cursor 	(SoliView       *view);

void 		 soli_view_set_font		(SoliView       *view,
						 gboolean         default_font,
						 const gchar     *font_name);

G_END_DECLS

#endif /* SOLI_VIEW_H */

/* ex:set ts=8 noet: */
