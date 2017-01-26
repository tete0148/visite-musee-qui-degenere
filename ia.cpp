#include "ia.h"
#include <vector>
#include "vecteur.h"
#include <ctime>
#include "vue.h"
#include <iostream>

using namespace std;

void loop(Globals* g)
{
	Personnage**& gardiens = g->musees[g->musee_courant]->gardiens;

	for (int i = 0; i < g->musees[g->musee_courant]->NB_GARDIENS; i++)
	{
		Personnage* gardien = gardiens[i];
		Personnage& voleur = *g->musees[g->musee_courant]->voleur;

		long currtime = SDL_GetTicks();
		if (currtime - gardien->dernier_deplacement >= 400) {

			int xDir;
			if (voleur.position.x - gardien->position.x > 0)
				xDir = 1;
			else if (voleur.position.x - gardien->position.x < 0)
				xDir = -1;
			else
				xDir = 0;
			int yDir;
			if (voleur.position.y - gardien->position.y > 0)
				yDir = 1;
			else if (voleur.position.y - gardien->position.y < 0)
				yDir = -1;
			else
				yDir = 0;
		
			gardien->position.y += yDir;
			gardien->position.x += xDir;

			while (g->musees[g->musee_courant]->cases[(int)gardien->position.y][(int)gardien->position.x] != 0) {
				xDir = -xDir;
				yDir = -yDir;
				gardien->position.x += xDir;
				gardien->position.y += yDir;
			}

			//detecte joueur
			for (int i = gardien->position.y - 1; i <= gardien->position.y - 1; i++)
				for (int j = gardien->position.x - 1; j <= gardien->position.x - 1; j++) {
					int x_ = voleur.position.x;
					int y_ = voleur.position.y;

					if (x_ - 1 == j || x_ + 1 == j || x_ == j)
						if (y_ - 1 == i || y_ + 1 == i || y_ == i)
							voleur.attrape = true;
				}

			affiche(g);
			gardien->dernier_deplacement = SDL_GetTicks();

		}
	}
	
}