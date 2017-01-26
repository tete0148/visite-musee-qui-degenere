#include"vue.h"
#include <iostream>
#include <sstream>
using namespace std;

void affiche(Globals* g)
{
	switch (g->vue_actuelle) {
	case ACCUEIL:
		affiche_menu(g);
		break;
	case MUSEE:
		affiche_musee(g);
		affiche_voleur(g);
		affiche_gardiens(g);
		break;
	case SELECTION_MUSEE:
		affiche_selection_musee(g);
		break;
	case EDITEUR:
		affiche_editeur(g);
		break;
	case GAGNE:
		affiche_gagne(g);
		break;
	case PERDU:
		affiche_perdu(g);
		break;
	}
}

void affiche_musee(Globals* g) {
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderClear(g->renderer);
	for (int i = 0; i < NbBlocsH; i++) {
		for (int j = 0; j < NbBlocsL; j++) {
			int contenuCase = g->musees[g->musee_courant]->cases[i][j];
			if (contenuCase == 0)
				SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
			else if (contenuCase == 2)
				SDL_SetRenderDrawColor(g->renderer, 0, 255, 0, 255);
			else if (contenuCase > 4) {
				int oeuvreIdx = 0;
				for (int k = 0; k < g->musees[g->musee_courant]->NB_OEUVRES; k++)
				{
					if (g->musees[g->musee_courant]->numerosOeuvres[k] == contenuCase) {
						oeuvreIdx = k;
						break;
					}
				}
				if (g->musees[g->musee_courant]->oeuvresVolees[oeuvreIdx])
					SDL_SetRenderDrawColor(g->renderer, 0, 0, 255, 255);
				else
					SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);

			}
			else if (contenuCase == 1)
				SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
			SDL_Rect rect;
			rect.x = j*TailleCaseL;
			rect.y = i*TailleCaseH;
			rect.w = TailleCaseL;
			rect.h = TailleCaseH;
			SDL_RenderFillRect(g->renderer, &rect);
		}
		
	}

	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_Rect rect = { 0,0,TailleCaseL*NbBlocsL,TailleCaseH*NbBlocsH };
	SDL_RenderDrawRect(g->renderer, &rect);
	char font[] = "data\\OpenSans-Regular.ttf";
	ostringstream string;
	string.str();
	string << "Inventaire: " 
		<< g->musees[g->musee_courant]->voleur->nElementInvetaire << '/'
		<< g->musees[g->musee_courant]->voleur->tailleInventaire;
	affiche_texte_erase(g, _strdup(string.str().c_str()), font, 20, 150, NbBlocsH * TailleCaseH + 10, { 255,255,255,25 });

	SDL_RenderPresent(g->renderer);
}

void affiche_partie_musee(Globals*g, int xDebut, int xFin, int yDebut, int yFin)
{
	for (int i = xDebut; i < xFin; i++) {
		for (int j = yDebut; j < yFin; j++) {

			if (j < 0 || j >= NbBlocsL || i < 0 || i >= NbBlocsH)
				continue;

			int contenuCase = g->musees[g->musee_courant]->cases[i][j];
			if (contenuCase == 0)
				SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
			else if (contenuCase == 2)
				SDL_SetRenderDrawColor(g->renderer, 0, 255, 0, 255);
			else if (contenuCase > 4) {
				int oeuvreIdx = 0;
				for (int k = 0; k < g->musees[g->musee_courant]->NB_OEUVRES; k++)
				{
					if (g->musees[g->musee_courant]->numerosOeuvres[k] == contenuCase) {
						oeuvreIdx = k;
						break;
					}
				}
				if (g->musees[g->musee_courant]->oeuvresVolees[oeuvreIdx])
					SDL_SetRenderDrawColor(g->renderer, 0, 0, 255, 255);
				else
					SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);

			}
			else if (contenuCase == 1)
				SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
			SDL_Rect rect;
			rect.x = j*TailleCaseL;
			rect.y = i*TailleCaseH;
			rect.w = TailleCaseL;
			rect.h = TailleCaseH;
			SDL_RenderFillRect(g->renderer, &rect);
		}
	}
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_Rect rect = { 0,0,TailleCaseL*NbBlocsL,TailleCaseH*NbBlocsH };
	SDL_RenderDrawRect(g->renderer, &rect);
	char font[] = "data\\OpenSans-Regular.ttf";
	ostringstream string;
	string.str();
	string << "Inventaire: "
		<< g->musees[g->musee_courant]->voleur->nElementInvetaire << '/'
		<< g->musees[g->musee_courant]->voleur->tailleInventaire;
	affiche_texte_erase(g, _strdup(string.str().c_str()), font, 20, 150, NbBlocsH * TailleCaseH + 10, { 255,255,255,25 });
	affiche_gardiens(g);
	SDL_RenderPresent(g->renderer);
}

