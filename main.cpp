#include <stdio.h>
#include <stdlib.h>

#define MAX 100
//decalartion d la sturcture noeud
typedef struct Noeud
{
    int val;
    struct Noeud *g;
    struct Noeud *d;
    int est_visite;
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
        c->est_visite = 1;

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
    n->est_visite = 0;

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
int main()
{
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
    return 0;
}
