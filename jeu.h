// jeu.h
// Auteur : selim Ben Tkhayat
// Version : 5.0
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

	Jeu(unsigned int score, std::vector<mobile::Particule> particules,
		std::vector<mobile::Faiseur> faiseurs, Chaine chaine, tools::Cercle arene,
		Mode mode);
	

	// Lecture des données à partir d'un fichier
	bool readFile(const std::string& nomFichier);

	// Décodage par lignes
	bool decodage_score(std::istringstream& data);
	void decodage_particule(std::istringstream& data);
	void decodage_faiseur(std::istringstream& data);
	void decodage_chaine(std::istringstream& data);
	void reset();
	void set_score(unsigned int score);
	void verifier_collisions_faiseurs() const;
	void ajouterFaiseur(const mobile::Faiseur& faiseur);
private:
	// Attributs principaux du jeu
	unsigned int score;
	std::vector<mobile::Particule> particules;
	std::vector<mobile::Faiseur> faiseurs;
	Chaine chaine;
	tools::Cercle arene;
	Mode mode;

};

#endif // JEU_H

