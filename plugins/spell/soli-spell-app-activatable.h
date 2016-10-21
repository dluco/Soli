/*
 * soli-spell-app-activatable.h
 * This file is part of soli
 *
 * Copyright (C) 2014 - Ignacio Casal Quinteiro
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
 * along with soli. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef SOLI_SPELL_APP_ACTIVATABLE_H
#define SOLI_SPELL_APP_ACTIVATABLE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define SOLI_TYPE_SPELL_APP_ACTIVATABLE (soli_spell_app_activatable_get_type ())
G_DECLARE_FINAL_TYPE (SoliSpellAppActivatable, soli_spell_app_activatable,
		      SOLI, SPELL_APP_ACTIVATABLE,
		      GObject)

void	soli_spell_app_activatable_register	(GTypeModule *module);

G_END_DECLS

#endif /* SOLI_SPELL_APP_ACTIVATABLE_H */
