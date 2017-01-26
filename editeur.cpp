#include "editeur.h"
#include <fstream>
#include "vue.h"
#include <iostream>
#include <sstream>
#include "fonctionsBD.h"
#include  "ressources.h"

using namespace std;

void charger_editeur(Globals* g)
{
	g->outilActuel = VIDE;
	g->oeuvreActuelle = g->oeuvres[0];
	ifstream fichier("data\\musee_template.txt", ios::in);
	char text[NbBlocsL + 1];
	fichier.getline(text, NbBlocsL + 1); //on ignore la première ligne (commentaire)
	g->oeuvreActuelle = g->oeuvres[0];
	if (g->museeEditeur == nullptr)
		g->museeEditeur = new Musee;
	g->museeEditeur->id = -1;
	g->museeEditeur->nom = new char[256];
	strcpy(g->museeEditeur->nom, "template");
	fichier >> text; // V
	fichier >> text; // x voleur
	g->museeEditeur->voleur = new Personnage;
	g->museeEditeur->voleur->position.x = atof(text);
	fichier >> text; // y voleur
	g->museeEditeur->voleur->position.y = atof(text);
	fichier.getline(text, NbBlocsL + 1); // on ignore le retour à la ligne
	fichier.getline(text, NbBlocsL + 1); // on ignore le nombre de gardiens
	fichier >> text; // g;
	g->museeEditeur->gardiens = new Personnage*[1];
	g->museeEditeur->gardiens[0] = new Personnage;
	fichier >> text; // x gardien
	g->museeEditeur->gardiens[0]->position.x = atof(text);
	fichier >> text; // y gardien
	g->museeEditeur->gardiens[0]->position.y = atof(text);

	g->museeEditeur->NB_GARDIENS = 1;
	g->museeEditeur->NB_OEUVRES = 1;
	g->museeEditeur->numerosOeuvres = new int[g->NB_OEUVRES];
	g->museeEditeur->numerosOeuvres[0] = 5;
	g->museeEditeur->idOeuvres = new int[g->NB_OEUVRES];
	g->museeEditeur->idOeuvres[0] = 1;

	for (int i = 0; i < NbBlocsH; i++)
	{
		for (int j = 0; j < NbBlocsL; j++)
		{
			fichier >> text;
			g->museeEditeur->cases[i][j] = atoi(text);
		}
	}

	fichier.close();
}

void decharger_editeur(Globals* g)
{
	if (g->museeEditeur == nullptr)
		return;
	for (int i = 0; i < g->museeEditeur->NB_GARDIENS; i++)
	{
		delete g->museeEditeur->gardiens[i];
	}
	delete[] g->museeEditeur->nom;
	delete[] g->museeEditeur->voleur;
	delete[] g->museeEditeur->gardiens;
	delete[] g->museeEditeur->numerosOeuvres;
}

void editeur_clic(Globals* g, int x, int y)
{
	static bool a_sortie = true;
	static bool a_voleur = true;
	if (x > 0 && x < NbBlocsL * TailleCaseL && y <= NbBlocsH * TailleCaseH) {
		int caseX = y / TailleCaseH;
		int caseY = x / TailleCaseL;
		if (g->museeEditeur->cases[caseX][caseY] == 2 && g->outilActuel != SORTIE)
			a_sortie = false;
		switch (g->outilActuel) {
		case MUR:
			g->museeEditeur->cases[caseX][caseY] = 1;
			break;
		case VIDE:
			g->museeEditeur->cases[caseX][caseY] = 0;
			break;
		case SORTIE:
			if (!a_sortie) {
				g->museeEditeur->cases[caseX][caseY] = 2;
				a_sortie = true;
			}
			break;
		case OEUVRE:
			g->museeEditeur->cases[caseX][caseY] = 5 + g->oeuvreActuelle->id - 1;
			g->museeEditeur->idOeuvres[g->museeEditeur->NB_OEUVRES] = g->oeuvreActuelle->id;
			g->museeEditeur->numerosOeuvres[g->museeEditeur->NB_OEUVRES++] = 5 + g->oeuvreActuelle->id - 1;
			break;
		case VOLEUR:
			g->museeEditeur->voleur->position.x = caseY;
			g->museeEditeur->voleur->position.y = caseX;
			break;
		case GARDIEN:
			Personnage** gardiens = new Personnage*[g->museeEditeur->NB_GARDIENS + 1];
			for (int i = 0; i < g->museeEditeur->NB_GARDIENS; i++) {
				gardiens[i] = g->museeEditeur->gardiens[i];
			}
			delete[] g->museeEditeur->gardiens;
			g->museeEditeur->gardiens = gardiens;
			Personnage* newGardien = new Personnage;
			newGardien->position.x = caseY;
			newGardien->position.x += .5;
			newGardien->position.y = caseX;
			newGardien->position.y += .5;
			g->museeEditeur->gardiens[g->museeEditeur->NB_GARDIENS++] = newGardien;
			break;
		}
	}
	else if (
		x > TailleCaseL * NbBlocsL + 150
		&& y > 25
		&& y < 25 + 50 + 5 + 50
		) {
		int x_ = x - (TailleCaseL * NbBlocsL + 150);
		int y_ = y - 25;
		int outilX = x_ / (50 + 25);
		int outilY = y_ / (50 + 15);
		if (outilX == 0) {
			if (outilY == 0)
				g->outilActuel = VIDE;
			else
				g->outilActuel = OEUVRE;
		}
		else if (outilX == 1) {
			if (outilY == 0)
				g->outilActuel = MUR;
			else
				g->outilActuel = SORTIE;
		}
		else if (outilX == 2) {
			if (outilY == 0)
				g->outilActuel = VOLEUR;
			else
				g->outilActuel = GARDIEN;
		}
	}
	else if (
		x > TailleCaseL * NbBlocsL + 150
		&& y > 25 + 50 + 5 + 50 + 20
		&& y < TailleCaseH * NbBlocsH
		) {
		int x_ = x - (TailleCaseL * NbBlocsL + 150);
		int y_ = y - (25 + 50 + 5 + 50 + 20);
		int outilX = x_ / (50);
		int outilY = y_ / (100);
		if (outilX == 0) {
			if (outilY == 0)
				g->oeuvreActuelle = g->oeuvres[0];
			else
				g->oeuvreActuelle = g->oeuvres[3];
		}
		else if (outilX == 1) {
			if (outilY == 0)
				g->oeuvreActuelle = g->oeuvres[1];
			else
				g->oeuvreActuelle = g->oeuvres[4];
		}
		else if (outilX == 2) {
			if (outilY == 0)
				g->oeuvreActuelle = g->oeuvres[2];
			else
				g->oeuvreActuelle = g->oeuvres[5];
		}
	}
	else if (
		x >= 15 &&
		x <= 15 + 110 &&
		y >= NbBlocsH * TailleCaseH &&
		y <= NbBlocsH * TailleCaseH + 38
		) {
		sauvegarder(g);
		return;
	}
	else if (
		x >= 150 &&
		x <= 150 + 110 &&
		y >= NbBlocsH * TailleCaseH &&
		y <= NbBlocsH * TailleCaseH + 38
		) {
		g->vue_actuelle = ACCUEIL;
		decharger_editeur(g);
		charger_editeur(g);
		affiche(g);
		return;
	}
	affiche_editeur(g);
}

