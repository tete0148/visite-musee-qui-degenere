#pragma once
#include "events.h"
#include "vue.h"
#include <iostream>
#include "ia.h"
#include "editeur.h"
#include "ressources.h"

using namespace std;

void gere_evenements(Globals* globals)
{
	SDL_Event event;
	bool continuer = true;
	long derniertemps = 0;
	while (continuer) {
		if (globals->musees[globals->musee_courant]->voleur->nElementInvetaire > 0)//les gardiens bougent quand on a volé quelquechose
			loop(globals);
		long tempsactuel = SDL_GetTicks();

		if (tempsactuel - derniertemps >= 60) {
			SDL_PollEvent(&event);
			switch (globals->vue_actuelle) {
				case MUSEE:
					event_vue_musee(globals, event);
					break;
				case ACCUEIL:
					event_vue_accueil(globals, event);

					break;
				case SELECTION_MUSEE:
					event_vue_selection_musee(globals, event);

					break;
				case EDITEUR:
					event_vue_editeur(globals, event);
					break;
				case GAGNE:
					if (event.type == SDL_KEYDOWN) {
						globals->vue_actuelle = ACCUEIL;
						affiche(globals);
					}
					break;
				case PERDU:
					if (event.type == SDL_KEYDOWN) {
						globals->vue_actuelle = ACCUEIL;
						SDL_Delay(2000);
						continuer = false;
					}
					break;
				case COMMANDES:
					event_vue_commandes(globals, event);
					break;
			}

			if (event.type == SDL_QUIT)
				continuer = false;

			derniertemps = SDL_GetTicks();
		}
		SDL_Delay(2);
		
	}
		
}

void event_vue_musee(Globals* globals, SDL_Event& event)
{
	static bool up = false;
	static bool down = false;
	static bool left = false;
	static bool right = false;
	Musee* musee = globals->musees[globals->musee_courant];

	if (musee->voleur->attrape) {
		globals->vue_actuelle = PERDU;
		affiche(globals);
		return;
	}

	if (up)
		musee->voleur->vitesse = 1;
	if (down)
		musee->voleur->vitesse = -1;
	if (left)
		musee->voleur->angle -= musee->voleur->vitesseRot;
	if (right)
		musee->voleur->angle += musee->voleur->vitesseRot;
	if (up || down || left || right) {
		deplace_voleur(globals);
		detecter_oeuvre(globals);
		affiche_voleur(globals);
	}

	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			up = true;
			break;
		case SDLK_DOWN:
			down = true;
			break;
		case SDLK_LEFT:
			left = true;
			break;
		case SDLK_RIGHT:
			right = true;
			break;
		case SDLK_SPACE:
			Oeuvre* oeuvre = detecter_oeuvre(globals);
			if (musee->voleur->nElementInvetaire < musee->voleur->tailleInventaire)
				if (oeuvre != nullptr) {
					bool volable = true;
					for (int i = 0; i < musee->NB_OEUVRES; i++)
						if (musee->idOeuvres[i] == oeuvre->id) {
							if (musee->oeuvresVolees[i] == false)
								musee->oeuvresVolees[i] = true;
							else
								volable = false;
							break;
						}
					if (volable) {
						musee->voleur->inventaire[musee->voleur->nElementInvetaire++] = oeuvre;
						affiche(globals);
					}

				}
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			musee->voleur->vitesse = 0;
			up = false;
			break;
		case SDLK_DOWN:
			musee->voleur->vitesse = 0;
			down = false;
			break;
		case SDLK_LEFT:
			left = false;
			break;
		case SDLK_RIGHT:
			right = false;
			break;
		}
	}
}

void event_vue_editeur(Globals* globals, SDL_Event& event)
{
	switch (event.type) {

	case SDL_MOUSEBUTTONDOWN: {
		editeur_clic(globals, event.button.x, event.button.y);
	}
	}
}

void event_vue_commandes(Globals* g, SDL_Event& event)
{
	affiche_commandes(g);
	if (event.type != SDL_MOUSEBUTTONDOWN)
		return;
	int xmin = 45;
	int xmax = xmin + 110;
	int ymin = NbBlocsH * TailleCaseH + 10;
	int ymax = ymin + 20;
	int x = event.button.x;
	int y = event.button.y;
	if (x >= xmin && x <= xmax && y >= ymin && y <= ymax) {
		g->vue_actuelle = ACCUEIL;
		affiche(g);
		return;
	}
}

void event_vue_accueil(Globals* globals, SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
		globals->vue_actuelle = MUSEE;
		SDL_RenderClear(globals->renderer);
		affiche(globals);
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		int x = event.button.x;
		int y = event.button.y;
		
		//bouton choisir musee
		int xmin = FenetreL/2 - 300;
		int xmax = FenetreL/2 + 300;
		int ymin = 150 - 12;
		int ymax = ymin + 70;

		if (x > xmin && x < xmax && y > ymin && y < ymax) {
			globals->vue_actuelle = SELECTION_MUSEE;
			SDL_RenderClear(globals->renderer);
			affiche(globals);
		}

		ymin = 200 - 12;
		ymax = ymin + 50;

		if (x > xmin && x < xmax && y > ymin && y < ymax) {
			globals->vue_actuelle = EDITEUR;
			affiche(globals);
		}

		ymin = 250 - 12;
		ymax = ymin + 70;
		if (x > xmin && x < xmax && y > ymin && y < ymax) {
			globals->vue_actuelle = COMMANDES;
			SDL_RenderClear(globals->renderer);
			affiche(globals);
		}
	}
	
}

