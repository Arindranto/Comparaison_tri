/* Comparaison de tri croissant entre tri par tas et tri à bulle*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define BASE "/Comparaison_tri.csv"  // Le nom du fichier CSV à générer
// Variable pour situer le programme dans le temps
clock_t b_time; // Marque le commencement du traitement
clock_t e_time; // Marque la fin du traitement
/***********************FONCTIONS********************/
// Procedure d'affichage de liste
void show(int *tab, unsigned int dim){
    unsigned int i;
    for (i=0; i<dim; i++)
        printf("%d  ", tab[i]);
    puts("");
}

/* Fonction nécessaire pour le tri par tas*/
// Fonction qui ajoute un élément a un tas ordonnée suivant une règle Rmax
void ajout(int *tab, unsigned int pos, int nbr){
    /* tab:  le tableau en quesion
    ** pos: position où on va placer nbr
    ** nbr: le nombre a ajouter dans le tas*/
    //int i= pos; // i: suivra la position de nbr dans le tas
    tab[pos]= nbr; // Ajout dnbr a la position dim et incrémentation de dim
    // Placement de nbr selon la regle Rmax
    while (pos>0 && tab[pos]>tab[(pos-1)/2]){
        // Swap
        int tmp= tab[pos];
        tab[pos]= tab[(pos-1)/2];
        tab[(pos-1)/2]= tmp;
        // Fils devient père
        pos= (pos-1)/2;
    }
}

// Passage d'une liste non ordonné a un tas ordonne
void entasser(int *tab, unsigned int n){
    /* tab: le tableau en question
    ** n: la dimension du tableau*/
    unsigned int i;
    for (i= 0; i<n; i++)
        ajout(tab, i, tab[i]);
}

// Fonction qui extrait l'élément prioritaire du tas ordonné
// obtenu après entassement
int extract(int *tab, unsigned int n){
    /* tab: le tableau en question
    ** n: la dimension du tableau*/
    int sortie= tab[0]; // Variable qui sera retourner
    tab[0]= tab[n-1]; // Le dernier element se place sur la racine et la dimension diminue
    unsigned int i= 0, k= 1; // i: racine
                    // k: fils gauche de la racine, marquera le fils prioritaire
    // S'il y a encore 2 fils et que le fils droite est prioritaire
    if (k+1<n && tab[k+1]>tab[k])
        k++; // changement de la valeur de k
    // Replacement de l'element suivant la regle Rmax
    // Si on n'est pas encore a la fin de la liste
    // et si le fils est encore prioritaire par rapport au père
    while (k<n && tab[k]>tab[i]){
        // Swap
        int tmp= tab[k];
        tab[k]= tab[i];
        tab[i]= tmp;
        // Pere devient fils
        i= k; // Nouvelle position du père
        k= 2*i+1; // Fils gauche
        // S'il y a encore 2 fils et que le fils droite est prioritaire
        if (k+1<n && tab[k+1]>tab[k])
            k++; // changement de la valeur de k
    }
    return sortie;
}

// Passage d'un tas ordonné à une liste ordonné
void detasser(int *tab, unsigned int n){
    /* tab: tableau contenant un tas ordonne
    ** n: dimension du tableau*/
    unsigned int i;
    for (i= n; i>0; i--){
        tab[i-1]= extract(tab, i);
    }
}

/****************************TRI**********************************/
/* Fonction de tri par tas*/
double tri_par_tas(int *tab, unsigned int n){
    //int *tmp= tab_temp(tab, n);
    /* tab: tableau contenant un tas ordonne
    ** n: dimension du tableau*/
    int t[n];
    unsigned int i;
    for (i= 0; i<n; i++)
        t[i]= tab[i]; // Recopie de tab dans le tableau temporaire t
    // TRI
    b_time= clock(); // Temps initial
    entasser(t, n);
    detasser(t, n);
    e_time= clock(); // Temps final
    //show(t,n);
    return (double)(e_time - b_time)*1000/CLOCKS_PER_SEC; // Temps d'exécution en ms
}

