/*
 * soli-encoding-items.h
 * This file is part of soli
 *
 * Copyright (C) 2014 - Jesse van den Kieboom
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

#ifndef SOLI_ENCODING_ITEMS_H
#define SOLI_ENCODING_ITEMS_H

#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

typedef struct _SoliEncodingItem SoliEncodingItem;

GSList				*soli_encoding_items_get		(void);

void				 soli_encoding_item_free		(SoliEncodingItem *item);
const GtkSourceEncoding		*soli_encoding_item_get_encoding	(SoliEncodingItem *item);
const gchar			*soli_encoding_item_get_name		(SoliEncodingItem *item);

G_END_DECLS

#endif /* SOLI_ENCODING_ITEMS_H */

/* ex:set ts=8 noet: */
