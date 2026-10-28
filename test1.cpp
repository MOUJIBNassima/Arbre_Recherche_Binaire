#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

/// ================================================
/// 1. Structure d'un noeud de l'arbre
/// ================================================
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

/// ================================================
/// 2. Insertion dans l'ABR
/// ================================================
Noeud* inserer(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return new Noeud(valeur);
    }

    if (valeur < racine->valeur) {
        racine->gauche = inserer(racine->gauche, valeur);
    }
    else if (valeur > racine->valeur) {
        racine->droite = inserer(racine->droite, valeur);
    }

    return racine;
}

/// ================================================
/// 3. Recherche d'une valeur
/// ================================================
Noeud* rechercher(Noeud* racine, int valeur) {
    while (racine != nullptr) {
        if (valeur == racine->valeur) {
            return racine;
        }

        if (valeur < racine->valeur) {
            racine = racine->gauche;
        }
        else {
            racine = racine->droite;
        }
    }

    return nullptr;
}

/// ================================================
/// 4. Recherche du minimum d'un sous-arbre
/// ================================================
Noeud* minimum(Noeud* racine) {
    while (racine != nullptr && racine->gauche != nullptr) {
        racine = racine->gauche;
    }

    return racine;
}

/// ================================================
/// 5. Suppression d'une valeur
/// ================================================
Noeud* supprimer(Noeud* racine, int valeur) {
    if (racine == nullptr) {
        return nullptr;
    }

    if (valeur < racine->valeur) {
        racine->gauche = supprimer(racine->gauche, valeur);
    }
    else if (valeur > racine->valeur) {
        racine->droite = supprimer(racine->droite, valeur);
    }
    else {
        /// Cas 1 : aucun fils
        if (racine->gauche == nullptr &&
            racine->droite == nullptr) {
            delete racine;
            return nullptr;
        }

        /// Cas 2 : un seul fils
        if (racine->gauche == nullptr) {
            Noeud* temporaire = racine->droite;
            delete racine;
            return temporaire;
        }

        if (racine->droite == nullptr) {
            Noeud* temporaire = racine->gauche;
            delete racine;
            return temporaire;
        }

        /// Cas 3 : deux fils
        Noeud* successeur = minimum(racine->droite);

        racine->valeur = successeur->valeur;

        racine->droite = supprimer(
            racine->droite,
            successeur->valeur
        );
    }

    return racine;
}

/// ================================================
/// 6. Calcul de la hauteur
/// Une feuille a une hauteur de 0.
/// Un arbre vide a une hauteur de -1.
/// ================================================
int hauteur(Noeud* racine) {
    if (racine == nullptr) {
        return -1;
    }

    int hg = hauteur(racine->gauche);
    int hd = hauteur(racine->droite);

    return 1 + max(hg, hd);
}

/// ================================================
/// 7. Comptage des noeuds
/// ================================================
long long compterNoeuds(Noeud* racine) {
    if (racine == nullptr) {
        return 0;
    }

    return 1
         + compterNoeuds(racine->gauche)
         + compterNoeuds(racine->droite);
}

/// ================================================
/// 8. Affichage limité des niveaux
/// Évite d'afficher un million de lignes.
/// ================================================
void afficherNiveaux(Noeud* racine, int niveau = 1,
                     int limite = 4) {
    if (racine == nullptr || niveau > limite) {
        return;
    }

    cout << "Valeur : " << racine->valeur
         << " | Niveau : " << niveau << '\n';

    afficherNiveaux(racine->gauche, niveau + 1, limite);
    afficherNiveaux(racine->droite, niveau + 1, limite);
}

/// ================================================
/// 9. Liberation de la memoire
/// ================================================
void libererArbre(Noeud* racine) {
    if (racine == nullptr) {
        return;
    }

    libererArbre(racine->gauche);
    libererArbre(racine->droite);

    delete racine;
}

