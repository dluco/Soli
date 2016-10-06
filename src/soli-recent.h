/*
 * soli-recent.h
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Maggi
 * Copyright (C) 2014 - Paolo Borelli
 * Copyright (C) 2014 - Jesse van den Kieboom
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

#ifndef SOLI_RECENT_H
#define SOLI_RECENT_H

#include <gtk/gtk.h>
#include "soli-document.h"

G_BEGIN_DECLS

typedef struct
{
	GtkRecentManager *manager;
	GtkRecentFilter *filter;

	gint limit;
	gchar *substring_filter;

	guint show_private : 1;
	guint show_not_found : 1;
	guint local_only : 1;
} SoliRecentConfiguration;

void		 soli_recent_add_document		 (SoliDocument            *document);

void		 soli_recent_remove_if_local		 (GFile                    *location);

void		 soli_recent_configuration_init_default (SoliRecentConfiguration *config);
void		 soli_recent_configuration_destroy	 (SoliRecentConfiguration *config);
GList		*soli_recent_get_items			 (SoliRecentConfiguration *config);

G_END_DECLS

#endif  /* SOLI_RECENT_H  */

/* ex:set ts=8 noet: */
