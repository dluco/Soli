/*
 * soli-app.h
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

#ifndef SOLI_APP_H
#define SOLI_APP_H

#include <gtk/gtk.h>
#include "soli-window.h"

G_BEGIN_DECLS

#define SOLI_TYPE_APP (soli_app_get_type())

G_DECLARE_DERIVABLE_TYPE (SoliApp, soli_app, SOLI, APP, GtkApplication)

struct _SoliAppClass
{
	GtkApplicationClass parent_class;

	gboolean (*show_help)                   (SoliApp    *app,
	                                         GtkWindow   *parent,
	                                         const gchar *name,
	                                         const gchar *link_id);

	gchar *(*help_link_id)                  (SoliApp    *app,
	                                         const gchar *name,
	                                         const gchar *link_id);

	void (*set_window_title)                (SoliApp    *app,
	                                         SoliWindow *window,
	                                         const gchar *title);

	SoliWindow *(*create_window)           (SoliApp    *app);

	gboolean (*process_window_event)        (SoliApp    *app,
	                                         SoliWindow *window,
	                                         GdkEvent    *event);
};

typedef enum
{
	SOLI_LOCKDOWN_COMMAND_LINE	= 1 << 0,
	SOLI_LOCKDOWN_PRINTING		= 1 << 1,
	SOLI_LOCKDOWN_PRINT_SETUP	= 1 << 2,
	SOLI_LOCKDOWN_SAVE_TO_DISK	= 1 << 3
} SoliLockdownMask;

/* We need to define this here to avoid problems with bindings and gsettings */
#define SOLI_LOCKDOWN_ALL 0xF

SoliWindow	*soli_app_create_window		(SoliApp    *app,
							 GdkScreen   *screen);

GList		*soli_app_get_main_windows		(SoliApp    *app);

GList		*soli_app_get_documents		(SoliApp    *app);

GList		*soli_app_get_views			(SoliApp    *app);

/* Lockdown state */
SoliLockdownMask soli_app_get_lockdown		(SoliApp    *app);

gboolean	 soli_app_show_help			(SoliApp    *app,
                                                         GtkWindow   *parent,
                                                         const gchar *name,
                                                         const gchar *link_id);

void		 soli_app_set_window_title		(SoliApp    *app,
                                                         SoliWindow *window,
                                                         const gchar *title);
gboolean	soli_app_process_window_event		(SoliApp    *app,
							 SoliWindow *window,
							 GdkEvent    *event);

G_END_DECLS

#endif /* SOLI_APP_H */

/* ex:set ts=8 noet: */
