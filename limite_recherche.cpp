//recherche pour les entier
/*
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

const int MAX = 1000000;
const double LIMITE = 300.0;

// Structure d'un noeud
typedef struct Noeud {
    int val;
    struct Noeud *g;
    struct Noeud *d;
} Noeud;

// Creation d'un noeud
Noeud* Creer_Noeud(int valeur) {
    Noeud* n = new Noeud;
    n->val = valeur;
    n->g = nullptr;
    n->d = nullptr;
    return n;
}

// Insertion iterative dans l'ABR
Noeud* AR_Inserer_I(Noeud* r, int valeur) {
    if (r == nullptr)
        return Creer_Noeud(valeur);

    Noeud* courant = r;

    while (true) {
        if (valeur < courant->val) {
            if (courant->g == nullptr) {
                courant->g = Creer_Noeud(valeur);
                break;
            }
            courant = courant->g;
        } else if (valeur > courant->val) {
            if (courant->d == nullptr) {
                courant->d = Creer_Noeud(valeur);
                break;
            }
            courant = courant->d;
        } else {
            break;
        }
    }

    return r;
}

// Recherche dans l'ABR
Noeud* AR_Rechercher(Noeud* r, int valeur) {
    while (r != nullptr) {
        if (r->val == valeur)
            return r;

        if (valeur < r->val)
            r = r->g;
        else
            r = r->d;
    }

    return nullptr;
}

// Liberer l'ABR sans recursivite
void Liberer(Noeud* r) {
    vector<Noeud*> pile;

    if (r != nullptr)
        pile.push_back(r);

    while (!pile.empty()) {
        Noeud* p = pile.back();
        pile.pop_back();

        if (p->g != nullptr) pile.push_back(p->g);
        if (p->d != nullptr) pile.push_back(p->d);

        delete p;
    }
}

int main() {
    // Tableau des entiers de 1 a 1 000 000
    vector<int> entiers(MAX);

    for (int i = 0; i < MAX; i++)
        entiers[i] = i + 1;

    mt19937 gen(random_device{}());

    // Tableau commun des recherches, melange
    vector<int> recherches = entiers;
    shuffle(recherches.begin(), recherches.end(), gen);

    // Ordre aleatoire pour construire le deuxieme ABR
    vector<int> aleatoires = entiers;
    shuffle(aleatoires.begin(), aleatoires.end(), gen);

    cout << "Construction des structures...\n";

    // 1. ABR trie : equivalent a l'insertion croissante
    Noeud* trie = Creer_Noeud(1);
    Noeud* p = trie;

    for (int i = 2; i <= MAX; i++) {
        p->d = Creer_Noeud(i);
        p = p->d;
    }

    // 2. ABR aleatoire
    Noeud* abr = nullptr;

    for (int x : aleatoires)
        abr = AR_Inserer_I(abr, x);

    // 3. std::map
    map<int, int> m;

    for (int x : entiers)
        m[x] = x;

    cout << "Structures construites.\n\n";

    int tailles[] = {
        10, 100, 1000, 10000,
        100000, 500000, 1000000
    };

    bool actif[3] = {true, true, true};

    cout << left
         << setw(14) << "N recherches"
         << setw(22) << "ABR trie (ms)"
         << setw(22) << "ABR aleatoire (ms)"
         << setw(22) << "map (ms)" << '\n';

    cout << string(80, '-') << '\n';

    for (int n : tailles) {
        double temps[3] = {0, 0, 0};
        bool limite[3] = {false, false, false};
        bool teste[3];

        for (int j = 0; j < 3; j++)
            teste[j] = actif[j];

        // ABR trie
        if (actif[0]) {
            auto debut = steady_clock::now();

            for (int i = 0; i < n; i++) {
                AR_Rechercher(trie, recherches[i]);

                if ((i + 1) % 100 == 0) {
                    double sec = duration<double>(
                        steady_clock::now() - debut
                    ).count();

                    if (sec >= LIMITE) {
                        limite[0] = true;
                        break;
                    }
                }
            }

            temps[0] = duration<double, milli>(
                steady_clock::now() - debut
            ).count();

            if (limite[0]) actif[0] = false;
        }

        // ABR aleatoire
        if (actif[1]) {
            auto debut = steady_clock::now();

            for (int i = 0; i < n; i++) {
                AR_Rechercher(abr, recherches[i]);

                if ((i + 1) % 100 == 0) {
                    double sec = duration<double>(
                        steady_clock::now() - debut
                    ).count();

                    if (sec >= LIMITE) {
                        limite[1] = true;
                        break;
                    }
                }
            }

            temps[1] = duration<double, milli>(
                steady_clock::now() - debut
            ).count();

            if (limite[1]) actif[1] = false;
        }

        // map
        if (actif[2]) {
            auto debut = steady_clock::now();

            for (int i = 0; i < n; i++) {
                m.find(recherches[i]);

                if ((i + 1) % 100 == 0) {
                    double sec = duration<double>(
                        steady_clock::now() - debut
                    ).count();

                    if (sec >= LIMITE) {
                        limite[2] = true;
                        break;
                    }
                }
            }

            temps[2] = duration<double, milli>(
                steady_clock::now() - debut
            ).count();

            if (limite[2]) actif[2] = false;
        }

        // Affichage
        cout << left << setw(14) << n;

        for (int j = 0; j < 3; j++) {
            if (limite[j])
                cout << setw(22) << "LIMITE 5 MIN";
            else if (!teste[j])
                cout << setw(22) << "ARRETE";
            else
                cout << left << setw(22) << fixed
                     << setprecision(3) << temps[j];
        }

        cout << '\n';

        if (!actif[0] && !actif[1] && !actif[2])
            break;
    }

    Liberer(trie);
    Liberer(abr);

    cout << "\nFin des tests.\n";

    return 0;
}

//recherche pour les chaines de caracteres
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

const int MAX = 1000000;
const double LIMITE = 300.0;

typedef struct Noeud {
    string val;
    struct Noeud *g;
    struct Noeud *d;
} Noeud;

Noeud* Creer_Noeud(string valeur) {
    Noeud* n = new Noeud;
    n->val = valeur;
    n->g = nullptr;
    n->d = nullptr;
    return n;
}

Noeud* AR_Inserer_I(Noeud* r, string valeur) {
    if (r == nullptr)
        return Creer_Noeud(valeur);

    Noeud* courant = r;

    while (true) {
        if (valeur < courant->val) {
            if (courant->g == nullptr) {
                courant->g = Creer_Noeud(valeur);
                break;
            }
            courant = courant->g;
        }
        else if (valeur > courant->val) {
            if (courant->d == nullptr) {
                courant->d = Creer_Noeud(valeur);
                break;
            }
            courant = courant->d;
        }
        else {
            break;
        }
    }

    return r;
}

Noeud* AR_Rechercher(Noeud* r, string valeur) {
    while (r != nullptr) {
        if (r->val == valeur)
            return r;

        if (valeur < r->val)
            r = r->g;
        else
            r = r->d;
    }

    return nullptr;
}

void Liberer(Noeud* r) {
    vector<Noeud*> pile;

    if (r != nullptr)
        pile.push_back(r);

    while (!pile.empty()) {
        Noeud* p = pile.back();
        pile.pop_back();

        if (p->g != nullptr)
            pile.push_back(p->g);

        if (p->d != nullptr)
            pile.push_back(p->d);

        delete p;
    }
}
//main de recherche
/*
int main() {
    ifstream fichier("mot.txt");

    if (!fichier) {
        cout << "Erreur : impossible d'ouvrir le fichier.\n";
        return 1;
    }

    vector<string> mots;
    string mot;

    while (fichier >> mot)
        mots.push_back(mot);

    fichier.close();

    if (mots.empty()) {
        cout << "Le fichier est vide.\n";
        return 1;
    }

    if (mots.size() < MAX) {
        cout << "Le fichier contient seulement "
             << mots.size() << " mots.\n";
        return 1;
    }

    mots.resize(MAX);

    mt19937 gen(random_device{}());

    vector<string> recherches = mots;
    shuffle(recherches.begin(), recherches.end(), gen);

    vector<string> tries = mots;
    sort(tries.begin(), tries.end());
    tries.erase(unique(tries.begin(), tries.end()), tries.end());

    cout << "Construction des structures...\n";

    // ABR trie : construction directe de l'arbre degenerate
    Noeud* trie = Creer_Noeud(tries[0]);
    Noeud* p = trie;

    for (size_t i = 1; i < tries.size(); i++) {
        p->d = Creer_Noeud(tries[i]);
        p = p->d;
    }

    // ABR aleatoire : insertion dans l'ordre du fichier
    Noeud* abr = nullptr;

    for (const string& s : mots)
        abr = AR_Inserer_I(abr, s);

    // map
    map<string, int> m;

    for (const string& s : mots)
        m[s] = 1;

    cout << "Structures construites.\n\n";

    int tailles[] = {
        10, 100, 1000, 10000,
        100000, 500000, 1000000
    };

    bool actif[3] = {true, true, true};

    cout << left << setw(14) << "N recherches"
         << setw(22) << "ABR trie (ms)"
         << setw(22) << "ABR aleatoire (ms)"
         << setw(22) << "map (ms)" << '\n';

    cout << string(80, '-') << '\n';

    for (int n : tailles) {
        double temps[3] = {0, 0, 0};
        bool limite[3] = {false, false, false};
        bool teste[3];

        for (int j = 0; j < 3; j++)
            teste[j] = actif[j];

        // Recherche dans l'ABR trie
        if (actif[0]) {
            auto debut = steady_clock::now();

            for (int i = 0; i < n; i++) {
                AR_Rechercher(trie, recherches[i]);

                if ((i + 1) % 100 == 0) {
                    double sec = duration<double>(
                        steady_clock::now() - debut
                    ).count();

                    if (sec >= LIMITE) {
                        limite[0] = true;
                        break;
                    }
                }
            }

            temps[0] = duration<double, milli>(
                steady_clock::now() - debut
            ).count();

            if (limite[0])
                actif[0] = false;
        }

        // Recherche dans l'ABR aleatoire
        if (actif[1]) {
            auto debut = steady_clock::now();

            for (int i = 0; i < n; i++) {
                AR_Rechercher(abr, recherches[i]);

                if ((i + 1) % 100 == 0) {
                    double sec = duration<double>(
                        steady_clock::now() - debut
                    ).count();

                    if (sec >= LIMITE) {
                        limite[1] = true;
                        break;
                    }
                }
            }

            temps[1] = duration<double, milli>(
                steady_clock::now() - debut
            ).count();

            if (limite[1])
                actif[1] = false;
        }

        // Recherche dans map
        if (actif[2]) {
            auto debut = steady_clock::now();

            for (int i = 0; i < n; i++) {
                m.find(recherches[i]);

                if ((i + 1) % 100 == 0) {
                    double sec = duration<double>(
                        steady_clock::now() - debut
                    ).count();

                    if (sec >= LIMITE) {
                        limite[2] = true;
                        break;
                    }
                }
            }

            temps[2] = duration<double, milli>(
                steady_clock::now() - debut
            ).count();

            if (limite[2])
                actif[2] = false;
        }

        cout << left << setw(14) << n;

        for (int j = 0; j < 3; j++) {
            if (limite[j])
                cout << setw(22) << "LIMITE 5 MIN";
            else if (!teste[j])
                cout << setw(22) << "ARRETE";
            else
                cout << setw(22) << fixed
                     << setprecision(3) << temps[j];
        }

        cout << '\n';

        if (!actif[0] && !actif[1] && !actif[2])
            break;
    }

    Liberer(trie);
    Liberer(abr);

    cout << "\nFin des tests.\n";

    return 0;
}
*/
