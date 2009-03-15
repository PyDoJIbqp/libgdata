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

#include <glib.h>
#include <unistd.h>

#include "gdata.h"
#include "common.h"

/* TODO: probably a better way to do this; some kind of data associated with the test suite? */
static GDataService *service = NULL;
static GMainLoop *main_loop = NULL;

static void
test_authentication (void)
{
	gboolean retval;
	GError *error = NULL;

	/* Create a service */
	service = GDATA_SERVICE (gdata_calendar_service_new (CLIENT_ID));

	g_assert (service != NULL);
	g_assert (GDATA_IS_SERVICE (service));
	g_assert_cmpstr (gdata_service_get_client_id (service), ==, CLIENT_ID);

	/* Log in */
	retval = gdata_service_authenticate (service, USERNAME, PASSWORD, NULL, &error);
	g_assert_no_error (error);
	g_assert (retval == TRUE);
	g_clear_error (&error);

	/* Check all is as it should be */
	g_assert (gdata_service_is_authenticated (service) == TRUE);
	g_assert_cmpstr (gdata_service_get_username (service), ==, USERNAME);
	g_assert_cmpstr (gdata_service_get_password (service), ==, PASSWORD);
}

static void
test_authentication_async_cb (GDataService *service, GAsyncResult *async_result, gpointer user_data)
{
	gboolean retval;
	GError *error = NULL;

	retval = gdata_service_authenticate_finish (service, async_result, &error);
	g_assert_no_error (error);
	g_assert (retval == TRUE);
	g_clear_error (&error);

	g_main_loop_quit (main_loop);

	/* Check all is as it should be */
	g_assert (gdata_service_is_authenticated (service) == TRUE);
	g_assert_cmpstr (gdata_service_get_username (service), ==, USERNAME);
	g_assert_cmpstr (gdata_service_get_password (service), ==, PASSWORD);
}

static void
test_authentication_async (void)
{
	/* Create a service */
	service = GDATA_SERVICE (gdata_calendar_service_new (CLIENT_ID));

	g_assert (service != NULL);
	g_assert (GDATA_IS_SERVICE (service));

	gdata_service_authenticate_async (service, USERNAME, PASSWORD, NULL, (GAsyncReadyCallback) test_authentication_async_cb, NULL);

	main_loop = g_main_loop_new (NULL, TRUE);
	g_main_loop_run (main_loop);
	g_main_loop_unref (main_loop);
}

static void
test_query_all_calendars (void)
{
	GDataFeed *feed;
	GError *error = NULL;

	g_assert (service != NULL);

	feed = gdata_calendar_service_query_all_calendars (GDATA_CALENDAR_SERVICE (service), NULL, NULL, NULL, NULL, &error);
	g_assert_no_error (error);
	g_assert (GDATA_IS_FEED (feed));
	g_clear_error (&error);

	/* TODO: check entries and feed properties */

	g_object_unref (feed);
}

static void
test_query_all_calendars_async_cb (GDataService *service, GAsyncResult *async_result, gpointer user_data)
{
	GDataFeed *feed;
	GError *error = NULL;

	feed = gdata_service_query_finish (service, async_result, &error);
	g_assert_no_error (error);
	g_assert (GDATA_IS_FEED (feed));
	g_clear_error (&error);

	/* TODO: Tests? */
	g_main_loop_quit (main_loop);

	g_object_unref (feed);
}

static void
test_query_all_calendars_async (void)
{
	g_assert (service != NULL);

	gdata_calendar_service_query_all_calendars_async (GDATA_CALENDAR_SERVICE (service), NULL, NULL, NULL,
							  NULL, (GAsyncReadyCallback) test_query_all_calendars_async_cb, NULL);

	main_loop = g_main_loop_new (NULL, TRUE);
	g_main_loop_run (main_loop);
	g_main_loop_unref (main_loop);
}

static void
test_query_own_calendars (void)
{
	GDataFeed *feed;
	GError *error = NULL;

	g_assert (service != NULL);

	feed = gdata_calendar_service_query_own_calendars (GDATA_CALENDAR_SERVICE (service), NULL, NULL, NULL, NULL, &error);
	g_assert_no_error (error);
	g_assert (GDATA_IS_FEED (feed));
	g_clear_error (&error);

	/* TODO: check entries and feed properties */

	g_object_unref (feed);
}

static void
test_query_own_calendars_async_cb (GDataService *service, GAsyncResult *async_result, gpointer user_data)
{
	GDataFeed *feed;
	GError *error = NULL;

	feed = gdata_service_query_finish (service, async_result, &error);
	g_assert_no_error (error);
	g_assert (GDATA_IS_FEED (feed));
	g_clear_error (&error);

	/* TODO: Tests? */
	g_main_loop_quit (main_loop);

	g_object_unref (feed);
}

static void
test_query_own_calendars_async (void)
{
	g_assert (service != NULL);

	gdata_calendar_service_query_own_calendars_async (GDATA_CALENDAR_SERVICE (service), NULL, NULL, NULL,
							  NULL, (GAsyncReadyCallback) test_query_own_calendars_async_cb, NULL);

	main_loop = g_main_loop_new (NULL, TRUE);
	g_main_loop_run (main_loop);
	g_main_loop_unref (main_loop);
}

static void
test_query_events (void)
{
	GDataFeed *feed, *calendar_feed;
	GDataCalendarCalendar *calendar;
	GList *calendars;
	GError *error = NULL;

	g_assert (service != NULL);

	/* Get a calendar */
	calendar_feed = gdata_calendar_service_query_own_calendars (GDATA_CALENDAR_SERVICE (service), NULL, NULL, NULL, NULL, &error);
	g_assert_no_error (error);
	g_assert (GDATA_IS_FEED (calendar_feed));
	g_clear_error (&error);

	calendars = gdata_feed_get_entries (calendar_feed);
	g_assert (calendars != NULL);
	calendar = calendars->data;
	g_assert (GDATA_IS_CALENDAR_CALENDAR (calendar));

	g_object_ref (calendar);
	g_object_unref (calendar_feed);

	/* Get the entry feed */
	feed = gdata_calendar_service_query_events (GDATA_CALENDAR_SERVICE (service), calendar, NULL, NULL, NULL, NULL, &error);
	g_assert_no_error (error);
	g_assert (GDATA_IS_FEED (feed));
	g_clear_error (&error);

	/* TODO: check entries and feed properties */

	g_object_unref (feed);
	g_object_unref (calendar);
}

int
main (int argc, char *argv[])
{
	gint retval;

	g_type_init ();
	g_thread_init (NULL);
	g_test_init (&argc, &argv, NULL);
	g_test_bug_base ("http://bugzilla.gnome.org/show_bug.cgi?id=");

	g_test_add_func ("/calendar/authentication", test_authentication);
	if (g_test_thorough () == TRUE)
		g_test_add_func ("/calendar/authentication_async", test_authentication_async);
	g_test_add_func ("/calendar/query/all_calendars", test_query_all_calendars);
	if (g_test_thorough () == TRUE)
		g_test_add_func ("/calendar/query/all_calendars_async", test_query_all_calendars_async);
	g_test_add_func ("/calendar/query/own_calendars", test_query_own_calendars);
	if (g_test_thorough () == TRUE)
		g_test_add_func ("/calendar/query/own_calendars_async", test_query_own_calendars_async);
	g_test_add_func ("/calendar/query/events", test_query_events);

	retval = g_test_run ();
	if (service != NULL)
		g_object_unref (service);

	return retval;
}