#include "includes.h"

Dependancies* getCover(Dependancies* deps)
{
    /*
        reçoit en paramètre la fermeture transitive mais vu la définition
        de la couverture minimale, nous pouvons prendre le résultat de la
        fonction dependance()
    */
    Dependancies* dump = copieDependance(deps);
    Dependancies* temp_dump = NULL;
    Dependancies* courant_dep;
    Dependancies* courant_dep_ot;
    Dependancies* temp_dep;
    Chaine* temp_chaine;

    // supprimer les DFs où |partie de gauche| > 1
    courant_dep = dump;
    while (courant_dep != NULL)
    {
        if ( taille_chaine(courant_dep->determinant) > 1)
            dump = deleteCellDependance(dump, courant_dep);
        courant_dep = courant_dep->suiv;
    }

    // supprimer les DFs pouvant être obtenues par transitivité
    courant_dep = dump;
    while (courant_dep != NULL)
    {
        courant_dep_ot = courant_dep->suiv;
        while (courant_dep_ot != NULL)
        {
            if (!are_anagramme_chaine(courant_dep->determinant, courant_dep_ot->soumis))
            {
                if (are_anagramme_chaine(courant_dep->soumis, courant_dep_ot->determinant))
                {
                    temp_dep = createCellDependance(courant_dep->determinant, courant_dep_ot->soumis);
                    dump = deleteCellDependance(dump, temp_dep);
                    free(temp_dep);
                }
            }
            courant_dep_ot = courant_dep_ot->suiv;
        }
        courant_dep = courant_dep->suiv;
    }

    // faire les unions
    courant_dep = copieDependance(dump);
    while (courant_dep != NULL)
    {
        courant_dep_ot = courant_dep->suiv;
        while (courant_dep_ot != NULL)
        {
            if (are_anagramme_chaine(courant_dep->determinant, courant_dep_ot->determinant))
            {
                temp_chaine = copie_chaine(courant_dep->soumis);
                temp_chaine = append_chaine(temp_chaine, courant_dep_ot->soumis);
                temp_dep = createCellDependance(courant_dep->determinant, temp_chaine);
                temp_dep->soumis = delete_repeated(temp_dep->soumis);
                dump = deleteCellDependance(dump, courant_dep);
                dump = deleteCellDependance(dump, courant_dep_ot);
                dump = appendDependance(dump, temp_dep);
            }
            courant_dep_ot = courant_dep_ot->suiv;
        }
        courant_dep = courant_dep->suiv;
    }

    return dump;
}

void afficher_couverture_minimale(GtkWidget* wid, Dependancies* datas_cover)
{
    GError* error;
    GtkWidget* win;
    GtkWidget* vbox;
    GtkWidget* hbox1;
    GtkWidget* hbox2;
    GtkWidget* label;
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Dépendances");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 250);
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_window_set_icon_from_file (GTK_WINDOW(win), "icon.ico", &error);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_widget_show(win);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);

    label = gtk_label_new("<span foreground='#546E7A' size='xx-large' face='Lucida Handwriting'><u>Dépendances de la relation</u> : </span>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);

    // content here
    char* chaine = (char*)malloc(sizeof(char)*NMAX);
    char* temp_chaine;
    Dependancies* courant_dep = datas_cover;
    Chaine* courant_chaine;

    while (courant_dep != NULL)
    {
        // determinant
        Chaine* courant_chaine = courant_dep->determinant;
        while (courant_chaine != NULL)
        {
            temp_chaine = g_strdup_printf("%s ", courant_chaine->attribut);
            chaine = strcat(chaine, temp_chaine);
            courant_chaine = courant_chaine->suiv;
        }

        chaine = strcat(chaine, "==> ");
        
        // soumis
        courant_chaine = courant_dep->soumis;
        while (courant_chaine != NULL)
        {
            temp_chaine = g_strdup_printf("%s ", courant_chaine->attribut);
            chaine = strcat(chaine, temp_chaine);
            courant_chaine = courant_chaine->suiv;
        }
        chaine = strcat(chaine, "\n");
        courant_dep = courant_dep->suiv;
    }
    label = gtk_label_new(chaine);
    gtk_box_pack_start (GTK_BOX(vbox), label, FALSE, FALSE, 0);     


    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);
}
