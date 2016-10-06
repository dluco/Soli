/*
 * soli-window-private.h
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
 * MERCHANWINDOWILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLI_WINDOW_PRIVATE_H
#define SOLI_WINDOW_PRIVATE_H

#include <libpeas/peas-extension-set.h>

#include "soli-window.h"
#include "soli-message-bus.h"
#include "soli-settings.h"
#include "soli-multi-notebook.h"

G_BEGIN_DECLS

/* WindowPrivate is in a separate .h so that we can access it from soli-commands */

struct _SoliWindowPrivate
{
	GSettings      *editor_settings;
	GSettings      *ui_settings;
	GSettings      *window_settings;

	SoliMultiNotebook *multi_notebook;

	GtkWidget      *side_panel_box;
	GtkWidget      *side_panel;
	GtkWidget      *side_stack_switcher;
	GtkWidget      *side_panel_inline_stack_switcher;
	GtkWidget      *bottom_panel_box;
	GtkWidget      *bottom_panel;

	GtkWidget      *hpaned;
	GtkWidget      *vpaned;

	SoliMessageBus *message_bus;
	PeasExtensionSet *extensions;

	/* statusbar and context ids for statusbar messages */
	GtkWidget      *statusbar;
	GtkWidget      *line_col_button;
	GtkWidget      *tab_width_button;
	GtkWidget      *language_button;
	GtkWidget      *language_button_label;
	GtkWidget      *language_popover;
	guint           generic_message_cid;
	guint           tip_message_cid;
	guint 	        bracket_match_message_cid;
	guint 	        tab_width_id;
	guint 	        language_changed_id;
	guint           wrap_mode_changed_id;

	gint            num_tabs_with_error;

	gint            width;
	gint            height;
	GdkWindowState  window_state;

	gint            side_panel_size;
	gint            bottom_panel_size;

	SoliWindowState state;

	guint           inhibition_cookie;

	gint            bottom_panel_item_removed_handler_id;

	GtkWindowGroup *window_group;

	GFile          *default_location;

	gchar          *direct_save_uri;

	GSList         *closed_docs_stack;

	guint           removing_tabs : 1;
	guint           dispose_has_run : 1;
};

G_END_DECLS

#endif  /* SOLI_WINDOW_PRIVATE_H  */
/* ex:set ts=8 noet: */
