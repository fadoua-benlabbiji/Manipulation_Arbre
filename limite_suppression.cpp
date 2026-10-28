//chaine
/*
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

const int MAX = 10000;
const double LIMITE = 300.0;

struct Noeud {
    string val;
    Noeud* g;
    Noeud* d;
};

// Creation d'un noeud
Noeud* Creer_Noeud(const string& valeur) {
    return new Noeud{valeur, nullptr, nullptr};
}

// Insertion iterative dans un ABR
Noeud* AR_Inserer_I(Noeud* r, const string& valeur) {
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

// Suppression iterative dans un ABR
Noeud* AR_Supprimer_I(Noeud* r, const string& valeur) {
    Noeud* parent = nullptr;
    Noeud* courant = r;

    // Recherche du noeud
    while (courant != nullptr && courant->val != valeur) {
        parent = courant;

        if (valeur < courant->val)
            courant = courant->g;
        else
            courant = courant->d;
    }

    if (courant == nullptr)
        return r;

    // Si le noeud possede deux fils
    if (courant->g != nullptr && courant->d != nullptr) {
        Noeud* parentMin = courant;
        Noeud* minimum = courant->d;

        while (minimum->g != nullptr) {
            parentMin = minimum;
            minimum = minimum->g;
        }

        courant->val = minimum->val;

        // On supprime ensuite le noeud minimum
        parent = parentMin;
        courant = minimum;
    }

    // Le noeud a maintenant au maximum un fils
    Noeud* fils = (courant->g != nullptr)
                    ? courant->g
                    : courant->d;

    if (parent == nullptr)
        r = fils;
    else if (parent->g == courant)
        parent->g = fils;
    else
        parent->d = fils;

    delete courant;
    return r;
}

// Liberation sans recursivite
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

// Construction directe de l'ABR trie
// Il s'agit volontairement d'un arbre degenere
Noeud* Construire_ABR_Trie(const vector<string>& mots) {
    if (mots.empty())
        return nullptr;

    Noeud* racine = Creer_Noeud(mots[0]);
    Noeud* courant = racine;

    for (size_t i = 1; i < mots.size(); i++) {
        courant->d = Creer_Noeud(mots[i]);
        courant = courant->d;
    }

    return racine;
}

int main() {
    ifstream fichier("mot.txt");

    if (!fichier) {
        cout << "Erreur : impossible d'ouvrir mot.txt\n";
        return 1;
    }

    vector<string> mots;
    string mot;

    while (mots.size() < MAX && fichier >> mot)
        mots.push_back(mot);

    fichier.close();

    if (mots.empty()) {
        cout << "Le fichier ne contient aucun mot.\n";
        return 1;
    }

    cout << "Nombre de mots lus : " << mots.size() << "\n";

    // Suppression des doublons pour comparer les memes cles
    vector<string> cles = mots;
    sort(cles.begin(), cles.end());
    cles.erase(unique(cles.begin(), cles.end()), cles.end());

    // Ordre aleatoire des suppressions
    vector<string> suppressions = cles;

    mt19937 gen(random_device{}());
    shuffle(suppressions.begin(), suppressions.end(), gen);

    int tailles[] = {10, 100, 1000, 10000};

    cout << "\nDebut des tests de suppression...\n\n";

    cout << left << setw(16) << "N suppressions"
         << setw(22) << "ABR trie (ms)"
         << setw(22) << "ABR aleatoire (ms)"
         << setw(22) << "map (ms)" << '\n';

    cout << string(82, '-') << '\n';

    for (int n : tailles) {
        // Eviter un acces hors limites
        if (static_cast<size_t>(n) > suppressions.size()) {
            cout << n << " : test ignore (cles insuffisantes)\n";
            continue;
        }

        // Reconstruction des trois structures pour chaque test
        Noeud* trie = Construire_ABR_Trie(cles);
        Noeud* abr = nullptr;
        map<string, int> m;

        vector<string> aleatoires = cles;
        shuffle(aleatoires.begin(), aleatoires.end(), gen);

        for (const string& s : aleatoires)
            abr = AR_Inserer_I(abr, s);

        for (const string& s : cles)
            m[s] = 1;

        double temps[3] = {0, 0, 0};
        bool limite[3] = {false, false, false};

        // 1. Suppression dans l'ABR trie
        auto debut = steady_clock::now();

        for (int i = 0; i < n; i++) {
            trie = AR_Supprimer_I(trie, suppressions[i]);

            if ((i + 1) % 100 == 0 &&
                duration<double>(steady_clock::now() - debut).count()
                    >= LIMITE) {
                limite[0] = true;
                break;
            }
        }

        temps[0] = duration<double, milli>(
            steady_clock::now() - debut).count();

        // 2. Suppression dans l'ABR aleatoire
        debut = steady_clock::now();

        for (int i = 0; i < n; i++) {
            abr = AR_Supprimer_I(abr, suppressions[i]);

            if ((i + 1) % 100 == 0 &&
                duration<double>(steady_clock::now() - debut).count()
                    >= LIMITE) {
                limite[1] = true;
                break;
            }
        }

        temps[1] = duration<double, milli>(
            steady_clock::now() - debut).count();

        // 3. Suppression dans map
        debut = steady_clock::now();

        for (int i = 0; i < n; i++) {
            m.erase(suppressions[i]);

            if ((i + 1) % 100 == 0 &&
                duration<double>(steady_clock::now() - debut).count()
                    >= LIMITE) {
                limite[2] = true;
                break;
            }
        }

        temps[2] = duration<double, milli>(
            steady_clock::now() - debut).count();

        cout << left << setw(16) << n;

        for (int j = 0; j < 3; j++) {
            if (limite[j])
                cout << setw(22) << "LIMITE 5 MIN";
            else
                cout << setw(22) << fixed
                     << setprecision(3) << temps[j];
        }

        cout << '\n';

        Liberer(trie);
        Liberer(abr);
    }

    cout << "\nFin des tests.\n";
    return 0;
}
*/
//entier

