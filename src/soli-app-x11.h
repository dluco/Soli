/*
 * soli-app-x11.h
 * This file is part of soli
 *
 * Copyright (C) 2010 - Jesse van den Kieboom
 *
 * soli is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * soli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with soli; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef SOLI_APP_X11_H
#define SOLI_APP_X11_H

#include "soli-app.h"

G_BEGIN_DECLS

#define SOLI_TYPE_APP_X11		(soli_app_x11_get_type ())

G_DECLARE_FINAL_TYPE (SoliAppX11, soli_app_x11, SOLI, APP_X11, SoliApp)

G_END_DECLS

#endif /* SOLI_APP_X11_H */

/* ex:set ts=8 noet: */
