// chaine.cc
// Auteur : selim Ben Khatayat
// Version : 5.0
#include "chaine.h"
#include "tools.h"

#include <cmath>
#include <iostream>
#include <cassert>

#include "gui.h"
using namespace std;
using namespace tools;

Chaine::Chaine(const S2d& racine, double r_capture)
    : racine(racine), r_capture(r_capture) {
    // Ajout de la racine comme première articulation
    articulations.push_back(racine);
}
Chaine::Chaine(const Chaine& other)
    : racine(other.racine), articulations(other.articulations),
      r_capture(other.r_capture) {}

Chaine& Chaine::operator=(Chaine&& other) noexcept {
    if (this != &other) {
        this->racine = move(other.racine);
        this->articulations = move(other.articulations);
        other.articulations.clear();
    }
    return *this;
}

void Chaine::ajouterArticulation(const S2d& articulation) {
    articulations.push_back(articulation);
}


// Récupérer la liste des articulations
const vector<S2d>& Chaine::getArticulations() const {
    return articulations;
}
void Chaine::modifier_articulations(int i,const S2d &target) {
    articulations[i] = target;
}

// Cette méthode reste inchangée car elle est utilisée pour le débogage console
void Chaine::affiche() const {
    cout << "Articulations de la chaîne :" << endl;
    for (size_t i = 0; i < articulations.size(); ++i) {
        cout << " (" << articulations[i].x << ", " << articulations[i].y << ")" ;
    }
    cout << endl;
}
/*
// Ajouter une nouvelle méthode pour le dessin graphique
void Chaine::dessiner(const Cairo::RefPtr<Cairo::Context>& cr) const {
    // Aucune articulation à dessiner
    if (articulations.empty()) return;

    // Dessiner les articulations
    cr->set_source_rgb(0.8, 0.2, 0.2); // Rouge pour les articulations

    for (const auto& articulation : articulations) {
        // Dessiner un cercle pour chaque articulation
        cr->arc(articulation.x, articulation.y, 0.1, 0, 2 * M_PI);
        cr->fill();
    }

    // Dessiner les segments reliant les articulations
    cr->set_source_rgb(0.7, 0.7, 0.7); // Gris pour les segments
    cr->set_line_width(0.05);

    if (articulations.size() > 1) {
        cr->move_to(articulations[0].x, articulations[0].y);
        for (size_t i = 1; i < articulations.size(); ++i) {
            cr->line_to(articulations[i].x, articulations[i].y);
        }
        cr->stroke();
    }

    // Dessiner un cercle spécial pour l'effecteur (dernière articulation)
    if (articulations.size() > 1) {
        cr->set_source_rgb(0.2, 0.8, 0.2); // Vert pour l'effecteur
        cr->arc(articulations.back().x, articulations.back().y, 0.15, 0, 2 * M_PI);
        cr->fill();
    }
}
void Chaine::clear_articulations() {
    articulations.clear();
}

void Chaine::reinitialiser() {
    articulations.clear();
    articulations.push_back(racine);
}

bool Chaine::guider(const S2d& but) {
    // Vérifier qu'il y a au moins une articulation (la racine)
    if (articulations.empty()) return false;

    // Si une seule articulation, on ne peut pas guider
    if (articulations.size() == 1) return false;

    // La cible est le but défini dans le jeu
    // Supposons qu'il y a une variable but accessible
    S2d target = but; // Cette variable doit être accessible, sinon il faut l'ajouter en paramètre

    // Longueur des segments (supposons qu'ils sont tous de même longueur)
    std::vector<double> distances;
    for (size_t i = 0; i < articulations.size() - 1; ++i) {
        distances.push_back(dist_deux_pts(articulations[i], articulations[i+1]));
    }

    // Sauvegarder la position initiale de la racine
    S2d initial_pos = articulations[0];

    // Itération de FABRIK
    // Phase 1: Passe avant - On place l'effecteur sur la cible et on remonte
    articulations.back() = target;

    for (int i = articulations.size() - 2; i >= 0; --i) {
        // Calculer la direction du segment
        S2d dir;
        dir.x = articulations[i+1].x - articulations[i].x;
        dir.y = articulations[i+1].y - articulations[i].y;
        double len = sqrt(dir.x * dir.x + dir.y * dir.y);

        // Normaliser et mettre à la longueur correcte
        dir = {dir.x / len * distances[i], dir.y / len * distances[i]};

        // Repositionner l'articulation
        articulations[i].x = articulations[i+1].x - dir.x;
        articulations[i].y = articulations[i+1].y - dir.y;
    }

    // Phase 2: Passe arrière - On remet la racine à sa place fixe et on descend
    articulations[0] = initial_pos;

    for (size_t i = 0; i < articulations.size() - 1; ++i) {
        // Calculer la direction du segment
        S2d dir = {articulations[i+1].x - articulations[i].x, articulations[i+1].y - articulations[i].y};
        double len = sqrt(dir.x * dir.x + dir.y * dir.y);

        // Normaliser et mettre à la longueur correcte
        dir = {dir.x / len * distances[i], dir.y / len * distances[i]};

        // Repositionner l'articulation suivante
        articulations[i+1].x = articulations[i].x + dir.x;
        articulations[i+1].y = articulations[i].y + dir.y;
    }
    // Vérifier si l'effecteur est assez proche de la cible
    return dist_deux_pts(articulations.back(), target) <= r_capture;
}*/
