#include "includes.h"

GtkListStore* create_model (Line_data* data, GType* types, int nb_lignes, int nb_colonnes)
{
    GtkListStore* model;
    GtkTreeIter iter;
    int i;

    // Stockage
    model = gtk_list_store_newv(nb_colonnes, types);
    int j;
    for (i = 0; i < nb_lignes; i++)
    {
        // création nouvelle ligne
        gtk_list_store_append (model, &iter);
        for (j=0; j < nb_colonnes; j++)
        {
            // ajout nouvelle valeur pour la ligne courante
            gtk_list_store_set(model, &iter, j, data[i].val[j], -1);
        }
    }

    return model;
}

/* Pour automatiser la création de l'afficheur avec nom de la colonne en paramètre */
GtkTreeViewColumn* column_renderer(GtkWidget* model ,char* name, int column_number)
{
    GtkTreeViewColumn* temp_column;
    GtkCellRenderer* renderer;
    renderer = gtk_cell_renderer_text_new();
    temp_column = gtk_tree_view_column_new_with_attributes(name, renderer, "text", column_number, NULL);
    gtk_tree_view_column_set_sort_column_id (temp_column, column_number);
    gtk_tree_view_append_column(GTK_TREE_VIEW(model), temp_column);
    gtk_tree_view_column_set_resizable(temp_column, TRUE);
    gtk_tree_view_column_set_sizing(temp_column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
    return temp_column;
}

GtkWidget* create_renderer(GtkListStore* model, Line_data line_data)
{
    /* Afficheur */
    GtkWidget* model_aff;
    GtkTreeViewColumn* column;
    int i;
    char* column_name;
    column_name = (char*)malloc(sizeof(char)*NMAX);

    model_aff = gtk_tree_view_new_with_model(model);
    for (i=0; i < line_data.nbre; i++)
    {
        column_name = UTF8(line_data.val[i]);
        column = column_renderer(model_aff, column_name, i);
    }

    return model_aff;
}

// lecture du fichier puis stockage dans une matrice bidimentionnelle
Line_data* extraction(FILE* file, int* p_nb_lignes, int* p_nb_colonnes, Line_data* p_column_titles)
{
    int i=0;
    char* my_string;
    line ligne;
    char* header_names;
    while ( ( my_string = fgets(ligne, NMAX, file) ) != NULL )
    {
        i++;
        if (i == 1)
        {
            // header_names contiendra le nom des attributs (colonnes)
            header_names = (char*)malloc(sizeof(char)*NMAX);
            strcpy(header_names, my_string);
        }
        printf("Ligne %d : %s",i,my_string);
    }
    *p_nb_lignes = i;

    /*
        NOTE POUR LA FONCTION EXPLODE (la même que celle qu'offre PHP)
        Séparation du contenu d'une ligne au hasard par le séparateur (ici un espace)
        afin d'obtenir le nombre de colonnes.
        Dans le cas d'un champ de type chaine de caractères contenant des espaces,
        prendre le soin d'indiquer le séparateur choisi lors de l'enregistrement
    */
    Line_data* exploded;
    exploded = explode(' ', header_names);
    *p_nb_colonnes = exploded->nbre + 1;
    p_column_titles->nbre = exploded->nbre + 1;
    p_column_titles->val = (char**)malloc(sizeof(char*)*p_column_titles->nbre);
    for (i=0; i<p_column_titles->nbre; i++)
    {
        p_column_titles->val[i] = exploded->val[i];
    }
    fseek(file,0,SEEK_SET);
    // fseek(file,nb_colonnes*sizeof(char),SEEK_SET);
    // fgets(ligne, NMAX, file);
    int k;
    int j;
    int temp_n;

    // allocation pour le tableau dynamique pour la dimension 1
    Line_data* tab_data = (Line_data*)malloc(sizeof(Line_data)*(*p_nb_lignes));
    k = 0;
    while ( ( my_string = fgets(ligne, NMAX, file) ) != NULL )
    {
        exploded = explode(' ', my_string);
        temp_n = exploded->nbre;

        // allocation pour le tableau dynamique pour la dimension 2
        tab_data[k].val = (char**)malloc(sizeof(char*)*(temp_n+1));
        for (j=0; j<temp_n+1; j++)
        {
            // allocation pour la chaine de caractère avant insertion
            tab_data[k].val[j] = (char*)malloc(sizeof(char)*(strlen(exploded->val[j])+1));
            strcpy(tab_data[k].val[j] , exploded->val[j]);
        }
        tab_data[k].nbre = temp_n;
        k++;
    }
    return tab_data;
}

void afficherDecomposition()
{
    GError error;
    GtkWidget* win;
    GtkWidget* vbox;
    GtkWidget* hbox1;
    GtkWidget* hbox2;
    GtkWidget* label;
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Résultat");
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

    label = gtk_label_new("<span foreground='#3F51B5' size='xx-large' face='Segoe Script'><u>Décomposition de la relation</u> : </span>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);



    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);
}

void hello()
{
    static int i;
    printf("Hello world %d time\n", i);
}

void debug(char* msg)
{
    if (msg == NULL)
        msg = "ok";
    // static int i=2;
    // if (i % 2 == 0)
    {
        printf("%s", msg);
        getchar();
    }
    // else
    // {
    //     printf("Fin");
    //     getchar();
    // }
    // i++;
}

/**********************/
/*****  CHAINE   ******/
/**********************/

int chars_in_linked_string(Chaine* attrs, char* motif)
{
    Chaine* courant = attrs;
    while (courant != NULL)
    {
        if (are_same_string(motif, courant->attribut))
            return 1;
        courant = courant->suiv;
    }
    return 0;
}

Chaine* create_cell_chaine(char* attr)
{
    Chaine* dump = (Chaine*)malloc(sizeof(Chaine));
    dump->attribut = (char*)malloc(sizeof(char)*strlen(attr));
    strcpy(dump->attribut, attr);
    dump->suiv = NULL;
    return dump;
}

Chaine* delete_cell_chaine(Chaine* param, Chaine* to_delete)
{
    Chaine* temp;
    // debut de liste
    if (are_same_string(param->attribut, to_delete->attribut))
    {
        temp = param->suiv;
        free(param);
        return temp;
    }
    else
    {
        // milieu de liste
        Chaine* courant = param->suiv;
        Chaine* precedent = param;
        while (courant != NULL)
        {
            if (are_same_string(courant->attribut, to_delete->attribut))
            {
                precedent->suiv = courant->suiv;
                free(courant);
                return param;
            }
            precedent = courant;
            courant = courant->suiv;
        }
        return param;
    }
}

Chaine* copie_chaine(Chaine* lk_string)
{
    Chaine* courant = lk_string;
    Chaine* temp_chaine;
    Chaine* dump = NULL;
    while (courant != NULL)
    {
        temp_chaine =  create_cell_chaine(courant->attribut);
        dump = append_chaine(dump, temp_chaine);
        courant = courant->suiv;
    }
    return dump;
}

int are_same_string(char* chaine1, char* chaine2)
{
    if (strcmp(chaine1, chaine2) == 0)
        return 1;
    else
        return 0;
}

int are_same_linked_string(Chaine* lk_chaine1, Chaine* lk_chaine2)
{
    if (taille_chaine(lk_chaine1) == taille_chaine(lk_chaine2))
    {
        Chaine* courant1 = lk_chaine1;
        Chaine* courant2 = lk_chaine2;
        while (courant1 != NULL)
        {
            if (!are_same_string(courant1->attribut, courant2->attribut))
                return 0;
            courant1 = courant1->suiv;
            courant2 = courant2->suiv;
        }
        return 1;
    }
    else
        return 0;
}

Chaine* append_chaine(Chaine* param, Chaine* to_append)
{
    if ( param == NULL )
        return to_append;
    Chaine* courant = param;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = to_append;
    return param;
}

Chaine* append_cell_chaine(Chaine* param, Chaine* cell_chaine)
{
    Chaine* temp = create_cell_chaine(cell_chaine->attribut);
    return append_chaine(param, temp);
}

Chaine* minus_chaine(Chaine* param1, Chaine* param2)
{
    // param1 - param2
    Chaine* courant_chaine;
    Chaine* courant_chaine_ot;
    Chaine* dump = copie_chaine(param1);

    courant_chaine = param1;
    while (courant_chaine != NULL)
    {
        courant_chaine_ot = param2;
        while (courant_chaine_ot != NULL)
        {
            if (are_same_string(courant_chaine->attribut, courant_chaine_ot->attribut))
                dump = delete_cell_chaine(dump, courant_chaine_ot);
            courant_chaine_ot = courant_chaine_ot->suiv;
        }
        courant_chaine = courant_chaine->suiv;
    }
    return dump;
}

Chaine* delete_repeated(Chaine* param)
{
    Chaine* courant = param;
    Chaine* courant_ot;
    Chaine* dump = copie_chaine(param);
    while (courant != NULL)
    {
        courant_ot = courant->suiv;
        while (courant_ot != NULL)
        {
            if (are_same_string(courant->attribut, courant_ot->attribut))
            {
                dump = delete_cell_chaine(dump, courant);
            }
            courant_ot = courant_ot->suiv;
        }
        courant = courant->suiv;
    }
    return dump;
}

void afficher_chaine(Chaine* param)
{
    Chaine* courant = param;
    while (courant != NULL)
    {
        printf("|%s| ", courant->attribut);
        courant = courant->suiv;
    }
}

int taille_chaine(Chaine* attr)
{
    int compteur = 0;
    Chaine* courant_chaine = attr;
    while (courant_chaine != NULL)
    {
        compteur++;
        courant_chaine = courant_chaine->suiv;
    }
    return compteur;
}

int in_linked_string(Chaine* attrs, Chaine* attr)
{
    Chaine* courant_chaine;
    Chaine* courant_chaine_ot;
    int nb_trouvailles = 0;
    int nb_voulu = taille_chaine(attr);
    /*
    *   Est dedans si on trouve dans l'ordre les maillons recherchés
    */

    courant_chaine_ot = attr;
    while (courant_chaine_ot != NULL)
    {
        courant_chaine = attrs;
        while (courant_chaine != NULL)
        {
            if (are_same_string(courant_chaine->attribut, courant_chaine_ot->attribut))
            {
                nb_trouvailles++;
            }
            else
                nb_trouvailles = 0;
            if (nb_voulu == nb_trouvailles)
                return 1;
            courant_chaine = courant_chaine->suiv;
        }
        courant_chaine_ot = courant_chaine_ot->suiv;
    }
    return 0;
}

int contained_in_linked_string(Chaine* attrs, Chaine* attr)
{

    Chaine* courant_chaine = attr;
    while (courant_chaine != NULL)
    {
        if (!chars_in_linked_string(attrs, courant_chaine->attribut))
            return 0;
        courant_chaine = courant_chaine->suiv;
    }
    return 1;
}

int are_anagramme_chaine(Chaine* ch1, Chaine* ch2)
{
    if (contained_in_linked_string(ch1, ch2) && contained_in_linked_string(ch2, ch1))
        return 1;
    else
        return 0; 
}

Chaine* get_relation_R(Line_data* tab_data, int nb_colonnes)
{
    Chaine* dump = NULL;
    int j;
    for (j=0; j<nb_colonnes; j++)
        dump = append_chaine(dump, create_cell_chaine(attribut(j, tab_data)));

    return dump;
}

//
int taille_set_of_attrs(Set_Of_Attrs* set)
{
    int compteur = 0;
    Set_Of_Attrs* courant = set;
    while (courant != NULL)
    {
        compteur++;
        courant = courant->suiv;
    }
    return compteur;
}

Set_Of_Attrs* create_cell_set_of_attrs(char* ch)
{
    Set_Of_Attrs* dump = (Set_Of_Attrs*)malloc(sizeof(Set_Of_Attrs));
    dump->attributs = create_cell_chaine(ch);
    dump->suiv = NULL;
}

Set_Of_Attrs* copie_set_of_attrs(Set_Of_Attrs* set)
{
    Set_Of_Attrs* dump = NULL;
    Set_Of_Attrs* temp_attr;
    Set_Of_Attrs* courant = set;
    while (courant != NULL)
    {
        temp_attr = (Set_Of_Attrs*)malloc(sizeof(Set_Of_Attrs));
        temp_attr->attributs = copie_chaine(courant->attributs);
        temp_attr->suiv = NULL;
        dump = append_set_of_attrs(dump, temp_attr);
        courant = courant->suiv;
    }
    return dump;
}

Set_Of_Attrs* append_set_of_attrs(Set_Of_Attrs* dump, Set_Of_Attrs* to_append)
{
    // rompre une éventuelle suite car ici on ne rajoute qu'une cellule
    // dupliquer
    if (to_append == NULL)
        return dump;
    Set_Of_Attrs* temp = (Set_Of_Attrs*)malloc(sizeof(Set_Of_Attrs));
    temp->attributs = (Chaine*)malloc(sizeof(Chaine));
    temp->attributs = copie_chaine(to_append->attributs);
    temp->suiv = NULL;
    if ( dump == NULL )
        return temp;
    Set_Of_Attrs* courant = dump;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = temp;
    return dump;
}

Set_Of_Attrs* fusion_set_of_attrs(Set_Of_Attrs* param1, Set_Of_Attrs* param2)
{
    if ( param1 == NULL )
        return param2;
    Set_Of_Attrs* courant = param1;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = param2;
    return param1;
}

int in_set_of_attrs(Set_Of_Attrs* ag, Set_Of_Attrs* wanted)
{
    if (ag == NULL)
        return 0;
    Set_Of_Attrs* courant = ag;
    Chaine* courant_chaine;
    Chaine* courant_chaine_ot;
    courant_chaine_ot = wanted->attributs;
    while (courant != NULL)
    {
        courant_chaine = courant->attributs;
        if (taille_chaine(courant_chaine) == taille_chaine(courant_chaine_ot))
        {
            while (courant_chaine != NULL)
            {
                if (are_same_linked_string(courant_chaine, courant_chaine_ot))
                    return 1;
                courant_chaine = courant_chaine->suiv;
            }
        }
        courant = courant->suiv;
    }
    return 0;
}

Set_Of_Attrs* delete_cell_set_of_attrs(Set_Of_Attrs* param1, Set_Of_Attrs* cell)
{
    // supprime la première occurence de cell dans param1
    Set_Of_Attrs* courant = param1;
    Set_Of_Attrs* precedent;

    // si l'élément se trouve en tête
    if ( are_same_linked_string(param1->attributs, cell->attributs) )
    {

        Set_Of_Attrs* temp = param1->suiv;
        free(param1);
        return temp;
    }

    while (courant != NULL && !are_same_linked_string(courant->attributs, cell->attributs))
    {
        precedent = courant;
        courant = courant->suiv;
    }
    if (courant != NULL)
    {
        precedent->suiv = courant->suiv;
        free(courant);
    }
    return param1;
}

Set_Of_Attrs* minus_set_of_attrs(Set_Of_Attrs* param1, Set_Of_Attrs* param2)
{
    // param1 - param2 (soustraction ensembliste)
    Set_Of_Attrs* courant1;
    Set_Of_Attrs* courant2 = param2;
    while (courant2 != NULL)
    {
        if (in_set_of_attrs(param1, courant2))
            param1 = delete_cell_set_of_attrs(param1, courant2);
        courant2 = courant2->suiv;
    }
}

/******************/
/***    tuples  ***/
/******************/

int cardinal_tuples (Entier* tuples)
{
    int compteur = 0;
    Entier* courant = tuples;
    while ( courant != NULL )
    {
        compteur++;
        courant = courant->suiv;
    }
    return compteur;
}

int in_tuples(Entier* tuples, Entier* voulu)
{
    Entier* courant = tuples;
    while (courant != NULL)
    {
        if (courant->val == voulu->val)
            return 1;
        courant = courant->suiv;
    }
    return 0;
}

Entier* append_tuple(Entier* ag, Entier* a_ajouter)
{
    if (ag == NULL)
        return a_ajouter;
    Entier* courant = ag;

    // aller au dernier élément
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = a_ajouter;

    return ag;
}

int get_last_tuple_value(Entier* tuple)
{
    Entier* courant = tuple;
    if (tuple == NULL)
        return EXIT_FAILURE;
    while (courant->suiv != NULL)
    {
        courant = courant->suiv;
    }
    return courant->val;
}

Entier* copie_tuple(Entier* a_copier)
{
    Entier* courant = a_copier;
    Entier* parcours;
    Entier* temp;
    Entier* retour = NULL;
    while (courant != NULL)
    {
        if (retour == NULL)
        {
            retour = (Entier*)malloc(sizeof(Entier));
            retour->val = courant->val;
            retour->suiv = NULL;
            parcours = retour;
        }
        else
        {
            temp = (Entier*)malloc(sizeof(Entier));
            temp->val = courant->val;
            temp->suiv = NULL;
            parcours->suiv = temp;
            parcours = parcours->suiv;
        }
        courant = courant->suiv;
    }

    return retour;
}

int tuples_identiques(Entier* t1, Entier* t2)
{
    if (cardinal_tuples(t1) != cardinal_tuples(t2))
        return 0;

    Entier* c1 = t1;
    Entier* c2 = t2;

    while (c1 != NULL)
    {
        if ( c1->val != c2->val )
            return 0;
        c1 = c1->suiv;
        c2 = c2->suiv;
    }
    return 1;
}

void afficher_tuple(Entier* tuple)
{
    Entier* c = tuple;
    while (c != NULL)
    {
        printf("tuple numero %d \n", c->val);
        c = c->suiv;
    }
    printf("\t---\n");
}

/******************/
/***   couples  ***/
/******************/
int in_couples(Couples* couples, Couples* couple)
{
    Couples* courant = couples;
    while (courant != NULL)
    {
        if (courant->cpl[0] == couple->cpl[0]
                && courant->cpl[1] == couple->cpl[1])
            return 1;
        courant = courant->suiv;
    }
    return 0;
}

Couples* append_couple(Couples* dump, Couples* to_append)
{
    if ( dump == NULL )
        return to_append;
    Couples* courant = dump;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = to_append;
    return dump;
}

//
char* attribut(int k, Line_data* tab_data)
{
    return tab_data[0].val[k];
}

int colonne(char* attribut, Line_data* tab_data, int nb_colonnes)
{
    int j;
    for (j=0; j<nb_colonnes; j++)
    {
        if (are_same_string(attribut, tab_data[0].val[j]))
            return j;
    }
    printf("L'attribut n'est pas dans le tableau de donnees\n");
    getchar();
    exit(-1);
    return EXIT_FAILURE;
}

Entier* generate_all_couples(int nb_colonnes)
{
    Entier* dump = NULL;
}

void afficher_part(Partition** part, int nb_colonnes)
{
    int j;
    int first_value;
    Partition* courant_part;
    Entier* courant_tuple;

    for (j=0; j<nb_colonnes; j++)
    {
        courant_part = part[j];
        printf("P%d : ", j+1);
        while (courant_part != NULL)
        {
            courant_tuple = courant_part->tuples;
            printf("{");
            first_value = 1;
            while (courant_tuple != NULL)
            {
                if (first_value == 1)
                {
                    printf("%d", courant_tuple->val);
                }
                else
                {
                    printf(", %d", courant_tuple->val);
                }
                first_value++;
                courant_tuple = courant_tuple->suiv;
            }
            if (courant_part->suiv == NULL)
                printf("}\n");
            else
                printf("}, ");
            courant_part = courant_part->suiv;
        }
    }
}
