#include "includes.h"

Combinaison* appendElementCombinaison(Combinaison* dump, Entier* to_append)
{
	Combinaison* temp;
	temp = (Combinaison*)malloc(sizeof(Combinaison));
	temp->combi = to_append;
	temp->suiv = NULL;
	if (dump == NULL)
	{
		return temp;
	}
	else
	{
		// parcours jusqu'au dernier élément
		Combinaison* courant = dump;
		while (courant->suiv != NULL)
			courant = courant->suiv;
		courant->suiv = temp;
		return dump;
	}
}

Combinaison* appendCellCombinaison(Combinaison* dump, Combinaison* to_append)
{
    if ( dump == NULL )
        return to_append;
    Combinaison* courant = dump;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = to_append;
    return dump;
}

Combinaison* getCombinaisonsNumbersLevel(Combinaison* previous_level, int niveau, int nb_colonnes)
{
	Combinaison* courant_combi = previous_level;
	Combinaison* dump = NULL;
	Combinaison* parcours;
	Entier* temp_tuple;
	Entier* temp_ot;
	int i, x;

	while (courant_combi != NULL)
	{
		x = get_last_tuple_value(courant_combi->combi);
		for (i=x+1; i<nb_colonnes; i++)
		{
			// dupliquer pour ne pas écraser l'ancienne combinaison
			temp_tuple = copie_tuple(courant_combi->combi);
			temp_ot = (Entier*)malloc(sizeof(Entier));
			temp_ot->val = i;
			temp_ot->suiv = NULL;
			temp_tuple = append_tuple(temp_tuple, temp_ot);
			dump = appendElementCombinaison(dump, temp_tuple);
		}
		courant_combi = courant_combi->suiv;
	}

	return dump;
}

Combinaison* getCombinaisonsNumbers(int nb_colonnes)
{
	Combinaison* dump = NULL;
	Combinaison* temp;
	int i;
	Entier* temp_tuple;

	// initialisation ( niveau 1 )
	for (i=0; i<nb_colonnes; i++)
	{
		temp_tuple = (Entier*)malloc(sizeof(Entier));
		temp_tuple->val = i;
		temp_tuple->suiv = NULL;
		dump = appendElementCombinaison(dump, temp_tuple);
	}

	// autres niveaux
	temp = dump;
	for (i=2; i<=nb_colonnes; i++)
	{
		temp = getCombinaisonsNumbersLevel(temp, i, nb_colonnes);
		dump = appendCellCombinaison(dump, temp);
	}

	return dump;
}

Set_Of_Attrs* getCombinaisonsAttrs(Combinaison* param, Line_data* tab_data)
{
	Set_Of_Attrs* dump_attr = NULL;
	Set_Of_Attrs* temp_attr;
	Combinaison* courant_combi = param;
	Entier* courant_tuple;
	Chaine* temp_chaine;
	Chaine* dump_chaine;

	while (courant_combi != NULL)
	{
		courant_tuple = courant_combi->combi;
		dump_chaine = NULL;
		while (courant_tuple != NULL)
		{
			temp_chaine = (Chaine*)malloc(sizeof(Chaine));
			temp_chaine->attribut = (char*)malloc(sizeof(char)*NMAX);
			temp_chaine->suiv = NULL;
			strcpy(temp_chaine->attribut, attribut(courant_tuple->val, tab_data) );
			dump_chaine = append_chaine(dump_chaine, temp_chaine);

			courant_tuple = courant_tuple->suiv;
		}
		temp_attr = (Set_Of_Attrs*)malloc(sizeof(Set_Of_Attrs));
		temp_attr->attributs = copie_chaine(dump_chaine);
		temp_attr->suiv = NULL;
		dump_attr = append_set_of_attrs(dump_attr, temp_attr);
		courant_combi = courant_combi->suiv;
	}

	return dump_attr;
}