void affiche_gagne(Globals* g)
{
	SDL_RenderClear(g->renderer);
	char font[] = "data\\vgafix.ttf";
	SDL_SetRenderDrawColor(g->renderer, 223, 109, 20, 255);
	SDL_Rect rect = { 0,0,FenetreL,FenetreH };
	SDL_RenderFillRect(g->renderer, &rect);
	affiche_texte(g, "GAGNE !!", font, 72, FenetreL / 2, FenetreH / 2 - 36, { 255,255,0,255 });
	SDL_RenderPresent(g->renderer);
}

void affiche_perdu(Globals* g)
{
	SDL_RenderClear(g->renderer);
	char font[] = "data\\vgafix.ttf";
	SDL_SetRenderDrawColor(g->renderer, 206, 206, 206, 255);

	SDL_Rect rect = { 0,0,FenetreL,FenetreH };
	SDL_RenderFillRect(g->renderer, &rect);
	affiche_texte(g, "PERDU !!", font, 72, FenetreL / 2, FenetreH / 2, { 255,255,0,255 });
	SDL_RenderPresent(g->renderer);
}

void affiche_voleur(Globals* g)
{
	Personnage& perso = *g->musees[g->musee_courant]->voleur;
	filledCircleRGBA(g->renderer, (int)(perso.position.x * TailleCaseL), (int)(perso.position.y * TailleCaseH), 5, 255, 0, 0, 255);
	Vecteur point2;
	point2.x =(perso.position.x + perso.longueur_trait * cos(perso.angle));
	point2.y = (perso.position.y + perso.longueur_trait * sin(perso.angle));

	SDL_RenderDrawLine(g->renderer, (int)( perso.position.x * TailleCaseL), (int)( perso.position.y * TailleCaseH), (int)(point2.x * TailleCaseL), (int)(point2.y * TailleCaseH));

	SDL_RenderPresent(g->renderer);
}

void affiche_gardiens(Globals* g)
{
	Personnage**& gardiens = g->musees[g->musee_courant]->gardiens;

	for (int i = 0; i < g->musees[g->musee_courant]->NB_GARDIENS; i++)
	{
		Personnage& perso = *g->musees[g->musee_courant]->gardiens[i];
		filledCircleRGBA(g->renderer, (int)perso.position.x * TailleCaseL, (int)perso.position.y * TailleCaseH, 5, 255, 255, 0, 255);
	}
	
	SDL_RenderPresent(g->renderer);
}

void affiche_menu(Globals * g)
{

	char font[] = "data\\vgafix.ttf";
	SDL_Texture* texture = loadImage(g->renderer, "data\\louvre.bmp");
	SDL_Rect src = { 0, 0, FenetreL, FenetreH};
	SDL_Rect dest = src;
	dest.w = FenetreL;
	dest.h = FenetreH;
	SDL_RenderCopy(g->renderer, texture, &src, &dest);
	SDL_DestroyTexture(texture);

	texture = loadImage(g->renderer, "data\\voleur.bmp", 200, 0, 255, 0);
	src = { 150, 0, 635, 357 };
	dest = src;
	dest.x = 0;
	dest.y = 50;
	SDL_RenderCopy(g->renderer, texture, &src, &dest);
	SDL_DestroyTexture(texture);
	affiche_texte(g, "MENU PRINCIPAL", "data\\IMPRISHA.ttf", 42, FenetreL / 2, 20, { 255,255,0,255 });
	affiche_texte(g, "Appuyez sur entree pour jouer", font, 24, FenetreL / 2, 320, { 255,255,255,255 });
	SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 0);
	dest.x = FenetreL / 2 - 150;
	dest.w = 300;
	dest.h = 37;
	dest.y = 150 -10;
	SDL_RenderFillRect(g->renderer, &dest);
	affiche_texte(g, "> Choisir un musee <", font, 24, FenetreL / 2, 150, { 255,255,255,225 });
	dest.x = FenetreL / 2 - 162;
	dest.w = 325;
	dest.h = 37;
	dest.y = 200 - 10;
	SDL_RenderFillRect(g->renderer, &dest);
	affiche_texte(g, "> Editeur de niveaux <", font, 24, FenetreL / 2, 200);
	dest.x = FenetreL / 2 - 100;
	dest.w = 200;
	dest.h = 37;
	dest.y = 250 - 10;
	SDL_RenderFillRect(g->renderer, &dest);
	affiche_texte(g, "> Commandes <", font, 24, FenetreL / 2, 250);


	SDL_RenderPresent(g->renderer);
}

