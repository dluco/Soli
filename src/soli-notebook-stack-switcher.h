/*
 * soli-notebook-stack-switcher.h
 * This file is part of soli
 *
 * Copyright (C) 2014 - Paolo Borelli
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

#ifndef SOLI_NOTEBOOK_STACK_SWITCHER_H
#define SOLI_NOTEBOOK_STACK_SWITCHER_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOLI_TYPE_NOTEBOOK_STACK_SWITCHER             (soli_notebook_stack_switcher_get_type())
#define SOLI_NOTEBOOK_STACK_SWITCHER(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), SOLI_TYPE_NOTEBOOK_STACK_SWITCHER, SoliNotebookStackSwitcher))
#define SOLI_NOTEBOOK_STACK_SWITCHER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), SOLI_TYPE_NOTEBOOK_STACK_SWITCHER, SoliNotebookStackSwitcherClass))
#define SOLI_IS_NOTEBOOK_STACK_SWITCHER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), SOLI_TYPE_NOTEBOOK_STACK_SWITCHER))
#define SOLI_IS_NOTEBOOK_STACK_SWITCHER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_NOTEBOOK_STACK_SWITCHER))
#define SOLI_NOTEBOOK_STACK_SWITCHER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), SOLI_TYPE_NOTEBOOK_STACK_SWITCHER, SoliNotebookStackSwitcherClass))

typedef struct _SoliNotebookStackSwitcher        SoliNotebookStackSwitcher;
typedef struct _SoliNotebookStackSwitcherClass   SoliNotebookStackSwitcherClass;
typedef struct _SoliNotebookStackSwitcherPrivate SoliNotebookStackSwitcherPrivate;

struct _SoliNotebookStackSwitcher
{
	GtkBin parent;

	/*< private >*/
	SoliNotebookStackSwitcherPrivate *priv;
};

struct _SoliNotebookStackSwitcherClass
{
	GtkBinClass parent_class;

	/* Padding for future expansion */
	void (*_soli_reserved1) (void);
	void (*_soli_reserved2) (void);
	void (*_soli_reserved3) (void);
	void (*_soli_reserved4) (void);
};

GType		 soli_notebook_stack_switcher_get_type   (void) G_GNUC_CONST;

GtkWidget	*soli_notebook_stack_switcher_new        (void);

void		 soli_notebook_stack_switcher_set_stack  (SoliNotebookStackSwitcher *switcher,
		                                           GtkStack                   *stack);

GtkStack	*soli_notebook_stack_switcher_get_stack  (SoliNotebookStackSwitcher *switcher);

G_END_DECLS

#endif  /* SOLI_NOTEBOOK_STACK_SWITCHER_H  */

/* ex:set ts=8 noet: */
