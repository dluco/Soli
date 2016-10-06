/*
 * soli-file-chooser-dialog.h
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

#ifndef SOLI_FILE_CHOOSER_DIALOG_H
#define SOLI_FILE_CHOOSER_DIALOG_H

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define SOLI_TYPE_FILE_CHOOSER_DIALOG (soli_file_chooser_dialog_get_type ())

G_DECLARE_INTERFACE (SoliFileChooserDialog, soli_file_chooser_dialog, SOLI, FILE_CHOOSER_DIALOG, GObject)

struct _SoliFileChooserDialogInterface
{
	GTypeInterface g_iface;

	/* Virtual public methods */
	void	(*set_encoding)		(SoliFileChooserDialog  *dialog,
					 const GtkSourceEncoding *encoding);

	const GtkSourceEncoding *
		(*get_encoding)		(SoliFileChooserDialog *dialog);

	void	(*set_newline_type)	(SoliFileChooserDialog  *dialog,
					 GtkSourceNewlineType     newline_type);

	GtkSourceNewlineType
		(*get_newline_type)	(SoliFileChooserDialog *dialog);

	void	(*set_current_folder)	(SoliFileChooserDialog *dialog,
					 GFile                  *folder);

	void	(*set_current_name)	(SoliFileChooserDialog *dialog,
					 const gchar            *name);

	void	(*set_file)		(SoliFileChooserDialog *dialog,
					 GFile                  *file);

	GFile *	(*get_file)		(SoliFileChooserDialog *dialog);

	GSList *(*get_files)		(SoliFileChooserDialog *dialog);

	void	(*set_do_overwrite_confirmation)
					(SoliFileChooserDialog *dialog,
					 gboolean                overwrite_confirmation);

	void	(*show)			(SoliFileChooserDialog *dialog);
	void	(*hide)			(SoliFileChooserDialog *dialog);

	void    (*destroy)		(SoliFileChooserDialog *dialog);

	void	(*set_modal)		(SoliFileChooserDialog *dialog,
					 gboolean                is_modal);

	GtkWindow *
		(*get_window)		(SoliFileChooserDialog *dialog);

	void	(*add_pattern_filter)	(SoliFileChooserDialog *dilaog,
					 const gchar            *name,
					 const gchar            *pattern);
};

typedef enum
{
	SOLI_FILE_CHOOSER_SAVE                   = 1 << 0,
	SOLI_FILE_CHOOSER_OPEN                   = 1 << 1,
	SOLI_FILE_CHOOSER_ENABLE_ENCODING        = 1 << 2,
	SOLI_FILE_CHOOSER_ENABLE_LINE_ENDING     = 1 << 3,
	SOLI_FILE_CHOOSER_ENABLE_DEFAULT_FILTERS = 1 << 4
} SoliFileChooserFlags;

SoliFileChooserDialog *
		soli_file_chooser_dialog_create		(const gchar              *title,
								 GtkWindow                *parent,
								 SoliFileChooserFlags     flags,
								 const GtkSourceEncoding  *encoding,
								 const gchar              *cancel_label,
								 GtkResponseType           cancel_response,
								 const gchar              *accept_label,
								 GtkResponseType           accept_response);

void		 soli_file_chooser_dialog_destroy		(SoliFileChooserDialog   *dialog);

void		 soli_file_chooser_dialog_set_encoding		(SoliFileChooserDialog   *dialog,
								 const GtkSourceEncoding  *encoding);

const GtkSourceEncoding *
		 soli_file_chooser_dialog_get_encoding		(SoliFileChooserDialog   *dialog);

void		 soli_file_chooser_dialog_set_newline_type	(SoliFileChooserDialog   *dialog,
								 GtkSourceNewlineType      newline_type);

GtkSourceNewlineType
		 soli_file_chooser_dialog_get_newline_type	(SoliFileChooserDialog   *dialog);

void		 soli_file_chooser_dialog_set_current_folder	(SoliFileChooserDialog   *dialog,
								 GFile                    *folder);

void		 soli_file_chooser_dialog_set_current_name	(SoliFileChooserDialog   *dialog,
								 const gchar              *name);

void		 soli_file_chooser_dialog_set_file		(SoliFileChooserDialog   *dialog,
								 GFile                    *file);

GFile		*soli_file_chooser_dialog_get_file		(SoliFileChooserDialog   *dialog);

GSList		*soli_file_chooser_dialog_get_files		(SoliFileChooserDialog   *dialog);

void		 soli_file_chooser_dialog_set_do_overwrite_confirmation (
								 SoliFileChooserDialog   *dialog,
								 gboolean                  overwrite_confirmation);

void		 soli_file_chooser_dialog_show			(SoliFileChooserDialog   *dialog);
void		 soli_file_chooser_dialog_hide			(SoliFileChooserDialog   *dialog);

void		 soli_file_chooser_dialog_set_modal		(SoliFileChooserDialog   *dialog,
								 gboolean                  is_modal);

GtkWindow	*soli_file_chooser_dialog_get_window		(SoliFileChooserDialog   *dialog);

void		 soli_file_chooser_dialog_add_pattern_filter	(SoliFileChooserDialog   *dialog,
								 const gchar              *name,
								 const gchar              *pattern);

G_END_DECLS

#endif /* SOLI_FILE_CHOOSER_DIALOG_H */

/* ex:set ts=8 noet: */