Combinaison* onlySizeN(Combinaison* param, int n)
{
	Combinaison* courant = param;
	Combinaison* dump = NULL;
	Combinaison* temp;
	while (courant != NULL)
	{
		if ( cardinal_tuples(courant->combi) == n )
		{
			temp = (Combinaison*)malloc(sizeof(Combinaison));
			temp->combi = copie_tuple(courant->combi);
			temp->suiv = NULL;
			dump = appendCellCombinaison(dump, temp);
		}
		courant = courant->suiv;
	}
	return dump;
}

Couples* getCouplesCombinaisons(Entier* feed)
{
	Couples* dump = NULL;
	Couples* temp_couple;
	Entier* courant_tuple;
	int n = cardinal_tuples(feed);
	int i,j;
	int* tab = (int*)malloc(sizeof(int)*n);
	courant_tuple = feed;
	for (i=0; i<n; i++)
	{
		tab[i] = courant_tuple->val;
		courant_tuple = courant_tuple->suiv;
	}
	for (i=0; i<n; i++)
	{
		for (j=i+1; j<n; j++)
		{
			temp_couple = (Couples*)malloc(sizeof(Couples));
			temp_couple->cpl[0] = tab[i];
			temp_couple->cpl[1] = tab[j];
			temp_couple->suiv = NULL;
			dump = append_couple(dump, temp_couple);
		}
	}

	return dump;
}

void afficher_combinaisons(Combinaison* combinaisons)
{
	Combinaison* courant = combinaisons;

	printf("*** Combinaisons *** \n");
	while (courant != NULL)
	{
		afficher_tuple(courant->combi);
		courant = courant->suiv;
	}
	printf("-----------\n");
}

void afficher_combinaisons_attributs(Combinaison* combinaisons, Line_data* tab_data)
{
	Combinaison* courant = combinaisons;
	Entier* courant_tuple;

	printf("*** Combinaisons *** \n");
	while (courant != NULL)
	{
		courant_tuple = courant->combi;
		while (courant_tuple != NULL)
		{
			printf("%s | ", attribut(courant_tuple->val, tab_data));
			courant_tuple = courant_tuple->suiv;
		}
		printf("\t**\n");
		courant = courant->suiv;
	}
	printf("-----------\n");
}

int dependanceFonctionnelle(int colonne_j, int colonne_k, Line_data* tab_data, int nb_lignes)
{
	int i,j;
	for (i=1; i<nb_lignes; i++)
	{
		for (j=i+1; j<nb_lignes; j++)
		{
			if ( are_same_string(tab_data[i].val[colonne_j], tab_data[j].val[colonne_j]) )
			{
				if ( !are_same_string(tab_data[i].val[colonne_k], tab_data[j].val[colonne_k]) )
					return 0;
			}
		}
	}
	return 1;
}

