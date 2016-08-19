/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-tab.h
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

#ifndef _SOLI_TAB_H_
#define _SOLI_TAB_H_

#include <gtk/gtk.h>
#include "soli-document.h"

G_BEGIN_DECLS

#define SOLI_TYPE_TAB             (soli_tab_get_type ())
#define SOLI_TAB(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_TAB, SoliTab))
#define SOLI_TAB_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_TAB, SoliTabClass))
#define SOLI_IS_TAB(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_TAB))
#define SOLI_IS_TAB_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_TAB))
#define SOLI_TAB_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_TAB, SoliTabClass))

typedef struct _SoliTabClass SoliTabClass;
typedef struct _SoliTab SoliTab;
typedef struct _SoliTabPrivate SoliTabPrivate;


struct _SoliTabClass
{
	GtkBoxClass parent_class;
};

struct _SoliTab
{
	GtkBox parent_instance;

	SoliTabPrivate *priv;
};

GType soli_tab_get_type (void) G_GNUC_CONST;

SoliTab *soli_tab_new (void);

void soli_tab_load (SoliTab *tab,
					GFile *location,
					const GtkSourceEncoding *encoding);

SoliDocument *soli_tab_get_document (SoliTab *tab);

G_END_DECLS

#endif /* _SOLI_TAB_H_ */

