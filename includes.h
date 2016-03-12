#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#define NMAX 256
#define UTF8(string) g_locale_to_utf8(string, -1, NULL, NULL, NULL)

typedef char line[NMAX];

typedef struct
{
  char** val;
  int nbre;
} Line_data;

// tuples enregistrer comme listes chainées d'entiers
typedef struct entier
{
	int val;
	struct entier* suiv;
} Entier;

// pour stocker les combinaisons des attributs
typedef struct combinaison
{
	Entier* combi;
	struct combinaison* suiv;
} Combinaison;

typedef struct partition
{
	Entier* tuples;
	int cardinal;
	struct partition* suiv;
} Partition;

// liste chainée permettant de stocker
// l'ensemble des couples générés par les composants de MC
typedef struct couples
{
	int cpl[2];
	struct couples* suiv;
} Couples;

typedef struct ec
{
	int colonne_k; // numéro de l'attribut
	int position;
	struct couples* suiv;
} EC;

typedef struct chaine
{
	char* attribut;
	struct chaine* suiv;
} Chaine;

// stocker ensemble en accord (attribut)
typedef struct set_Of_Attrs
{
	Chaine* attributs;
	struct set_Of_Attrs* suiv;
} Set_Of_Attrs;

typedef struct depend
{
	Chaine* determinant;
	Chaine* soumis;
	struct depend* suiv;
} Dependancies;

typedef struct schema
{
	Chaine* relation;
	Dependancies* dependances;
	struct schema* suiv;
} Schema;

// stocker les décompositions valides et/ou en 3NF
typedef struct decomp
{
	Schema* schemas;
	struct decomp* suiv;
} Decomposition;

typedef struct datas_partition
{
	Partition** part_relat;
	int nb_colonnes;
} Datas_partition;


// explode.c
Line_data* explode(char separateur, char *chaine);

// run_gtk_window.c
int run_gtk_window(char* filename);

// Notifier.c
void print_info (GtkWidget* win, gchar* msg);
void print_error (GtkWidget* win, gchar* msg);
void print_warning (GtkWidget* win, gchar* msg);

// fonctions.c
GtkListStore* create_model (Line_data* data, GType* types, int nb_lignes, int nb_colonnes);
GtkTreeViewColumn* column_renderer(GtkWidget* list_aff ,char* name, int column_number);
GtkWidget* create_renderer(GtkListStore* model, Line_data line_data);
Line_data* extraction(FILE* file, int* p_nb_lignes, int* p_nb_colonnes, Line_data* p_column_titles);
void afficherDecomposition();
void hello();
void debug(char* msg);
// -- chaines
int chars_in_linked_string(Chaine* attrs, char* motif);
Chaine* create_cell_chaine(char* attr);
Chaine* delete_cell_chaine(Chaine* param, Chaine* to_delete);
Chaine* copie_chaine(Chaine* lk_string);
int are_same_string(char* chaine1, char* chaine2);
int are_same_linked_string(Chaine* lk_chaine1, Chaine* lk_chaine2);
Chaine* append_chaine(Chaine* dump, Chaine* to_append);
Chaine* append_cell_chaine(Chaine* param, Chaine* cell_chaine);
Chaine* minus_chaine(Chaine* param1, Chaine* param2);
Chaine* delete_repeated(Chaine* param);
void afficher_chaine(Chaine* param);
int taille_chaine(Chaine* attr);
int in_linked_string(Chaine* attrs, Chaine* attr);
int contained_in_linked_string(Chaine* attrs, Chaine* attr);
int are_anagramme_chaine(Chaine* ch1, Chaine* ch2);
int in_linked_string(Chaine* attrs, Chaine* attr);
Set_Of_Attrs* append_set_of_attrs(Set_Of_Attrs* dump, Set_Of_Attrs* to_append);
// -- tuples
int cardinal_tuples (Entier* tuples);
int in_tuples(Entier* tuples, Entier* voulu);
Entier* append_tuple(Entier* ag, Entier* to_append);
int get_last_tuple_value(Entier* tuple);
int tuples_identiques(Entier* t1, Entier* t2);
Entier* copie_tuple(Entier* a_copier);
void afficher_tuple(Entier* tuple);
// -- couples
int in_couples(Couples* couples, Couples* couple);

//
char* attribut(int k, Line_data* tab_data);
int colonne(char* attribut, Line_data* tab_data, int nb_colonnes);

// partition.c
int in_partition_colonne(Partition* p, Entier* tuple);
Partition* partition_colonne (int colonne, Line_data* tab_data, int nb_lignes);
Partition* partition_relation(Line_data* tab_data, int nb_lignes, int nb_colonnes);
Entier* get_tuples_reg(char* valeur, Line_data* tab_data, int taille_tab, int colonne);
void afficher_partition_colonne(Partition* p);
// -- elagué
	Partition* elaguer_partition_attribut(Partition* partition);
	Partition** elaguer_partition_relation(Partition** partition, int nb_colonnes);
void afficher_partitions(GtkWidget* wid, Datas_partition* datas_part);
void afficher_partitions_elaguees(GtkWidget* wid, Datas_partition* datas_part_elag);

// couverture_minimale.c
Dependancies* getCover(Dependancies* deps);
void afficher_couverture_minimale(GtkWidget* wid, Dependancies* datas_cover);