void event_vue_selection_musee(Globals* globals, SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN) {

		//clique sur musee
		int x = event.button.x;
		int y = event.button.y - 60;
		int hauteur_bouton = 30;
		int xmin = FenetreL / 2 - 50;
		int xmax = xmin + 100;
		int ymin = 0;
		int ymax = hauteur_bouton * (globals->NB_MUSEES+1);

		if (x > xmin && x < xmax && y > ymin && y < ymax) {
			int bouton = y / (hauteur_bouton);
			if (bouton == globals->NB_MUSEES) {
				globals->vue_actuelle = ACCUEIL;
				reset_musee_actuel(globals);
				SDL_RenderClear(globals->renderer);
				affiche(globals);
			}
			else {
				globals->musee_courant = bouton;
				affiche(globals);
			}
		}

		int musee = y / 50;

		//clic sur bouton supprimer
		xmin = FenetreL / 2 + 65;
		xmax = xmin + 20;
		if (x > xmin && x < xmax && y > ymin && y < ymax) {
			int bouton = y / (hauteur_bouton); //id musee
			if (bouton == globals->musee_courant || bouton >= globals->NB_MUSEES)
				return;
			if (globals->NB_MUSEES == 2) {
				globals->musee_courant = 0;
			}
			supprimer_musee(globals, bouton);
		}

		//clic sur le bouton editer
		xmin = FenetreL / 2 + 90;
		xmax = xmin + 75;
		ymin -= 10;
			
		if (x > xmin && x < xmax && y > ymin && y < ymax) {
			int bouton = y / (hauteur_bouton); //id musee
			if (bouton == globals->musee_courant || bouton >= globals->NB_MUSEES)
				return;
			globals->museeEditeur = globals->musees[bouton];
			globals->vue_actuelle = EDITEUR;
			affiche(globals);
			return;
		}
	}
}

void deplace_voleur(Globals* g)
{
	Personnage& voleur = *g->musees[g->musee_courant]->voleur;
	Vecteur& pos = voleur.position;
	double& angle = voleur.angle;
	double vitesse = voleur.vitesse;
	pos.x += vitesse * cos(angle);
	pos.y += vitesse * sin(angle);
	if (g->musees[g->musee_courant]->cases[(int)pos.y][(int)pos.x] != 0) {
		if (g->musees[g->musee_courant]->cases[(int)pos.y][(int)pos.x] == 2) {
			g->vue_actuelle = GAGNE;
			affiche(g);
			SDL_Delay(2000); //on affiche l'écran Gagne au moins 2 secondes
			return;
		}
		else {
			pos.x -= vitesse * cos(angle);
			pos.y -= vitesse * sin(angle);
		}
		
	} 
	int efface_rayon = 4;
	affiche_partie_musee(g, (int)pos.y - efface_rayon, (int)pos.y + efface_rayon, (int)pos.x - efface_rayon, (int)pos.x + efface_rayon);


}

Oeuvre* detecter_oeuvre(Globals* g)
{
	Musee& musee = *g->musees[g->musee_courant];

	Vecteur vue;
	vue.x = musee.voleur->position.x + musee.voleur->longueur_trait * cos(musee.voleur->angle);
	vue.y = musee.voleur->position.y + musee.voleur->longueur_trait * sin(musee.voleur->angle);
	int case_ = musee.cases[(int)vue.y][(int)vue.x];
	if (vue.y < 0 || vue.y >= NbBlocsH || vue.x < 0 || vue.x >= NbBlocsL)
		return nullptr;
	if (case_ > 4) {
		Oeuvre* oeuvre = recup_oeuvre_depuis_case_num(g, case_);
		affiche_tableau(g, oeuvre);
		return oeuvre;
	}
	else
		efface_tableau(g);
	return nullptr;
}

Oeuvre* recup_oeuvre_depuis_case_num(Globals* g, int case_)
{
	int indexOeuvre = 0;
	for (int i = 0; i < g->musees[g->musee_courant]->NB_OEUVRES; i++)
		if (g->musees[g->musee_courant]->numerosOeuvres[i] == case_) {
			indexOeuvre = i;
			break;
		}
	int id = g->musees[g->musee_courant]->idOeuvres[indexOeuvre];
	return recup_oeuvre(g, id);
}

Oeuvre* recup_oeuvre(Globals* g, int id)
{
	for (int i = 0; i < g->NB_OEUVRES; i++) {
		if (g->oeuvres[i]->id == id)
			return g->oeuvres[i];
	}
	return nullptr;
}