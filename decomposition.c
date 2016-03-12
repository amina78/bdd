#include "includes.h"

Dependancies* getComplementFromKeys(Dependancies* deps, Set_Of_Attrs* keys, Line_data* tab_data, int nb_colonnes)
{
	Dependancies* dump = copieDependance(deps);
	Set_Of_Attrs* courant_soattrs = keys;
	Chaine* determinant;
	Chaine* soumis;
	Dependancies* temp_dep;
	int j;
	while (courant_soattrs != NULL)
	{
		// Ajout aux dépendances si pas déjà existants
		for (j=0; j<nb_colonnes; j++)
		{
			determinant = copie_chaine(courant_soattrs->attributs);
			soumis = create_cell_chaine(attribut(j, tab_data));
			if ( !contained_in_linked_string(determinant, soumis))
			{
				temp_dep = createCellDependance(determinant, soumis);
				if (!hasAnagrammeInDependance(dump, temp_dep))
					dump = appendCellDependance(dump, temp_dep);
				else
					free(temp_dep);
			}
		}
		courant_soattrs = courant_soattrs->suiv;
	}
	return dump;
}

Chaine* getSubmited(Dependancies* deps, Chaine* determinant)
{
	Dependancies* courant = deps;
	Chaine*	temp_chaine = NULL;
	while (courant != NULL)
	{
		if (are_same_linked_string(determinant, courant->determinant))
			temp_chaine = append_chaine(temp_chaine, copie_chaine(courant->soumis));
		courant = courant->suiv;
	}
	return delete_repeated(temp_chaine);
}

Schema* initialSchema(Dependancies* f_transi, Line_data* tab_data, int nb_colonnes)
{
	Chaine* temp_chaine = NULL;
	int j;
	for (j=0; j<nb_colonnes; j++)
	{
		temp_chaine = append_chaine(temp_chaine, create_cell_chaine(attribut(j, tab_data)) );
	}
	return createCellSchema(temp_chaine, f_transi);
}

// Fonctions de manipulations du type (Schema)
Schema* createCellSchema(Chaine* relation, Dependancies* dependances)
{
	Schema* dump = NULL;
	dump = (Schema*)malloc(sizeof(Schema));
	dump->relation = copie_chaine(relation);
	dump->dependances = copieDependance(dependances);
	dump->suiv = NULL;
	return dump;
}

Schema* appendSchema(Schema* param, Schema* to_append)
{
	if ( param == NULL )
        return copieSchema(to_append);
    Schema* courant = param;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = copieSchema(to_append);
    return param;
}

Schema* appendCellSchema(Schema* param, Schema* to_append)
{
	Schema* temp = createCellSchema(to_append->relation, to_append->dependances);
	if ( param == NULL )
        return temp;
    Schema* courant = param;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = temp;
    return param;
}

Schema* copieSchema(Schema* sch)
{
    Schema* dump = NULL;
    Schema* temp;
    Schema* parcours;

    Schema* courant = sch;
    while (courant != NULL)
    {
        temp = createCellSchema(courant->relation, courant->dependances);
        if (dump == NULL)
        {
        	dump = temp;
        	parcours = temp;
        }
        else
        {
        	parcours->suiv = temp;
        	parcours = parcours->suiv;
        }
        courant = courant->suiv;
    }
    return dump;
}

int sizeSchema(Schema* sch)
{
	int compteur = 0;
	Schema* courant = sch;
	while (courant != NULL)
	{
		compteur++;
		courant = courant->suiv;
	}
	return compteur;
}

int areSameSchema(Schema* sch1, Schema* sch2)
{
	if (sizeSchema(sch1) != sizeSchema(sch2))
		return 0;
	Schema* courant_1 = sch1;
	Schema* courant_2 = sch2;
	while (courant_1 != NULL)
	{
		if (!are_same_linked_string(courant_1->relation, courant_2->relation)
			|| !areSameDependancies(courant_1->dependances, courant_2->dependances) )
			return 0;
		courant_1 = courant_1->suiv;
		courant_2 = courant_2->suiv;
	}
	return 1;
}

int hasAnagrammeInSchema(Schema* schs, Schema* sch)
{
	Schema* courant = schs;
	while (courant != NULL)
	{
		if ( are_anagramme_chaine(sch->relation, courant->relation) )
			// && areAnagrammeDependance(sch->dependances, courant->dependances) )
			return 1;
		courant = courant->suiv;
	}

	return 0;
}

int areAnagrammesSchema(Schema* sch1, Schema* sch2)
{
	// tout ce qui est ici est là-bas
	// tout ce qui est là-bas est ici
	Schema* courant;

	courant = sch1;
	while (courant != NULL)
	{
		if (!hasAnagrammeInSchema(sch2, courant))
			return 0;
		courant = courant->suiv;
	}

	courant = sch2;
	while (courant != NULL)
	{
		if (!hasAnagrammeInSchema(sch1, courant))
			return 0;
		courant = courant->suiv;
	}

	return 1;

}

