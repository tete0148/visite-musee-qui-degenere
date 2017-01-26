#include "vecteur.h"

double longueur(Vecteur* v1, Vecteur* v2)
{
	return sqrt(pow(v2->x - v1->x, 2) + pow(v2->y - v1->y, 2));
}

double scalaire(Vecteur* v1, Vecteur* v2)
{
	return v1->x * v2->x + v2->y * v1->y;
}

double norme(Vecteur* v)
{
	return sqrt(pow(v->x, 2) + pow(v->y, 2));
}