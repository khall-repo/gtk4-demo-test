/**
 * @file main.c
 * @brief GTK4 Application demo
 * @date 01-25-2025
 * @par This is me trying to understand the GTK4 Application demo, that is
 * included in the GTK4 dev package. I copied the code from the "Application
 * Class" demo. Adding things to it, to see how it works.
 * @note The main() is actually located in application.c, that is how it
 * is in the GTK4 dev package.
 */
#include <gtk/gtk.h>

static gboolean name_seen;
static GtkWidget *placeholder;

static void on_name_appeared (GDBusConnection *connection,
                  const char      *name,
                  const char      *name_owner,
                  gpointer         user_data)
{
  name_seen = TRUE;
}

static void on_name_vanished (GDBusConnection *connection,
                  const char      *name,
                  gpointer         user_data)
{
  if (!name_seen)
    return;

  g_clear_object (&placeholder);
}

#ifdef G_OS_WIN32
#define APP_EXTENSION ".exe"
#else`
#define APP_EXTENSION
#endif

GtkWidget *do_application_demo (GtkWidget *toplevel)
{
  static guint watch = 0;

  if (watch == 0)
    watch = g_bus_watch_name (G_BUS_TYPE_SESSION,
                              "org.gtk.Demo4.App",
                              0,
                              on_name_appeared,
                              on_name_vanished,
                              NULL, NULL);

  if (placeholder == NULL)
    {
      const char *command;
      GError *error = NULL;

      if (g_file_test ("./gtk4-demo-application" APP_EXTENSION, G_FILE_TEST_IS_EXECUTABLE))
        command = "./gtk4-demo-application" APP_EXTENSION;
      else
        command = "gtk4-demo-application";

      if (!g_spawn_command_line_async (command, &error))
        {
          g_warning ("%s", error->message);
          g_error_free (error);
        }

      placeholder = gtk_label_new ("");
      g_object_ref_sink (placeholder);
    }
  else
    {
      g_dbus_connection_call_sync (g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, NULL),
                                   "org.gtk.Demo4.App",
                                   "/org/gtk/Demo4/App",
                                   "org.gtk.Actions",
                                   "Activate",
                                   g_variant_new ("(sava{sv})", "quit", NULL, NULL),
                                   NULL,
                                   0,
                                   G_MAXINT,
                                   NULL, NULL);
    }

  return placeholder;
}
