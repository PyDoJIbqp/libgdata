/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * GData Client
 * Copyright (C) Philip Withnall 2009 <philip@tecnocode.co.uk>
 * 
 * GData Client is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GData Client is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GData Client.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GDATA_CALENDAR_EVENT_H
#define GDATA_CALENDAR_EVENT_H

#include <glib.h>
#include <glib-object.h>

#include "gdata-entry.h"
#include "gdata-gdata.h"
#include "gdata-types.h"

G_BEGIN_DECLS

#define GDATA_TYPE_CALENDAR_EVENT		(gdata_calendar_event_get_type ())
#define GDATA_CALENDAR_EVENT(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), GDATA_TYPE_CALENDAR_EVENT, GDataCalendarEvent))
#define GDATA_CALENDAR_EVENT_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GDATA_TYPE_CALENDAR_EVENT, GDataCalendarEventClass))
#define GDATA_IS_CALENDAR_EVENT(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GDATA_TYPE_CALENDAR_EVENT))
#define GDATA_IS_CALENDAR_EVENT_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GDATA_TYPE_CALENDAR_EVENT))
#define GDATA_CALENDAR_EVENT_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GDATA_TYPE_CALENDAR_EVENT, GDataCalendarEventClass))

typedef struct _GDataCalendarEventPrivate	GDataCalendarEventPrivate;

typedef struct {
	GDataEntry parent;
	GDataCalendarEventPrivate *priv;
} GDataCalendarEvent;

typedef struct {
	GDataEntryClass parent;
} GDataCalendarEventClass;

GType gdata_calendar_event_get_type (void);

GDataCalendarEvent *gdata_calendar_event_new (void);
GDataCalendarEvent *gdata_calendar_event_new_from_xml (const gchar *xml, gint length, GError **error);

G_END_DECLS

#endif /* !GDATA_CALENDAR_EVENT_H */
