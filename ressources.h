#pragma once
#include "main.h"
#include "musee.h"
#include "fonctionsBD.h"

void charger_oeuvres(Globals* g);
void decharger_oeuvres(Globals* g);
void charger_musees(Globals* g);
void decharger_musees(Globals* g);
void supprimer_musee(Globals* g, int id_musee);