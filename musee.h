#pragma once
#include "personnage.h"

typedef struct {
	int id = 0;
	char* nom;
	char* path;
	int cases[NbBlocsH][NbBlocsL];
	Personnage* voleur;
	Personnage** gardiens;
	int NB_GARDIENS = 0;
	int* idOeuvres; //tableux d'id des oeuvres;
	int NB_OEUVRES = 0;
	int* numerosOeuvres;
	bool* oeuvresVolees;
} Musee;