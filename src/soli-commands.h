/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-commands.h
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 * 
 * Soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SOLI_COMMANDS_
#define _SOLI_COMMANDS_

#include <gtk/gtk.h>

void soli_cmd_open (GSimpleAction *action, GVariant *parameter, gpointer app);

void soli_cmd_preferences (GSimpleAction *action, GVariant *parameter, gpointer app);

void soli_cmd_quit (GSimpleAction *action, GVariant *parameter, gpointer app);

void soli_cmd_about (GSimpleAction *action, GVariant *parameter, gpointer app);

#endif /* _SOLI_COMMANDS_ */