#include <iostream>
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

struct Noeud {
    int val;
    Noeud* g;
    Noeud* d;
};

Noeud* Creer_Noeud(int valeur) {
    return new Noeud{valeur, nullptr, nullptr};
}

// Insertion iterative dans un ABR
Noeud* AR_Inserer_I(Noeud* r, int valeur) {
    if (r == nullptr)
        return Creer_Noeud(valeur);

    Noeud* p = r;

    while (true) {
        if (valeur < p->val) {
            if (p->g == nullptr) {
                p->g = Creer_Noeud(valeur);
                break;
            }
            p = p->g;
        }
        else if (valeur > p->val) {
            if (p->d == nullptr) {
                p->d = Creer_Noeud(valeur);
                break;
            }
            p = p->d;
        }
        else {
            break;
        }
    }

    return r;
}

// Construction directe d'un ABR trie degenere
Noeud* Construire_ABR_Trie(int n) {
    if (n <= 0)
        return nullptr;

    Noeud* racine = Creer_Noeud(1);
    Noeud* p = racine;

    for (int i = 2; i <= n; i++) {
        p->d = Creer_Noeud(i);
        p = p->d;
    }

    return racine;
}

// Suppression iterative
Noeud* AR_Supprimer_I(Noeud* r, int valeur) {
    Noeud* parent = nullptr;
    Noeud* p = r;

    while (p != nullptr && p->val != valeur) {
        parent = p;

        if (valeur < p->val)
            p = p->g;
        else
            p = p->d;
    }

    if (p == nullptr)
        return r;

    // Deux fils : chercher le successeur
    if (p->g != nullptr && p->d != nullptr) {
        Noeud* parentMin = p;
        Noeud* minimum = p->d;

        while (minimum->g != nullptr) {
            parentMin = minimum;
            minimum = minimum->g;
        }

        p->val = minimum->val;
        parent = parentMin;
        p = minimum;
    }

    Noeud* fils = (p->g != nullptr) ? p->g : p->d;

    if (parent == nullptr)
        r = fils;
    else if (parent->g == p)
        parent->g = fils;
    else
        parent->d = fils;

    delete p;
    return r;
}

// Liberation iterative
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

// Mesure du temps de suppression
double Tester_ABR(Noeud*& r, const vector<int>& suppressions,
                  int n, bool& limite) {
    auto debut = steady_clock::now();

    for (int i = 0; i < n; i++) {
        r = AR_Supprimer_I(r, suppressions[i]);

        if ((i + 1) % 100 == 0 &&
            duration<double>(steady_clock::now() - debut).count()
                >= LIMITE) {
            limite = true;
            break;
        }
    }

    return duration<double, milli>(
        steady_clock::now() - debut).count();
}

double Tester_Map(map<int, int>& m,
                  const vector<int>& suppressions,
                  int n, bool& limite) {
    auto debut = steady_clock::now();

    for (int i = 0; i < n; i++) {
        m.erase(suppressions[i]);

        if ((i + 1) % 100 == 0 &&
            duration<double>(steady_clock::now() - debut).count()
                >= LIMITE) {
            limite = true;
            break;
        }
    }

    return duration<double, milli>(
        steady_clock::now() - debut).count();
}

int main() {
    mt19937 gen(random_device{}());

    // Generation des 1 000 000 entiers
    vector<int> valeurs(MAX);

    for (int i = 0; i < MAX; i++)
        valeurs[i] = i + 1;

    vector<int> suppressions = valeurs;
    shuffle(suppressions.begin(), suppressions.end(), gen);

    int tailles[] = {
        10, 100, 1000, 10000,
        100000, 500000, 1000000
    };

    cout << "Debut des tests de suppression des entiers...\n\n";

    cout << left << setw(16) << "N suppressions"
         << setw(22) << "ABR trie (ms)"
         << setw(22) << "ABR aleatoire (ms)"
         << setw(22) << "map (ms)" << '\n';

    cout << string(82, '-') << '\n';

    for (int n : tailles) {
        cout << "Test en cours : " << n << " suppressions..."
             << flush;

        // Construction des structures hors mesure
        Noeud* trie = Construire_ABR_Trie(MAX);
        Noeud* abr = nullptr;
        map<int, int> m;

        vector<int> ordreInsertion = valeurs;
        shuffle(ordreInsertion.begin(), ordreInsertion.end(), gen);

        for (int x : ordreInsertion)
            abr = AR_Inserer_I(abr, x);

        for (int x : valeurs)
            m[x] = 1;

        bool limite[3] = {false, false, false};

        double t1 = Tester_ABR(
            trie, suppressions, n, limite[0]);

        double t2 = Tester_ABR(
            abr, suppressions, n, limite[1]);

        double t3 = Tester_Map(
            m, suppressions, n, limite[2]);

        cout << "\r" << left << setw(16) << n;

        double temps[] = {t1, t2, t3};

        for (int j = 0; j < 3; j++) {
            if (limite[j])
                cout << setw(22) << "LIMITE 5 MIN";
            else
                cout << setw(22) << fixed
                     << setprecision(3) << temps[j];
        }

        cout << '\n' << flush;

        Liberer(trie);
        Liberer(abr);
    }

    cout << "\nFin des tests.\n";
    return 0;
}
