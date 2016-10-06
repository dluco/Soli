/*
 * soli-app-private.h
 * This file is part of soli
 *
 * Copyright (C) 2015 - Sébastien Wilmet <swilmet@gnome.org>
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

#ifndef SOLI_APP_PRIVATE_H
#define SOLI_APP_PRIVATE_H

#include "soli-app.h"
#include "soli-settings.h"
#include "soli-menu-extension.h"

G_BEGIN_DECLS

void		 _soli_app_set_lockdown		(SoliApp          *app,
							 SoliLockdownMask  lockdown);

void		 _soli_app_set_lockdown_bit		(SoliApp          *app,
							 SoliLockdownMask  bit,
							 gboolean           value);

/* This one is a soli-window function, but we declare it here to avoid
 * #include headaches since it needs the SoliLockdownMask declaration.
 */
void		 _soli_window_set_lockdown		(SoliWindow       *window,
							 SoliLockdownMask  lockdown);

/* global print config */
GtkPageSetup		*_soli_app_get_default_page_setup	(SoliApp         *app);
void			 _soli_app_set_default_page_setup	(SoliApp         *app,
								 GtkPageSetup     *page_setup);
GtkPrintSettings	*_soli_app_get_default_print_settings	(SoliApp         *app);
void			 _soli_app_set_default_print_settings	(SoliApp         *app,
								 GtkPrintSettings *settings);

SoliSettings		*_soli_app_get_settings		(SoliApp  *app);

GMenuModel		*_soli_app_get_hamburger_menu		(SoliApp  *app);

GMenuModel		*_soli_app_get_notebook_menu		(SoliApp  *app);

GMenuModel		*_soli_app_get_tab_width_menu		(SoliApp  *app);

GMenuModel		*_soli_app_get_line_col_menu		(SoliApp  *app);

SoliMenuExtension	*_soli_app_extend_menu			(SoliApp    *app,
								 const gchar *extension_point);

G_END_DECLS

#endif /* SOLI_APP_PRIVATE_H */

/* ex:set ts=8 noet: */
