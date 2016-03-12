#include "includes.h"

void afficher_chaine(Chaine* param)
{
    Chaine* courant = param;
    while (courant != NULL)
    {
        printf("|%s| ", courant->attribut);
        courant = courant->suiv;
    }
    printf("\n");
}

Chaine* create_cell_chaine(char* attr)
{
    Chaine* dump = (Chaine*)malloc(sizeof(Chaine));
    dump->attribut = attr;
    dump->suiv = NULL;
    return dump;
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

int are_same_string(char* chaine1, char* chaine2)
{
    if (strcmp(chaine1, chaine2) == 0)
        return 1;
    else
        return 0;
}

void main()
{
	// chaine a
	Chaine* chaine_a = create_cell_chaine("A");
	Chaine* courant = chaine_a;
	courant->suiv = create_cell_chaine("B");
	courant = courant->suiv;
	courant->suiv = create_cell_chaine("C");
	courant = courant->suiv;
	courant->suiv = create_cell_chaine("D");
	// chaine b
	Chaine* chaine_b = create_cell_chaine("B");
	courant = chaine_b;
	courant->suiv = create_cell_chaine("D");
	courant = courant->suiv;
	courant->suiv = create_cell_chaine("C");
	// chaine b
	

	//
	afficher_chaine(chaine_a);
	afficher_chaine(chaine_b);

	if (contained_in_linked_string(chaine_a, chaine_b))
		printf("vrai\n");
	else
		printf("FAUX\n");
}