int inSchemas(Schema* schs, Schema* sch)
{
	Schema* courant = schs;
	while (courant != NULL)
	{
		if (areSameSchema(sch, courant))
			return 1;
		courant = courant->suiv;
	}
	return 0;
}

//
Dependancies* projection(Dependancies* deps, Chaine* U, Line_data* tab_data, int nb_colonnes)
{
	Dependancies* courant_dep;
	Dependancies* dump = NULL;

	courant_dep = fermetureTransitive(deps, tab_data, nb_colonnes);
	while (courant_dep != NULL)
	{
		if ( contained_in_linked_string(U, courant_dep->determinant)
			&& contained_in_linked_string(U, courant_dep->soumis) )
			dump = appendCellDependance(dump, courant_dep);
		courant_dep = courant_dep->suiv;
	}
	return dump;
}

Schema* getDecomposition(Schema* init, Dependancies* cover, Set_Of_Attrs* keys, Line_data* tab_data, int nb_colonnes)
{
	Schema* dump = NULL;
	Schema* temp_schema = NULL;
	Dependancies* courant_dep;
	Dependancies* all_deps;
	Dependancies* temp_dep;
	Chaine* temp_chaine;

	/*
	    Les clés obtenues avec l'algorithme vont être ajoutées a la fermeture
	    transitive premièrement calculée si elles n'y sont pas déjà, car alors elles
	    représentes une information supplémentaire que nous n'avions pas au début.
	*/

	// Ajout de ces potentielles informations
    all_deps = getComplementFromKeys(init->dependances, keys, tab_data, nb_colonnes);

    // reconstitution de la fermeture transitive
    all_deps = fermetureTransitive(all_deps, tab_data, nb_colonnes);

	// Unification appliquée à l'ensemble des dépendances afin d'unifier les informations
	// tels que les déterminants et les déterminés (soumis) en une seule information non redondante
	Dependancies* unified_deps = unification(all_deps);
	courant_dep = unified_deps;
	while (courant_dep != NULL)
	{
		temp_chaine = NULL;
		temp_chaine = append_chaine(temp_chaine, courant_dep->determinant);
		temp_chaine = append_chaine(temp_chaine, getSubmited(init->dependances, courant_dep->determinant));
		temp_chaine = delete_repeated(temp_chaine);
		temp_dep = projection(cover, temp_chaine, tab_data, nb_colonnes);
		temp_schema = createCellSchema(temp_chaine, temp_dep);
		if (!hasAnagrammeInSchema(dump, temp_schema) )
			dump = appendSchema(dump, temp_schema);

		courant_dep = courant_dep->suiv;
	}

	// dernière vérification s'il n'y pas de clés dans les éléments de S
	if (!keysInSchemaElements(dump, keys))
	{
		temp_schema = (Schema*)malloc(sizeof(Schema));
		temp_schema->dependances = NULL;
		temp_schema->relation = copie_chaine(keys->attributs);
		temp_schema->suiv = NULL;
		dump = appendSchema(dump, temp_schema);
	}

	return dump;
}

void afficherSchemas(Schema* decomp)
{
	Schema* courant = decomp;
	int i = 1;
	while (courant != NULL)
	{
		printf("R%d(", i);
		afficher_chaine(courant->relation);
		printf(")\n");
		// printf("Deps%d\n", i);
		// afficher_dependances_cons(courant->dependances);
		courant = courant->suiv;
		i++;
	}
}

// return the last found schema in the list
Schema* getLastSchema(Schema* schs)
{
	Schema* courant = schs;
	if (schs == NULL)
		return schs;
	while (courant->suiv != NULL)
		courant = courant->suiv;
	return courant;
}

Schema* getNext(Schema* init, Schema* current)
{
	Schema* courant = init;
	while (courant != NULL &&
		!( are_anagramme_chaine(courant->relation, current->relation)
			&& areAnagrammeDependance(courant->dependances, current->dependances)) )
	{
		courant = courant->suiv;
	}
	return (courant != NULL ? courant->suiv : NULL);
}

int keysInSchemaElements(Schema* sch, Set_Of_Attrs* set)
{
	Schema* courant_schema;
	Set_Of_Attrs* courant_attr = set;
	while (courant_attr != NULL)
	{
		courant_schema = sch;
		while (courant_schema != NULL)
		{
			if (contained_in_linked_string(courant_schema->relation, courant_attr->attributs) )
				return 1;
			courant_schema = courant_schema->suiv;
		}
		courant_attr = courant_attr->suiv;
	}
	return 0;
}

