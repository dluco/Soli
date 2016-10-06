/*
 * soli-documents-commands.c
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "soli-commands.h"
#include "soli-commands-private.h"

#include <gtk/gtk.h>

#include "soli-window.h"
#include "soli-notebook.h"
#include "soli-multi-notebook.h"
#include "soli-debug.h"

void
_soli_cmd_documents_previous_document (GSimpleAction *action,
                                        GVariant      *parameter,
                                        gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	GtkNotebook *notebook;

	soli_debug (DEBUG_COMMANDS);

	notebook = GTK_NOTEBOOK (_soli_window_get_notebook (window));
	gtk_notebook_prev_page (notebook);
}

void
_soli_cmd_documents_next_document (GSimpleAction *action,
                                    GVariant      *parameter,
                                    gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	GtkNotebook *notebook;

	soli_debug (DEBUG_COMMANDS);

	notebook = GTK_NOTEBOOK (_soli_window_get_notebook (window));
	gtk_notebook_next_page (notebook);
}

void
_soli_cmd_documents_move_to_new_window (GSimpleAction *action,
                                         GVariant      *parameter,
                                         gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliTab *tab;

	soli_debug (DEBUG_COMMANDS);

	tab = soli_window_get_active_tab (window);

	if (tab == NULL)
		return;

	_soli_window_move_tab_to_new_window (window, tab);
}

/* Methods releated with the tab groups */
void
_soli_cmd_documents_new_tab_group (GSimpleAction *action,
                                    GVariant      *parameter,
                                    gpointer       user_data)
{
	soli_multi_notebook_add_new_notebook (SOLI_MULTI_NOTEBOOK (_soli_window_get_multi_notebook (SOLI_WINDOW (user_data))));
}

void
_soli_cmd_documents_previous_tab_group (GSimpleAction *action,
                                         GVariant      *parameter,
                                         gpointer       user_data)
{
	soli_multi_notebook_previous_notebook (SOLI_MULTI_NOTEBOOK (_soli_window_get_multi_notebook (SOLI_WINDOW (user_data))));
}

void
_soli_cmd_documents_next_tab_group (GSimpleAction *action,
                                     GVariant      *parameter,
                                     gpointer       user_data)
{
	soli_multi_notebook_next_notebook (SOLI_MULTI_NOTEBOOK (_soli_window_get_multi_notebook (SOLI_WINDOW (user_data))));
}

/* ex:set ts=8 noet: */
