/*
 * soli-notebook.h
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Maggi
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

/* This file is a modified version of the epiphany file ephy-notebook.h
 * Here the relevant copyright:
 *
 *  Copyright (C) 2002 Christophe Fergeau
 *  Copyright (C) 2003 Marco Pesenti Gritti
 *  Copyright (C) 2003, 2004 Christian Persch
 *
 */

#ifndef SOLI_NOTEBOOK_H
#define SOLI_NOTEBOOK_H

#include "soli-tab.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_NOTEBOOK		(soli_notebook_get_type ())
#define SOLI_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SOLI_TYPE_NOTEBOOK, SoliNotebook))
#define SOLI_NOTEBOOK_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), SOLI_TYPE_NOTEBOOK, SoliNotebookClass))
#define SOLI_IS_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SOLI_TYPE_NOTEBOOK))
#define SOLI_IS_NOTEBOOK_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SOLI_TYPE_NOTEBOOK))
#define SOLI_NOTEBOOK_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SOLI_TYPE_NOTEBOOK, SoliNotebookClass))

typedef struct _SoliNotebook		SoliNotebook;
typedef struct _SoliNotebookClass	SoliNotebookClass;
typedef struct _SoliNotebookPrivate	SoliNotebookPrivate;

/* This is now used in multi-notebook but we keep the same enum for
 * backward compatibility since it is used in the gsettings schema */
typedef enum
{
	SOLI_NOTEBOOK_SHOW_TABS_NEVER,
	SOLI_NOTEBOOK_SHOW_TABS_AUTO,
	SOLI_NOTEBOOK_SHOW_TABS_ALWAYS
} SoliNotebookShowTabsModeType;

struct _SoliNotebook
{
	GtkNotebook notebook;

	/*< private >*/
	SoliNotebookPrivate *priv;
};

struct _SoliNotebookClass
{
	GtkNotebookClass parent_class;

	/* Signals */
	void	(* tab_close_request)	(SoliNotebook *notebook,
					 SoliTab      *tab);
	void	(* show_popup_menu)	(SoliNotebook *notebook,
					 GdkEvent      *event,
					 SoliTab      *tab);
	gboolean(* change_to_page)      (SoliNotebook *notebook,
	                                 gint           page_num);
};

GType		soli_notebook_get_type		(void) G_GNUC_CONST;

GtkWidget      *soli_notebook_new		(void);

void		soli_notebook_add_tab		(SoliNotebook *nb,
						 SoliTab      *tab,
						 gint           position,
						 gboolean       jump_to);

void		soli_notebook_move_tab		(SoliNotebook *src,
						 SoliNotebook *dest,
						 SoliTab      *tab,
						 gint           dest_position);

void		soli_notebook_remove_all_tabs 	(SoliNotebook *nb);

G_END_DECLS

#endif /* SOLI_NOTEBOOK_H */

/* ex:set ts=8 noet: */
