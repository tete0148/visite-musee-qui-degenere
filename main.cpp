/*
GROUPE 12
CHAMPION Théo
LAMURE Arthur
PUZENAT Florent
TCHA Siong
*/

#include "main.h"
#include "ressources.h"
#include "events.h"
#include "vue.h"
#include "editeur.h"
#include <ctime>

int main(int argc, char* argv[])
{
	srand(time(NULL));
	Globals* globals = new Globals;
	init_jeu(globals);
	charger_oeuvres(globals);
	charger_musees(globals);
	charger_editeur(globals);

	affiche(globals); // afficher le jeu
	gere_evenements(globals);
		
	quit_jeu(globals);

	return 0;
}

void init_jeu(Globals* globals)
{
	SDL_Init(SDL_INIT_VIDEO);
	globals->window   = SDL_CreateWindow("Visite du musee", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FenetreL, FenetreH, SDL_WINDOW_SHOWN);
	globals->renderer = SDL_CreateRenderer(globals->window, -1, SDL_RENDERER_ACCELERATED);
	globals->museeEditeur = new Musee;
	TTF_Init();
}

void quit_jeu(Globals* globals)
{
	decharger_editeur(globals);
	delete globals->museeEditeur;
	decharger_oeuvres(globals);
	decharger_musees(globals);
	TTF_Quit();
	SDL_DestroyRenderer(globals->renderer);
	SDL_DestroyWindow(globals->window);
	SDL_Quit();

	delete globals;
}

void reset_musee_actuel(Globals* g)
{
	Musee* musee = g->musees[g->musee_courant];
	musee->voleur->nElementInvetaire = 0;
	musee->voleur->position = musee->voleur->defaultPos;
	for (int i = 0; i < musee->NB_GARDIENS; i++)
		musee->gardiens[i]->position = musee->gardiens[i]->defaultPos;
}