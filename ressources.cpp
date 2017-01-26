#include "ressources.h"
#include "vue.h"
#include <fstream>
#include <sstream>

using namespace std;

void charger_oeuvres(Globals* g)
{
	sqlite3* bd = bd_ouvrir("data\\bdMusees.sqlite");
	char*** resultats;
	int nL = 0, nC = 0;
	bd_requeteSelect(bd, "SELECT * FROM tableaux", resultats, nL, nC);

	g->NB_OEUVRES = nL;
	g->oeuvres = new Oeuvre*[nL];

	if (resultats != NULL) {
		for (int i = 0; i < nL; i++)
		{
			g->oeuvres[i] = new Oeuvre;
			g->oeuvres[i]->id = atoi(resultats[i][0]);
			g->oeuvres[i]->nom = new char[256];
			strcpy_s(g->oeuvres[i]->nom, 255, resultats[i][1]);
			g->oeuvres[i]->descriptif = new char[2048];
			strcpy_s(g->oeuvres[i]->descriptif, 2047, resultats[i][2]);
			g->oeuvres[i]->texture = loadImage(g->renderer, resultats[i][3]);
		}
	}

	bd_detruireTab(resultats, nL, nC);

	bd_fermer(bd);

}

void decharger_oeuvres(Globals* g)
{
	for (int i = 0; i < g->NB_OEUVRES; i++)
	{
		delete[] g->oeuvres[i]->nom;
		delete[] g->oeuvres[i]->descriptif;
		SDL_DestroyTexture(g->oeuvres[i]->texture);
		delete g->oeuvres[i];
	}
	delete[] g->oeuvres;
}

void charger_musees(Globals* g)
{
	sqlite3* bd = bd_ouvrir("data\\bdMusees.sqlite");
	char*** resultats;
	int nL = 0, nC = 0;
	bd_requeteSelect(bd, "SELECT musees.*, (SELECT COUNT(idTableau) FROM association WHERE idMusee = musees.id), association.* FROM musees LEFT OUTER JOIN association ON idMusee = musees.id", resultats, nL, nC);

	g->NB_MUSEES = 0;
	g->musees = new Musee*[nL];

	int dernierIdMusee = 0;
	int museeIdx = -1;
	int positionOeuvre = 0;
	if (resultats != NULL) {
		for (int i = 0; i < nL; i++)
		{
			if (dernierIdMusee != atoi(resultats[i][0])) {
				positionOeuvre = 0;
				museeIdx++;
				g->NB_MUSEES++;
				g->musees[museeIdx] = new Musee;
				g->musees[museeIdx]->id = atoi(resultats[i][0]);
				g->musees[museeIdx]->nom = new char[256];
				g->musees[museeIdx]->path = new char[256];
				strcpy_s(g->musees[museeIdx]->nom, 256, resultats[i][1]);
				strcpy_s(g->musees[museeIdx]->path, 256, resultats[i][2]);
				ifstream file(resultats[i][2], ios::in);
				if (!file)
					break;
				g->musees[museeIdx]->voleur = new Personnage;
				g->musees[museeIdx]->voleur->inventaire = new Oeuvre*[g->musees[museeIdx]->voleur->tailleInventaire];
				char texte[10];
				file >> texte; // correspond au V
				file >> texte; // position x joueur
				g->musees[museeIdx]->voleur->position.x = atof(texte);
				g->musees[museeIdx]->voleur->defaultPos.x = atof(texte);
				file >> texte; // position y joueur
				g->musees[museeIdx]->voleur->position.y = atof(texte);
				g->musees[museeIdx]->voleur->defaultPos.y = atof(texte);
				file >> texte; // correspond au G
				file >> texte; // nombre de gardiens
				g->musees[museeIdx]->NB_GARDIENS = atoi(texte);
				g->musees[museeIdx]->gardiens = new Personnage*[atoi(texte)];
				for (int j = 0; j < g->musees[museeIdx]->NB_GARDIENS; j++)
				{
					file >> texte; // correpond au G
					g->musees[museeIdx]->gardiens[j] = new Personnage;
					file >> texte; // pos x du gardien j
					g->musees[museeIdx]->gardiens[j]->position.x = atoi(texte);
					g->musees[museeIdx]->gardiens[j]->defaultPos.x = atoi(texte);
					file >> texte; // pos y du gardien j
					g->musees[museeIdx]->gardiens[j]->position.y = atoi(texte);
					g->musees[museeIdx]->gardiens[j]->defaultPos.y = atoi(texte);
				}

				for (int j = 0; j < NbBlocsH; j++)
				{
					for (int k = 0; k < NbBlocsL; k++)
					{
						file >> texte;
						g->musees[museeIdx]->cases[j][k] = atoi(texte);
					}
				}
				if(file.is_open())
					file.close();

				g->musees[museeIdx]->idOeuvres = new int[atoi(resultats[i][3])]; //nb oeuvres
				g->musees[museeIdx]->numerosOeuvres = new int[atoi(resultats[i][3])]; //nb oeuvres
				g->musees[museeIdx]->NB_OEUVRES = atoi(resultats[i][3]);
				g->musees[museeIdx]->idOeuvres[positionOeuvre] = atoi(resultats[i][5]);
				g->musees[museeIdx]->numerosOeuvres[positionOeuvre++] = atoi(resultats[i][6]);
				g->musees[museeIdx]->oeuvresVolees = new bool[atoi(resultats[i][3])];
				for (int j = 0; j < g->musees[museeIdx]->NB_OEUVRES; j++)
					g->musees[museeIdx]->oeuvresVolees[j] = false;

				dernierIdMusee = atoi(resultats[i][0]);
			}
			else {
				g->musees[museeIdx]->idOeuvres[positionOeuvre] = atoi(resultats[i][5]);
				g->musees[museeIdx]->numerosOeuvres[positionOeuvre++] = atoi(resultats[i][6]);
			}
		 	

		}
	}


	bd_detruireTab(resultats, nL, nC);

	bd_fermer(bd);

}