Dependancies* dependances(Set_Of_Attrs* agree, Line_data* tab_data, int nb_lignes, int nb_colonnes)
{
	Dependancies* dump = NULL;
	Dependancies* temp_dep;
	Dependancies* param;
	Set_Of_Attrs* courant_agree = agree;
	Chaine* courant_chaine;
	Entier* temp_tuple;
	Entier* tuples;
	Couples* couples = NULL;
	Couples* courant_couple;
	Chaine* determinant;
	Chaine* soumis;

	while (courant_agree != NULL)
	{
		courant_chaine = courant_agree->attributs;
		if (taille_chaine(courant_chaine) >= 2)
		{
			tuples = NULL;
			while (courant_chaine != NULL)
			{
				temp_tuple = (Entier*)malloc(sizeof(Entier));
				temp_tuple->val = colonne(courant_chaine->attribut, tab_data, nb_colonnes);
				temp_tuple->suiv = NULL;
				tuples = append_tuple(tuples, temp_tuple);
				courant_chaine = courant_chaine->suiv;
			}
			couples = getCouplesCombinaisons(tuples);
			courant_couple = couples;
			// Pour les dépendances fonctionnelles élémentaires
			while (courant_couple != NULL)
			{
				if (dependanceFonctionnelle(courant_couple->cpl[0], courant_couple->cpl[1], tab_data, nb_lignes) )
				{
					determinant = (Chaine*)malloc(sizeof(Chaine));
					determinant->attribut = (char*)malloc(sizeof(char)*NMAX);
					strcpy( determinant->attribut, attribut(courant_couple->cpl[0], tab_data));
					determinant->suiv = NULL;
					soumis = (Chaine*)malloc(sizeof(Chaine));
					soumis->attribut = (char*)malloc(sizeof(char)*NMAX);
					strcpy( soumis->attribut, attribut(courant_couple->cpl[1], tab_data));
					soumis->suiv = NULL;
					temp_dep = createCellDependance(determinant, soumis);
					dump = appendDependance(dump, temp_dep);
				}
				if (dependanceFonctionnelle(courant_couple->cpl[1], courant_couple->cpl[0], tab_data, nb_lignes) )
				{
					determinant = (Chaine*)malloc(sizeof(Chaine));
					determinant->attribut = (char*)malloc(sizeof(char)*NMAX);
					strcpy( determinant->attribut, attribut(courant_couple->cpl[1], tab_data));
					determinant->suiv = NULL;
					soumis = (Chaine*)malloc(sizeof(Chaine));
					soumis->attribut = (char*)malloc(sizeof(char)*NMAX);
					strcpy( soumis->attribut, attribut(courant_couple->cpl[0], tab_data));
					soumis->suiv = NULL;
					temp_dep = createCellDependance(determinant, soumis);
					dump = appendDependance(dump, temp_dep);
				}
				courant_couple = courant_couple->suiv;
			}
		}
		courant_agree = courant_agree->suiv;
	}
	return dump;
}

// Armstrong
Dependancies* armstrong_1er_augmentation(Dependancies* param, Line_data* tab_data, int nb_colonnes)
{
	Dependancies* temp = NULL;
	int j;
	Dependancies* temp_dep;
	Dependancies* courant_dep = copieDependance(param);

	// première itération
	while (courant_dep != NULL)
	{
		for (j=0; j<nb_colonnes; j++)
		{
			if ( !are_same_string( attribut(j, tab_data), (courant_dep->determinant)->attribut )
				&& !are_same_string(attribut(j, tab_data), (courant_dep->soumis)->attribut) )
			{
				temp_dep = (Dependancies*)malloc(sizeof(Dependancies));
				temp_dep->determinant = create_cell_chaine(attribut(j, tab_data));
				temp_dep->determinant = append_chaine(temp_dep->determinant, courant_dep-> determinant);
				temp_dep->soumis = copie_chaine(courant_dep->soumis);
				temp_dep->suiv = NULL;
				if (temp == NULL)
                    temp = temp_dep;
                else
                {
                    if (!hasAnagrammeInDependance(temp, temp_dep))
                        temp = appendDependance(temp, temp_dep);
                    else
                    	free(temp_dep);
                }
			}
		}
		courant_dep = courant_dep->suiv;
	}
	return temp = appendDependance(temp, param);
}

