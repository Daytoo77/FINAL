// jeu.h
// Auteur : selim Ben Tkhayat
// Version : 5.0
#include "jeu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include "tools.h"
#include "mobile.h"
#include "chaine.h"
#include "message.h"
using namespace std;
using namespace tools;
Jeu::Jeu() : score(0), chaine(S2d{}, r_capture),
arene({0.0, 0.0}, r_max), mode() {
}

/**
 * Constructeur avec paramètres complets
 *  score Score initial
 *  particules Liste des particules initiales
 *  faiseurs Liste des faiseurs initiaux
 *  chaine Chaîne initiale
 *  arene Arène de jeu
 *  mode Mode initial du jeu
 */
Jeu::Jeu(unsigned int score, vector<mobile::Particule> particules,
    vector<mobile::Faiseur> faiseurs,Chaine chaine, Cercle arene, Mode mode)
    : score(score), particules(particules), faiseurs(faiseurs),
    chaine(chaine),arene(arene), mode(CONSTRUCTION) {}


void Jeu::set_score(unsigned int newScore) {
    score = newScore;
}

/**
 * Réinitialise complètement l'état du jeu
 * Remet le score à zéro et supprime tous les objets
 */
void Jeu::reset() {
    score = 0;
    particules.clear();
    faiseurs.clear();
    chaine.clear_articulations();
}

/**
 * Lit et charge la configuration du jeu depuis un fichier
 * Analyse section par section: score, particules, faiseurs, articulations, mode
 * param nomFichier Chemin vers le fichier de configuration
 * return true si la lecture a réussi, false sinon
 */
bool Jeu::readFile(const string& nomFichier) {
    ifstream file(nomFichier);
    reset();
    string line;
    enum Section {
        SCORE, NB_PARTICULES, PARTICULES, NB_FAISEURS, FAISEURS, NB_ARTICULATIONS,
        ARTICULATIONS, MODE
    };
    Section section = SCORE;
    int nbPart = 0, nbFais = 0, nbArt = 0;
    auto passerAuxDonnees = [&](int nombre, Section actuelle, Section suivante,
                                Section saut) {
        section = (nombre == 0) ? saut : suivante;
    };

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#' || line.find_first_not_of(" \t\r\n") ==
            string::npos) continue;
        istringstream iss(line);
        iss >> ws;
        switch (section) {
            case SCORE:
                if (!decodage_score(iss)) {
                    cout << message::score_outside(score);
                    exit(EXIT_FAILURE);
                }
                section = NB_PARTICULES;
                break;

            case NB_PARTICULES:
                if (!(iss >> nbPart) || static_cast<size_t>(nbPart) > nb_particule_max
                    || nbPart < 0) {
                    cout << message::nb_particule_outside(nbPart);
                    exit(EXIT_FAILURE);
                }
                passerAuxDonnees(nbPart, NB_PARTICULES, PARTICULES, NB_FAISEURS);
                break;

            case PARTICULES:
                decodage_particule(iss);
                if (particules.size() >= static_cast<size_t>(nbPart))
                    section = NB_FAISEURS;
                break;

            case NB_FAISEURS:
                if (!(iss >> nbFais)) {
                    cout << message::faiseur_nbe(nbFais) << endl;
                    exit(EXIT_FAILURE);
                }
                if (nbFais < 0) exit(EXIT_FAILURE);
                passerAuxDonnees(nbFais, NB_FAISEURS, FAISEURS, NB_ARTICULATIONS);
                break;

            case FAISEURS:
                decodage_faiseur(iss);
                if (faiseurs.size() >= static_cast<size_t>(nbFais))
                    section = NB_ARTICULATIONS;
                break;

            case NB_ARTICULATIONS:
                if (!(iss >> nbArt) || nbArt < 0) exit(EXIT_FAILURE);
                passerAuxDonnees(nbArt, NB_ARTICULATIONS, ARTICULATIONS, MODE);
                break;

            case ARTICULATIONS:
                decodage_chaine(iss);
                if (chaine.getArticulations().size() >= static_cast<size_t>(nbArt))
                    section = MODE;
                break;

            case MODE:
                string modeStr;
                if (iss >> modeStr) {
                    mode = (modeStr == "CONSTRUCTION") ? CONSTRUCTION :
                           (modeStr == "GUIDAGE") ? GUIDAGE : mode;
                }
                break;
        }
    }
    cout << message::success() << endl;
    file.close();
    return true;
}

/**
 * Vérifie et charge le score depuis le flux d'entrée
 *  data Flux contenant le score
 * @return true si le score est valide (entre 1 et SCORE_MAX)
 */
bool Jeu::decodage_score(istringstream& data) {
    return (data >> score) && score > 0 && score <= score_max;
}

/**
 * Lit et crée une particule à partir du flux d'entrée
 * Vérifie la validité des paramètres (position dans l'arène, vitesse dans les limites)
 *  data Flux contenant les données de la particule
 */
