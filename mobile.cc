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
//-----------------------------------------------------------PARTICULE-------------

Particule::Particule(S2d position, Polar vecteurVitesse, double compteur)
        : Mobile(position, vecteurVitesse),  compteur(compteur) {}

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
Faiseur::Faiseur() : rayon(0.0), taille(0) {}

Faiseur::Faiseur(S2d position,Polar vecteurVitesse, double rayon, int taille)
: Mobile(position,vecteurVitesse), rayon(rayon),taille(taille) {
    if (taille<=0) {
        cout<<message::faiseur_nbe(taille)<<endl;
        exit(EXIT_FAILURE);
        }

    corps.resize(taille);

    for (size_t i(0); i < corps.size(); ++i) {
        if (!Arene_ex.point_appartient_cercle(corps[i].get_centre())) {
            cout<<message::faiseur_outside(corps[i].get_centre().x,
                                           corps[i].get_centre().y)<<endl;
            exit(EXIT_FAILURE);
            }
        }
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
    double angle = vecteurVitesse.theta + M_PI; // sens opposée
    renormalisation(angle);
    for (size_t i = 0; i < corps.size(); ++i) {
        S2d pos = (i == 0) ? position : corps[i-1].get_centre();
        if (i != 0) {
            Polar vector = {vecteurVitesse.r, angle};
            if (!Arene_ex.cercle_appartient_cercle({
                nextDestination(pos, vector), rayon
            })) {
                rebond(pos, vector);
            }
            pos = nextDestination(pos, vector);
        }
        corps[i].change_centre(pos);
        corps[i].change_rayon(rayon);
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


