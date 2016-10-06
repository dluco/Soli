/*
 * soli-print-job.h
 * This file is part of soli
 *
 * Copyright (C) 2000-2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2008 Paolo Maggi
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

#ifndef SOLI_PRINT_JOB_H
#define SOLI_PRINT_JOB_H

#include <gtk/gtk.h>
#include "soli-view.h"

G_BEGIN_DECLS

#define SOLI_TYPE_PRINT_JOB (soli_print_job_get_type())

G_DECLARE_FINAL_TYPE (SoliPrintJob, soli_print_job, SOLI, PRINT_JOB, GObject)

typedef enum
{
	SOLI_PRINT_JOB_STATUS_PAGINATING,
	SOLI_PRINT_JOB_STATUS_DRAWING
} SoliPrintJobStatus;

typedef enum
{
	SOLI_PRINT_JOB_RESULT_OK,
	SOLI_PRINT_JOB_RESULT_CANCEL,
	SOLI_PRINT_JOB_RESULT_ERROR
} SoliPrintJobResult;

SoliPrintJob		*soli_print_job_new			(SoliView                *view);

GtkPrintOperationResult	 soli_print_job_print			(SoliPrintJob            *job,
								 GtkPrintOperationAction   action,
								 GtkPageSetup             *page_setup,
								 GtkPrintSettings         *settings,
								 GtkWindow                *parent,
								 GError                  **error);

void			 soli_print_job_cancel			(SoliPrintJob            *job);

const gchar		*soli_print_job_get_status_string	(SoliPrintJob            *job);

gdouble			 soli_print_job_get_progress		(SoliPrintJob            *job);

GtkPrintSettings	*soli_print_job_get_print_settings	(SoliPrintJob            *job);

GtkPageSetup		*soli_print_job_get_page_setup		(SoliPrintJob            *job);

G_END_DECLS

#endif /* SOLI_PRINT_JOB_H */

/* ex:set ts=8 noet: */
