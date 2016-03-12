#include "includes.h"

int appartient_class_equivalence(Entier* tuples, int a)
{
	Entier* courant = tuples;
	while (courant != NULL)
	{
		if (courant->val == a)
			return 1;
		courant = courant->suiv;
	}
	return 0;
}

/******************/
/****	EC	   ****/
/******************/
EC* ec(int ligne_i, Partition** base_elaguee, int nb_colonnes)
{
	EC* dump = NULL;
	EC* temp;
	EC* parcours;
	Partition* courant_part;
	Entier* courant_tuple;
	int i, j, postion_courante;

	for (j=0; j<nb_colonnes; j++)
	{

		// aller au t-ieme élément de la partition de la colonne
		courant_part = base_elaguee[j];
		postion_courante = 0;
		while (courant_part != NULL)
		{
			courant_tuple = courant_part->tuples;
			while (courant_tuple != NULL)
			{
				if (appartient_class_equivalence(courant_tuple, ligne_i))
				{
					if (dump == NULL)
					{
						dump = (EC*)malloc(sizeof(EC));
						dump->colonne_k = j;
						dump->position = postion_courante;
						dump->suiv = NULL;
						parcours = dump;
					}
					else
					{
						temp = (EC*)malloc(sizeof(EC));
						temp->colonne_k = j;
						temp->position = postion_courante;
						temp->suiv = NULL;
						parcours->suiv = temp;
						parcours = parcours->suiv;
					}
				}
				courant_tuple = courant_tuple->suiv;
			}
			courant_part = courant_part->suiv;
			postion_courante++;
		}
	}

	return dump;
}

int in_ecs(EC* ecs, EC* ec)
{
	EC* courant = ecs;
	while (courant != NULL)
	{
		if (courant->colonne_k == ec->colonne_k
			&& courant->position == ec->position)
			return 1;
		courant = courant->suiv;
	}
	return 0;
}

EC* append_ec(EC* ecs, EC* ec)
{
	EC* courant = ecs;
	// aller au dernier élément
	if (ecs == NULL)
		return ec;

	// vérifier si pas déjà présent
	if (in_ecs(ecs, ec))
		return ecs;

	// aller au dernier élément
	while (courant->suiv != NULL)
		courant = courant->suiv;
	courant->suiv = ec;
	return ecs;
}

void afficher_ecs(EC* ecs)
{
	EC* courant = ecs;
	printf("*** EC\n");
	while (courant != NULL)
	{
		printf("[+] __EC\n");
		printf("colonne_k : %d\n", courant->colonne_k);
		printf("position : %d\n", courant->position);
		courant = courant->suiv;
	}
}
//
int in_agree_set(Set_Of_Attrs* ag, Set_Of_Attrs* wanted)
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
	        	if (contained_in_linked_string(courant_chaine, courant_chaine_ot))
	        		return 1;
				courant_chaine = courant_chaine->suiv;
	        }
	    }
		courant = courant->suiv;
	}
	return 0;
}

Set_Of_Attrs* intersections(EC* ec_a, EC* ec_b, Line_data* tab_data)
{
	EC* dump = NULL;
	EC* temp;
	EC* parcours;
	EC* courant_eca = ec_a;
	EC* courant_ecb = ec_b;

	// ceux qui sont ici et là-bas
	while (courant_eca != NULL)
	{
		if (in_ecs(ec_b, courant_eca))
		{
			if (dump == NULL)
			{
				dump = (EC*)malloc(sizeof(EC));
				dump->colonne_k = courant_eca->colonne_k;
				dump->position = courant_eca->position;
				dump->suiv = NULL;
				parcours = dump;
			}
			else
			{
				temp = (EC*)malloc(sizeof(EC));
				temp->colonne_k = courant_eca->colonne_k;
				temp->position = courant_eca->position;
				temp->suiv = NULL;

				// afin d'éviter les répétitions
				if (in_ecs(dump, temp))
					free(temp);
				else
				{
					parcours->suiv = temp;
					parcours = parcours->suiv;
				}
			}
		}
		courant_eca = courant_eca->suiv;
	}

	// ceux qui sont là-bas et ici
	while (courant_ecb != NULL)
	{
		if (in_ecs(ec_a, courant_ecb))
		{
			if (dump == NULL)
			{
				dump = (EC*)malloc(sizeof(EC));
				dump->colonne_k = courant_ecb->colonne_k;
				dump->position = courant_ecb->position;
				dump->suiv = NULL;
				parcours = dump;
			}
			else
			{
				temp = (EC*)malloc(sizeof(EC));
				temp->colonne_k = courant_ecb->colonne_k;
				temp->position = courant_ecb->position;
				temp->suiv = NULL;

				// afin d'éviter les répétitions
				if (in_ecs(dump, temp))
					free(temp);
				else
				{
					parcours->suiv = temp;
					parcours = parcours->suiv;
				}
			}
		}
		courant_ecb = courant_ecb->suiv;
	}

	// traitement sur le retour
	Set_Of_Attrs* retour = (Set_Of_Attrs*)malloc(sizeof(Set_Of_Attrs));
	retour->suiv = NULL;
	Chaine* dump_chaine = NULL;
	Chaine* courant_chaine;
	Chaine* temp_chaine;
	EC* courant = dump;
	while (courant != NULL)
	{
		if (dump_chaine == NULL)
		{
			dump_chaine = (Chaine*)malloc(sizeof(Chaine));
			dump_chaine->attribut = attribut(courant->colonne_k, tab_data);
			dump_chaine->suiv = NULL;
			courant_chaine = dump_chaine;
		}
		else
		{
			temp_chaine = (Chaine*)malloc(sizeof(Chaine));
			temp_chaine->attribut = attribut(courant->colonne_k, tab_data);
			temp_chaine->suiv = NULL;
			courant_chaine->suiv = temp_chaine;
			courant_chaine = courant_chaine->suiv;
		}
		courant = courant->suiv;
	}
	retour->attributs = dump_chaine;
	return retour;
}