// agree_set.c
int tuple_in_partition(Partition* parti, Entier* tuple);
int appartient_class_equivalence(Entier* tuples, int a);
Set_Of_Attrs* append_to_agree_set(Set_Of_Attrs* ag, Set_Of_Attrs* to_append);
Set_Of_Attrs* intersections(EC* ec_a, EC* ec_b, Line_data* tab_data);
Partition* get_MC(Partition** base_elaguee, int nb_colonnes);
Couples* get_couples(Partition* MC);
Set_Of_Attrs* agree_set(Partition** base_elaguee, int nb_colonnes, int nb_lignes, Line_data* tab_data);
void print_set_of_attrs(Set_Of_Attrs* accord);
void afficher_ensembles_en_accord(GtkWidget* wid, Set_Of_Attrs* datas_agree);
int in_agree_set(Set_Of_Attrs* ag, Set_Of_Attrs* wanted);
Chaine* ag(int ligne_i, int ligne_j, Line_data* tab_data, int nb_colonnes);
// -- ec
EC* ec(int ligne_i, Partition** base_elaguee, int nb_colonnes);
int in_ecs(EC* ecs, EC* ec);
EC* append_ec(EC* ecs, EC* ec);
void afficher_ecs(EC* ecs);

// dependances.c
Combinaison* appendElementCombinaison(Combinaison* dump, Entier* to_append);
Combinaison* appendCellCombinaison(Combinaison* dump, Combinaison* to_append);
Combinaison* getCombinaisonsNumbersLevel(Combinaison* previous_level, int niveau, int nb_colonnes);
Combinaison* getCombinaisonsNumbers(int nb_colonnes);
Set_Of_Attrs* getCombinaisonsAttrs(Combinaison* param, Line_data* tab_data);
Combinaison* onlySizeN(Combinaison* param, int n);
Couples* getCouplesCombinaisons(Entier* feed);
void afficher_combinaisons(Combinaison* combinaisons);
void afficher_combinaisons_attributs(Combinaison* combinaisons, Line_data* tab_data);
void afficher_dependances_cons(Dependancies* param);
int dependanceFonctionnelle(int colonne_j, int colonne_k, Line_data* tab_data, int nb_lignes);
Dependancies* dependances(Set_Of_Attrs* agree, Line_data* tab_data, int nb_lignes, int nb_colonnes);
Dependancies* appendDependance(Dependancies* param, Dependancies* to_append);
Dependancies* appendCellDependance(Dependancies* param, Dependancies* cell_dep);
Dependancies* createCellDependance(Chaine* determinant, Chaine* soumis);
int sizeDependance(Dependancies* deps);
Dependancies* copieDependance(Dependancies* to_copie);
Dependancies* deleteCellDependance(Dependancies* deps, Dependancies* to_delete);
Dependancies* deleteAllOccurencesDependance(Dependancies* deps, Dependancies* to_delete);
int areSameDependancies(Dependancies* dep1, Dependancies* dep2);
int inDependancies(Dependancies* deps, Dependancies* dep);
Dependancies* desunification(Dependancies* param);
void afficher_dependances(GtkWidget* wid, Dependancies* datas_dependance);

// keys.c
Set_Of_Attrs* get_keylevel(Set_Of_Attrs* Li, Set_Of_Attrs* agree_set, Line_data* tab_data, int nb_colonnes);
void afficher_cles(GtkWidget* wid, Set_Of_Attrs* datas_keys);

// decomposition.c
Dependancies* getComplementFromKeys(Dependancies* deps, Set_Of_Attrs* keys, Line_data* tab_data, int nb_colonnes);
Chaine* getSubmited(Dependancies* deps, Chaine* determinant);
Schema* initialSchema(Dependancies* f_transi, Line_data* tab_data, int nb_colonnes);
Schema* createCellSchema(Chaine* relation, Dependancies* depandances);
Schema* appendSchema(Schema* param, Schema* to_append);
Schema* appendCellSchema(Schema* param, Schema* to_append);
Schema* copieSchema(Schema* sch);
int sizeSchema(Schema* sch);
int areSameSchema(Schema* sch1, Schema* sch2);
int hasAnagrammeInSchema(Schema* schs, Schema* sch);
int areAnagrammesSchema(Schema* sch1, Schema* sch2);
int inSchemas(Schema* schs, Schema* sch);
Dependancies* projection(Dependancies* deps, Chaine* U, Line_data* tab_data, int nb_colonnes);
Schema* getDecomposition(Schema* init, Dependancies* cover, Set_Of_Attrs* keys, Line_data* tab_data, int nb_colonnes);
void afficherSchemas(Schema* decomp);
//
Schema* getLastSchema(Schema* param);
Schema* getNext(Schema* init, Schema* current);
int keysInSchemaElements(Schema* sch, Set_Of_Attrs* set);
int tailleDecomp(Decomposition* param);
int schemaComplete(Schema* schs, Chaine* relation_R);
Decomposition* createCellDecomp(Schema* sch);
Decomposition* copieDecomp(Decomposition* to_copie);
Decomposition* appendDecomp(Decomposition* param, Decomposition* to_append);
int isAddableSchema(Schema* schs, Schema* cell_sch);
Decomposition* getCombinaisonsDecompLevel(Decomposition* previous_level, Schema* schema_collection, Chaine* relation_R);
Decomposition* getCombinaisonsDecomp(Schema* all_schemas, Chaine* relation_R);
Decomposition* decomp(Schema* sch, Chaine* initial_relation);
void afficherDecomp(Decomposition* vd);
