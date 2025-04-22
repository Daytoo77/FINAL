// mobile.cc
// Auteur : theo brochier
// Version : 5.0

#include "mobile.h"
#include "message.h"
#include "constantes.h"
#include "tools.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
using namespace std;
using namespace mobile;
using namespace tools;
static Cercle Arene_ex({0.0,0.0},r_max );
Mobile::Mobile(S2d p,Polar v):position(p),vecteurVitesse(v){}
Mobile::Mobile():position({0.0,0.0}),vecteurVitesse({0.0,0.0}){}

// Implémentation des getters pour Mobile
const tools::S2d& Mobile::get_position() const {
    return position;
}

const tools::Polar& Mobile::get_vitesse() const {
    return vecteurVitesse;
}

//-----------------------------------------------------------PARTICULE-------------

Particule::Particule(S2d position, Polar vecteurVitesse, double compteur)
        : Mobile(position, vecteurVitesse),  compteur(static_cast<unsigned>(compteur)) {} // Cast explicite

// Implémentation des getters pour Particule
const tools::S2d& Particule::get_position() const {
    return Mobile::get_position(); // Appel au getter de la classe de base
}

const tools::Polar& Particule::get_vitesse() const {
    return Mobile::get_vitesse(); // Appel au getter de la classe de base
}

unsigned Particule::get_compteur() const {
    return compteur;
}


//------------------------------------FAISEUR-------------------------------------
Faiseur::Faiseur() : Mobile(), rayon(0.0), taille(0) {} // Initialiser aussi Mobile

Faiseur::Faiseur(S2d position,Polar vecteurVitesse, double rayon, int taille)
: Mobile(position,vecteurVitesse), rayon(rayon),taille(taille) {
    if (taille<=0) {
        cout<<message::faiseur_nbe(taille)<<endl;
        exit(EXIT_FAILURE);
        }

    corps.resize(taille);
    if ((rayon>r_max_faiseur)or(rayon<r_min_faiseur)) {
        cout<<message::faiseur_radius(rayon)<<endl;
        exit(EXIT_FAILURE);
    }

}

bool Faiseur::collision_element(const Faiseur& autre_faiseur) {
    Cercle c1(this->position, this->rayon);
    Cercle c2(autre_faiseur.position, autre_faiseur.rayon);
    return collisionEntreCercles(c1, c2);
}




void Faiseur::initialisation_corps() {
    if (taille <= 0) return; // Ne rien faire si la taille est invalide

    corps.resize(taille); // S'assurer que le vecteur a la bonne taille
    double angle_oppose = vecteurVitesse.theta + M_PI;
    renormalisation(angle_oppose);

    for (size_t i = 0; i < corps.size(); ++i) {
        S2d current_pos;
        if (i == 0) {
            current_pos = position; // Le premier élément est à la position de la tête
        } else {
            // Calculer la position basée sur l'élément précédent
            S2d prev_pos = corps[i-1].get_centre();
            Polar vector_to_prev = {VecteurVitesse.r, angle_oppose}; 
            S2d next_pos_candidate = nextDestination(prev_pos, vector_to_prev);

            // Vérifier si la position candidate est dans l'arène
            // Note: Cette vérification de rebond simple peut être insuffisante
            // pour garantir que le cercle entier reste dans l'arène.
            if (!Arene_ex.cercle_appartient_cercle({next_pos_candidate, rayon})) {
                 // Un simple rebond pourrait ne pas suffire.
                 // Une logique plus complexe pourrait être nécessaire pour gérer
                 // les contraintes de l'arène correctement pendant l'initialisation.
                 // Pour l'instant, on garde le rebond simple:
                rebond(prev_pos, vector_to_prev); // Modifie vector_to_prev.theta
                next_pos_candidate = nextDestination(prev_pos, vector_to_prev);
                // Re-vérifier après rebond pourrait être utile, mais peut causer des blocages.
            }
             current_pos = next_pos_candidate;
        }

        // Mettre à jour le cercle actuel
        corps[i].change_centre(current_pos);
        corps[i].change_rayon(rayon);

        // Vérifier si l'élément initialisé est hors de l'arène (sécurité)
        if (!Arene_ex.cercle_appartient_cercle(corps[i])) {
             const auto& centre_corp = corps[i].get_centre();
             cout << message::faiseur_outside(centre_corp.x, centre_corp.y) << endl;
             // Gérer l'erreur - idéalement, cela ne devrait pas arriver si la logique est correcte
             // exit(EXIT_FAILURE); // Ou une autre gestion d'erreur
        }
    }
}

// Implémentation des getters pour Faiseur
const tools::S2d& Faiseur::get_position() const {
    return Mobile::get_position(); // Appel au getter de la classe de base
}

const tools::Polar& Faiseur::get_vitesse() const {
    return Mobile::get_vitesse(); // Appel au getter de la classe de base
}

double Faiseur::get_rayon() const {
    return rayon;
}

int Faiseur::get_taille() const {
    return taille;
}




