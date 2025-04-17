// jeu.h
// Auteur : selim Ben Tkhayat
//Version : 5.0
#ifndef JEU_H
#define JEU_H

#include <string>
#include <vector>
#include <sstream>
#include "mobile.h"
#include "chaine.h"
#include "constantes.h"
#include "tools.h"

class Jeu {
public:
	Jeu() ;

	Jeu(unsigned int score, const std::vector<mobile::Particule> &particules,
	const std::vector<mobile::Faiseur> &faiseurs, const Chaine &chaine, const tools::Cercle &arene,
	Mode mode);


	// Lecture des données à partir d'un fichier
	bool readFile(const std::string& nomFichier);

	// Décodage par lignes
	bool decodage_score(std::istringstream& data);
	bool decodage_particule(std::istringstream& data);
	bool decodage_faiseur(std::istringstream& data);
	bool decodage_chaine(std::istringstream& data);
	void reset();
	void set_score(unsigned int score);
	void verifier_collisions_faiseurs() const;

	// Sauvegarde de l'état actuel du jeu dans un fichier
	bool sauvegarder(const std::string& nomFichier);
	bool miseAJour();
	void etape();
private:
	// Attributs principaux du jeu
	unsigned int score;
	std::vector<mobile::Particule> particules;
	std::vector<mobile::Faiseur> faiseurs;
	Chaine chaine;
	tools::S2d but;
	tools::Cercle arene;
	Mode mode;

};

#endif // JEU_H