void affiche_selection_musee(Globals* g)
{
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderClear(g->renderer);
	char fontTitle[] = "data\\OpenSans-Regular.ttf";//25
	char fontText[]  = "data\\OpenSans-Regular.ttf";//14

	affiche_texte(g, "Choisir un musee:", fontTitle, 25, FenetreL / 2, 25);

	for (int i = 0; i <= g->NB_MUSEES; i++) {
		//max musee affiches = 10
		if (i >= 10)
			break;
		if (i == g->NB_MUSEES) {
			affiche_texte(g, "Retour", fontText, 14, FenetreL / 2, 70 + i * 30);
		}
		else {
			Musee& musee = *g->musees[i];
			if (i == g->musee_courant) {
				char* nom = new char[strlen(musee.nom) + 2];
				strcpy(nom, musee.nom);
				nom[strlen(nom) + 1] = '\0';
				nom[strlen(nom)] = '<';
				affiche_texte(g, nom, fontText, 14, FenetreL / 2, 70 + i * 30, { 255,0,0,255 });

			}
			else {
				affiche_texte(g, musee.nom, fontText, 14, FenetreL / 2, 70 + i * 30);
				//affiche bouton supprimer
				affiche_texte(g, "x", fontText, 16, FenetreL / 2 + 75, 70 + i * 30, { 255,0,0,255 });
				//affiche bouton editer
				affiche_texte(g, "Editer", fontText, 16, FenetreL / 2 + 125, 70 + i * 30, { 255,0,0,255 });
			}
		}
		
	}

	SDL_RenderPresent(g->renderer);
}

void affiche_tableau(Globals* g, Oeuvre* oeuvre) {
	efface_tableau(g);
	SDL_Rect rect;
	SDL_QueryTexture(oeuvre->texture, NULL, NULL, &rect.w, &rect.h);
	rect.w = (int) (rect.w /1.4);
	rect.h = (int)(rect.h / 1.4);
	rect.x = NbBlocsL*TailleCaseL + (FenetreL - (NbBlocsL*TailleCaseL)) / 2 - rect.w/2;
	rect.y = 0;
	SDL_RenderCopy(g->renderer, oeuvre->texture, NULL, &rect);

	rect.x = NbBlocsL*TailleCaseL + (FenetreL - (NbBlocsL*TailleCaseL)) / 2;
	rect.y += rect.h + 10;
	affiche_texte(g, oeuvre->nom, "data\\Lucida.ttf", 20, rect.x, rect.y);

	/* Affichage de la description */
	rect.y += 30;

	int posLigne = 0;
	char ligne[2048];//on estime chaque ligne à 2048 caractères max
	int taille = strlen(oeuvre->descriptif);
	for (int i = 0; i < taille; i++) {
		const char& c = oeuvre->descriptif[i];
		if (c != '\n' && posLigne <= 45)
			ligne[posLigne++] = c;
		else {
			ligne[posLigne] = '\0';
			posLigne = 0;
			affiche_texte(g, ligne, "data\\calibri.ttf", 12, rect.x, rect.y, { 255,255,255,255 });
			rect.y += 15;
		}
	}
	SDL_RenderPresent(g->renderer);
}

void efface_tableau(Globals* g)
{
	SDL_Rect rect;
	rect.x = NbBlocsL* TailleCaseL;
	rect.y = 0;
	rect.w = FenetreL - rect.x;
	rect.y = FenetreH;
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(g->renderer, &rect);
	SDL_RenderPresent(g->renderer);
}

void affiche_texte(Globals* g, char* texte, char* font, int taille, int x, int y, SDL_Color color)
{
	TTF_Font* font_ = TTF_OpenFont(font, taille);
	SDL_Texture* texture = loadText(g->renderer, texte, color, font_);
	int strw = (int)strlen(texte)*(int)(taille / 1.5);
	SDL_Rect source = { 0, 0, strw,(int)( taille*1.5) };
	SDL_Rect dest = { x - strw/2, y - taille/2, source.w, source.h };
	SDL_RenderCopy(g->renderer, texture, &source, &dest);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font_);
}

