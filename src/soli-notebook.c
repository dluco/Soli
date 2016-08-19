/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-notebook.c
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

#include "soli-notebook.h"
#include "soli-tab.h"

struct _SoliNotebookPrivate
{
	gchar dummy;
};


G_DEFINE_TYPE_WITH_PRIVATE (SoliNotebook, soli_notebook, GTK_TYPE_NOTEBOOK);

static void
soli_notebook_init (SoliNotebook *notebook)
{
	notebook->priv = soli_notebook_get_instance_private (notebook);

	/* TODO: Add initialization code here */
}

static void
soli_notebook_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (soli_notebook_parent_class)->finalize (object);
}

static void
soli_notebook_class_init (SoliNotebookClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = soli_notebook_finalize;
}

SoliNotebook *
soli_notebook_new (void)
{
	return g_object_new (SOLI_TYPE_NOTEBOOK, NULL);
}

// TODO: use a SoliTabLabel widget
static gchar *
get_tab_name (SoliTab *tab)
{
	SoliDocument *doc;
	GFile *location;
	
	g_return_val_if_fail (SOLI_IS_TAB (tab), NULL);
	
	doc = soli_tab_get_document(tab);
	
	location = soli_document_get_location(doc);
	
	if (location != NULL)
	{
		return g_file_get_basename (location);
	}
	else
	{
		return NULL;
	}
}

void
soli_notebook_add_tab (SoliNotebook *notebook,
						SoliTab *tab,
						gint position)
{
	GtkWidget *tab_label;
	
	g_return_if_fail (SOLI_IS_NOTEBOOK (notebook));
	g_return_if_fail (SOLI_IS_TAB (tab));
	
	tab_label = gtk_label_new (get_tab_name (tab));
	
	gtk_notebook_insert_page (GTK_NOTEBOOK (notebook),
								GTK_WIDGET (tab),
								tab_label,
								position);
}

