/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-window.h
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

#ifndef _SOLI_WINDOW_H_
#define _SOLI_WINDOW_H_

#include <gtk/gtk.h>
#include "soli-app.h"
#include "soli-tab.h"
#include "soli-view.h"

G_BEGIN_DECLS

#define SOLI_TYPE_WINDOW             (soli_window_get_type ())
#define SOLI_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_WINDOW, SoliWindow))
#define SOLI_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_WINDOW, SoliWindowClass))
#define SOLI_IS_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_WINDOW))
#define SOLI_IS_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_WINDOW))
#define SOLI_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_WINDOW, SoliWindowClass))

typedef struct _SoliWindow SoliWindow;
typedef struct _SoliWindowClass SoliWindowClass;
typedef struct _SoliWindowPrivate SoliWindowPrivate;

struct _SoliWindow
{
	GtkApplicationWindow parent;
	
	SoliWindowPrivate *priv;
};

struct _SoliWindowClass
{
	GtkApplicationWindowClass parent_class;
};


GType soli_window_get_type (void) G_GNUC_CONST;
SoliWindow *soli_window_new (SoliApp *app);

SoliTab *
soli_window_get_active_tab (SoliWindow *window);

SoliView *
soli_window_get_active_view (SoliWindow *window);

void soli_window_open (SoliWindow *win, GFile *file);

G_END_DECLS

#endif /* _SOLI_WINDOW_H_ */

