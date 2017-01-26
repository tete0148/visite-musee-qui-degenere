#pragma once
#include "main.h"
#include "personnage.h"

void gere_evenements(Globals* globals);
void deplace_voleur(Globals* g);
void event_vue_musee(Globals* globals, SDL_Event& event);
void event_vue_accueil(Globals* globals, SDL_Event& event);
void event_vue_selection_musee(Globals* globals, SDL_Event& event);
void event_vue_editeur(Globals* globals, SDL_Event& event);
void event_vue_commandes(Globals* g, SDL_Event& event);
Oeuvre* detecter_oeuvre(Globals* g);
Oeuvre* recup_oeuvre_depuis_case_num(Globals* g, int case_);
Oeuvre* recup_oeuvre(Globals* g, int id);