Dependancies* armstrongAugmentation(Dependancies* param, Line_data* tab_data, int nb_colonnes)
{
	int j;
	Dependancies* courant_dep;
	Dependancies* temp_dep;
	Dependancies* temp = NULL;
	// vérification de la nécessité de de la 1ère augmentation
	courant_dep = param;
	while (courant_dep != NULL && taille_chaine(courant_dep->determinant) <= 1)
	{
		courant_dep = courant_dep->suiv;
	}
	if (courant_dep == NULL)
		temp = armstrong_1er_augmentation(param, tab_data, nb_colonnes);
	else
		temp = param;
	Chaine* temp_chaine;

	// autres itérations
	int peut_continuer = 1;
	courant_dep = param;
	while (peut_continuer)
	{
		peut_continuer = 0;
		courant_dep = temp;
		while (courant_dep != NULL)
		{
			for (j=0; j<nb_colonnes; j++)
			{
				temp_chaine = create_cell_chaine(attribut(j, tab_data));
				if ( !in_linked_string(courant_dep->determinant, temp_chaine)
					&& !in_linked_string(courant_dep->soumis, temp_chaine) )
				{
					temp_dep = (Dependancies*)malloc(sizeof(Dependancies));
					temp_dep->determinant = copie_chaine(courant_dep->determinant);
					temp_dep->determinant = append_chaine(temp_dep->determinant, temp_chaine);
					temp_dep->soumis = copie_chaine(courant_dep->soumis);
					temp_dep->suiv = NULL;
					if (!hasAnagrammeInDependance(temp, temp_dep))
					{
						temp = appendDependance(temp, temp_dep);
						if (peut_continuer == 0 )
							peut_continuer = 1;
					}
				}
			}
			courant_dep = courant_dep->suiv;
		}
	}
	return temp;
}

Dependancies* armstrongTransitivite(Dependancies* param, Line_data* tab_data, int nb_colonnes)
{
	Dependancies* dump = copieDependance(param);
	Dependancies* courant_dep;
	Dependancies* courant_dep_ot;
	Dependancies* temp_dep;
	int peut_continuer = 1;
	while (peut_continuer)
	{
		courant_dep = dump;
		peut_continuer = 0;
		while (courant_dep != NULL)
		{
			courant_dep_ot = courant_dep->suiv;
			while (courant_dep_ot != NULL)
			{
				if (are_same_linked_string(courant_dep->soumis, courant_dep_ot->determinant))
				{
					temp_dep = (Dependancies*)malloc(sizeof(Dependancies));
					temp_dep->determinant = copie_chaine(courant_dep->determinant);
					temp_dep->soumis = copie_chaine(courant_dep_ot->soumis);
					temp_dep->suiv = NULL;
					if (!hasAnagrammeInDependance(dump, temp_dep))
					{
						if (!in_linked_string(temp_dep->determinant, temp_dep->soumis))
						{
							temp_dep->determinant = delete_repeated(temp_dep->determinant);
							dump = appendDependance(dump, temp_dep);
							if (peut_continuer == 0 )
								peut_continuer = 1;
						}
					}
				}
				courant_dep_ot = courant_dep_ot->suiv;
			}
			courant_dep = courant_dep->suiv;
		}
	}
	return dump;
}

Dependancies* armstrongPseudoTransitivite(Dependancies* param, Line_data* tab_data, int nb_colonnes)
{
	Dependancies* dump = copieDependance(param);
	Dependancies* courant_dep;
	Dependancies* courant_dep_ot;
	Dependancies* temp_dep;
	int peut_continuer = 1;
	while (peut_continuer)
	{
		courant_dep = dump;
		peut_continuer = 0;
		while (courant_dep != NULL)
		{
			courant_dep_ot = courant_dep->suiv;
			while (courant_dep_ot != NULL)
			{
				if (in_linked_string(courant_dep_ot->determinant, courant_dep->soumis))
				{
					temp_dep = (Dependancies*)malloc(sizeof(Dependancies));
					temp_dep->determinant = copie_chaine(courant_dep->determinant);
					temp_dep->determinant = append_chaine(temp_dep->determinant, minus_chaine(courant_dep_ot->determinant, courant_dep->soumis));
					temp_dep->soumis = copie_chaine(courant_dep_ot->soumis);
					temp_dep->suiv = NULL;
					if (!hasAnagrammeInDependance(dump, temp_dep))
					{
						if (!in_linked_string(temp_dep->determinant, temp_dep->soumis))
						{
							temp_dep->determinant = delete_repeated(temp_dep->determinant);
							if (!hasAnagrammeInDependance(dump, temp_dep))
							{
								dump = appendDependance(dump, temp_dep);
								if (peut_continuer == 0 )
									peut_continuer = 1;
							}
						}
					}
				}
				courant_dep_ot = courant_dep_ot->suiv;
			}
			courant_dep = courant_dep->suiv;
		}
	}
	return dump;
}

