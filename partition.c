#include "includes.h"

int in_partition_colonne(Partition* p, Entier* tuple)
{
	// vrai si la liste chainée 'Partition p' contient les mêmes valeurs dans
	// un de ses tuples
	Partition* cp = p;
	while (cp != NULL)
	{
		if ( tuples_identiques (cp->tuples, tuple) )
			return 1;
		cp = cp->suiv;
	}
	return 0;
}

Partition* partition_relation(Line_data* tab_data, int nb_lignes, int nb_colonnes)
{
	Partition** tab = (Partition**)malloc(sizeof(Partition*)*nb_colonnes);
	int j;
	for (j=0; j<nb_colonnes; j++)
	{
		tab[j] = partition_colonne(j, tab_data, nb_lignes);
	}
	return tab;
}


Partition* partition_colonne (int colonne, Line_data* tab_data, int nb_lignes)
{
	int i;
	Partition* p = NULL;
	Partition* temp = NULL;
	Partition* courant = NULL;

	// on commence à 1 aulieu de 0 car le 1 correspond à la ligne
	// contenant le nom des attributs
	for (i=1; i<nb_lignes; i++)
	{
		if (p == NULL)
		{
			// création de maillon
			p = (Partition*)malloc(sizeof(Partition));
			p->tuples = get_tuples_reg(tab_data[i].val[colonne], tab_data, nb_lignes, colonne);
			p->cardinal = cardinal_tuples(p->tuples);
			p->suiv = NULL;
			courant = p;
		}
		else
		{
			// création de maillon
			temp = (Partition*)malloc(sizeof(Partition));
			temp->tuples = get_tuples_reg(tab_data[i].val[colonne], tab_data, nb_lignes, colonne);
			temp->cardinal = cardinal_tuples(temp->tuples);
			temp->suiv = NULL;

			if (in_partition_colonne(p, temp->tuples))
			{
				// on ne fait rien
				free(temp);
			}
			else
			{
				// on l'ajoute à la partition de la colonne
				courant->suiv = temp;
				courant = courant->suiv;
			}
		}
	}

	return p;
}

Entier* get_tuples_reg(char* valeur, Line_data* tab_data, int nb_lignes, int colonne)
{
	// renvoi dans une liste chainée renseignant sur les fois où valeur
	// a été rencontrée dans la colonne précisée
	int i;
	Entier* retour = NULL;
	Entier* courant = NULL;
	Entier* temp = NULL;

	for (i=1; i<nb_lignes; i++)
	{
		if (strcmp(tab_data[i].val[colonne], valeur) == 0)
		{
			if (retour == NULL)
			{
				retour = (Entier*)malloc(sizeof(Entier));
				retour->val = i;
				retour->suiv = NULL;
				courant = retour;
			}
			else
			{
				temp = (Entier*)malloc(sizeof(Entier));
				temp->val = i;
				temp->suiv = NULL;
				courant->suiv = temp;
				courant = courant->suiv;
			}
		}
	}
	return retour;
}

/*
void afficher_partition_colonne(Partition* p)
{
	if (p == NULL)
		printf("Partition vide\n");
	else
		printf("---Partition de la colonne\n");
	Partition* c = p;
	while (c != NULL)
	{
		afficher_tuple(c->tuples);
		printf("Card = %d\n", c->cardinal);
		printf("***\n");
		c = c->suiv;
	}
	printf("Fin affichage p col\n");
}
*/

/**************
** 	ÉLAGUÉ	***
**************/

Partition* elaguer_partition_attribut(Partition* partition)
{
	Partition* courant = partition;
	Partition* temp;
	Partition* parcours;
	Partition* retour = NULL;
	while (courant != NULL)
	{
		if (courant->cardinal >=2)
		{
			if (retour == NULL)
			{
				// alors liste vide
				retour = (Partition*)malloc(sizeof(Partition));
				retour->tuples =  copie_tuple(courant->tuples);
				retour->cardinal = courant->cardinal;
				retour->suiv = NULL;
				parcours = retour;
			}
			else
			{
				temp = (Partition*)malloc(sizeof(Partition));
				temp->tuples =  copie_tuple(courant->tuples);
				temp->cardinal = courant->cardinal;
				temp->suiv = NULL;
				parcours->suiv = temp;
				parcours = 	parcours->suiv;
			}
		}
		courant = courant->suiv;
	}

	return retour;
}

Partition** elaguer_partition_relation(Partition** partition, int nb_colonnes)
{
	int j;
	Partition** retour = (Partition**)malloc(sizeof(Partition*)*nb_colonnes);
	for (j=0; j<nb_colonnes; j++)
	{
		retour[j] = elaguer_partition_attribut(partition[j]);
	}
	return retour;
}

/****************************/
/***	   GRAPHICS		  ***/
/****************************/