void Jeu::decodage_particule(istringstream& data) {
    double x, y, angle, distance;
    unsigned compteur;
    (data >> x >> y >> angle >> distance >> compteur);

    renormalisation(angle);
    S2d pos{x, y};
    Polar vit{angle, distance};

    if (!arene.point_appartient_cercle(pos)) {
        cout << message::particule_outside(x, y) << endl;
        exit(EXIT_FAILURE);
    }

    if (distance < 0 || distance > d_max) {
        cout << message::mobile_displacement(distance) << endl;
        exit(EXIT_FAILURE);
    }

    if (compteur < 0 || compteur >= time_to_split) {
        cout << message::particule_counter(compteur) << endl;
        exit(EXIT_FAILURE);
    }

    particules.emplace_back(pos, vit, compteur);
}

/**
 * Lit et crée un faiseur à partir du flux d'entrée
 * Vérifie les contraintes de taille, rayon et position
 * Initialise le corps du faiseur et vérifie les collisions
 *  data Flux contenant les données du faiseur
 */
void Jeu::decodage_faiseur(istringstream& data) {
    double x, y, a, d, r;  // Coordonnées, angle, distance, rayon
    int taille;            // Taille du faiseur

    (data >> x >> y >> a >> d >> r >> taille);
    // Vérification que la taille est valide
    if (taille <= 0) {
        cout << message::faiseur_nbe(taille) << endl;
        exit(EXIT_FAILURE);
    }

    if (d < 0 || d > d_max) {
        cout << message::mobile_displacement(d) << endl;
        exit(EXIT_FAILURE);
    }

    if (r < r_min_faiseur || r > r_max_faiseur) {
        cout << message::faiseur_radius(r) << endl;
        exit(EXIT_FAILURE);
    }

    faiseurs.emplace_back(S2d{x, y}, Polar{d, a}, r, taille);
    if (!arene.cercle_appartient_cercle({{x, y}, r})) {
        cout << message::faiseur_outside(x, y) << endl;
        exit(EXIT_FAILURE);
    }
    faiseurs.back().initialisation_corps();
    verifier_collisions_faiseurs();
    // Vérification que chaque élément du corps appartient à l’arène
    const auto& corps2 = faiseurs.back().get_corps();
    for (const auto& corps_individuel : corps2) {
        const auto& centre_corp = corps_individuel.get_centre();
        if (!arene.cercle_appartient_cercle(corps_individuel)) {
            cout << message::faiseur_outside(centre_corp.x, centre_corp.y) << endl;
            exit(EXIT_FAILURE);
        }
    }
}

void Jeu::decodage_chaine(istringstream& data) {
    double x, y;
    ((data >> x >> y));
    S2d pos{x, y};
    auto& arts = chaine.getArticulations();
    if (!arene.point_appartient_cercle(pos)) {
        cout << message::articulation_outside(x, y) << endl;
        exit(EXIT_FAILURE);
    }
    if (arts.empty()) { // Vérifie si c'est la première articulation (racine)
        double dist_au_centre = dist_deux_pts(pos, arene.get_centre());
        if (dist_au_centre < r_max - r_capture || dist_au_centre > r_max) {
            cout << message::chaine_racine(pos.x, pos.y) << endl;
            exit(EXIT_FAILURE);
        }
        chaine.ajouterArticulation(pos);
    } else {
        // Vérifier la distance avec l'articulation précédente
        if ((dist_deux_pts(arts.back(), pos) > r_capture)) {
            cout <<
                message::chaine_max_distance(static_cast<unsigned>(arts.size() - 1));
            exit(EXIT_FAILURE);
        }
        if (!arene.point_appartient_cercle(pos)) {
            cout << message::articulation_outside(x, y) << endl;
            exit(EXIT_FAILURE);
        }
        chaine.ajouterArticulation(pos);
    }
    // Vérifier les collisions avec les faiseurs
    for (size_t i = 0; i < faiseurs.size(); ++i) {
        for (size_t j = 0; j < faiseurs[i].get_corps().size(); ++j) {
            if (faiseurs[i].get_corps()[j].point_appartient_cercle(pos)) {
                cout <<
                message::chaine_articulation_collision(arts.size() - 1, i, j)
                << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}

void Jeu::verifier_collisions_faiseurs() const {
    for (size_t i = 0; i < faiseurs.size(); ++i) {
        for (size_t j = i + 1; j < faiseurs.size(); ++j) {
            // Parcourir tous les éléments des deux faiseurs
            const auto& corps1 = faiseurs[i].get_corps();
            const auto& corps2 = faiseurs[j].get_corps();

            for (size_t k = 0; k < corps1.size(); ++k) {
                for (size_t l = 0; l < corps2.size(); ++l) {
                    double distance = dist_deux_pts(
                        corps1[k].get_centre(), corps2[l].get_centre());
                    double somme_rayons=corps1[k].get_rayon() + corps2[l].get_rayon();

                    if (distance <= somme_rayons) {
                        cout <<message::faiseur_element_collision(i, k, j, l) <<endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }
}