void affiche_texte_erase(Globals* g, char* texte, char* font, int taille, int x, int y, SDL_Color color)
{
	TTF_Font* font_ = TTF_OpenFont(font, taille);
	SDL_Texture* texture = loadText(g->renderer, texte, color, font_);
	int strw = (int)strlen(texte)*(int)(taille / 1.5);
	SDL_Rect source = { 0, 0, (int)strlen(texte)*(int)(taille / 1.5), (int)(taille*1.5) };
	SDL_Rect dest = { x - strw / 2, y - taille / 2, source.w, source.h };
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(g->renderer, &dest);
	SDL_RenderCopy(g->renderer, texture, &source, &dest);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font_);
}


void affiche_texte(Globals* g, char* texte, char* font, int taille, int x, int y)
{
	affiche_texte(g, texte, font, taille, x, y, { 255,255,255,255 });
}

void affiche_texte_erase(Globals* g, char* texte, char* font, int taille, int x, int y)
{
	affiche_texte_erase(g, texte, font, taille, x, y, { 255,255,255,255 });
}

void affiche_editeur(Globals* g)
{
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	SDL_RenderClear(g->renderer);

	for (int i = 0; i < NbBlocsH; i++) {
		for (int j = 0; j < NbBlocsL; j++) {
			int contenuCase = g->museeEditeur->cases[i][j];
			if (contenuCase == 0)
				SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
			else if (contenuCase == 2)
				SDL_SetRenderDrawColor(g->renderer, 0, 255, 0, 255);
			else if (contenuCase > 4) {
				SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);
			}
			else if (contenuCase == 1)
				SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);

			SDL_Rect rect;
			rect.x = j*TailleCaseL;
			rect.y = i*TailleCaseH;
			rect.w = TailleCaseL;
			rect.h = TailleCaseH;
			SDL_RenderFillRect(g->renderer, &rect);
			SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(g->renderer, &rect);
		}
	}

	//affiche le perso
	Personnage& perso = *g->museeEditeur->voleur;
	filledCircleRGBA(g->renderer, (int)(perso.position.x * TailleCaseL), (int)(perso.position.y * TailleCaseH), 5, 255, 0, 0, 255);
	Vecteur point2;
	point2.x = (perso.position.x + perso.longueur_trait * cos(perso.angle));
	point2.y = (perso.position.y + perso.longueur_trait * sin(perso.angle));

	SDL_RenderDrawLine(g->renderer, (int)(perso.position.x * TailleCaseL), (int)(perso.position.y * TailleCaseH), (int)(point2.x * TailleCaseL), (int)(point2.y * TailleCaseH));
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_Rect rect = { 0,0,TailleCaseL*NbBlocsL,TailleCaseH*NbBlocsH };
	SDL_RenderDrawRect(g->renderer, &rect);

	//affiche gardiens
	Personnage**& gardiens = g->museeEditeur->gardiens;

	for (int i = 0; i < g->museeEditeur->NB_GARDIENS; i++)
	{
		Personnage& perso = *g->museeEditeur->gardiens[i];
		filledCircleRGBA(g->renderer, (int)perso.position.x * TailleCaseL, (int)perso.position.y * TailleCaseH, 5, 255, 255, 0, 255);
	}
	
	//outils
	int x = TailleCaseL * NbBlocsL + 150;
	int y = 25;
	rect = { x, y, 50, 50 };

	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(g->renderer, &rect);
	if (g->outilActuel == VIDE) {
		SDL_SetRenderDrawColor(g->renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(g->renderer, &rect);
	}

	rect.x += 75;
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(g->renderer, &rect);
	if (g->outilActuel == MUR) {
		SDL_SetRenderDrawColor(g->renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(g->renderer, &rect);
	}

	rect.x += 75;
	SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(g->renderer, &rect);
	if (g->outilActuel == VOLEUR) {
		SDL_SetRenderDrawColor(g->renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(g->renderer, &rect);
	}

	rect.y += 65;
	SDL_SetRenderDrawColor(g->renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(g->renderer, &rect);
	if (g->outilActuel == GARDIEN) {
		SDL_SetRenderDrawColor(g->renderer, 255, 255, 0, 255);
		SDL_RenderDrawRect(g->renderer, &rect);
	}

	rect.x -= 75;
	SDL_SetRenderDrawColor(g->renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(g->renderer, &rect);
	if (g->outilActuel == SORTIE) {
		SDL_SetRenderDrawColor(g->renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(g->renderer, &rect);
	}

	rect.x -= 75;
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(g->renderer, &rect);
	if (g->outilActuel == OEUVRE) {
		SDL_SetRenderDrawColor(g->renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(g->renderer, &rect);
	}


	//oeuvres
	x = TailleCaseL * NbBlocsL + 150;
	y = 25 + 50 + 5 + 50 + 20;
	rect.w = 50;
	rect.h = 100;
	SDL_Rect source = { 0,0,50,100 };
	for (int i = 0; i < g->NB_OEUVRES; i++)
	{
		int x_ = i % 3;
		
		int y_ = i / 3;

		SDL_QueryTexture(g->oeuvres[i]->texture, NULL, NULL, &source.w, &source.h);
		rect.x = x + (x_*(rect.w));
		rect.y = y + (y_*(rect.h));
		SDL_RenderCopy(g->renderer, g->oeuvres[i]->texture, &source, &rect);
		if (g->oeuvreActuelle->id == g->oeuvres[i]->id) {
			SDL_SetRenderDrawColor(g->renderer, 255,0,255,255);
			SDL_RenderDrawRect(g->renderer, &rect);
		}
	}

	//bouton sauvegarder
	rect.w = 200;
	rect.h = 50;
	rect.x = 20;
	rect.y = NbBlocsH * TailleCaseH + 15;
	char* font = "data\\OpenSans-Regular.ttf";
	affiche_texte(g, "Sauvegarder", font, 14, rect.x + 50, rect.y);
	SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);
	rect.y -= 5;
	rect.w = rect.w / 2 + 10;
	rect.x -= 5;
	rect.h = 20;
	SDL_RenderDrawRect(g->renderer, &rect);
	SDL_RenderPresent(g->renderer);

	//bouton quitter
	rect.w = 200;
	rect.h = 50;
	rect.x = 150;
	rect.y = NbBlocsH * TailleCaseH + 15;
	affiche_texte(g, "Quitter", font, 14, rect.x + 50, rect.y);
	SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);
	rect.y -= 5;
	rect.w = rect.w / 2 + 10;
	rect.x -= 5;
	rect.h = 20;
	SDL_RenderDrawRect(g->renderer, &rect);
	SDL_RenderPresent(g->renderer);
}

void affiche_commandes(Globals* g)
{
	SDL_SetRenderDrawColor(g->renderer, 127, 127, 127, 255);
	SDL_RenderClear(g->renderer);

	//bouton flèches directionnelles
	SDL_Rect src = { 0, 0, 337, 227 };
	SDL_Rect dest = src;
	dest.w = 200;
	dest.h = 150;
	dest.x = 50;
	dest.y = 120;
	char* police = "data\\OpenSans-Regular.ttf";
	affiche_texte(g, "Se deplacer", police, 24, dest.x + dest.w / 2, dest.y - 25);
	char* chemin = "data\\fleches.bmp";
	SDL_Texture* texture = loadImage(g->renderer, chemin, 200, 255, 255, 255);

	affiche_texte(g, "Voler une oeuvre", police, 24, dest.x + 400, dest.y- 25);
	affiche_texte(g, "Touche espace", police, 14, dest.x + 400, dest.y + 25);

	SDL_RenderCopy(g->renderer, texture, &src, &dest);
	SDL_DestroyTexture(texture);


	affiche_texte(g, "Outils edition", police, 24, dest.x + 800, dest.y - 25);
	affiche_texte(g, "Blanc: vide", police, 14, dest.x + 800, dest.y + 25, { 255,255,255,255 });
	affiche_texte(g, "Noir: mur", police, 14, dest.x + 800, dest.y + 50, { 0,0,0,255 });
	affiche_texte(g, "Vert: sortie", police, 14, dest.x + 800, dest.y + 75, { 0,255,0,255 });
	affiche_texte(g, "Bleu: oeuvre", police, 14, dest.x + 800, dest.y + 100, { 0,0,255,255 });
	affiche_texte(g, "Rouge: voleur", police, 14, dest.x + 800, dest.y + 125, { 255,0,0,255 });
	affiche_texte(g, "Jaune: gardien", police, 14, dest.x + 800, dest.y + 150, { 255,255,0,255 });


	//bouton quitter
	SDL_Rect rect;
	rect.w = 200;
	rect.h = 50;
	rect.x = 50;
	rect.y = NbBlocsH * TailleCaseH + 15;
	affiche_texte(g, "Quitter", police, 14, 100, rect.y);
	SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
	rect.y -= 5;
	rect.w = rect.w / 2 + 10;
	rect.x -= 5;
	rect.h = 20;
	SDL_RenderDrawRect(g->renderer, &rect);
	SDL_RenderPresent(g->renderer);


}