void afficher_partitions(GtkWidget* wid, Datas_partition* datas_part)
{
	datas_part = (Datas_partition*)datas_part;
	GError error;
    GtkWidget* win;
    GtkWidget* vbox;
    GtkWidget* hbox1;
    GtkWidget* hbox2;
    GtkWidget* label;
    gchar* titre;
    int i;
    int k;

    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Partitions de la relation");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 250);
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_window_set_icon_from_file (GTK_WINDOW(win), "icon.ico", &error);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);

    label = gtk_label_new("<span foreground='#009688' size='xx-large' face='Viner Hand ITC'><u>Partitions de la relation</u> : </span>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);
    free(label);

    // parcourir partition sur attribut fixé
	Partition** part = datas_part->part_relat;
	int nb_colonnes = datas_part->nb_colonnes;
	int j;
	int first_value;
	Partition* courant_part;
	Entier* courant_tuple;
	char* chaine;
	char* final_chaine;
	char* temp_chaine;

	// alors première entrée dans la fonction
	for (j=0; j<nb_colonnes; j++)
	{
	    courant_part = part[j];
	    chaine = g_strdup_printf("P%d : ", j+1);
	    while (courant_part != NULL)
	    {
	        courant_tuple = courant_part->tuples;
	        chaine = strcat(chaine, "{");
	        first_value = 1;
	        while (courant_tuple != NULL)
	        {
	            if (first_value == 1)
	            {
	            	temp_chaine = g_strdup_printf("%d", courant_tuple->val);
	                chaine = strcat(chaine, temp_chaine);
	            }
	            else
	            {
	                temp_chaine = g_strdup_printf(", %d", courant_tuple->val);
	                chaine = strcat(chaine, temp_chaine);
	            }
	            first_value++;
	            courant_tuple = courant_tuple->suiv;
	        }
	        if (courant_part->suiv == NULL)
	            chaine = strcat(chaine, "}\n");
	        else
	            chaine = strcat(chaine, "}, ");
	        // then add it to the view
	        courant_part = courant_part->suiv;
	    }
		label = gtk_label_new(UTF8(chaine));
		gtk_box_pack_start (GTK_BOX(vbox), label, FALSE, FALSE, 0);
		free(label);
    }

	// final_chaine = " ";
	// final_chaine = strcat(final_chaine, "<span face='comic sans ms'>");
	// final_chaine = strcat(final_chaine, chaine);
	// final_chaine = strcat(final_chaine, "</span>");
	// label = gtk_label_new(final_chaine);
	// gtk_label_set_use_markup(GTK_LABEL(label), TRUE);


    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);

}

void afficher_partitions_elaguees(GtkWidget* wid, Datas_partition* datas_part_elag)
{
	datas_part_elag = (Datas_partition*)datas_part_elag;
	GError error;
    GtkWidget* win;
    GtkWidget* vbox;
    GtkWidget* hbox1;
    GtkWidget* hbox2;
    GtkWidget* label;

    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Partitions de la relation");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 250);
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_window_set_icon_from_file (GTK_WINDOW(win), "icon.ico", &error);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
    hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start (GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);

    label = gtk_label_new("<span foreground='#009688' size='xx-large' face='Viner Hand ITC'><u>Partitions de la relation</u> : </span>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_box_pack_start (GTK_BOX(hbox1), label, FALSE, FALSE, 0);
    free(label);

    // parcourir partition sur attribut fixé
	Partition** part = datas_part_elag->part_relat;
	int nb_colonnes = datas_part_elag->nb_colonnes;
	int j;
	int first_value;
	Partition* courant_part;
	Entier* courant_tuple;
	char* chaine;
	char* final_chaine;
	char* temp_chaine;

	for (j=0; j<nb_colonnes; j++)
	{
	    courant_part = part[j];
	    chaine = g_strdup_printf("P%d : ", j+1);
	    while (courant_part != NULL)
	    {
	        courant_tuple = courant_part->tuples;
	        chaine = strcat(chaine, "{");
	        first_value = 1;
	        while (courant_tuple != NULL)
	        {
	            if (first_value == 1)
	            {
	            	temp_chaine = g_strdup_printf("%d", courant_tuple->val);
	                chaine = strcat(chaine, temp_chaine);
	            }
	            else
	            {
	                temp_chaine = g_strdup_printf(", %d", courant_tuple->val);
	                chaine = strcat(chaine, temp_chaine);
	            }
	            first_value++;
	            courant_tuple = courant_tuple->suiv;
	        }
	        if (courant_part->suiv == NULL)
	            chaine = strcat(chaine, "}\n");
	        else
	            chaine = strcat(chaine, "}, ");
	        // then add it to the view
	        courant_part = courant_part->suiv;
	    }
	    // final_chaine = " ";
	    // final_chaine = strcat(final_chaine, "<span face='comic sans ms'>");
	    // final_chaine = strcat(final_chaine, chaine);
	    // final_chaine = strcat(final_chaine, "</span>");
	    label = gtk_label_new(UTF8(chaine));
	    // label = gtk_label_new(final_chaine);
   		// gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
        gtk_box_pack_start (GTK_BOX(vbox), label, FALSE, FALSE, 0);
        free(label);
	}


    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);
}
