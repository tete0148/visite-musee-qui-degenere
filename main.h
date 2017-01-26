#ifndef __MAIN
#define __MAIN

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "config_sdl.h"

const int NbBlocsH = 24;
const int NbBlocsL = 32;

const int TailleCaseH = 15;
const int TailleCaseL = 20;

const int FenetreL = 1010; // size 700 + 300 pour l'affichage des images
const int FenetreH = 400;

const double PI = (4. * atan(1.));

#include "musee.h"
#include "oeuvre.h"

typedef enum VUE_ACTUELLE { ACCUEIL, SELECTION_MUSEE, AJOUT_MUSEE, MUSEE, EDITEUR, GAGNE, PERDU, COMMANDES } VUE_ACTUELLE;
typedef enum OUTILS { MUR, VIDE, OEUVRE, SORTIE, GARDIEN, VOLEUR } OUTILS;

typedef struct {
	SDL_Window* window = nullptr;
	SDL_Renderer * renderer = nullptr;
	Oeuvre** oeuvres = nullptr;
	int NB_OEUVRES = -1;
	Musee** musees;
	Musee* museeEditeur = nullptr;
	int NB_MUSEES = -1;
	
	int musee_courant = 0;
	VUE_ACTUELLE vue_actuelle = ACCUEIL;
	Oeuvre* oeuvreActuelle = nullptr;
	OUTILS outilActuel = VIDE;
} Globals;

void reset_musee_actuel(Globals* g);

void init_jeu(Globals* globals);
void quit_jeu(Globals* globals);

#endif