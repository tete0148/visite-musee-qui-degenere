#pragma once
#include "main.h"
#include "vecteur.h"
#include "oeuvre.h"

typedef struct {
	Vecteur position;
	Vecteur defaultPos;
	double angle = PI / 3;
	int vitesse = 0;
	double vitesseRot = PI / 12;
	float longueur_trait = 2;
	int tailleInventaire = 3;
	int nElementInvetaire = 0;
	Oeuvre** inventaire;
	bool attrape = false;
	long dernier_deplacement = 0;
} Personnage;