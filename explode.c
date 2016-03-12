#include "includes.h"

Line_data* explode(char separateur, char* chaine)
{
    char *pSep;
    char *pLastSep;
    int nb_sep = 0;
    char* duplicate; /* On duplique la chaine, l'utilisateur n'aimerait pas que la sienne soit modifiée */
    char **pResult;  /* Futur pointeur sur le tableau des résultats */

    Line_data* elem;
    elem = (Line_data*)malloc(sizeof(Line_data));
    /* On duplique la chaine */
    duplicate = (char*) malloc(sizeof(char) * (strlen(chaine) + 1));
    pLastSep = duplicate;
    // vérification si allocation bien effectuée
    if (duplicate == NULL)
      return NULL;
    strcpy(duplicate, chaine);

    /* On compte le nombre de séparateurs */
    nb_sep = 0;
    while ((pSep = strchr(pLastSep, separateur)) != NULL)
    {
      nb_sep++;
      pLastSep = pSep + 1;
    }

    /* On alloue un tableau de pointeurs qui contiendra les résultats */
    pResult = malloc(sizeof(char*) * (nb_sep+1));
    // vérification si allocation bien effectuée
    if (pResult == NULL)
        return NULL;

    /* Réinitialisation des variables */
    nb_sep = 0;
    pLastSep = duplicate;
    pResult[0] = duplicate;
    /* On remplace les séparateurs par '\0' et on remplit le tableau des résultats */
    while ((pSep = strchr(pLastSep, separateur)) != NULL)
    {
        nb_sep++;
        *pSep = '\0';
        pLastSep = pSep + 1;
        pResult[nb_sep] = pSep + 1;
        // printf("Coupe: %s\n", pLastSep);
    }

    /* On renvoie tout dans la structure passée par pointeur */
    elem->val = pResult;
    elem->nbre = nb_sep;
    // remplacer l'éventuelle \n accompagnant le dernier élément résultat
    if ( (pSep = strchr(elem->val[nb_sep], '\n')) != NULL )
        *pSep = '\0';
    return elem;
}