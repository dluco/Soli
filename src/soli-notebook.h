/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-notebook.h
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

#ifndef _SOLI_NOTEBOOK_H_
#define _SOLI_NOTEBOOK_H_

#include <gtk/gtk.h>
#include "soli-tab.h"

G_BEGIN_DECLS

#define SOLI_TYPE_NOTEBOOK             (soli_notebook_get_type ())
#define SOLI_NOTEBOOK(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_NOTEBOOK, SoliNotebook))
#define SOLI_NOTEBOOK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_NOTEBOOK, SoliNotebookClass))
#define SOLI_IS_NOTEBOOK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_NOTEBOOK))
#define SOLI_IS_NOTEBOOK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_NOTEBOOK))
#define SOLI_NOTEBOOK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_NOTEBOOK, SoliNotebookClass))

typedef struct _SoliNotebookClass SoliNotebookClass;
typedef struct _SoliNotebook SoliNotebook;
typedef struct _SoliNotebookPrivate SoliNotebookPrivate;

struct _SoliNotebookClass
{
	GtkNotebookClass parent_class;
};

struct _SoliNotebook
{
	GtkNotebook parent_instance;

	SoliNotebookPrivate *priv;
};

GType soli_notebook_get_type (void) G_GNUC_CONST;

SoliNotebook *soli_notebook_new (void);

SoliTab *soli_notebook_get_active_tab (SoliNotebook *notebook);

void soli_notebook_add_tab (SoliNotebook *notebook,
							SoliTab *tab,
							gint position);
							
void
soli_notebook_close_tab (SoliNotebook *notebook,
						SoliTab *tab);

G_END_DECLS

#endif /* _SOLI_NOTEBOOK_H_ */