/// ================================================
/// 10. Programme principal
/// ================================================
int main() {
    const int N = 1000000;

    Noeud* racine = nullptr;

    // Generer les nombres de 1 a 1 000 000
    vector<int> valeurs(N);

    for (int i = 0; i < N; i++) {
        valeurs[i] = i + 1;
    }

    // Melanger les valeurs aleatoirement
    random_device rd;
    mt19937 generateur(rd());

    shuffle(valeurs.begin(), valeurs.end(), generateur);

    cout << "====================================\n";
    cout << "      ARBRE BINAIRE DE RECHERCHE\n";
    cout << "====================================\n";
    cout << "Nombre de valeurs : " << N << "\n\n";

    // --------------------------------------------
    // Insertion et mesure du temps total
    // --------------------------------------------
    auto debutInsertion = high_resolution_clock::now();

    for (int valeur : valeurs) {
        racine = inserer(racine, valeur);
    }

    auto finInsertion = high_resolution_clock::now();

    auto tempsInsertion = duration_cast<milliseconds>(
        finInsertion - debutInsertion
    ).count();

    cout << "----- INSERTION -----\n";
    cout << "Temps total : " << tempsInsertion << " ms\n";

    cout << fixed << setprecision(3);

    cout << "Temps moyen par insertion : "
         << (tempsInsertion * 1000.0 / N)
         << " microsecondes\n\n";

    // --------------------------------------------
    // Statistiques de l'arbre
    // --------------------------------------------
    cout << "----- STATISTIQUES DE L'ARBRE -----\n";
    cout << "Nombre de noeuds : "
         << compterNoeuds(racine) << '\n';

    cout << "Nombre de niveaux : "
         << hauteur(racine) + 1 << "\n\n";

    cout << "Exemple de noeuds sur les 4 premiers niveaux :\n";
    afficherNiveaux(racine, 1, 4);
    cout << '\n';

    // --------------------------------------------
    // Recherche d'une valeur
    // --------------------------------------------
    uniform_int_distribution<int> distribution(1, N);
    int valeurRecherchee = distribution(generateur);

    auto debutRecherche = high_resolution_clock::now();

    Noeud* resultat = rechercher(racine, valeurRecherchee);

    auto finRecherche = high_resolution_clock::now();

    auto tempsRecherche = duration_cast<nanoseconds>(
        finRecherche - debutRecherche
    ).count();

    cout << "----- RECHERCHE -----\n";
    cout << "Valeur recherchee : " << valeurRecherchee << '\n';
    cout << "Resultat : "
         << (resultat != nullptr ? "Trouvee" : "Non trouvee")
         << '\n';
    cout << "Temps : " << tempsRecherche << " ns\n\n";

    /// --------------------------------------------
    /// Suppression d'une valeur
    /// --------------------------------------------
    int valeurSupprimee = valeurRecherchee;

    auto debutSuppression = high_resolution_clock::now();

    racine = supprimer(racine, valeurSupprimee);

    auto finSuppression = high_resolution_clock::now();

    auto tempsSuppression = duration_cast<nanoseconds>(
        finSuppression - debutSuppression
    ).count();

    cout << "----- SUPPRESSION -----\n";
    cout << "Valeur supprimee : " << valeurSupprimee << '\n';
    cout << "Temps : " << tempsSuppression << " ns\n";

    if (rechercher(racine, valeurSupprimee) == nullptr) {
        cout << "Verification : suppression reussie.\n";
    }
    else {
        cout << "Verification : valeur encore presente.\n";
    }

    cout << "Nombre de noeuds apres suppression : "
         << compterNoeuds(racine) << '\n';

    cout << "Nombre de niveaux apres suppression : "
         << hauteur(racine) + 1 << '\n';

    /// --------------------------------------------
    /// Liberation de la memoire
    /// --------------------------------------------
    libererArbre(racine);
    racine = nullptr;

    cout << "\nMemoire liberee.\n";
    cout << "Fin du programme.\n";

    return 0;
}