void sauvegarder(Globals* g)
{
	Musee& musee = *g->museeEditeur;
	//todo: verifier si id defini: si oui alors il faut update
	if (musee.id == -1) {//template
		sqlite3* bd = bd_ouvrir("data\\bdMusees.sqlite");

		//on recupere le nb de musees actuels
		char*** resultats;
		int nbLignes(0), nbColonnes(0);
		bd_requeteSelect(bd, "SELECT id+1 FROM musees ORDER BY id DESC LIMIT 1", resultats, nbLignes, nbColonnes);
		if (resultats != NULL)
			musee.id = atoi(resultats[0][0]);
		strcpy(musee.nom, "musee");
		char id[11];
		_itoa(musee.id, id, 10);
		strcat(musee.nom, id);
		char path[256];
		strcpy(path, "data\\");
		strcat(path, musee.nom);
		strcat(path, ".txt");
		bd_detruireTab(resultats, nbLignes, nbColonnes);

		//creation fichier
		ofstream fichier(path, ios::out);
		fichier << "V " << musee.voleur->position.x << ' ' << musee.voleur->position.y << endl;
		fichier << "G " << musee.NB_GARDIENS << endl;
		for (int i = 0; i < musee.NB_GARDIENS; i++)
			fichier << "G " << musee.gardiens[i]->position.x << ' ' << musee.gardiens[i]->position.y << endl;

		for (int i = 0; i < NbBlocsH; i++) {
			for (int j = 0; j < NbBlocsL; j++)
				if (j == 0)
					fichier << musee.cases[i][j];
				else
					fichier << ' ' << musee.cases[i][j];
			fichier << endl;
		}
		fichier.close();


		//insertion base de données
		ostringstream requete;
		requete.str("");
		requete << "INSERT INTO musees('id','nom','path') VALUES(" << musee.id << ",'" << musee.nom << "','" << path << "');";
		bd_requeteUpdate(bd, requete.str().c_str());

		for (int i = 0; i < musee.NB_OEUVRES; i++) {
			ostringstream req;
			req.str("");
			req << "INSERT INTO association('idMusee','idTableau','numero') VALUES(" 
				<< musee.id << "," 
				<< musee.idOeuvres[i] << "," 
				<< musee.numerosOeuvres[i] << ");";
			bd_requeteUpdate(bd, req.str().c_str());
		}

		
		bd_fermer(bd);
	}
	else {
		sqlite3* bd = bd_ouvrir("data\\bdMusees.sqlite");

		//creation fichier
		ofstream fichier(musee.path, ios::out);
		fichier << "V " << musee.voleur->position.x << ' ' << musee.voleur->position.y << endl;
		fichier << "G " << musee.NB_GARDIENS << endl;
		for (int i = 0; i < musee.NB_GARDIENS; i++)
			fichier << "G " << musee.gardiens[i]->position.x << ' ' << musee.gardiens[i]->position.y << endl;

		for (int i = 0; i < NbBlocsH; i++) {
			for (int j = 0; j < NbBlocsL; j++)
				if (j == 0)
					fichier << musee.cases[i][j];
				else
					fichier << ' ' << musee.cases[i][j];
			fichier << endl;
		}
		fichier.close();

		//associtations
		ostringstream request;
		request.str();
		request << "DELETE FROM association WHERE idMusee = " << musee.id << ";";
		bd_requeteUpdate(bd, request.str().c_str());
		for (int i = 0; i < musee.NB_OEUVRES; i++) {
			ostringstream req;
			req.str("");
			req << "INSERT INTO association('idMusee','idTableau','numero') VALUES(" 
				<< musee.id << "," 
				<< musee.idOeuvres[i] << "," 
				<< musee.numerosOeuvres[i] << ");";
			bd_requeteUpdate(bd, req.str().c_str());
		}


		bd_fermer(bd);

	}
	
	if (strcmp(musee.nom, "template") != 0) {
		g->museeEditeur = nullptr;
	}


	decharger_musees(g);
	charger_musees(g);
	decharger_editeur(g);
	charger_editeur(g);

	g->vue_actuelle = ACCUEIL;
	affiche(g);
}