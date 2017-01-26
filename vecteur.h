#pragma once
#include <math.h>

typedef struct {
	double x;
	double y;
} Vecteur;

double longueur(Vecteur* v1, Vecteur* v2);

double scalaire(Vecteur* v1, Vecteur* v2);

double norme(Vecteur* v);
