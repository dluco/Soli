/*
 * soli-document.h
 * This file is part of soli
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2005 Paolo Maggi
 * Copyright (C) 2014 Sébastien Wilmet
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

#ifndef SOLI_DOCUMENT_PRIVATE_H
#define SOLI_DOCUMENT_PRIVATE_H

#include "soli-document.h"

G_BEGIN_DECLS

#ifdef G_OS_WIN32
#define SOLI_METADATA_ATTRIBUTE_POSITION "position"
#define SOLI_METADATA_ATTRIBUTE_ENCODING "encoding"
#define SOLI_METADATA_ATTRIBUTE_LANGUAGE "language"
#else
#define SOLI_METADATA_ATTRIBUTE_POSITION "metadata::soli-position"
#define SOLI_METADATA_ATTRIBUTE_ENCODING "metadata::soli-encoding"
#define SOLI_METADATA_ATTRIBUTE_LANGUAGE "metadata::soli-language"
#endif

glong		 _soli_document_get_seconds_since_last_save_or_load	(SoliDocument       *doc);

gboolean	 _soli_document_needs_saving				(SoliDocument       *doc);

gboolean	 _soli_document_get_empty_search			(SoliDocument       *doc);

void		 _soli_document_set_create				(SoliDocument       *doc,
									 gboolean             create);

gboolean	 _soli_document_get_create				(SoliDocument       *doc);

G_END_DECLS

#endif /* SOLI_DOCUMENT_PRIVATE_H */
/* ex:set ts=8 noet: */
