#include "includes.h"

int run_gtk_window(char* filename)
{
    int i;
    int j;
    int k;
    // type int propre à GTK
    GtkWidget* MainWindow = NULL;
    GtkWidget* sw = NULL;
    GtkWidget* label = NULL;
    GtkWidget* vbox = NULL;
    GtkWidget* hbox1 = NULL;
    GtkWidget* hbox2 = NULL;
    GtkWidget* hbox3 = NULL;
    GtkWidget* button = NULL;
    GtkTreeIter iter;
    GtkListStore* list = NULL;
    GtkTreeView* list_aff = NULL;
    GError* error02;
    gchar* temp_text;

    /*
    *  L'interface graphique est découpée en trois grandes parties :
    *   - l'entête (hbox1)
    *   - le corps (hbox2)
    *   - et le pied de la fenêtre (hbox3)
    *   Tout cela contenu dans le vbox
    */

    /* Création de la fenêtre */
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(MainWindow), "Fenetre de testBCD");
    gtk_window_set_default_size(GTK_WINDOW(MainWindow), 800, 400);
    gtk_window_set_position (GTK_WINDOW (MainWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_icon_from_file (GTK_WINDOW(MainWindow), "icon.ico", &error02);
    gtk_container_set_border_width (GTK_CONTAINER (MainWindow), 8);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    hbox3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);

    gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
    gtk_container_set_border_width (GTK_CONTAINER (hbox1), 10);
    gtk_container_set_border_width (GTK_CONTAINER (hbox2), 10);
    gtk_container_set_border_width (GTK_CONTAINER (hbox3), 10);


    /* ajout d'un contenu scrollable qui contiendra la liste */
    sw = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                       GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);

    // Ouverture du fichier reçu en paramètre
    printf("Filename : %s\n", filename);
    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        temp_text = "Erreur survenu au cours de l'ouverture du fichier!\n";
        print_error(MainWindow,temp_text);
        return -1;
    }
    else
    {
        /********************************/
        /***        EN TÊTE           ***/
        /********************************/
        label = gtk_label_new("<span size='x-large' face='comic sans ms'><b><u>Projet réalisé par</u> : </b> <i>Babacar Cissé Dia, Samsidine Aïdara</i></span>");
        gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
        gtk_box_pack_start (GTK_BOX(vbox), label, FALSE, FALSE, 0);

        label = gtk_label_new("<u>Fichier en cours</u> : ");
        gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
        gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);

        label = gtk_label_new(filename);
        // gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
        gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);

        /********************************/
        /***        CORPS             ***/
        /********************************/

        // extraction des données du fichier pour les stocker dans un tableau bi-dimensionnelle
        int nb_lignes, nb_colonnes;
        Line_data column_titles; // variable contenant le nom des attributs
        Line_data* tab_data;

        tab_data = extraction(file, &nb_lignes, &nb_colonnes, &column_titles);
        // Notification
        temp_text = g_strdup_printf("Le fichier comporte %d ligne(s) et %d colonne(s)", nb_lignes, nb_colonnes);
        label = gtk_label_new(temp_text);
        // gtk_label_set_justify (label, GTK_JUSTIFY_CENTER);
        gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);

        // types des valeurs des colonnes du fichier
        GType* types = (GType*)malloc(sizeof(GType)*nb_colonnes);
        for (i=0; i<nb_colonnes;i++)
        {
            types[i] = G_TYPE_STRING;
        }

        // remplit la liste avec les données contenues dans le tableau data de taille n
        list = create_model(tab_data, types, nb_lignes, nb_colonnes);

        // supprimer la première ligne car elle est déjà mise en en-tête
        gtk_tree_model_get_iter_first (list, &iter);
        gtk_list_store_remove (GTK_LIST_STORE (list), &iter);

        // afficheur recevant le model et le nombre de colonnes
        list_aff = create_renderer(list, column_titles);

        // puis le mettre dans un contenu scrollable
        gtk_container_add(GTK_CONTAINER(sw), list_aff);

        //
        printf("*********   Partition   *********\n");
        printf("*********               *********\n");
        Partition** partition;
        partition = partition_relation(tab_data, nb_lignes, nb_colonnes);
        printf("[+] __Partition : \n");
        afficher_part(partition, nb_colonnes);
        printf("\t======\n");
        Datas_partition* datas_part;
        datas_part = (Datas_partition*)malloc(sizeof(Datas_partition));
        datas_part->part_relat = partition;
        datas_part->nb_colonnes = nb_colonnes;
        //
        printf("*********   Partition elaguee  *********\n");
        printf("*********                      *********\n");
        Partition** partition_elaguee;
        Datas_partition* datas_part_elag;
        partition_elaguee = elaguer_partition_relation(partition, nb_colonnes);
        printf("[+] __Partition elaguee : \n");
        afficher_part(partition_elaguee, nb_colonnes);
        printf("\t======\n");

        // données pour procédure d'affichage graphique
        datas_part_elag = (Datas_partition*)malloc(sizeof(Datas_partition));
        datas_part_elag->part_relat = partition_elaguee;
        datas_part_elag->nb_colonnes = nb_colonnes;
        //
        printf("\n*********  Ensemble en accord  *********\n");
        printf("*********                       *********\n");
        Set_Of_Attrs* en_accord = agree_set(partition_elaguee, nb_colonnes, nb_lignes, tab_data);
        printf("[+] __Ensemble en accord : \n");
        print_set_of_attrs(en_accord);
        printf("\t======\n");

        // données pour procédure d'affichage graphique
        Set_Of_Attrs* datas_agree = copie_set_of_attrs(en_accord);

        //
        printf("\n*********  Dépendances  *********\n");
        printf("*********                *********\n");
        Dependancies* dependancies = dependances(en_accord, tab_data, nb_lignes, nb_colonnes);
        dependancies = fermetureTransitive(dependancies, tab_data, nb_colonnes);
        printf("[+] __Fermeture transitive : \n");
        afficher_dependances_cons(dependancies);
        printf("\t======\n");

        /*
            Les clés obtenues avec l'algorithme vont être ajoutées a la fermeture
            transitive premièrement calculée si elles n'y sont pas déjà, car alors elles
            représentes une information supplémentaire que nous n'avions pas au début.
        */

        //
        Dependancies* datas_dependance = copieDependance(dependancies);

        //
        printf("\n*********  Couverture minimale  *********\n");
          printf("*********                       *********\n");
        Dependancies* temp_dep = dependances(en_accord, tab_data, nb_lignes, nb_colonnes);
        Dependancies* cover = getCover(temp_dep);
        printf("[+] __Couverture minimale : \n");
        afficher_dependances_cons(cover);
        printf("\t======\n");
        //
        Dependancies* datas_cover = copieDependance(cover);
        //
        printf("\n*********  Recherche des cles  *********\n");
          printf("*********                      *********\n");
        Set_Of_Attrs* keys = getKeys(en_accord, tab_data, nb_colonnes);
        printf("[+] __Cles : \n");
        print_set_of_attrs(keys);
        printf("\t======\n");

        Set_Of_Attrs* datas_keys = copie_set_of_attrs(keys);

        //
        printf("\n*********     Decomposition    *********\n");
          printf("*********                      *********\n");
        Dependancies* f_transi = copieDependance(dependancies);
        Schema* init = initialSchema(f_transi, tab_data, nb_colonnes);
        Schema* schemas_collection = getDecomposition(init, copieDependance(cover), copie_set_of_attrs(keys), tab_data, nb_colonnes);
        printf("[+] : __Collection de sous-schemas : \n");
        afficherSchemas(schemas_collection);
        printf("\t======\n");
        Chaine* relation_R = get_relation_R(tab_data, nb_colonnes);
        printf("[+] : __Combinaisons de sous-schemas : \n");
        Decomposition* schema_combinaisons = getCombinaisonsDecomp(schemas_collection, relation_R);
        afficherDecomp(schema_combinaisons);
        printf("\t======\n");
        debug("DIE");
        /*
        button = gtk_button_new_with_label("Agree set path");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficher_agr_graphic), agree);
        */

        /********************************/
        /***        PIED              ***/
        /********************************/
        button = gtk_button_new_with_label("Partitions");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficher_partitions), datas_part);

        button = gtk_button_new_with_label("Partitions élaguées");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficher_partitions_elaguees), datas_part_elag);

        button = gtk_button_new_with_label("Ensembles en accord");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficher_ensembles_en_accord), datas_agree);

        button = gtk_button_new_with_label("Dépendances");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficher_dependances), datas_dependance);

        button = gtk_button_new_with_label("Couverture minimale");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficher_couverture_minimale), datas_cover);

        button = gtk_button_new_with_label("Clé(s)");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
       g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficher_cles), datas_keys);

        button = gtk_button_new_with_label("Décomposition");
        gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (afficherDecomposition), NULL);

    }

    // button = gtk_button_new_from_stock (GTK_STOCK_APPLY);
    // gtk_box_pack_start (GTK_BOX (hbox3), button, FALSE, FALSE, 0);
    // g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (hello), NULL);

    // button = gtk_button_new_from_stock (GTK_STOCK_OK);
    // g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(call_dialog_box), MainWindow);
    // gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, FALSE, 50);

    // button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    // g_signal_connect (button, "clicked", gtk_main_quit, NULL);
    // gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

    // Ajout composants à la VBOX
    gtk_box_pack_start (GTK_BOX(hbox2), sw, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hbox2, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(MainWindow), vbox);

    // affichage de la fenetre principale ainsi que ces composants
    gtk_widget_show_all(MainWindow);

    // rend le bouton "fermer" de la fenetre actif
    g_signal_connect(G_OBJECT(MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    // lancement de la boucle événementielle
    gtk_main();
    return 0;
}
