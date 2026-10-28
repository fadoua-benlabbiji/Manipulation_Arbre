/*#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <iomanip>
#include <string>
#include <map>
using namespace std;
#define MAX 100
//decalartion d la sturcture noeud
typedef struct Noeud
{
    int val;
    struct Noeud *g;
    struct Noeud *d;

} Noeud;
//decalartion de la structure File
typedef struct {
    Noeud *tab[MAX];
    int debut;
    int fin;
} File;
//fonction enfiler ,ajout en fin
void enfiler(File *f, Noeud *n) {
    if (f->fin < MAX) {
        f->tab[f->fin] = n;
        f->fin++;
    }
}
//focntion defiler ,supprimer debut
Noeud *defiler(File *f) {
    if (f->debut < f->fin) {
        return f->tab[f->debut++];
    }
    return NULL;
}
//parcour
void Parcour_Largeur(Noeud *r)
{
    if (r == NULL)
    {
        printf("Arbre vide");
        return;
    }

    File F = {0, 0};
    enfiler(&F, r);

    while (F.debut < F.fin)
    {
        Noeud *c = defiler(&F);

        printf(" %d ", c->val);

        if (c->g != NULL)
            enfiler(&F, c->g);

        if (c->d != NULL)
            enfiler(&F, c->d);
    }
}
Noeud *inserer(Noeud *r, int valeur)
{
    if (valeur == -1)
        return r;

    Noeud *n = (Noeud *)malloc(sizeof(Noeud));

    if (n == NULL)
        return r;

    n->val = valeur;
    n->g = NULL;
    n->d = NULL;

    // Si l'arbre est vide
    if (r == NULL)
        return n;

    // Initialiser la file
    File f = {0, 0};
    enfiler(&f, r);

    // Chercher la première place libre
    while (f.debut < f.fin)
    {
        Noeud *c = defiler(&f);

        if (c->g == NULL)
        {
            c->g = n;
            return r;
        }
        else
        {
            enfiler(&f, c->g);
        }

        if (c->d == NULL)
        {
            c->d = n;
            return r;
        }
        else
        {
            enfiler(&f, c->d);
        }
    }

    free(n);
    return r;
}
//focntion de creation d un noeud
Noeud* Creer_Noeud(int valeur) {
    Noeud *n = (Noeud*)malloc(sizeof(Noeud));

    if (n == NULL)
        return NULL;

    n->val = valeur;
    n->g = NULL;
    n->d = NULL;

    return n;
}
//implementation d'insertion pour une arbre binaire de recherche
Noeud* AR_Inserer(Noeud *r, int valeur){
     if(r==nullptr) return Creer_Noeud(valeur);
     if(valeur < r->val) r->g=AR_Inserer(r->g,valeur);
     else  r->d=AR_Inserer(r->d,valeur);
     return r;
}
//recherche
Noeud* AR_Rechercher(Noeud *r, int valeur) {
    if (r == nullptr || r->val == valeur)
        return r;

    if (valeur < r->val)
        return AR_Rechercher(r->g, valeur);
    else
        return AR_Rechercher(r->d, valeur);
}
//Suppression
Noeud* AR_Supprimer(Noeud *r, int valeur) {
    if (r == nullptr)
        return nullptr;

    if (valeur < r->val) {
        r->g = AR_Supprimer(r->g, valeur);
    }
    else if (valeur > r->val) {
        r->d = AR_Supprimer(r->d, valeur);
    }
    else {
        // aucun fils gauche
        if (r->g == nullptr) {
            Noeud *temp = r->d;
            free(r);
            return temp;
        }

        //  aucun fils droit
        if (r->d == nullptr) {
            Noeud *temp = r->g;
            free(r);
            return temp;
        }

        // deux fils
        Noeud *temp = r->d;

        while (temp->g != nullptr)
            temp = temp->g;

        r->val = temp->val;
        r->d = AR_Supprimer(r->d, temp->val);
    }

    return r;
}
//insertion iterative

Noeud* AR_Inserer_I(Noeud *r, int valeur) {
    Noeud* n = Creer_Noeud(valeur);

    if (n == nullptr)
        return r;

    // Si l'arbre est vide
    if (r == nullptr)
        return n;

    Noeud* courant = r;
    Noeud* parent = nullptr;

    // Chercher la position d'insertion
    while (courant != nullptr) {
        parent = courant;

        if (valeur < courant->val)
            courant = courant->g;
        else
            courant = courant->d;
    }

    // Inserer le nouveau noeud
    if (valeur < parent->val)
        parent->g = n;
    else
        parent->d = n;

    return r;
}

//afficher
void Afficher(Noeud *r, int espace = 0, int ecart = 6) {
    if (r == nullptr) return;

    espace += ecart;

    Afficher(r->d, espace, ecart);

    cout << endl;
    for (int i = ecart; i < espace; i++)
        cout << " ";

    cout << r->val << endl;

    Afficher(r->g, espace, ecart);
}
//remplissage d'un tableau aleatoire
void Tab_aleatoire(int tab[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = tab[i];
        tab[i] = tab[j];
        tab[j] = temp;
    }
}
// Tester les performances de l'ABR
void TesterABR(int n, int methode, const int operations[]) {

    int *Tab_ordre = new int[n];

    // Initialiser les valeurs de 1 a n
    for (int i = 0; i < n; i++)
        Tab_ordre[i] = i + 1;

    // Methode 0 : insertion triee
    // Methode 1 : insertion aleatoire
    if (methode == 1)
        Tab_aleatoire(Tab_ordre, n);

    Noeud *r = nullptr;

    // 1. Temps d'insertion
    auto debut = chrono::steady_clock::now();

    for (int i = 0; i < n; i++)
        r = AR_Inserer(r, Tab_ordre[i]);

    auto fin = chrono::steady_clock::now();

    double tempsInsertion =
        chrono::duration<double, milli>(fin - debut).count();


    // 2. Temps de recherche de n/2 valeurs aleatoires
    int nbRecherche = n / 2;
    int trouvees = 0;

    debut = chrono::steady_clock::now();

    for (int i = 0; i < nbRecherche; i++) {
        Noeud *p = AR_Rechercher(r, operations[i]);

        if (p != nullptr)
            trouvees++;
    }

    fin = chrono::steady_clock::now();

    double tempsRecherche =
        chrono::duration<double, milli>(fin - debut).count();


    // 3. Temps de suppression des n valeurs
    debut = chrono::steady_clock::now();

    for (int i = 0; i < n/2; i++)
        r = AR_Supprimer(r, operations[i]);

    fin = chrono::steady_clock::now();

    double tempsSuppression =
        chrono::duration<double, milli>(fin - debut).count();


    // 4. Affichage des resultats
    cout << left << setw(10) << n
         << setw(20) << (methode == 0 ? "ABR-Trie"
                                      : "ABR-Aleatoire")
         << right << fixed << setprecision(4)
         << setw(18) << tempsInsertion
         << setw(18) << tempsRecherche
         << setw(18) << tempsSuppression
         << endl;

    if (trouvees != nbRecherche)
        cout << "Erreur pendant la recherche !" << endl;

    delete[] Tab_ordre;
}

void TesterLimiteInsertion(int n, int methode) {
    Noeud* r = nullptr;

    auto debut = chrono::steady_clock::now();
    bool limiteTemps = false;

    for (int i = 1; i <= n; i++) {
        r = AR_Inserer(r, methode == 0 ? i : rand() % n + 1);

        if (i % 100 == 0) {
            double secondes =
                chrono::duration<double>(
                    chrono::steady_clock::now() - debut
                ).count();

            if (secondes >= 5.0) {
                limiteTemps = true;
                break;
            }
        }
    }

    double temps =
        chrono::duration<double, milli>(
            chrono::steady_clock::now() - debut
        ).count();

    cout << "N = " << n
         << " | " << (methode == 0 ? "ABR trie" : "ABR aleatoire")
         << " | Temps insertion = " << fixed << setprecision(2)
         << temps << " ms"
         << " | " << (limiteTemps ? "LIMITE DE TEMPS" : "TERMINE")
         << endl;
}

/*
int main()
{  //Arbre Binaire

    Noeud *a =nullptr;
   a = inserer(a, 7);
    a = inserer(a, 9);
    a = inserer(a, 67);
    a = inserer(a, 1);
    a = inserer(a, 28);
    a = inserer(a, 8);
    a = inserer(a, 10);
    a = inserer(a, -1);
    a = inserer(a, -1);
    a = inserer(a, 29);
    a = inserer(a, 2);
    a = inserer(a, 57);
    a = inserer(a, 40);
    //printf(" %d ",a->val);
    a = inserer(a, -1);
    a = inserer(a, 6);
    printf("Parcours en largeur : ");
    Parcour_Largeur(a);

    printf("\n");

     Noeud *a =nullptr;
     for(int i=1;i<=100;i++){
        a=AR_Inserer(a,i);
     }
     printf("Parcours en largeur : ");
     Parcour_Largeur(a);
     //printf("\nAffichage de l'arbre : ");
     //Afficher(a);
     Noeud *r=nullptr;
     int valeurs[] = {5, 3, 8, 2, 4, 7, 9, 1, 6, 10};
     for (int i = 0; i < 10; i++) {
        r = AR_Inserer(r, valeurs[i]);
    }
     printf("Parcours en largeur : ");
     Parcour_Largeur(r);
     printf("\nAffichage de l'arbre : \n");
     Afficher(r);
     auto debut = chrono::high_resolution_clock::now();
     Noeud *p = AR_Rechercher(a, 2);
     auto fin = chrono::high_resolution_clock::now();
     chrono::duration<double, micro> temps = fin - debut;
     if (p != nullptr)
        cout << "Valeur trouvee : " << p->val << endl;
     else
        cout << "Valeur inexistante" << endl;
     cout << "Temps de recherche : "
     << temps.count() << " microsecondes" << endl;
     a = AR_Supprimer(r, 8);
     cout << "\nArbre apres suppression de 8 :\n";
     Afficher(r);
         int tailles[] = {10, 100, 1000, 10000};

    // Generateur aleatoire
    srand((unsigned)time(nullptr));

    int tailles[] = {10, 100, 1000, 10000};

    cout << left << setw(10) << "N"
         << setw(15) << "Methode"
         << right
         << setw(18) << "Insertion (ms)"
         << setw(18) << "Recherche (ms)"
         << setw(18) << "Suppression (ms)"
         << endl;

    cout << string(79, '-') << endl;

    for (int n : tailles) {
        TesterABR(n, 0);  // Insertion triee
        TesterABR(n, 1);  // Insertion aleatoire

        cout << string(79, '-') << endl;
    }

     return 0;
}
*/
//teste 1
/*
int main() {

    srand((unsigned)time(nullptr));

int tailles[] = {10, 100, 1000, 10000};

    cout << left << setw(10) << "N"
         << setw(20) << "Structure"
         << right
         << setw(18) << "Insertion (ms)"
         << setw(18) << "Recherche (ms)"
         << setw(18) << "Suppression (ms)"
         << endl;

    cout << string(84, '-') << endl;

    for (int n : tailles) {

        // Creer le tableau commun de recherche et suppression
        int *operations = new int[n];

        for (int i = 0; i < n; i++)
            operations[i] = i + 1;

        // Melanger les valeurs sans doublons
        Tab_aleatoire(operations, n);

        // ===================================
        // 1. ABR avec insertion triee
        // ===================================
        TesterABR(n, 0, operations);

        // ===================================
        // 2. ABR avec insertion aleatoire
        // ===================================
        TesterABR(n, 1, operations);


        // ===================================
        // 3. Test de std::map
        // ===================================
        map<int, int> m;

        // Insertion
        auto debut = chrono::steady_clock::now();

        for (int i = 1; i <= n; i++)
            m[i] = i;

        auto fin = chrono::steady_clock::now();

        double tempsInsertion =
            chrono::duration<double, milli>(fin - debut).count();


        // Recherche de n/2 valeurs aleatoires
        int nbRecherche = n / 2;
        int trouvees = 0;

        debut = chrono::steady_clock::now();

        for (int i = 0; i < nbRecherche; i++) {
            auto p = m.find(operations[i]);

            if (p != m.end())
                trouvees++;
        }

        fin = chrono::steady_clock::now();

        double tempsRecherche =
            chrono::duration<double, milli>(fin - debut).count();


        // Suppression des n valeurs
        debut = chrono::steady_clock::now();

        for (int i = 0; i < n/2; i++)
            m.erase(operations[i]);

        fin = chrono::steady_clock::now();

        double tempsSuppression =
            chrono::duration<double, milli>(fin - debut).count();


        // Afficher les resultats de map
        cout << left << setw(10) << n
             << setw(20) << "map"
             << right << fixed << setprecision(4)
             << setw(18) << tempsInsertion
             << setw(18) << tempsRecherche
             << setw(18) << tempsSuppression
             << endl;

        if (trouvees != nbRecherche)
            cout << "Erreur pendant la recherche dans map !" << endl;

        delete[] operations;

        cout << string(84, '-') << endl;
    }

    cout << "\n===== TEST DES LIMITES DE LA MACHINE =====\n";

    int taillesLimites[] = {
        10000, 50000, 100000, 250000,
        500000, 750000, 1000000
    };

    for (int n : taillesLimites) {
        cout << "\n--- Taille : " << n << " ---\n";

        TesterLimiteInsertion(n, 1);
    }

    cout << "\n===== FIN DES TESTS =====\n";


    return 0;
}
/*
//teste les limite machine
int main() {
    srand((unsigned)time(nullptr));

    const double LIMITE_SECONDES = 300.0; // 5 minutes

    cout << left
         << setw(12) << "N"
         << setw(22) << "ABR trie (ms)"
         << setw(22) << "ABR aleatoire (ms)"
         << setw(22) << "map (ms)"
         << endl;

    cout << string(78, '-') << endl;

    // Tailles croissantes par puissances de 10
    for (long long N = 10; ; N *= 10) {

        double temps[3] = {0, 0, 0};
        bool limite[3] = {false, false, false};

        // Tester les 3 structures
        for (int methode = 0; methode < 3; methode++) {

            auto debut = chrono::steady_clock::now();

            if (methode == 0 || methode == 1) {
                // ABR trie ou aleatoire
                Noeud* r = nullptr;

                vector<int> valeurs(N);
                iota(valeurs.begin(), valeurs.end(), 1);

                if (methode == 1)
                    shuffle(valeurs.begin(), valeurs.end(),
                            mt19937(random_device{}()));

                for (long long i = 0; i < N; i++) {
                    r = AR_Inserer_I(r, valeurs[i]);

                    if (i % 100 == 0) {
                        double sec = chrono::duration<double>(
                            chrono::steady_clock::now() - debut
                        ).count();

                        if (sec >= LIMITE_SECONDES) {
                            limite[methode] = true;
                            break;
                        }
                    }
                }

                temps[methode] = chrono::duration<double, milli>(
                    chrono::steady_clock::now() - debut
                ).count();

                // Libération de l'arbre non incluse dans le chronométrage
                // Voir la remarque ci-dessous concernant l'ABR dégénéré.

            } else {
                // std::map
                map<int, int> m;

                for (long long i = 1; i <= N; i++) {
                    m[(int)i] = (int)i;

                    if (i % 100 == 0) {
                        double sec = chrono::duration<double>(
                            chrono::steady_clock::now() - debut
                        ).count();

                        if (sec >= LIMITE_SECONDES) {
                            limite[methode] = true;
                            break;
                        }
                    }
                }

                temps[methode] = chrono::duration<double, milli>(
                    chrono::steady_clock::now() - debut
                ).count();
            }
        }

        cout << left << setw(12) << N;

        for (int i = 0; i < 3; i++) {
            if (limite[i])
                cout << setw(22) << "LIMITE 5 MIN";
            else
                cout << setw(22) << fixed << setprecision(2)
                     << temps[i];
        }

        cout << endl;

        // Arrêt global lorsque les trois structures ont atteint
        // leur limite de temps.
        if (limite[0] && limite[1] && limite[2])
            break;

        // Protection contre le dépassement de long long
        if (N > 100000000000000000LL)
            break;
    }

    return 0;
}
*/