Partition* get_MC(Partition** base_elaguee, int nb_colonnes)
{
	/*
	*    MC maximum au sens de l'inclusion
	*   il suffit de parcourir la base de la partition élaguée et
	*  de ne prendre un élément qu'une seule fois
	/*
	*	Pour respecter la définition de MC il faudrait ensuite supprimer les tuples inclus
	*	dans des tuples les contenant, avec cette méthode. Mais nous n'opérerons pas ainsi
	*	car on veut exploiter ce surplus que contient MC
	*/
	Partition* MC = NULL;
	Partition* temp;
	Partition* courant;
	Partition* parcours_part;
	int i, j;

	for (i=0; i<nb_colonnes; i++)
	{
		parcours_part = base_elaguee[i];
		while (parcours_part != NULL)
		{
			if (!in_partition_colonne(MC, parcours_part->tuples))
			{
				// ajouter à MC
				if (MC == NULL)
				{
					// crée la tete
					MC = (Partition*)malloc(sizeof(Partition));
					MC->tuples = copie_tuple(parcours_part->tuples);
					MC->cardinal = parcours_part->cardinal;
					MC->suiv = NULL;
					courant = MC;
				}
				else
				{
					// rajoute à l'ensemble
					temp = (Partition*)malloc(sizeof(Partition));
					temp->tuples = copie_tuple(parcours_part->tuples);
					temp->cardinal = parcours_part->cardinal;
					temp->suiv = NULL;
					courant->suiv = temp;
					courant = courant->suiv;
				}
			}
			parcours_part = parcours_part->suiv;
		}
	}
	return MC;
}

Couples* get_couples(Partition* MC)
{
	/*
	*	d'abord prendre les éléments de qui ont deux éléments
	*	ensuite s'il y a une ensemble de MC qui contient plus de 2 tuples
	*	alors faire la combinaison de ces éléments entre eux afin d'obtenir
	*	tous les couples possibles
	*/
	/*
	*	liste chainée permettant de stocker l'ensemble des couples générés
	*	par les composants de MC
	*/
	/*
	*	1. Mettre toutes les valeurs (tuples) dans un tableau dynamique
	*	2. prendre les combinaisons de tab[i] et tab[j] comme défini plus bas
	*/
	int i, j;
	int* tab;
	Couples* couples = NULL;
	Couples* parcours_couple;
	Couples* temp_couple;
	Entier* courant_tuple;
	Partition* courant = MC;
	while (courant != NULL)
	{
		// mettre toutes les valeurs dans le tableau
		tab = (int*)malloc(sizeof(int)*courant->cardinal);
		courant_tuple = courant->tuples;
		for (i=0; i<courant->cardinal; i++)
		{
			tab[i] = courant_tuple->val;
			if (courant_tuple != NULL)
				courant_tuple = courant_tuple->suiv;
		}

		// mettre les combinaisons dans 'couples'
		for (i=0; i<courant->cardinal; i++)
		{
			for (j=i+1; j<courant->cardinal; j++)
			{
				if (couples == NULL)
				{
					couples = (Couples*)malloc(sizeof(Couples));
					couples->cpl[0] = tab[i];
					couples->cpl[1] = tab[j];
					couples->suiv = NULL;
					parcours_couple = couples;
				}
				else
				{
					temp_couple = (Couples*)malloc(sizeof(Couples));
					// fait une copie du tableau en paramètre
					// création
					temp_couple->cpl[0] = tab[i];
					temp_couple->cpl[1] = tab[j];
					temp_couple->suiv = NULL;
					// vérification
					if (in_couples(couples, temp_couple))
						free(temp_couple);
					else
					{
						parcours_couple->suiv = temp_couple;
						parcours_couple = parcours_couple->suiv;
					}
				}
			}
		}
		courant = courant->suiv;
		// afin de soulager la mémoire à chaque fois
		free(tab);
	}
	return couples;
}

