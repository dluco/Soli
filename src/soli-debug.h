/*
 * soli-debug.h
 * This file is part of soli
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002 - 2005 Paolo Maggi
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

#ifndef SOLI_DEBUG_H
#define SOLI_DEBUG_H

#include <glib.h>

/**
 * SoliDebugSection:
 *
 * Enumeration of debug sections.
 *
 * Debugging output for a section is enabled by setting an environment variable
 * of the same name. For example, setting the <code>SOLI_DEBUG_PLUGINS</code>
 * environment variable enables all debugging output for the %SOLI_DEBUG_PLUGINS
 * section. Setting the special environment variable <code>SOLI_DEBUG</code>
 * enables output for all sections.
 */
typedef enum {
	SOLI_NO_DEBUG       = 0,
	SOLI_DEBUG_VIEW     = 1 << 0,
	SOLI_DEBUG_PREFS    = 1 << 1,
	SOLI_DEBUG_WINDOW   = 1 << 2,
	SOLI_DEBUG_PANEL    = 1 << 3,
	SOLI_DEBUG_PLUGINS  = 1 << 4,
	SOLI_DEBUG_TAB      = 1 << 5,
	SOLI_DEBUG_DOCUMENT = 1 << 6,
	SOLI_DEBUG_COMMANDS = 1 << 7,
	SOLI_DEBUG_APP      = 1 << 8,
	SOLI_DEBUG_UTILS    = 1 << 9,
	SOLI_DEBUG_METADATA = 1 << 10,
} SoliDebugSection;

#define	DEBUG_VIEW	SOLI_DEBUG_VIEW,    __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_PREFS	SOLI_DEBUG_PREFS,   __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_WINDOW	SOLI_DEBUG_WINDOW,  __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_PANEL	SOLI_DEBUG_PANEL,   __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_PLUGINS	SOLI_DEBUG_PLUGINS, __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_TAB	SOLI_DEBUG_TAB,     __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_DOCUMENT	SOLI_DEBUG_DOCUMENT,__FILE__, __LINE__, G_STRFUNC
#define	DEBUG_COMMANDS	SOLI_DEBUG_COMMANDS,__FILE__, __LINE__, G_STRFUNC
#define	DEBUG_APP	SOLI_DEBUG_APP,     __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_UTILS	SOLI_DEBUG_UTILS,   __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_METADATA	SOLI_DEBUG_METADATA,__FILE__, __LINE__, G_STRFUNC

void soli_debug_init (void);

void soli_debug (SoliDebugSection  section,
		  const gchar       *file,
		  gint               line,
		  const gchar       *function);

void soli_debug_message (SoliDebugSection  section,
			  const gchar       *file,
			  gint               line,
			  const gchar       *function,
			  const gchar       *format, ...) G_GNUC_PRINTF(5, 6);

void soli_debug_plugin_message (const gchar       *file,
				 gint               line,
				 const gchar       *function,
				 const gchar       *message);

#endif /* SOLI_DEBUG_H */
/* ex:set ts=8 noet: */