Dependancies* fermetureTransitive(Dependancies* param, Line_data* tab_data, int nb_colonnes)
{
	Dependancies* dump = copieDependance(param);
	dump = desunification(dump);
	dump = armstrongAugmentation(dump, tab_data, nb_colonnes);
	dump = armstrongTransitivite(dump, tab_data, nb_colonnes);
	dump = armstrongPseudoTransitivite(dump, tab_data, nb_colonnes);
	return dump;
}

Dependancies* unification(Dependancies* param)
{
	Dependancies* dump = copieDependance(param);
	Dependancies* courant_dep;
	Dependancies* courant_dep_ot;
	Chaine* temp_chaine;
	Dependancies* temp_dep;

	courant_dep = param;
	while (courant_dep != NULL)
	{
		courant_dep_ot = courant_dep->suiv;
		while (courant_dep_ot != NULL)
		{
			if ( are_anagramme_chaine(courant_dep->determinant, courant_dep_ot->determinant) )
			{
				if (!contained_in_linked_string(courant_dep->soumis, courant_dep_ot->soumis)
					&& !contained_in_linked_string(courant_dep_ot->soumis, courant_dep->soumis)
					)
				{
					dump = deleteCellDependance(dump, courant_dep);
					dump = deleteCellDependance(dump, courant_dep_ot);
					temp_chaine = NULL;
					temp_chaine = append_chaine(temp_chaine, courant_dep->soumis);
					temp_chaine = append_chaine(temp_chaine, courant_dep_ot->soumis);
					temp_chaine = delete_repeated(temp_chaine);
					temp_dep = createCellDependance(courant_dep->determinant, temp_chaine);
					dump = appendCellDependance(dump, temp_dep);
				}
			}
			courant_dep_ot = courant_dep_ot->suiv;
		}
		courant_dep = courant_dep->suiv;
	}

	return dump;
}

Dependancies* desunification(Dependancies* param)
{
	Dependancies* dump = copieDependance(param);
	Dependancies* courant_dep = dump;
	Dependancies* temp_dep;
	Dependancies* suivant;
	Chaine* courant_chaine;
	Chaine* temp_chaine;

	while (courant_dep != NULL)
	{
		if (taille_chaine(courant_dep->soumis)>1)
		{
			courant_chaine = courant_dep->soumis;
			while (courant_chaine != NULL)
			{
				temp_chaine = NULL;
				temp_chaine = append_cell_chaine(temp_chaine, courant_chaine);
				temp_dep = createCellDependance(courant_dep->determinant, temp_chaine);
				dump = appendDependance(dump, temp_dep);
				courant_chaine = courant_chaine->suiv;
			}
			suivant = courant_dep->suiv;
			dump = deleteCellDependance(dump, courant_dep);
			courant_dep = suivant;
		}
		else
		{
			courant_dep = courant_dep->suiv;
		}
	}
	return dump;
}

//
Dependancies* appendDependance(Dependancies* param, Dependancies* to_append)
{
    if ( param == NULL)
        return to_append;
    Dependancies* courant = param;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = copieDependance(to_append);
    return param;
}

Dependancies* appendCellDependance(Dependancies* param, Dependancies* cell_dep)
{
	if ( param == NULL)
        return createCellDependance(cell_dep->determinant, cell_dep->soumis);
    Dependancies* courant = param;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = createCellDependance(cell_dep->determinant, cell_dep->soumis);
    return param;
}

Dependancies* createCellDependance(Chaine* determinant, Chaine* soumis)
{
    Dependancies* dump = (Dependancies*)malloc(sizeof(Dependancies));
	dump->determinant = copie_chaine(determinant);
	dump->soumis = copie_chaine(soumis);
	dump->suiv = NULL;
    return dump;
}