// renvoie l'ensemble en accord sous forme d'une liste chainée de tuples
Set_Of_Attrs* agree_set(Partition** base_elaguee, int nb_colonnes, int nb_lignes, Line_data* tab_data)
{
	// Détermination de MC
	Partition* MC = get_MC(base_elaguee, nb_colonnes);

	// couples générés à partir de MC
	int i, j;
	Couples* couples = get_couples(MC);
	Entier* courant_tuple;

	// afficher couples
	Couples* courant_couple = couples;
	i = 0;
	while (courant_couple != NULL)
	{
		printf("***couple %d \n",++i);
		printf("(%d,%d)\n", courant_couple->cpl[0], courant_couple->cpl[1]);
		courant_couple = courant_couple->suiv;
	}

	// intersections des ec(t) et ec(t'); t,t' appartiennent à 'couples'
	courant_couple = couples;
	Set_Of_Attrs* ag = NULL;
	Set_Of_Attrs* inter;
	while (courant_couple != NULL)
	{
		inter = intersections(ec(courant_couple->cpl[0], base_elaguee, nb_colonnes), ec(courant_couple->cpl[1], base_elaguee, nb_colonnes), tab_data);
		if (!in_agree_set(ag, inter))
		{
			ag = append_set_of_attrs(ag, inter);
		}
		courant_couple = courant_couple->suiv;
	}

	return ag;
}

void print_set_of_attrs(Set_Of_Attrs* accord)
{
	printf("Agree set = {");
	if (accord == NULL)
		printf("\tVIDE\n");
	Set_Of_Attrs* courant = accord;
	Chaine* courant_chaine;
	int i,j;
	i = 1;
	while (courant != NULL)
	{
		courant_chaine = courant->attributs;
		if (i>=2)
			printf(", ");
		j = 1;
		while (courant_chaine != NULL)
		{
			if (j >= 2)
				printf(". %s", courant_chaine->attribut);
			else
				printf("%s", courant_chaine->attribut);
			courant_chaine = courant_chaine->suiv;
			j++;
		}
		courant = courant->suiv;
		i++;
	}
	printf("}\n");
}

void afficher_ensembles_en_accord(GtkWidget* wid, Set_Of_Attrs* datas_agree)
{
	GError error;
    GtkWidget* win;
    GtkWidget* vbox;
    GtkWidget* hbox1;
    GtkWidget* hbox2;
    GtkWidget* label;
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Ensembles en accord");
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

    label = gtk_label_new("<span foreground='#009688' size='xx-large' face='Lucida Handwriting'><u>Ensembles en accord de la relation</u> : </span>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);
    free(label);

    // content here
    if (datas_agree == NULL)
		printf("\tVIDE\n");
	Set_Of_Attrs* courant = datas_agree;
	Chaine* courant_chaine;
	int i,j;
	char* chaine = (char*)malloc(sizeof(char)*NMAX);
	char* temp_chaine = (char*)malloc(sizeof(char)*NMAX);
	i = 1;
	while (courant != NULL)
	{
		courant_chaine = courant->attributs;
		if (i>=2)
			chaine = strcat(chaine, ", ");
		j = 1;
		while (courant_chaine != NULL)
		{
			if (j >= 2)
				temp_chaine = g_strdup_printf(". %s", courant_chaine->attribut);
			else
				temp_chaine = g_strdup_printf("%s", courant_chaine->attribut);
			chaine = strcat(chaine, temp_chaine);
			free(temp_chaine);
			courant_chaine = courant_chaine->suiv;
			j++;
		}
		courant = courant->suiv;
		i++;
	}
	label = gtk_label_new(UTF8(chaine));
	gtk_box_pack_start (GTK_BOX(vbox), label, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);
}

Chaine* ag(int ligne_i, int ligne_j, Line_data* tab_data, int nb_colonnes)
{
	int j;
	Chaine* dump = NULL;
	Chaine* temp_chaine;
	for (j=0; j<nb_colonnes; j++)
	{
		if ( strcmp(tab_data[ligne_i].val[j] , tab_data[ligne_j].val[j]) == 0 )
		{
			temp_chaine = (Chaine*)malloc(sizeof(Chaine));
			temp_chaine->attribut = (char*)malloc(sizeof(char)*NMAX);
			strcpy(temp_chaine->attribut, attribut(j, tab_data));
			temp_chaine->suiv = NULL;
			if (!in_linked_string(dump, temp_chaine))
				dump = append_chaine(dump, temp_chaine);
			else
				free(temp_chaine);
		}
	}
	return dump;
}
