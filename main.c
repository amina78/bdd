#include "includes.h"

int main (int argc, char* argv[])
{
  printf("Nous avons recu en parametre :\n");
  int i, j;
  line ligne;
  char* my_string;

  for (i = 0; i < argc; i++)
  {
    printf("Argument %d = %s\n",i+1,argv[i]);
  }
  // Nous considérerons que l'argument numéro 2 est le nom du fichier.

  // Initialisation de GTK
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  // lancement fenetre fenetre principal
  run_gtk_window(argv[1]); //retourne 0 si tout s'est bien passé

  return EXIT_SUCCESS;
}