void decharger_musees(Globals* g)
{
	for (int i = 0; i < g->NB_MUSEES; i++)
	{
		delete[] g->musees[i]->voleur->inventaire;
		delete g->musees[i]->voleur;
		delete[] g->musees[i]->nom;
		delete[] g->musees[i]->path;
		for (int j = 0; j < g->musees[i]->NB_GARDIENS; j++)
		{
			delete g->musees[i]->gardiens[j];
		}
		delete[] g->musees[i]->idOeuvres;
		delete[] g->musees[i]->numerosOeuvres;
		delete[] g->musees[i]->gardiens;
		delete[] g->musees[i]->oeuvresVolees;
		delete g->musees[i];
	}
	delete[] g->musees;
}

void supprimer_musee(Globals* g, int id_musee)
{
	sqlite3* bd = bd_ouvrir("data\\bdMusees.sqlite");
	char*** resultats;
	int nL = 0, nC = 0;
	ostringstream req;
	req.str("");
	req << "SELECT path FROM musees WHERE id =" << g->musees[id_musee]->id << ';';
	bd_requeteSelect(bd, req.str().c_str(), resultats, nL, nC);
	if (resultats != NULL) {
		char* path = resultats[0][0];
		remove(path);//supprime le fichier
		req.str("");

		req << "DELETE FROM association WHERE idMusee = " << g->musees[id_musee]->id << ';'
			<< "DELETE FROM musees WHERE id = " << g->musees[id_musee]->id << ';';
		bd_requeteUpdate(bd, req.str().c_str());

		decharger_musees(g);
		charger_musees(g);
		affiche(g);
	}
	bd_detruireTab(resultats, nC, nL);

	bd_fermer(bd);
}