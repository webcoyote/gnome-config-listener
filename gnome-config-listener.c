// gnome-config-listener
// by Patrick Wyatt (2/4/2013)
// A program to monitor gnome configuration changes

#include <gtk/gtk.h>
#include <gconf/gconf-client.h>
#include <signal.h>

typedef struct _Listener {
  GConfClient* client;
  guint cnxn_id;
} Listener;

static
gboolean
is_default_value (
  GConfClient* client,
  const GConfEntry* entry
) {
  // This doesn't work:
  // return gconf_entry_get_is_default(entry);

  GError* error = NULL;
  const GConfValue* def = gconf_client_get_default_from_schema(
    client,
    gconf_entry_get_key(entry),
    &error
  );
  if (error || !def)
      return FALSE;
  return gconf_value_compare(def, entry->value) == 0;
}

static
void
entry_changed_callback (
  GConfClient* client,
  guint cnxn_id,
  GConfEntry* entry,
  gpointer user_data
) {
  if (!entry->value)
    return;

  gboolean is_default = is_default_value(client, entry);
  #define PRINTIZE(TYPE, type, fmt)           \
    case GCONF_VALUE_##TYPE:                  \
      printf(                                 \
        "\"%s\" \"" #fmt "\" " #type " %s\n", \
        gconf_entry_get_key(entry),           \
        gconf_value_get_##type(entry->value), \
        is_default ? "default" : "set"        \
      );                                      \
    break                                     //
  #define ERRIZE(TYPE, type)                          \
    case GCONF_VALUE_##TYPE:                          \
      fprintf(                                        \
        stderr,                                       \
        "Error: <" #type "> not implemented (%s)\n",  \
        gconf_entry_get_key(entry)                    \
      );                                              \
    break                                             //
  switch (entry->value->type) {
    PRINTIZE(STRING, string, %s);
    PRINTIZE(INT, int, %d);
    PRINTIZE(FLOAT, float, %f);
    PRINTIZE(BOOL, bool, %d);

    // TODO: implement these types
    ERRIZE(SCHEMA, schema);
    ERRIZE(LIST, list);
    ERRIZE(PAIR, pair);

    ERRIZE(INVALID, invalid);
  }
  #undef PRINTIZE
  #undef ERRIZE
  fflush(stdout);
}

static
void
listen_init (Listener* listener)
{
  // Monitor "everything"
  listener->client = gconf_client_get_default();
  gconf_client_add_dir(
    listener->client,
    "/",
    GCONF_CLIENT_PRELOAD_NONE,
    NULL
  );

  // Set callback
  listener->cnxn_id = gconf_client_notify_add(
    listener->client,
    "/",
    entry_changed_callback,
    NULL,
    NULL,
    NULL
  );
}

static
void
listen_cleanup (Listener* listener)
{
  gconf_client_notify_remove(listener->client, listener->cnxn_id);
  gconf_client_remove_dir(listener->client, "/", NULL);
}

//***********************************************
// Signals

static
void
termination_handler (int signum)
{
  gtk_main_quit();
}

int
main (int argc, char **argv)
{
  gtk_init(&argc, &argv);
  gconf_init(argc, argv, NULL);

  // TODO: take "listen-directory" as an argument
  // TODO: take "print-default-values" as a flag
  Listener listener;
  listen_init(&listener);

  // TODO: implement "press any key to quit"
  signal(SIGINT, termination_handler);
  signal(SIGHUP, termination_handler);
  signal(SIGTERM, termination_handler);
  gtk_main();

  printf("\n");
  listen_cleanup(&listener);
  return 0;
}
