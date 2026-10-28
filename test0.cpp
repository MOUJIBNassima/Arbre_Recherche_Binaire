#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>
using namespace std;
using namespace chrono;

/// Structure d'un noeud de l'arbre
struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droite;

    Noeud(int v) {
        valeur = v;
        gauche = nullptr;
        droite = nullptr;
    }
};

/// 1. Insertion dans l'ABR
Noeud* inserer(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return new Noeud(valeur);
    }

    if (valeur < racine->valeur) {
        racine->gauche = inserer(racine->gauche, valeur);
    } else if (valeur > racine->valeur) {
        racine->droite = inserer(racine->droite, valeur);
    }
    /// Une valeur deja presente n'est pas inseree une deuxieme fois.

    return racine;
}

/// 2. Recherche d'une valeur
bool rechercher(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return false;
    }

    if (racine->valeur == valeur) {
        return true;
    }

    if (valeur < racine->valeur) {
        return rechercher(racine->gauche, valeur);
    }

    return rechercher(racine->droite, valeur);
}

/// Trouver le plus petit noeud d'un sous-arbre
Noeud* minimum(Noeud* racine) {
    while (racine != nullptr && racine->gauche != nullptr) {
        racine = racine->gauche;
    }
    return racine;
}

/// 3. Suppression d'une valeur
Noeud* supprimer(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return nullptr;
    }

    if (valeur < racine->valeur) {
        racine->gauche = supprimer(racine->gauche, valeur);
    } else if (valeur > racine->valeur) {
        racine->droite = supprimer(racine->droite, valeur);
    } else {
        /// Cas 1 : aucun fils gauche
        if (racine->gauche == nullptr) {
            Noeud* temp = racine->droite;
            delete racine;
            return temp;
        }

        /// Cas 2 : aucun fils droit
        if (racine->droite == nullptr) {
            Noeud* temp = racine->gauche;
            delete racine;
            return temp;
        }

        /// Cas 3 : deux fils
        /// Remplacer par le successeur : minimum du sous-arbre droit
        Noeud* temp = minimum(racine->droite);
        racine->valeur = temp->valeur;
        racine->droite = supprimer(racine->droite, temp->valeur);
    }

    return racine;
}

/// 4. Calcul de la hauteur
/// Convention : la hauteur d'une feuille est 0.
/// Un arbre vide a une hauteur de -1.
int hauteur(Noeud* racine) {
    if (racine == nullptr) {
        return -1;
    }

    return 1 + max(hauteur(racine->gauche),
                   hauteur(racine->droite));
}

/// Afficher chaque noeud avec son niveau
void afficherNiveaux(Noeud* racine, int niveau = 1) {
    if (racine == nullptr) {
        return;
    }

    afficherNiveaux(racine->gauche, niveau + 1);

    cout << "Valeur : " << setw(2) << racine->valeur
         << " | Niveau : " << niveau << '\n';

    afficherNiveaux(racine->droite, niveau + 1);
}

/// Affichage de l'arbre dans l'ordre croissant
void afficherInfixe(Noeud* racine) {
    if (racine == nullptr) {
        return;
    }

    afficherInfixe(racine->gauche);
    cout << racine->valeur << " ";
    afficherInfixe(racine->droite);
}

/// Liberer la memoire
void libererArbre(Noeud* racine) {
    if (racine == nullptr) {
        return;
    }

    libererArbre(racine->gauche);
    libererArbre(racine->droite);
    delete racine;
}

/// Mesurer la duree d'une operation
template <typename Fonction>
long long mesurerTemps(Fonction operation) {
    auto debut = high_resolution_clock::now();

    operation();

    auto fin = high_resolution_clock::now();

    return duration_cast<nanoseconds>(fin - debut).count();
}

int main() {
    Noeud* racine = nullptr;

    int valeurs[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int taille = sizeof(valeurs) / sizeof(valeurs[0]);

    cout << "====================================\n";
    cout << "   ARBRE BINAIRE DE RECHERCHE (ABR)\n";
    cout << "====================================\n\n";

    /// Insertion et mesure du temps pour chaque valeur
    cout << "--- INSERTION ---\n";

    for (int i = 0; i < taille; i++) {
        int valeur = valeurs[i];

        long long temps = mesurerTemps([&]() {
            racine = inserer(racine, valeur);
        });

        cout << "Insertion de " << setw(2) << valeur
             << " : " << temps << " ns\n";
    }

    /// Afficher les niveaux
    cout << "\n--- NIVEAU DE CHAQUE NOEUD ---\n";
    afficherNiveaux(racine);

    cout << "\nHauteur de l'arbre : " << hauteur(racine) << '\n';
    cout << "Nombre de niveaux  : " << hauteur(racine) + 1 << '\n';

    /// Affichage trie
    cout << "\n--- PARCOURS INFIXE ---\n";
    afficherInfixe(racine);
    cout << '\n';

    /// Recherche et mesure du temps
    int valeurRecherchee = 7;

    cout << "\n--- RECHERCHE ---\n";

    bool trouve = false;
    long long tempsRecherche = mesurerTemps([&]() {
        trouve = rechercher(racine, valeurRecherchee);
    });

    cout << "Valeur recherchee : " << valeurRecherchee << '\n';
    cout << "Resultat : "
         << (trouve ? "Valeur trouvee" : "Valeur absente") << '\n';
    cout << "Temps de recherche : "
         << tempsRecherche << " ns ("
         << fixed << setprecision(3)
         << tempsRecherche / 1000.0 << " us)\n";

    /// Suppression et mesure du temps
    int valeurSupprimee = 5;

    cout << "\n--- SUPPRESSION ---\n";

    long long tempsSuppression = mesurerTemps([&]() {
        racine = supprimer(racine, valeurSupprimee);
    });

    cout << "Valeur supprimee : " << valeurSupprimee << '\n';
    cout << "Temps de suppression : "
         << tempsSuppression << " ns ("
         << fixed << setprecision(3)
         << tempsSuppression / 1000.0 << " us)\n";

    cout << "\n--- ARBRE APRES SUPPRESSION ---\n";
    afficherInfixe(racine);
    cout << '\n';

    cout << "\nNouvelle hauteur : " << hauteur(racine) << '\n';
    cout << "Nouveaux niveaux : " << hauteur(racine) + 1 << '\n';

    /// Liberation de la memoire
    libererArbre(racine);
    racine = nullptr;

    cout << "\nProgramme termine.\n";

    return 0;
}
