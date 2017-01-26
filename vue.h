#pragma once
#include"main.h"
#include "SDL2_gfxPrimitives.h"

void affiche_musee(Globals* g);
void affiche_voleur(Globals* g);
void affiche_partie_musee(Globals*g, int xDebut, int xFin, int yDebut, int yFin);
void affiche(Globals* g);
void affiche_menu(Globals* g);
void affiche_selection_musee(Globals* g);
void affiche_tableau(Globals* g, Oeuvre* oeuvre);
void efface_tableau(Globals* g);
void affiche_gardiens(Globals* g);
void affiche_texte(Globals* g, char* texte, char* font, int taille, int x, int y, SDL_Color color);
void affiche_texte(Globals* g, char* texte, char* font, int taille, int x, int y);
void affiche_texte_erase(Globals* g, char* texte, char* font, int taille, int x, int y, SDL_Color color);
void affiche_texte_erase(Globals* g, char* texte, char* font, int taille, int x, int y);
void affiche_editeur(Globals* g);
void affiche_gagne(Globals* g);
void affiche_perdu(Globals* g);
void affiche_commandes(Globals* g);