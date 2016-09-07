/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-dirs.h
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

#ifndef _SOLI_DIRS_
#define _SOLI_DIRS_

#include <glib.h>

G_BEGIN_DECLS

/* This function must be called before starting soli */
void		 soli_dirs_init			(void);
/* This function must be called before exiting soli */
void		 soli_dirs_shutdown 	(void);

const gchar	*soli_dirs_get_user_config_dir	(void);

const gchar	*soli_dirs_get_user_cache_dir	(void);

const gchar	*soli_dirs_get_user_styles_dir	(void);

const gchar	*soli_dirs_get_user_plugins_dir	(void);

const gchar	*soli_dirs_get_soli_locale_dir	(void);

const gchar	*soli_dirs_get_soli_lib_dir		(void);

const gchar	*soli_dirs_get_soli_plugins_dir	(void);

const gchar	*soli_dirs_get_soli_plugins_data_dir	(void);

G_END_DECLS

#endif /* _SOLI_COMMANDS_ */
