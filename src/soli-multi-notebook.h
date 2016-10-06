/*
 * soli-multi-notebook.h
 * This file is part of soli
 *
 * Copyright (C) 2010 - Ignacio Casal Quinteiro
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
 * along with soli; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef SOLI_MULTI_NOTEBOOK_H
#define SOLI_MULTI_NOTEBOOK_H

#include <gtk/gtk.h>

#include "soli-tab.h"
#include "soli-notebook.h"

G_BEGIN_DECLS

#define SOLI_TYPE_MULTI_NOTEBOOK		(soli_multi_notebook_get_type ())
#define SOLI_MULTI_NOTEBOOK(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_MULTI_NOTEBOOK, SoliMultiNotebook))
#define SOLI_MULTI_NOTEBOOK_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_MULTI_NOTEBOOK, SoliMultiNotebook const))
#define SOLI_MULTI_NOTEBOOK_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_MULTI_NOTEBOOK, SoliMultiNotebookClass))
#define SOLI_IS_MULTI_NOTEBOOK(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_MULTI_NOTEBOOK))
#define SOLI_IS_MULTI_NOTEBOOK_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_MULTI_NOTEBOOK))
#define SOLI_MULTI_NOTEBOOK_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_MULTI_NOTEBOOK, SoliMultiNotebookClass))

typedef struct _SoliMultiNotebook		SoliMultiNotebook;
typedef struct _SoliMultiNotebookClass		SoliMultiNotebookClass;
typedef struct _SoliMultiNotebookPrivate	SoliMultiNotebookPrivate;

struct _SoliMultiNotebook
{
	GtkGrid parent;

	SoliMultiNotebookPrivate *priv;
};

struct _SoliMultiNotebookClass
{
	GtkGridClass parent_class;

	/* Signals */
	void	(* notebook_added)		(SoliMultiNotebook *mnb,
						 SoliNotebook      *notebook);
	void	(* notebook_removed)		(SoliMultiNotebook *mnb,
						 SoliNotebook      *notebook);
	void	(* tab_added)			(SoliMultiNotebook *mnb,
						 SoliNotebook      *notebook,
						 SoliTab           *tab);
	void	(* tab_removed)			(SoliMultiNotebook *mnb,
						 SoliNotebook      *notebook,
						 SoliTab           *tab);
	void	(* switch_tab)			(SoliMultiNotebook *mnb,
						 SoliNotebook      *old_notebook,
						 SoliTab           *old_tab,
						 SoliNotebook      *new_notebook,
						 SoliTab           *new_tab);
	void	(* tab_close_request)		(SoliMultiNotebook *mnb,
						 SoliNotebook      *notebook,
						 SoliTab           *tab);
	GtkNotebook *	(* create_window)	(SoliMultiNotebook *mnb,
						 SoliNotebook      *notebook,
						 GtkWidget          *page,
						 gint                x,
						 gint                y);
	void	(* page_reordered)		(SoliMultiNotebook *mnb);
	void	(* show_popup_menu)		(SoliMultiNotebook *mnb,
						 GdkEvent           *event,
						 SoliTab           *tab);
};

GType			 soli_multi_notebook_get_type			(void) G_GNUC_CONST;

SoliMultiNotebook	*soli_multi_notebook_new			(void);

SoliNotebook		*soli_multi_notebook_get_active_notebook	(SoliMultiNotebook *mnb);

gint			 soli_multi_notebook_get_n_notebooks		(SoliMultiNotebook *mnb);

SoliNotebook		*soli_multi_notebook_get_nth_notebook		(SoliMultiNotebook *mnb,
									 gint                notebook_num);

SoliNotebook		*soli_multi_notebook_get_notebook_for_tab	(SoliMultiNotebook *mnb,
									 SoliTab           *tab);

gint			 soli_multi_notebook_get_notebook_num		(SoliMultiNotebook *mnb,
									 SoliNotebook      *notebook);

gint			 soli_multi_notebook_get_n_tabs		(SoliMultiNotebook *mnb);

gint			 soli_multi_notebook_get_page_num		(SoliMultiNotebook *mnb,
									 SoliTab           *tab);

SoliTab		*soli_multi_notebook_get_active_tab		(SoliMultiNotebook *mnb);
void			 soli_multi_notebook_set_active_tab		(SoliMultiNotebook *mnb,
									 SoliTab           *tab);

void			 soli_multi_notebook_set_current_page		(SoliMultiNotebook *mnb,
									 gint                page_num);

GList			*soli_multi_notebook_get_all_tabs		(SoliMultiNotebook *mnb);

void			 soli_multi_notebook_close_tabs		(SoliMultiNotebook *mnb,
									 const GList        *tabs);

void			 soli_multi_notebook_close_all_tabs		(SoliMultiNotebook *mnb);

void			 soli_multi_notebook_add_new_notebook		(SoliMultiNotebook *mnb);

void			 soli_multi_notebook_add_new_notebook_with_tab (SoliMultiNotebook *mnb,
									 SoliTab           *tab);

void			 soli_multi_notebook_remove_active_notebook	(SoliMultiNotebook *mnb);

void			 soli_multi_notebook_previous_notebook		(SoliMultiNotebook *mnb);
void			 soli_multi_notebook_next_notebook		(SoliMultiNotebook *mnb);

void			 soli_multi_notebook_foreach_notebook		(SoliMultiNotebook *mnb,
									 GtkCallback         callback,
									 gpointer            callback_data);

void			 soli_multi_notebook_foreach_tab		(SoliMultiNotebook *mnb,
									 GtkCallback         callback,
									 gpointer            callback_data);

void			_soli_multi_notebook_set_show_tabs		(SoliMultiNotebook *mnb,
									 gboolean            show);

G_END_DECLS

#endif /* SOLI_MULTI_NOTEBOOK_H */

/* ex:set ts=8 noet: */
