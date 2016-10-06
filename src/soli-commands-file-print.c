/*
 * soli-commands-file-print.c
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

#include "soli-commands.h"
#include "soli-commands-private.h"

#include "soli-window.h"
#include "soli-tab.h"
#include "soli-tab-private.h"
#include "soli-debug.h"

void
_soli_cmd_file_print (GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer       user_data)
{
	SoliWindow *window = SOLI_WINDOW (user_data);
	SoliTab *tab;

	soli_debug (DEBUG_COMMANDS);

	tab = soli_window_get_active_tab (window);

	if (tab != NULL)
	{
		_soli_tab_print (tab);
	}
}

/* ex:set ts=8 noet: */