Dependancies* copieDependance(Dependancies* to_copie)
{
	Dependancies* courant = to_copie;
	Dependancies* dump = NULL;
	Dependancies* temp_dep;
	while (courant != NULL)
	{
		temp_dep = createCellDependance(copie_chaine(courant->determinant), copie_chaine(courant->soumis) );
		dump = appendDependance(dump, temp_dep);
		courant = courant->suiv;
	}
	return dump;
}

int sizeDependance(Dependancies* deps)
{
    int compteur = 0;
    Dependancies* courant_dep = deps;
    while (courant_dep != NULL)
    {
        compteur++;
        courant_dep = courant_dep->suiv;
    }
    return compteur;
}

Dependancies* deleteCellDependance(Dependancies* deps, Dependancies* to_delete)
{
	Dependancies* temp;
    // debut de liste
	if (areSameDependancies(deps, to_delete))
    {
        temp = deps->suiv;
        free(deps);
        return temp;
    }
    else
    {
        // milieu de liste
        Dependancies* courant_dep = deps->suiv;
        Dependancies* precedent = deps;
        while (courant_dep != NULL)
        {
			if (areSameDependancies(courant_dep, to_delete))
            {
                precedent->suiv = courant_dep->suiv;
                free(courant_dep);
                return deps;
            }
            precedent = courant_dep;
            courant_dep = courant_dep->suiv;
        }
        return deps;
    }
}

Dependancies* deleteAllOccurencesDependance(Dependancies* deps, Dependancies* to_delete)
{
	Dependancies* dump = copieDependance(deps);
	int last_size = sizeDependance(dump);
	int new_size;
	do
	{
		dump = deleteCellDependance(dump, to_delete);
		last_size = new_size;
		new_size = sizeDependance(dump);
	}	while ( new_size < last_size);
	return dump;
}

int areSameDependancies(Dependancies* dep1, Dependancies* dep2)
{
    if (are_same_linked_string(dep1->determinant, dep2->determinant))
        if ( are_same_linked_string(dep1->soumis, dep2->soumis))
            return 1;
    return 0;
}

int inDependancies(Dependancies* deps, Dependancies* dep)
{
    Dependancies* courant = deps;
    while (courant != NULL)
    {
        if (areSameDependancies(courant, dep))
            return 1;
        courant = courant->suiv;
    }
    return 0;
}

int hasAnagrammeInDependance(Dependancies* deps, Dependancies* dep)
{
    Chaine* courant_chaine;
    Dependancies* courant_dep = deps;

    while (courant_dep != NULL)
    {
        if ( are_anagramme_chaine(courant_dep->soumis, dep->soumis)
        	&& are_anagramme_chaine(courant_dep->determinant, dep->determinant) )
                return 1;
        courant_dep = courant_dep->suiv;
    }

    return 0;
}

int areAnagrammeDependance(Dependancies* dep1, Dependancies* dep2)
{
	Dependancies* courant;

	courant = dep1;
	while (courant != NULL)
	{
		if (!hasAnagrammeInDependance(dep2, courant))
			return 0;
		courant = courant->suiv;
	}

	courant = dep2;
	while (courant != NULL)
	{
		if (!hasAnagrammeInDependance(dep1, courant))
			return 0;
		courant = courant->suiv;
	}

	return 1;
}

void afficher_dependances_cons(Dependancies* param)
{
	if (param == NULL)
		printf("Vide. Pas de dependances\n");
    Dependancies* courant = param;
    while (courant != NULL)
    {
        printf("+\n");
        afficher_chaine(courant->determinant);
        printf("==> ");
        afficher_chaine(courant->soumis);
        printf("+\n");
        courant = courant->suiv;
    }
}

void afficher_dependances(GtkWidget* wid, Dependancies* datas_dependance)
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
    char* temp_chaine;
    Dependancies* courant_dep = datas_dependance;
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
	free(label);

    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);
}
