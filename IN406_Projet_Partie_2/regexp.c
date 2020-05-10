#include "regexp.h"
#include <string.h>

ADERIV nouvel_arbre(STATE s, char c){
	ADERIV a = malloc(sizeof(struct aderiv));
	if(!a) {printf("Plus de mémoire, malloc a échoué.\n"); exit(5);}
	a->s = s;
	a->caractere = c;
	a->fils[0] = NULL;
	a->fils[1] = NULL;
	a->fils[2] = NULL;
	return a;
}

void liberer_arbre(ADERIV a){
	if(a){
		for(int i = 0; i < 3; i++) liberer_arbre(a->fils[i]);
		free(a);
	}
}


int indice_char(char c){//retourne l'indice correspondant au caractère dans le tableau des états
	switch(c){
		case '+': return 0;
		case '.': return 1;
		case '*': return 2;
		case '(': return 3;
		case ')': return 4;
		case '#': return 6;
		default:
			if( c >= 'a' && c <= 'z') return 5;
			return -1;
	}
}

// ADERIV algo_recur(PILE *p, STATELISTE *table, char *expr){
// 	ADERIV *a = NULL;

// 	STATE etat_courant = depiler(p);
// 	int indice = indice_char(expr[0]);
// 	if(etat_courant == S || etat_courant == A || etat_courant == B || etat_courant == C || etat_courant == D || etat_courant == E || etat_courant == F){
// 		for (int i = 0; i < table[etat_courant][indice].taille; ++i)
// 		{
// 			printf("yes\n");
// 		}
// 	}
// 	if(etat_courant == POINT || etat_courant == PARO || etat_courant == PARF || etat_courant == PLUS || etat_courant == ETOILE || etat_courant == CAR) 
// 		return a;

// 	return a;
// }

ADERIV construire_arbre_derivation(char *expr){
	STATELISTE table[7][7] = {//cette table représente la table des transitions de l'énoncé
		{{-1},{-1},{-1},{2,{A,B}},{-1},{2,{A,B}},{-1}}, // transition quand le STATE S est lu
		{{-1},{-1},{-1},{2,{C,D}},{-1},{2,{C,D}},{-1}},//STATE A
		{{3,{PLUS,A,B}},{-1},{-1},{-1},{0},{-1},{1,{CAR}}},//STATE B
		{{-1},{-1},{-1},{2,{E,F}},{-1},{2,{E,F}},{-1}},//STATE C
		{{0},{3,{POINT,C,D}},{-1},{-1},{0},{-1},{0}},//STATE D
		{{0},{0},{0},{3,{PARO,S,PARF}},{-1},{1,{CAR}},{-1}},//STATE E
		{{0},{0},{2,{ETOILE, F}},{-1},{0},{-1},{0}}//STATE F
	};
	//Une STATELISTE de taille 0 correspond à une règle dont la production est epsilon.
	//Une STATELISTE de taille -1 correspond à une erreur (expression rejetée)
	int taille = strlen(expr);
	PILE p = nouvelle_pile(taille*2);
	int symbole = 0;
	int indice = 0;
	ADERIV a;
	
	p = empiler(p, S);
	affiche_pile(p);
	a = nouvel_arbre(S, 0);
	indice = indice_char(expr[0]);
	symbole = p.contenu[p.sommet-1];

	//a = algo_recur(&p, &table, &expr);


	while(!est_vide(p))
	{
		//NON TERMINAL
		if(symbole == S || symbole == A || symbole == B || symbole == C || symbole == D || symbole == E || symbole == F)
		{
			if(table[symbole][indice].taille != -1)
			{
				switch(table[symbole][indice].taille)
				{
					case(0):
						depiler(&p);
						affiche_pile(p);
						symbole = p.contenu[p.sommet-1];

						break;
				
					case(1):
						depiler(&p);
						// p = empiler(p, table[symbole][indice].liste[0]);
						// affiche_pile(p);
						for (int i = 0; i >= 0; --i)
						{
							p = empiler(p, table[symbole][indice].liste[i]);
						}
						affiche_pile(p);
						symbole = p.contenu[p.sommet-1];

						break;
				
					case(2):
						depiler(&p);
						// p = empiler(p, table[symbole][indice].liste[1]);
						// affiche_pile(p);
						// p = empiler(p, table[symbole][indice].liste[0]);
						// affiche_pile(p);
						for (int i = 1; i >= 0; --i)
						{
							p = empiler(p, table[symbole][indice].liste[i]);
						}
						affiche_pile(p);
						symbole = p.contenu[p.sommet-1];

						break;
				
					case(3):
						depiler(&p);
						// p = empiler(p, table[symbole][indice].liste[2]);
						// affiche_pile(p);
						// p = empiler(p, table[symbole][indice].liste[1]);
						// affiche_pile(p);
						// p = empiler(p, table[symbole][indice].liste[0]);
						// affiche_pile(p);
						for (int i = 2; i >= 0; --i)
						{
							p = empiler(p, table[symbole][indice].liste[i]);
						}
						affiche_pile(p);
						symbole = p.contenu[p.sommet-1];

						break;	
				}
			}
			else
			{
				printf("Erreur : expression non reconnue\n");
				return NULL;
			}
		}

		//TERMINAL
		if(symbole == POINT || symbole == PARO || symbole == PARF || symbole == PLUS || symbole == ETOILE || symbole == CAR)
		{
			if(p.sommet == 1 && expr[strlen(expr ) -1] == '#')
				printf("Expression reconnu\n");
			
			printf("Depilement de |"); 
			affiche_state(symbole);
			printf("\n");
			depiler(&p);
			affiche_pile(p);
			for (int j = 0; j < strlen(expr); ++j)
			{
				expr[j] = expr[j+1];
			}
			indice = indice_char(expr[0]);
			symbole = p.contenu[p.sommet-1];
			
		}
	}
	return a;
}

void affiche_aderiv(ADERIV a, int space){//rendre joli
	//affiche les fils de gauche à droite
	if(a){
		affiche_aderiv(a->fils[2], space + 1);
		affiche_aderiv(a->fils[1], space + 1);
		for(int i = 0; i < space; i++) printf("   ");
		affiche_state(a->s);
		if(a->s == CAR) printf(" : %c",a->caractere);
		printf("\n");
	    affiche_aderiv(a->fils[0], space + 1);
	}
}