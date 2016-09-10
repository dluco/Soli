/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-plugins-engine.h
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

#ifndef _SOLI_PLUGINS_ENGINE_H_
#define _SOLI_PLUGINS_ENGINE_H_

#include <glib.h>
#include <libpeas/peas.h>

G_BEGIN_DECLS

#define SOLI_TYPE_PLUGINS_ENGINE (soli_plugins_engine_get_type ())

G_DECLARE_FINAL_TYPE(SoliPluginsEngine, soli_plugins_engine, SOLI, PLUGINS_ENGINE, PeasEngine)

SoliPluginsEngine *soli_plugins_engine_get_default (void);

G_END_DECLS

#endif /* _SOLI_PLUGINS_ENGINE_H_ */