/* Fonction de tri à bulle*/
double tri_a_bulle(int *tab, unsigned int n){
    /* tab: tableau contenant un tas ordonne
    ** n: dimension du tableau*/
    int t[n];
    unsigned int i, j, k;
    for (i= 0; i<n; i++)
        t[i]= tab[i]; // Recopie de tab dans le tableau temporaire t
    i= n-1;
    clock_t b_time= clock(); // Marque le commencement du traitement
    while (i>0){
        for (j=0, k=0; j<i; j++){
            if (t[j] > t[j+1]){
                int tmp= t[j];
                t[j]= t[j+1];
                t[j+1]= tmp;
                k= j;
            }
        }
        i= k;
    }
    clock_t e_time= clock(); // Marque la fin du traitement
    //show(t,n);
    return (double)(e_time - b_time)*1000/CLOCKS_PER_SEC;
}

/***************PROGRAMME PRINDIPAL******************/
int main()
{
    FILE *f_csv; // Contiendra le flot de données (sorte de pointeur sur FILE)
    char chemin[100];
    unsigned int dep, arr, pas; // départ, arrivé et pas

    printf("Entrez le chemin de destination du fichier csv: ");
    scanf("%s", chemin);    // Le chemin du fichier csv sera stocker dans CHEMIN
    strcat(chemin, BASE);   // Concaténation du chemin à la base

    do {
        // Lecture des informations utile sur les nombres d'éléments à trier
        printf("Entrez le nombre d'elements de depart: ");
        scanf("%u", &dep);
        printf("Entrez le nombre d'elements final: ");
        scanf("%u", &arr);
        printf("Entrez le pas: ");
        scanf("%u", &pas);

        if (dep > arr)
            printf("Le nombre de depart doit être plus petit que l'arrivee...\n");
        if (pas > (arr - dep))
            printf("Le pas est trop large pour l'intervalle donnee...\n");
    } while (dep > arr || pas > (arr - dep));
    // En cas d'erreur
    if ((f_csv= fopen(chemin, "w")) == NULL){   // Ouverture d'un fichier csv en mode écriture qui sera géré par le flot de données f_csv
        printf("Erreur...Chemin non trouve!");
        return (EXIT_FAILURE);
    }

    // Commencer la comparaison
    printf("Ecriture des donnees de la comparaison vers %s\n", chemin);

    fprintf(f_csv, "Element;Tri par tas;Tri a bulle;\n");   // En-tête

    int *tab; // Le pointeur pour générer le tableau dynamiquement
    unsigned int n; // Taille variable du tableau
    double t, b; // Enregistrera le temps de traitement du tri par tas et du tri à bulle respectivement

    srand(time(0)); // Initialisation de la germe pour générer les nombres aléatoires
    for (n= dep; n <= arr; n+= pas){
        tab= (int *)malloc(n*sizeof(int)); // Génération du tableau dynamiquement
        unsigned int i;
        for (i= 0; i<n; i++)
            tab[i]= rand()%(n+13); // tab autocomplete
        // Création du fichier csv
        // Enregistrement des temps
        t= tri_par_tas(tab,n);
        b= tri_a_bulle(tab,n);
        free(tab); // Libération de la mémoire utilisé
        // Ecriture
        fprintf(f_csv, "%u;%.2f;%.2f;\n", n, t, b);
        // Affihcage a l'ecran du progres de la comparaison
        /* 250000 => 100
        ** 5000 => 5000 * 100 / 250000 = 2
        ** n => n*100/250000 */
        printf("\r%.2f %c", n*100.0/arr, 37);    // Carriage return est utilisé pour une affichage plus dynamique au console
                                                // N.B.: char(37) == '%'
    }
    printf("\r100 %c", 37); // Afficher 100% a la fin
    fclose(f_csv); // Fermeture du flot
    printf("Fichier CSV cree avec succes");
    return (EXIT_SUCCESS);
    //return 0;
}
