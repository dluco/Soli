/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-window.c
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

#include "soli-app.h"
#include "soli-window.h"

G_DEFINE_TYPE (SoliWindow, soli_window, GTK_TYPE_APPLICATION_WINDOW);

static void
soli_window_init (SoliWindow *soli_window)
{
	gtk_widget_init_template (GTK_WIDGET (soli_window));
}

static void
soli_window_class_init (SoliWindowClass *klass)
{
	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
	                                             "/org/gnome/soli/window.ui");
}

SoliWindow *
soli_window_new (SoliApp *app)
{
	return g_object_new (SOLI_WINDOW_TYPE, "application", app, NULL);
}

void
soli_window_open (SoliWindow *win, GFile *file)
{
	return;
}









