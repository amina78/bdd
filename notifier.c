#include "includes.h"

void notifier (gpointer* parent)
{
//   Dialog_box* p = (Dialog_box*)malloc(sizeof(Dialog_box));
//   p->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
//   p->hbox = gtk_box_new
//    (GTK_ORIENTATION_HORIZONTAL, 8);
//   p = gtk_dialog_new_with_buttons ("Sauvegarder", GTK_WINDOW(parent),
//     GTK_DIALOG_MODAL,
//     GTK_STOCK_YES,
//     GTK_RESPONSE_YES,
//     GTK_STOCK_NO,
//     GTK_RESPONSE_NO,
//     GTK_STOCK_CANCEL,
//     GTK_RESPONSE_CANCEL,
//     NULL);
//   GtkWidget *p_label = NULL;
//   p_label = gtk_label_new ("Voulez-vous sauvegarder les modifications ?");
//   gtk_box_pack_start (GTK_BOX (p->vbox), p_label, TRUE, TRUE, 0);

//   switch (gtk_dialog_run (GTK_DIALOG (p)))
//   {
//     case GTK_RESPONSE_YES:
//       printf("You have choosen to quit");
//       break;
//     case GTK_RESPONSE_NO:
//       printf("You have choosen NOT to quit");
//       break;
//     case GTK_RESPONSE_CANCEL:
//       printf("Bye bye...\n");
//       break;
//   }
//   gtk_widget_destroy (p);
}

// fonctions de notification
void print_info (GtkWidget* win, gchar* msg)
{
    GtkWidget *p_dialog = NULL;
    p_dialog = gtk_message_dialog_new (win, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, msg);
    gtk_dialog_run (GTK_DIALOG (p_dialog));
    gtk_widget_destroy (p_dialog);
}

void print_error (GtkWidget* win, gchar* msg)
{
    GtkWidget *p_dialog = NULL;
    p_dialog = gtk_message_dialog_new (win, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, msg);
    gtk_dialog_run (GTK_DIALOG (p_dialog));
    gtk_widget_destroy (p_dialog);
}

void print_warning (GtkWidget* win, gchar* msg)
{
    GtkWidget *p_dialog = NULL;
    p_dialog = gtk_message_dialog_new (win, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, msg);
    gtk_dialog_run (GTK_DIALOG (p_dialog));
    gtk_widget_destroy (p_dialog);
}