int tailleDecomp(Decomposition* param)
{
	Decomposition* courant = param;
	int compteur = 0;
	while (courant != NULL)
	{
		compteur++;
		courant = courant->suiv;
	}
	return compteur;
}

int schemaComplete(Schema* schs, Chaine* relation_R)
{
	Schema* courant_schema = schs;
	Chaine* relation_join = NULL;
	while (courant_schema != NULL)
	{
		relation_join = append_chaine(relation_join, copie_chaine(courant_schema->relation));
		courant_schema = courant_schema->suiv;
	}
	if (contained_in_linked_string(relation_join, relation_R))
		return 1;
	else
		return 0;
}

// Fonctions de manipulations de ce type (Decomposition)
Decomposition* createCellDecomp(Schema* sch)
{
	Decomposition* dump = (Decomposition*)malloc(sizeof(Decomposition));
	dump->schemas = copieSchema(sch);
	dump->suiv = NULL;
	return dump;
}

Decomposition* copieDecomp(Decomposition* to_copie)
{
	Decomposition* courant = to_copie;
	Decomposition* dump = NULL;
	Decomposition* temp;
	Decomposition* parcours;
	while (courant != NULL)
	{
		temp = createCellDecomp(courant->schemas);
		if (dump == NULL)
		{
			dump = temp;
			parcours = temp;
		}
		else
		{
			parcours->suiv = temp;
			parcours = parcours->suiv;
		}
		courant = courant->suiv;
	}

	return dump;
}

Decomposition* appendDecomp(Decomposition* param, Decomposition* to_append)
{
	if ( param == NULL)
        return copieDecomp(to_append);
    Decomposition* courant = param;
    while (courant->suiv != NULL)
        courant = courant->suiv;
    courant->suiv = copieDecomp(to_append);
    return param;
}

int isAddableSchema(Schema* schs, Schema* cell_sch)
{
	Schema* courant = schs;
	while (courant != NULL)
	{
		if ( contained_in_linked_string(courant->relation, cell_sch->relation)
			|| contained_in_linked_string(cell_sch->relation, courant->relation) )
			return 0;
		courant = courant->suiv;
	}
	return 1;
}

Decomposition* getCombinaisonsDecompLevel(Decomposition* previous_level, Schema* schema_collection, Chaine* relation_R)
{

	Decomposition* courant_decomp = previous_level;
	Decomposition* dump = NULL;
	Schema* temp_schema;
	Schema* courant_schema;
	Decomposition* temp_decomp;

	while (courant_decomp != NULL)
	{
		temp_schema = getLastSchema(courant_decomp->schemas);
		courant_schema = getNext(schema_collection, temp_schema);
		while (courant_schema != NULL)
		{
			temp_decomp = createCellDecomp(courant_decomp->schemas);
			if (!schemaComplete(temp_decomp->schemas, relation_R))
			{
				if (isAddableSchema(temp_decomp->schemas, courant_schema))
				{
					temp_decomp->schemas = appendCellSchema(temp_decomp->schemas, courant_schema);
					dump = appendDecomp(dump, temp_decomp);
				}
				else
					free(temp_decomp);
			}
			else
				free(temp_decomp);
			courant_schema = courant_schema->suiv;
		}
		courant_decomp = courant_decomp->suiv;
	}
	return dump;
}

Decomposition* getCombinaisonsDecomp(Schema* all_schemas, Chaine* relation_R)
{
	Decomposition* dump = NULL;
	Decomposition* temp;
	Decomposition* other_temp;
	Schema* courant_schema;

	// initialisation niveau n = 1
	courant_schema = all_schemas;
	while (courant_schema != NULL)
	{
		temp = createCellDecomp(createCellSchema(courant_schema->relation, courant_schema->dependances));
		dump = appendDecomp(dump, temp);
		courant_schema = courant_schema->suiv;
	}
	// autres niveaux n>=2
	int n = tailleDecomp(dump);
	int i = 2;
	temp = dump;
	while (i<=n)
	{
		temp = getCombinaisonsDecompLevel(temp, all_schemas, relation_R);
		// if (tailleDecomp(other_temp) > tailleDecomp(temp))
			dump = appendDecomp(dump, temp);
		i++;
		// temp = other_temp;
	}

	return dump;
}

void afficherDecomp(Decomposition* vd)
{
	if (vd == NULL)
		printf("VIDE\n");
	Decomposition* courant = vd;
	int i = 1;
	while (courant != NULL)
	{
		printf("[----] decomp %d : \n", i);
		afficherSchemas(courant->schemas);
		printf("-----\n");
		courant = courant->suiv;
		i++;
	}
}

// algorithmes de validation

//Decomposition* losslessJoin()
