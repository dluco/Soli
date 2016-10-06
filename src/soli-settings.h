/*
 * soli-settings.h
 * This file is part of soli
 *
 * Copyright (C) 2009 - Ignacio Casal Quinteiro
 *               2002 - Paolo Maggi
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

#ifndef SOLI_SETTINGS_H
#define SOLI_SETTINGS_H

#include <glib-object.h>
#include <glib.h>
#include "soli-app.h"

G_BEGIN_DECLS

#define SOLI_TYPE_SETTINGS (soli_settings_get_type ())

G_DECLARE_FINAL_TYPE (SoliSettings, soli_settings, SOLI, SETTINGS, GObject)

SoliSettings		*soli_settings_new				(void);

SoliLockdownMask	 soli_settings_get_lockdown			(SoliSettings *gs);

gchar			*soli_settings_get_system_font			(SoliSettings *gs);

GSList			*soli_settings_get_candidate_encodings		(gboolean      *default_candidates);

/* Utility functions */
GSList			*soli_settings_get_list			(GSettings     *settings,
									 const gchar   *key);

void			 soli_settings_set_list			(GSettings     *settings,
									 const gchar   *key,
									 const GSList  *list);

/* key constants */
#define SOLI_SETTINGS_USE_DEFAULT_FONT			"use-default-font"
#define SOLI_SETTINGS_EDITOR_FONT			"editor-font"
#define SOLI_SETTINGS_SCHEME				"scheme"
#define SOLI_SETTINGS_CREATE_BACKUP_COPY		"create-backup-copy"
#define SOLI_SETTINGS_AUTO_SAVE			"auto-save"
#define SOLI_SETTINGS_AUTO_SAVE_INTERVAL		"auto-save-interval"
#define SOLI_SETTINGS_MAX_UNDO_ACTIONS			"max-undo-actions"
#define SOLI_SETTINGS_WRAP_MODE			"wrap-mode"
#define SOLI_SETTINGS_WRAP_LAST_SPLIT_MODE		"wrap-last-split-mode"
#define SOLI_SETTINGS_TABS_SIZE			"tabs-size"
#define SOLI_SETTINGS_INSERT_SPACES			"insert-spaces"
#define SOLI_SETTINGS_AUTO_INDENT			"auto-indent"
#define SOLI_SETTINGS_DISPLAY_LINE_NUMBERS		"display-line-numbers"
#define SOLI_SETTINGS_HIGHLIGHT_CURRENT_LINE		"highlight-current-line"
#define SOLI_SETTINGS_BRACKET_MATCHING			"bracket-matching"
#define SOLI_SETTINGS_DISPLAY_RIGHT_MARGIN		"display-right-margin"
#define SOLI_SETTINGS_RIGHT_MARGIN_POSITION		"right-margin-position"
#define SOLI_SETTINGS_SMART_HOME_END			"smart-home-end"
#define SOLI_SETTINGS_RESTORE_CURSOR_POSITION		"restore-cursor-position"
#define SOLI_SETTINGS_SYNTAX_HIGHLIGHTING		"syntax-highlighting"
#define SOLI_SETTINGS_SEARCH_HIGHLIGHTING		"search-highlighting"
#define SOLI_SETTINGS_TOOLBAR_VISIBLE			"toolbar-visible"
#define SOLI_SETTINGS_TOOLBAR_BUTTONS_STYLE		"toolbar-buttons-style"
#define SOLI_SETTINGS_DISPLAY_OVERVIEW_MAP		"display-overview-map"
#define SOLI_SETTINGS_BACKGROUND_PATTERN		"background-pattern"
#define SOLI_SETTINGS_STATUSBAR_VISIBLE		"statusbar-visible"
#define SOLI_SETTINGS_SIDE_PANEL_VISIBLE		"side-panel-visible"
#define SOLI_SETTINGS_BOTTOM_PANEL_VISIBLE		"bottom-panel-visible"
#define SOLI_SETTINGS_MAX_RECENTS			"max-recents"
#define SOLI_SETTINGS_PRINT_SYNTAX_HIGHLIGHTING	"print-syntax-highlighting"
#define SOLI_SETTINGS_PRINT_HEADER			"print-header"
#define SOLI_SETTINGS_PRINT_WRAP_MODE			"print-wrap-mode"
#define SOLI_SETTINGS_PRINT_LINE_NUMBERS		"print-line-numbers"
#define SOLI_SETTINGS_PRINT_FONT_BODY_PANGO		"print-font-body-pango"
#define SOLI_SETTINGS_PRINT_FONT_HEADER_PANGO		"print-font-header-pango"
#define SOLI_SETTINGS_PRINT_FONT_NUMBERS_PANGO		"print-font-numbers-pango"
#define SOLI_SETTINGS_PRINT_MARGIN_LEFT		"margin-left"
#define SOLI_SETTINGS_PRINT_MARGIN_TOP			"margin-top"
#define SOLI_SETTINGS_PRINT_MARGIN_RIGHT		"margin-right"
#define SOLI_SETTINGS_PRINT_MARGIN_BOTTOM		"margin-bottom"
#define SOLI_SETTINGS_CANDIDATE_ENCODINGS		"candidate-encodings"
#define SOLI_SETTINGS_ACTIVE_PLUGINS			"active-plugins"
#define SOLI_SETTINGS_ENSURE_TRAILING_NEWLINE		"ensure-trailing-newline"

/* window state keys */
#define SOLI_SETTINGS_WINDOW_STATE			"state"
#define SOLI_SETTINGS_WINDOW_SIZE			"size"
#define SOLI_SETTINGS_SHOW_TABS_MODE			"show-tabs-mode"
#define SOLI_SETTINGS_SIDE_PANEL_SIZE			"side-panel-size"
#define SOLI_SETTINGS_SIDE_PANEL_ACTIVE_PAGE		"side-panel-active-page"
#define SOLI_SETTINGS_BOTTOM_PANEL_SIZE		"bottom-panel-size"
#define SOLI_SETTINGS_BOTTOM_PANEL_ACTIVE_PAGE		"bottom-panel-active-page"
#define SOLI_SETTINGS_ACTIVE_FILE_FILTER		"filter-id"

G_END_DECLS

#endif /* SOLI_SETTINGS_H */

/* ex:set ts=8 noet: */
