#include "includes.h"

Set_Of_Attrs* initialize_li(Line_data* tab_data, int nb_colonnes)
{
	int j;
	Set_Of_Attrs* temp_attr;
	Set_Of_Attrs* Li = NULL;
	for (j=0; j<nb_colonnes; j++)
	{
		temp_attr = create_cell_set_of_attrs(attribut(j, tab_data) );
		Li = append_set_of_attrs(Li, temp_attr);
	}
	return Li;
}

int not_contained_at_all(Set_Of_Attrs* set, Chaine* attr)
{
    Set_Of_Attrs* courant_attr = set;
    while (courant_attr != NULL)
    {
        if (contained_in_linked_string(courant_attr->attributs, attr))
            return 0;
        courant_attr = courant_attr->suiv;
    }
    return 1;
}

int all_in_condition(Line_data* tab_data, int nb_colonnes, Set_Of_Attrs* agree_set)
{
	int j;
	Set_Of_Attrs* courant_agree = agree_set;
	Set_Of_Attrs* temp_attr;
	while (courant_agree != NULL)
	{
		for (j=0; j<nb_colonnes; j++)
		{
			temp_attr = create_cell_set_of_attrs(attribut(j, tab_data));
			if (!in_linked_string(courant_agree->attributs, temp_attr->attributs))
				return 0;
			free(temp_attr);
		}
		courant_agree = courant_agree->suiv;
	}
	return 1;
}

Set_Of_Attrs* get_keylevel(Set_Of_Attrs* Li, Set_Of_Attrs* agree_set, Line_data* tab_data, int nb_colonnes)
{
	Set_Of_Attrs* keylevels = NULL;
	Set_Of_Attrs* courant_attr;
	Set_Of_Attrs* temp_attr;
	Set_Of_Attrs* courant_agree;
	int j;

	courant_attr = Li;
	while (courant_attr != NULL)
	{
		if (not_contained_at_all(agree_set, courant_attr->attributs)
			|| all_in_condition(tab_data, nb_colonnes, agree_set))
		{
			temp_attr = (Set_Of_Attrs*)malloc(sizeof(Set_Of_Attrs));
			temp_attr->attributs = copie_chaine(courant_attr->attributs);
			temp_attr->suiv = NULL;
			keylevels = append_set_of_attrs(keylevels, temp_attr);
		}

		courant_attr = courant_attr->suiv;
	}

	return keylevels;
}

Set_Of_Attrs* get_li(int level, int nb_colonnes, Line_data* tab_data, Set_Of_Attrs* agree_set)
{
	Set_Of_Attrs* dump = NULL;
	Combinaison* combi = getCombinaisonsNumbers(nb_colonnes);
	combi = onlySizeN(combi, level);
	dump = getCombinaisonsAttrs(combi, tab_data);

	// suppression de ceux qui sont dans l'ensemble en accord
	Set_Of_Attrs* courant_agree;
	Set_Of_Attrs* courant_attr = copie_set_of_attrs(dump);
	while (courant_attr != NULL)
	{
		courant_agree = agree_set;
		while (courant_agree != NULL)
		{
			if ( contained_in_linked_string(courant_agree->attributs, courant_attr->attributs) )
                dump = delete_cell_set_of_attrs(dump, courant_attr);
			courant_agree = courant_agree->suiv;
		}
		courant_attr = courant_attr->suiv;
	}
	return dump;
}

Set_Of_Attrs* get_minimals_only(Set_Of_Attrs* param)
{
	if (param == NULL)
		return NULL;
	Set_Of_Attrs* courant = param->suiv;
	int mini = taille_chaine(param->attributs);
	while (courant != NULL)
	{
		if ( taille_chaine(courant->attributs) < mini )
			mini = taille_chaine(courant->attributs);
		courant = courant->suiv;
	}

	// only
	courant = param;
	Set_Of_Attrs* dump = NULL;
	while (courant != NULL)
	{
		if ( taille_chaine(courant->attributs) == mini )
			dump = append_set_of_attrs(dump, courant);
		courant = courant->suiv;
	}
	return dump;
}

Set_Of_Attrs* getKeys(Set_Of_Attrs* agree_set, Line_data* tab_data, int nb_colonnes)
{
	// initialisations
	int i;
	Set_Of_Attrs* key_level;
	Set_Of_Attrs* keys = NULL;
	Set_Of_Attrs* courant_li;
	Set_Of_Attrs* Li;
	Set_Of_Attrs* combi;

	Li = initialize_li(tab_data, nb_colonnes);
	courant_li = Li;
	int n = 1;
	while ( courant_li != NULL)
	{
		key_level = get_keylevel(courant_li, agree_set, tab_data, nb_colonnes);
		courant_li = minus_set_of_attrs(courant_li, key_level);
		n++;
		keys = fusion_set_of_attrs(keys, key_level);
		courant_li = get_li(n, nb_colonnes, tab_data, agree_set);
	}

	keys = get_minimals_only(keys);

	return keys;
}

void afficher_cles(GtkWidget* wid, Set_Of_Attrs* datas_keys)
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
    free(label);

    // content here
    char* chaine = (char*)malloc(sizeof(char)*NMAX);
    char* temp_chaine = (char*)malloc(sizeof(char)*NMAX);
    Dependancies* courant_dep = datas_keys;
    Chaine* courant_chaine;

    while (courant_dep != NULL)
    {
        // determinant
        Chaine* courant_chaine = courant_dep->determinant;
        while (courant_chaine != NULL)
        {
            temp_chaine = g_strdup_printf("%s ", courant_chaine->attribut);
            chaine = strcat(chaine, temp_chaine);
            free(temp_chaine);
            courant_chaine = courant_chaine->suiv;
        }

        chaine = strcat(chaine, "==> ");

        // soumis
        courant_chaine = courant_dep->soumis;
        while (courant_chaine != NULL)
        {
            temp_chaine = g_strdup_printf("%s ", courant_chaine->attribut);
            chaine = strcat(chaine, temp_chaine);
            free(temp_chaine);
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
