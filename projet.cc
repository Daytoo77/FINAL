#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>  // Pour std::remove
#include <iomanip> // Pour std::setprecision, std::fixed
#include "jeu.h"
#include "constantes.h"
#include "mobile.h"
#include "tools.h"

// Helper function to read file content into a string (optional, for manual checks)
std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Non-critical error for this helper
        return "[Erreur: Impossible de lire le fichier " + filename + "]";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Fonction pour exécuter un test sur un fichier donné
void run_test_on_file(const std::string& input_filename) {
    std::cout << "--- Test du fichier: " << input_filename << " ---" << std::endl;
    Jeu jeu;
    bool lecture_ok = jeu.readFile(input_filename);

    if (lecture_ok) {
        std::cout << "Résultat readFile: SUCCES" << std::endl;

        // Construire le nom du fichier de sortie
        std::string output_filename = "output_" + input_filename;
        std::remove(output_filename.c_str()); // Nettoyer ancien fichier de sortie

        bool sauvegarde_ok = jeu.sauvegarder(output_filename);

        if (sauvegarde_ok) {
            std::cout << "Sauvegarde réussie dans: " << output_filename << std::endl;
            // Optionnel: Afficher le contenu sauvegardé pour vérification rapide
            // std::cout << "Contenu sauvegardé:\n" << readFileContent(output_filename) << std::endl;
        } else {
            std::cout << "ERREUR: La sauvegarde a échoué pour " << output_filename << std::endl;
        }
    } else {
        std::cout << "Résultat readFile: ECHEC (comme attendu pour les fichiers invalides ?)" << std::endl;
        // L'état du jeu doit être vide/réinitialisé par readFile en cas d'échec
    }
    std::cout << "--------------------------------------" << std::endl << std::endl;
}

int main() {
    std::cout << "--- Début des tests de lecture/sauvegarde pour t00.txt à t14.txt ---" << std::endl << std::endl;

    for (int i = 0; i <= 14; ++i) {
        std::stringstream ss;
        ss << "t" << std::setw(2) << std::setfill('0') << i << ".txt";
        std::string filename = ss.str();

        // Vérifier si le fichier d'entrée existe avant de lancer le test
        std::ifstream infile(filename);
        if (infile.good()) {
            infile.close();
            run_test_on_file(filename);
        } else {
            std::cout << "--- Fichier: " << filename << " INTROUVABLE ---" << std::endl << std::endl;
        }
    }

    std::cout << "--- Fin des tests ---" << std::endl;
    return 0;
}