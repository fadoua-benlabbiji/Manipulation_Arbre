/*
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

const double LIMITE_SECONDES = 300.0;

// Noeud de l'ABR
struct Noeud {
    string mot;
    Noeud* gauche;
    Noeud* droite;

    Noeud(const string& m)
        : mot(m), gauche(nullptr), droite(nullptr) {}
};

// Insertion iterative dans l'ABR
Noeud* AR_Inserer_I(Noeud* racine, const string& mot) {
    if (racine == nullptr)
        return new Noeud(mot);

    Noeud* courant = racine;

    while (true) {
        if (mot < courant->mot) {
            if (courant->gauche == nullptr) {
                courant->gauche = new Noeud(mot);
                break;
            }
            courant = courant->gauche;
        }
        else if (mot > courant->mot) {
            if (courant->droite == nullptr) {
                courant->droite = new Noeud(mot);
                break;
            }
            courant = courant->droite;
        }
        else {
            break; // Ignorer les doublons
        }
    }

    return racine;
}

// Liberer l'ABR sans recursivite
void libererABR(Noeud* racine) {
    vector<Noeud*> pile;

    if (racine != nullptr)
        pile.push_back(racine);

    while (!pile.empty()) {
        Noeud* n = pile.back();
        pile.pop_back();

        if (n->gauche != nullptr)
            pile.push_back(n->gauche);

        if (n->droite != nullptr)
            pile.push_back(n->droite);

        delete n;
    }
}

struct Resultat {
    double temps;
    bool limite;
    size_t nombreInsere;
};

// Mesurer le temps d'insertion dans un ABR
Resultat testerABR(const vector<string>& mots,
                   size_t N, bool trie) {
    Noeud* racine = nullptr;
    auto debut = chrono::steady_clock::now();

    size_t i = 0;
    bool depasse = false;

    for (; i < N; ++i) {
        const string& mot = trie ? mots[i] : mots[i];

        racine = AR_Inserer_I(racine, mot);

        if ((i + 1) % 100 == 0) {
            double sec = chrono::duration<double>(
                chrono::steady_clock::now() - debut
            ).count();

            if (sec >= LIMITE_SECONDES) {
                ++i;
                depasse = true;
                break;
            }
        }
    }

    double millisecondes = chrono::duration<double, milli>(
        chrono::steady_clock::now() - debut
    ).count();

    // Liberation hors chronometrage
    libererABR(racine);

    return {millisecondes, depasse, i};
}

// Mesurer le temps d'insertion dans std::map
Resultat testerMap(const vector<string>& mots, size_t N) {
    map<string, int> dictionnaire;

    auto debut = chrono::steady_clock::now();

    size_t i = 0;
    bool depasse = false;

    for (; i < N; ++i) {
        dictionnaire.emplace(mots[i], 1);

        if ((i + 1) % 100 == 0) {
            double sec = chrono::duration<double>(
                chrono::steady_clock::now() - debut
            ).count();

            if (sec >= LIMITE_SECONDES) {
                ++i;
                depasse = true;
                break;
            }
        }
    }

    double millisecondes = chrono::duration<double, milli>(
        chrono::steady_clock::now() - debut
    ).count();

    return {millisecondes, depasse, i};
}

void afficherTemps(double temps, bool limite) {
    if (limite)
        cout << left << setw(22) << "LIMITE 5 MIN";
    else
        cout << left << setw(22) << fixed
             << setprecision(2) << temps;
}

int main() {
    ifstream fichier("mot.txt");

    if (!fichier) {
        cerr << "Erreur : impossible d'ouvrir mot.txt\n";
        return 1;
    }

    vector<string> mots;
    mots.reserve(1000000);

    string mot;

    while (mots.size() < 1000000 && fichier >> mot)
        mots.push_back(mot);

    fichier.close();

    if (mots.empty()) {
        cerr << "Le fichier est vide.\n";
        return 1;
    }

    cout << "Nombre de mots lus : " << mots.size() << '\n';

    // ABR trie : copie triee des mots
    vector<string> motsTries = mots;
    sort(motsTries.begin(), motsTries.end());

    // Tailles des tests
    vector<size_t> tailles = {
        10, 100, 1000, 10000,
        100000, 500000, 1000000
    };

    // true = la structure peut encore etre testee
    bool actif[3] = {true, true, true};

    cout << "\n=== TABLEAU COMPARATIF ===\n";
    cout << "Temps d'insertion (millisecondes)\n\n";

    cout << left
         << setw(14) << "N (mots)"
         << setw(22) << "ABR trie"
         << setw(22) << "ABR aleatoire"
         << setw(22) << "std::map"
         << '\n';

    cout << string(80, '-') << '\n';

    for (size_t N : tailles) {
        if (N > mots.size())
            continue;

        double temps[3] = {0, 0, 0};
        bool limite[3] = {false, false, false};

        // 0 : ABR trie
        if (actif[0]) {
            Resultat r = testerABR(motsTries, N, true);
            temps[0] = r.temps;
            limite[0] = r.limite;

            if (r.limite)
                actif[0] = false;
        }

        // 1 : ABR aleatoire (ordre du fichier)
        if (actif[1]) {
            Resultat r = testerABR(mots, N, false);
            temps[1] = r.temps;
            limite[1] = r.limite;

            if (r.limite)
                actif[1] = false;
        }

        // 2 : std::map
        if (actif[2]) {
            Resultat r = testerMap(mots, N);
            temps[2] = r.temps;
            limite[2] = r.limite;

            if (r.limite)
                actif[2] = false;
        }

        cout << left << setw(14) << N;

        for (int i = 0; i < 3; ++i) {
            if (!actif[i] && !limite[i]) {
                // Structure arretee lors d'une taille precedente
                cout << left << setw(22) << "ARRETE";
            }
            else {
                afficherTemps(temps[i], limite[i]);
            }
        }

        cout << '\n';

        // Arreter globalement si les trois structures sont arretees
        if (!actif[0] && !actif[1] && !actif[2])
            break;
    }

    cout << "\nFin des tests.\n";
    return 0;
}
*/
