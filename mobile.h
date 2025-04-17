// mobile.h
// Auteur : theo brochier
// Version : 5.0
#ifndef MOBILE_H
#define MOBILE_H
#include <vector>
#include "tools.h"
namespace mobile {
	/**
	 * Classe de base pour les objets mobiles du jeu
	 * Contient la position et le vecteur vitesse
	 */
	// Dans la classe Mobile, ajoutez ces méthodes publiques
	class Mobile {
	public:
	  Mobile(tools::S2d p, tools::Polar v);
	  Mobile();

	  // Ajout des getters virtuels
	  virtual tools::S2d getPosition() const { return position; }
	  virtual tools::Polar getVecteurVitesse() const { return vecteurVitesse; }

	protected:
	  tools::S2d position;
	  tools::Polar vecteurVitesse;
	};

	// Dans la classe Particule
	class Particule : protected Mobile {
	public:
	Particule(tools::S2d position, tools::Polar vecteurVitesse, double compteur);
	void update(); // P9ac3
	// Surcharge des getters avec override
	tools::S2d getPosition() const override { return position; }
	tools::Polar getVecteurVitesse() const override { return vecteurVitesse; }
	// Getter spécifique pour compteur
	unsigned getCompteur() const { return compteur; }
	void incrementerCompteur() { compteur++; }
	void resetCompteur() { compteur = 0; }
	void setVecteurVitesse(const tools::Polar& vit) { vecteurVitesse = vit; }
	void deplacer();
	private:
	unsigned compteur;
	};

	// Dans la classe Faiseur
	class Faiseur : protected Mobile {
	public:
	  Faiseur(tools::S2d position_tete, tools::Polar vecteurVitesse,
	         double rayon, int taille);
	  Faiseur();
	  void initialisation_corps();
	  bool collision_element(const Faiseur& autre_faiseur);
	  const std::vector<tools::Cercle>& get_corps() const { return corps; }
	  void update(); // P2e09

	  // Surcharge des getters avec override
	  tools::S2d getPosition() const override { return position; }
	  tools::Polar getVecteurVitesse() const override { return vecteurVitesse; }
	  // Getters spécifiques
	  double getRayon() const { return rayon; }
	  int getTaille() const { return taille; }

		tools::S2d calculer_nouvelle_position() const {
		    return nextDestination(position, vecteurVitesse);
		}
	static bool collision_tete_elements(const tools::S2d& position_tete, double rayon_tete, const Faiseur& autre_faiseur);
	private:
	  std::vector<tools::Cercle> corps;
	  double rayon;
	  int taille;
	};
}

#endif //MOBILE_H
