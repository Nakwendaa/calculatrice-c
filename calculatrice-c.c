#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Chiffre qui est chaîné à au poids le plus fort
typedef struct cell{ char chiffre; struct cell *suivant; } cell;

// Structure d'un nombre
typedef struct num{ int compteurRef; int negatif; struct cell *chiffres; } num;

/* Noeud appartenant à une pile. Contient un pointeur noeud sur le noeud le précédant dans la pile
   afin de pouvoir effectuer des recherches de nombre dans la pile. */
typedef struct node {num *nombre; struct node *suivant;} node;

/* Une structure de pile classique, mais qui ne contient pas de taille maximale prédéfinie afin
   d'optimiser l'utilisation mémoire à l'aide de malloc. */
typedef struct pile {int length; struct node *top;} pile;

/*
   Une structure mémoire afin de pouvoir conserver les variables déjà affectées
   Le buffer qui utilisera les valeurs actuellement affectées par l'expression permet de protéger la mémoire en cas d'erreurs lors de l'évaluation
   de l'expression postfixe. La mémoire est une liste chaînée également. */

typedef struct memoire {struct variable *tete; } memoire;

// var est une case mémoire qui contiendra la variable et le nombre auquel elles est associée.

typedef struct variable {char var; struct num *nombre; struct variable *suivant; } variable;



/* OPÉRATIONS MÉMOIRE */

//Ajoute la variable dans la mémoire, et décrémente le compteur de référence.
int affecterVal(memoire *mem, char var, num* nombre);

// Rétablis les valeurs du buffer dans la mémoire. S'effectue après une expression postfixe entièrement valide.
int retablirValeurs(memoire *mem, memoire* buf);

/*
 Efface tous les variables d'une mémoire. Détruit les nombres/objets dont les compteurs de référence sont à 0 0, sauf
 pour le nombre passé en paramètre qui sera détruit éventuellement plus tard */
void deleteMem(memoire *mem, num *nombre);

// Vérifie qu'un nombre n'est pas déjà présent en mémoire.
num* checkMem(num *nombre, memoire *mem);

// Retourne un pointeur sur la variable recherchée en memoire (ou buffer) sinon NULL si absente.
variable* rechercherVar(memoire *mem, char var);



/* OPÉRATIONS D'ÉVALUATION D'EXPRESSION. */

// Transforme une string en nombre.
num* transformationStructure(memoire *buffer, memoire *mem, pile *stack, char *str);


/* Retourne 0 si out of memory, 1 si tout s'est bien passé pour l'évaluation du mot dans une expression postfixe,
   et 2 s'il y a une erreur de syntaxe. */
int postfixeEvaluation(memoire *buffer, memoire *mem, pile *stack, char *mot);


// Retourne le nombre calculé par l'opération binaire.
num* evaluerOpBin(num *nombre1, num *nombre2, char operator);

// Retourne le nombre calculé par l'opération unaire.
num* evaluerOpUn(memoire *buffer, memoire *mem, pile *stack, num *nombre, char *opun);

// Retourne 1 si la chaîne de cractères est un literal: a,...,z et les nombres sans 0 en poids forts.
int validLiteral(char *literal);

// Retourne 1 si la chaîne de cacactères est un opérateur binaire (+, -, *), 0 sinon.
int validOpBin(char *opbin);

// Retourne 1 si la chaîne de caractères est un opérateur unaire (=a , ?) valide, 0 sinon.
int validOpUn(char *opun);


/* OPÉRATIONS AVEC LES NOMBRES */

// Renvoie le résultat de l'addtion d'un nombre1 avec un nombre2. Pour les nombres négatifs, peut faire appel à soustraction
num* addition(num *nombre1, num *nombre2);

// Renvoie le résultat de la soustraction d'un nombre1 avec un nombre2. Peut faire appel à l'addition.
num* soustraction(num *nombre1,num *nombre2);

// Renvoie le résultat de la multiplication d'un nombre1 avec un nombre2. Fait appel à des additions successives.
num* multiplication(num *nombre1,num *nombre2);

// Permet d'enlever les poids les plus forts qui ont la valeur 0
void enleverPoidsForts0(num *nombre);


/* PRINT NOMBRES */

// Méthode récursive afin d'imprimer un nombre du bits de poids le plus fort au plus faible (ex: 10000)
void printRev(cell *chiffre);

// Imprimer un nombre avec son signe (+ ou -) et du poids le plus fort au plus faible
void printNumReverse(num* nombre);


/* LONGUEUR ET COMPARAISON DE NOMBRES */

// Trouver la longueur d'un nombre.
int findLenNum(num *nombre);

// Renvoie 0 si nombre1 < nombre2, ou 1 si nombre1 >= nombre2
int compareNum(num *nombre1, num *nombre2);

// Renvoie 0 si nombre1 < nombre2, ou 1 si nombre1 >= nombre2
int isEqual(num *nombre1, num *nombre2);



/* OPÉRATIONS STRUCTURES NOMBRES */

// Renvoie 0 si out of memory. Ajoute un chiffre de poids le plus faible à un nombre.
int addHeadNum(num *nombre, char chiffre);


// Renvoie 0 si out of memory. Ajoute un chiffre de poids le plus fort à un nombre.
int addTailNum(num *nombre, char chiffre);


// Renvoie un pointeur sur la dernier chiffre d'un nombre, qui correspond à son chiffre de poids le plus fort.
cell* checkTailNum(num *nombre);

// Supprime un chiffre de poids le plus fort à un nombre. Utile pour enlever les zéros de le plus fort.
void deleteTailNum(num *nombre);

// Permet de copier un nombre dans un autre nombre. Permet de limiter les fuites mémoires à cause de pointeurs fous.
int copyNum(num *nombre1, num *nombre2);

// Détruit tous les chiffres chaînés d'un nombre. Désallocation du pointeur
void deleteNumber(num *nombre);

// Détruit seulement les chiffres chaînés d'un nombre, mais pas le pointeur du nombre.
void deleteChiffres(num *nombre);


/* OPÉRATIONS SUR LA PILE QUI PERMET DE STOCKER LES POINTEURS AFIN DE FAIRE L'ÉVALUATION POSTFIXE. */


// La pile est initialisé à length = 0 et top = NULL;
void initPile(pile *stack);

// Destruction de la pile d'exécution.
void deletePile(pile* stack);

// 1 si push réussi, 0 si out of memory
int push(pile* stack, num *nombre);

// Retourne NULL si pop échoué, sinon retourne le nombre qui était au sommet de la pile
num* pop(pile* stack);

// 1 si la pile est vide, 0 sinon.
int isEmpty(pile* stack);

// Retourne le pointeur d'un nombre si présent dans la pile. 0 sinon.
num* checkPile(num* nombre, pile* stack);



int main(int argc, char*argv[]) {

    char *mot; // Un mot que l'on veut étudier (une opérande, un opérateur ou une affection, ou un mot hors langage également).
    pile *stack = NULL; // Pile qui va nous permettre d'évaluer l'expression postfixe.
    memoire *buffer = NULL;
    int c; // Un caractère que je récupère dans la console.
    int erreur; // Permet de vérifier qu'il n'y pas eu d'erreurs dans mes appels de fonctions.
    int debut; // Permet de marquer le début d'un nouveau mot.
    int exception = 0;

    memoire* mem = malloc(sizeof(memoire));
    if (!mem)
        exception = 1;
    else
        mem->tete = NULL;

    init:

    while (1) {
        stack = malloc(sizeof(pile));
        if (!stack)
            exception = 1;
        else
            initPile(stack);

        buffer = malloc(sizeof(memoire));
        if (!buffer)
            exception = 1;
        else
            buffer->tete = NULL;

        mot = NULL;
        char *motCpy = NULL;
        int longueurMot = 0;
        debut = 1;
        printf("> ");
        while ((c = getchar())) {
            if (!exception && debut && c == ' ')
                exception = 2; // Pas d'espace au début d'un mot. SYNTAXE
            else if (c == '\n' || c == EOF) {  // À la fin de la ligne, on arrête.
                if (!exception) {
                    erreur = postfixeEvaluation(buffer, mem, stack, mot); // Derniere opération à effectuer puisqu'on a atteint la fin de ligne ou de fichier.
                    if (erreur)
                        exception = erreur; // Out of Memory ou Erreur de syntaxe
                }
                if (!exception && stack->length == 1) {
                    num* val1 = pop(stack);
                    if (val1) {
                        // Si tout est bon, on peut actualiser les valeurs de la mémoire à partir de celles du buffer
                        if (retablirValeurs(buffer, mem))
                            exception = 1; // Out of memory
                        if (!exception) {
                            deleteMem(buffer, val1); // Destruction du buffer.
                            buffer = NULL; // Afin d'éviter de re-désallouer le buffer en bas de la boucle, on met à NULL.
                            printNumReverse(val1); // Impression du résultat
                            if (!val1->compteurRef)
                                deleteNumber(val1); // Si la veuleur popé n'est plus référencé, on peut la détruire
                        }
                    }
                }
                else if (!exception) exception = 2; // La stack n'est pas de longueur 1 à la fin, c'est qu'il y a une erreur dans l'expression postfixée.

                if (!exception)
                    printf("\n");
                if (c == EOF)
                    goto stop; // Fin de fichier, le programme peut s'arrêter.
                else if (c == '\n')
                    break; //Si fin de ligne, on reprend la boucle principale pour relire une nouvelle ligne.
            }

            else if (!exception && c == ' ' && !debut) { // Un nouveau mot a été lu.
                erreur = postfixeEvaluation(buffer, mem, stack, mot);
                if (erreur)
                    exception = erreur; // Out of Memory ou Erreur de syntaxe
                if (mot)
                    free(mot);
                mot = NULL;
                debut = 1;
            }
            else if (!exception) { // On est en train d'enregistrer un mot
                debut = 0;
                if (!mot) {
                    mot = malloc(sizeof(char) + 1);
                    if (!mot)
                        exception = 1;
                    else {
                        strncpy(mot, (char *) &c, 2);
                        longueurMot++;
                    }
                }
                else {
                    motCpy = malloc(sizeof(char)*(longueurMot + 2));
                    if (!motCpy)
                        exception = 1;
                    else {
                        strcpy(motCpy, mot);
                        strcat(motCpy, (char *) &c);
                        free(mot);
                    }

                    mot = malloc(sizeof(char)*(longueurMot + 2));
                    if (!mot)
                        exception = 1;
                    else {
                        strcpy(mot, motCpy);
                        longueurMot++;
                    }
                    free(motCpy);
                }
            }
        }

        // On attend d'avoir atteint la fin de ligne pour sortir de la boucle de getChar() afin de traiter les exceptions.
        // En effet, si on avait pas géré l'exception de cette manière, getChar() continuerait à traiter la suite de l'expression à partir
        // de là où l'exception a été levée.

        switch (exception) {
            case 1:
                printf("Out of Memory. Le programme n'a plus assez de mémoire pour fonctionner normalement.\n");
                exception = 0;
                break;

            case 2:
                printf("Erreur de syntaxe:\n     L'expression doit être une expression postfixe\n"
                               "     L'expression ne doit pas commencer par un espace, ni se terminer par un espace\n"
                               "     Chaque opérande et opérateur ou affection doivent être espacés d'un espace exactement\n"
                               "     Une variable doit être affectée pour pouvoir être utilisée.\n");
                exception = 0;
                break;

            default:
                break;
        }

        // Enfin, après une exception ou une fin de boucle, on s'assure bien d'avoir vider la pile, le mot, et le buffer
        // Avant de recommencer un traitement d'une nouvelle ligne.

        if (mot) free(mot);
        if (stack) deletePile(stack);
        if (buffer) deleteMem(buffer, NULL);
        goto init;
    }

    // Étiquette de fin du programme. On détruit toutes les cases mémoires possiblement allouées.
    stop:
    if (mot) free(mot);
    if (buffer) deleteMem(buffer, NULL);
    if (stack) deletePile(stack);
    if (mem) deleteMem(mem, NULL);

    return 0;
}

num* transformationStructure(memoire* buffer, memoire* mem, pile * stack, char *str) {
    int longueurChaine = (int) strlen(str);
    num* nombre = malloc(sizeof(num));
    if (!nombre)
        return nombre;

    nombre->compteurRef = 0;
    nombre->negatif = 0;
    nombre->chiffres = NULL;
    if (!(longueurChaine == 1 && str[0] == '0')) { // Prendre en compte le cas 0 pour le mémoire
        int i;
        for(i = longueurChaine - 1; i >= 0; i--){
            if (addTailNum(nombre, str[i])) {
                deleteNumber(nombre);
                return NULL;
            }
        }
    }
    num* num1 = checkPile(nombre, stack); // et surtout checkMemoire;
    num* num2 = checkMem(nombre, mem);
    num* num3 = checkMem(nombre, buffer);
    if (num1) {
        deleteNumber(nombre);
        return num1;
    }
    if (num2) {
        deleteNumber(nombre);
        return num2;
    }
    if (num3) {
        deleteNumber(nombre);
        return num3;
    }
    return nombre;
}

int postfixeEvaluation(memoire *buffer, memoire* mem, pile* stack, char* mot) {

    if (validLiteral(mot)) {

        if (mot[0] >= 'a' && *mot <= 'z') {
            int erreur;
            variable *ptr = rechercherVar(mem, *mot);
            variable *ptrBuf = rechercherVar(buffer, *mot);
            if (!ptr && !ptrBuf)
                return 2; // Buf et memoire vide. Cette variable n'a pas été affectée.
            else
                erreur = ptrBuf ? push(stack, ptrBuf->nombre) : push(stack, ptr->nombre); // On va chercher en priorité la valeur contenue dans le buffer qui est la plus actuelle.
            // si elle n'est pas présente dans le buffer, on push le pointeur de nombre de la variable contenu en memoire.

            if (erreur)
                return 1; // Out of Memory
            return 0; // Tout s'est bien passé
        }

        else {
            num* val1 = transformationStructure(buffer, mem, stack, mot);
            if (!val1)
                return 1; // Out of memory, il n'y avait plus de place pour malloc dans transformationStructure;
            if (push(stack, val1)) { // En cas d'échec, on doit détruire ce nombre si son compteur de référence est à 0, sinon on en perdrait la trace.
                if (!val1->compteurRef)
                    deleteNumber(val1);
                return 1; // Out of Memory, il n'y a plus de place pour la pile.
            }
            return 0;
        }
    }
    else if(validOpBin(mot)) {
        num *val2 = pop(stack);// Destack val1
        num *val1 = pop(stack);// Destack val2

        if (val1 && val2) {
            num *resultat = evaluerOpBin(val1, val2, *mot); // Evaluer l'expression
            if (!resultat) {
                // Le resultat est NULL, echec de l'opération. On vérifie que les valeurs ne pointent pas sur le même nombre, sinon on pourrait
                // désalloué un même nombre deux fois. S'ils ne sont pas égaux, et que leur compteur de ref vaut 0, on peut les détruire sans soucis.
                // De plus, il faut désallouer, car sinon risque d'objets morts en sortant de la méthode.
                if ((isEqual(val1, val2) && !val1->compteurRef))
                    deleteNumber(val1);
                else if (!isEqual(val1, val2) && !val1->compteurRef && !val2->compteurRef) {
                    deleteNumber(val1);
                    deleteNumber(val2);
                }
                else if (!isEqual(val1, val2) && !val1->compteurRef && val2->compteurRef)
                    deleteNumber(val1);
                else if (!isEqual(val1, val2) && val1->compteurRef && !val2->compteurRef)
                    deleteNumber(val2);
                return 1;// Out of memory pour l'allocation dans evaluerOpBin;
            }

            // Le résultat n'est pas NULL. On devrait donc vérifier dans la memoire (buffer et memoire principale) ainsi
            // que la pile que ce nombre n'existe pas déjà en mémoire. S'il existe déjà, on peut détruire le résultat.

            num *num1 = checkPile(resultat, stack);
            num *num2 = checkMem(resultat, mem);
            num *num3 = checkMem(resultat, buffer);

            if (num1) {
                deleteNumber(resultat);
                resultat = num1;
            }
            else if (num2) {
                deleteNumber(resultat);
                resultat = num2;
            }
            else if (num3) {
                deleteNumber(resultat);
                resultat = num3;
            }
            else if (isEqual(val1, resultat)) {
                deleteNumber(resultat);
                resultat = val1;
            }
            else if (isEqual(val2, resultat)) {
                deleteNumber(resultat);
                resultat = val2;
            }
            else
                resultat->compteurRef = 0;

            // Si le resultat a la même valeur que val1 ou val2, on pourrait désallouer val1 ou val2  alors que resultat pointe
            // sur le même nombre. Ainsi, afin de ne pas pas désallouer resultat, on incrémente son compteur de référence.
            resultat->compteurRef++;

            if ((isEqual(val1, val2) && !val1->compteurRef))
                deleteNumber(val1);
            else if (!isEqual(val1, val2) && !val1->compteurRef && !val2->compteurRef) {
                deleteNumber(val1);
                deleteNumber(val2);
            }
            else if (!isEqual(val1, val2) && !val1->compteurRef && val2->compteurRef)
                deleteNumber(val1);
            else if (!isEqual(val1, val2) && val1->compteurRef && !val2->compteurRef)
                deleteNumber(val2);

            // Passé cette étape, on peut décrémenter le compteur de référence, qui sera re-incrémenter par le push dans la pile.
            resultat->compteurRef--;

            if (push(stack, resultat)) {
                if (!resultat->compteurRef)
                    deleteNumber(resultat);
                return 1;
            }
            return 0;
        }

        else {
            // Si on arrive dans ce cas, la pile n'a pu pop les nombres requis pour l'opération.
            // On vérifie les compteurs de références de ces nombres, et on désalloue si nécessaire.
            if (val1 && !val1->compteurRef)
                deleteNumber(val1);
            if (val2 && !val2->compteurRef)
                deleteNumber(val2);
            return 2; // Erreur de syntaxe
        }
    }

    else if(validOpUn(mot)) {
        num* val1 = pop(stack);// Destack val1
        if (val1) {
            num* resultat = evaluerOpUn(buffer, mem, stack, val1, mot);
            if (!resultat && !val1->compteurRef) { // Cas out of memory avec resultat = NULL
                deleteNumber(val1);
                return 1; // Out of memory pour l'alloc dans evaluer expUn
            }
            if (!isEqual(val1, resultat) && !val1->compteurRef)
                deleteNumber(val1);
            if (push(stack, resultat)) {
                if (!resultat->compteurRef)
                    deleteNumber(resultat);
                return 1; // Out of Memory
            }
            return 0; // Tout s'est bien passé
        }
        return 2; // Erreur de Syntaxe: impossible de pop un opérande.
    }
    return 2; // Ni literral, ni binaire, ni unaire ( ? et =a)
}

num* evaluerOpBin(num *nombre1, num *nombre2, char operator) {
    num* resultat = malloc(sizeof(num));
    if (!resultat)
        return NULL;
    num *temp = NULL;

    switch(operator) {
        case '+':
            temp = addition(nombre1, nombre2); break;
        case '-':
            temp = soustraction(nombre1, nombre2); break;
        case '*':
            temp = multiplication(nombre1, nombre2); break;
        default:
            free(resultat); return NULL;
    }

    if (copyNum(temp, resultat)) {
        if(resultat)
            deleteNumber(resultat);
        if (temp)
            deleteNumber(temp);
        return NULL;
    }

    deleteNumber(temp);
    return resultat;
}

int nombreChiffre(int n) {
    if (n < 10)
        return 1;
    return 1 + nombreChiffre(n / 10);
} // Compter le nombre de chiffres que prend le compteur de reference (p.e 4560 prend 4 chiffres en longueur)

num* evaluerOpUn(memoire *buffer, memoire *mem, pile *stack, num *nombre, char *opun) {
    num *resultat, *num2, *num3, *num4;
    char str[nombreChiffre(nombre->compteurRef)];

    if (opun) {
        int longueur = (int) strlen(opun);
        switch(longueur) {
            case 1: // Cas où l'on veut obtenir le compteur de référence d'un nombre.
                if (sprintf(str, "%d", nombre->compteurRef) < 0)
                    return NULL;
                resultat = transformationStructure(buffer,mem, stack, str); // Fais la verif checkMem etc.
                if (!resultat)
                    return NULL;
                if (isEqual(nombre, resultat)) {
                    deleteNumber(resultat);
                    resultat = nombre;
                }
                return resultat;

            case 2:
                if (affecterVal(buffer, opun[1], nombre))
                    return NULL;
                return nombre;
            default:
                break;
        }
    }
    return NULL;
}

int validLiteral(char *nombre) {
    if (!nombre)
        return 0;
    int longueurLiteral = (int) strlen(nombre);
    if (longueurLiteral == 1 && *nombre >= 'a' && *nombre <= 'z')
        return 1; // Accepter 'a', 'b', 'c'
    if (longueurLiteral > 1 && *nombre == '0')
        return 0; // Cas 0123 , 004324 mais on garde juste 0;
    int i;
    for(i = 0; i < longueurLiteral; i++)
        if (!(*(nombre+i) >= '0' && *(nombre+i) <= '9'))
            return 0;
    return 1;
}

int validOpBin(char *opbin) {
    if (!opbin)
        return 0;
    int longueur = (int) strlen(opbin);
    if (longueur > 1 || (*opbin != '+' && *opbin != '-' && *opbin != '*'))
        return 0;
    return 1;
}

int validOpUn(char *opun) {
    if (!opun)
        return 0;

    int longueur = (int) strlen(opun);
    if (longueur == 1 && opun[0] == '?')
        return 1;
    else if (longueur == 2 && opun[0] == '=' && opun[1] >= 'a' && opun[1] <= 'z')
        return 1;
    return 0;
}

void initPile(pile *stack) {
    stack->length = 0;
    stack->top = NULL;
}

int isEmpty(pile* stack) {
    if (stack->top)
        return 0;
    return 1;
}

int push(pile* stack, num *nombre) { // 1 si out of memory, 0 sinon
    node *noeud = malloc(sizeof(node));
    if(!noeud)
        return 1;
    else {
        noeud->nombre = nombre;
        noeud->nombre->compteurRef++;
        noeud->suivant = stack->top;
        stack->top = noeud;
        stack->length++;
        return 0;
    }
}

num* pop(pile* stack) {
    node *noeud;
    num *nombre;

    if(isEmpty(stack))
        return NULL;
    noeud = stack->top;
    stack->top = noeud->suivant;
    nombre = noeud->nombre;
    free(noeud);
    stack->length--;

    if (!stack->length)
        stack->top = NULL;
    nombre->compteurRef--;
    return nombre;
}

num* checkPile(num *nombre, pile* stack) {
    node *ptr = stack->top;
    while(ptr) {
        if (isEqual(nombre, ptr->nombre))
            return ptr->nombre; // On a trouvé un nombre égal à nombre dans la pile
        ptr = ptr->suivant;
    }
    return NULL;
}

void deletePile(pile* stack) {
    num *nombre = pop(stack);
    while (nombre) {
        if (!nombre->compteurRef)
            deleteNumber(nombre);
        nombre = pop(stack);
    }
    free(stack);
}

num* addition(num *nombre1, num *nombre2) { // Addition de deux entiers positifs
    num* temp;
    num* resultatSomme = malloc(sizeof(num));
    if (!resultatSomme)
        return NULL;

    // En cas d'addition avec 0
    if (!nombre1->chiffres && nombre2->chiffres) { // 0 + (-1234) ou 0 + 1234, il suffit de renvoyer num2
        if (copyNum(nombre2, resultatSomme))
            goto deleteSomme;
        return resultatSomme;
    }
    else if (nombre1->chiffres && !nombre2->chiffres) { // 1234 + 0 ou -1234 + 0.
        if (copyNum(nombre1, resultatSomme))
            goto deleteSomme;
        return resultatSomme;
    }
    else if (!nombre1->chiffres && !nombre2->chiffres) { // 0 + 0
        resultatSomme->negatif = 0;
        resultatSomme->chiffres = NULL;
        return resultatSomme;
    }

    // En cas d'addition de nombre négatifs
    if(nombre1->negatif && nombre2->negatif)
        resultatSomme->negatif = 1; // Si on fait (-1234) + (-1234) = - (1234 + 1234)
    else if (!nombre1->negatif && nombre2->negatif) { // Si on fait 1234 + (-1234) = 1234 - 1234
        nombre2->negatif = 0;
        temp= soustraction(nombre1,nombre2);
        if (!temp)
            goto deleteSomme;
        if (copyNum(temp, resultatSomme))
            goto deleteTempSomme;
        deleteNumber(temp);
        nombre2->negatif = 1;
        return resultatSomme;
    }
    else if (nombre1->negatif && !nombre2->negatif) { // Si on fait -1234 + 33
        nombre1->negatif = 0;
        temp= soustraction(nombre2,nombre1);
        if (!temp)
            goto deleteSomme;
        if (copyNum(temp, resultatSomme))
            goto deleteTempSomme;
        deleteNumber(temp);
        nombre1->negatif = 1;
        return resultatSomme;
    }
    else
        resultatSomme->negatif = 0;

    // Sinon on fait l'addition
    int sommeIntermediaire;
    int reste = 0;

    resultatSomme->chiffres = NULL;
    cell *chiffre1 = nombre1->chiffres; // Pointeur sur le premier chiffre de num1
    cell *chiffre2 = nombre2->chiffres; // Pointeur sur le premier chiffre de num2

    while(chiffre1 && chiffre2) {
        sommeIntermediaire = (chiffre1->chiffre - '0') + (chiffre2->chiffre - '0') + reste;
        reste = sommeIntermediaire / 10;
        if (addTailNum(resultatSomme, (char) (sommeIntermediaire % 10 + '0')))
            goto deleteSomme;
        chiffre1 = chiffre1->suivant;
        chiffre2 = chiffre2->suivant;
    }

    while(chiffre1) {
        sommeIntermediaire = (chiffre1->chiffre - '0') + reste;
        reste = sommeIntermediaire / 10;
        if (addTailNum(resultatSomme, (char) (sommeIntermediaire % 10 + '0')))
            goto deleteSomme;
        chiffre1 = chiffre1->suivant;
    }

    while(chiffre2) {
        sommeIntermediaire = (chiffre2->chiffre - '0') + reste;
        reste = sommeIntermediaire / 10;
        if (addTailNum(resultatSomme, (char) (sommeIntermediaire % 10 + '0')))
            goto deleteSomme;
        chiffre2 = chiffre2->suivant;
    }

    if (reste > 0 && addTailNum(resultatSomme, (char) (reste + '0')))
        goto deleteSomme;
    return resultatSomme;

    deleteSomme:
    deleteNumber(resultatSomme);
    return NULL;

    deleteTempSomme:
    deleteNumber(resultatSomme);
    deleteNumber(temp);
    return NULL;
}

num* soustraction(num *nombre1, num *nombre2) {
    int soustractionIntermediaire;
    int reste = 0;
    cell* chiffre1, *chiffre2;
    num *temp;

    num* resultatSoustraction = malloc(sizeof(num));
    if (!resultatSoustraction)
        return NULL;

    // En cas de soustraction avec 0
    if (!nombre1->chiffres && nombre2->chiffres) { // 0 - 1234 ou 0 - (-1234)
        if (copyNum(nombre2, resultatSoustraction))
            goto deleteSous;
        if (nombre2->negatif)
            resultatSoustraction->negatif = 0; // 0 - (-1234) si num2 negatif, alors le resultat est positif.
        else
            resultatSoustraction->negatif = 1; // 0 - 1234 si c'est 1234, alors ca devient négatif
        return resultatSoustraction;
    }
    else if (nombre1->chiffres && !nombre2->chiffres) { // 1234 - 0 ou -1234 - 0; On garde num1 seulement
        if (copyNum(nombre1, resultatSoustraction))
            goto deleteSous;
        return resultatSoustraction;
    }
    else if (!nombre1->chiffres && !nombre2->chiffres) { // Cas 0 - 0
        resultatSoustraction->negatif = 0;
        resultatSoustraction->chiffres = NULL;
        return resultatSoustraction;
    }


    // Traitement des cas négatifs
    if(nombre1->negatif && nombre2->negatif) { // (-12) - (-11) =  11 - 12
        nombre1->negatif = 0;
        nombre2->negatif = 0;
        temp = soustraction(nombre2, nombre1);
        if (!temp)
            goto deleteSous;
        if (copyNum(temp, resultatSoustraction))
            goto deleteTempSous;
        deleteNumber(temp);
        nombre1->negatif = 1;
        nombre2->negatif = 1;
        return resultatSoustraction;
    }
    else if(nombre1->negatif && !nombre2->negatif) { // (-12) - (11) => - 12 - 11 = - (12+11) => addition(12,11) puis je prends le négatif
        nombre1->negatif = 0;
        temp = addition(nombre1, nombre2);
        if (!temp)
            goto deleteSous;
        if (copyNum(temp, resultatSoustraction))
            goto deleteTempSous;
        deleteNumber(temp);
        nombre1->negatif = 1;
        resultatSoustraction->negatif = 1;
        return resultatSoustraction;
    }
    else if (!nombre1->negatif && nombre2->negatif) { // (12) - (-11) => addition(12,11) SEULEMENT
        nombre2->negatif = 0;
        temp = addition(nombre1, nombre2);
        if (!temp)
            goto deleteSous;
        if (copyNum(temp, resultatSoustraction))
            goto deleteTempSous;
        deleteNumber(temp);
        nombre2->negatif = 1;
        return resultatSoustraction;
    }

    // Si num2 plus grand que num1, par exemple 11 - 12
    if (!compareNum(nombre1, nombre2)) { // Si num1 < num2
        temp = soustraction(nombre2, nombre1);
        if (!temp)
            goto deleteSous;
        if (copyNum(temp, resultatSoustraction))
            goto deleteTempSous;
        deleteNumber(temp);
        resultatSoustraction->negatif = 1;
        return resultatSoustraction;
    }

    resultatSoustraction->chiffres = NULL;
    chiffre1 = nombre1->chiffres; // Pointeur sur le premier chiffre de num1
    chiffre2 = nombre2->chiffres; // Pointeur sur le premier chiffre de num2

    while(chiffre2) {

        // Je fais la soustraction entre les deux chiffres.
        soustractionIntermediaire = (chiffre1->chiffre - '0') - ((chiffre2->chiffre - '0') + reste);
        if (soustractionIntermediaire < 0) {
            soustractionIntermediaire += 10;
            reste = 1;
        }
        else
            reste = 0;

        if (addTailNum(resultatSoustraction, (char) (soustractionIntermediaire + '0')))
            goto deleteSous;
        chiffre1 = chiffre1->suivant;
        chiffre2 = chiffre2->suivant;
    }

    while(chiffre1) {
        soustractionIntermediaire = (chiffre1->chiffre - '0') - reste;
        if (soustractionIntermediaire < 0) {
            soustractionIntermediaire += 10;
            reste = 1;
        }
        else
            reste = 0;

        if (addTailNum(resultatSoustraction, (char) (soustractionIntermediaire + '0')))
            goto deleteSous;
        chiffre1 = chiffre1->suivant;
    }

    resultatSoustraction->negatif = 0;
    enleverPoidsForts0(resultatSoustraction);
    return resultatSoustraction;

    deleteSous:
    deleteNumber(resultatSoustraction);
    return NULL;

    deleteTempSous:
    deleteNumber(resultatSoustraction);
    deleteNumber(temp);
    return NULL;
}

num* multiplication(num *nombre1, num *nombre2) {
    num* resultatMultiplication = malloc(sizeof(num));
    if (!resultatMultiplication)
        return NULL;

    num *temp;

    if (!nombre1->chiffres || !nombre2->chiffres) {
        resultatMultiplication->negatif = 0;
        resultatMultiplication->chiffres = NULL;
        return resultatMultiplication;
    }

    if (findLenNum(nombre2) > findLenNum(nombre1)) {  // Afin de s'assurer que ce soit toujours mon nombre le plus grand en premier.
        temp = multiplication(nombre2, nombre1);
        if (!temp)
            goto deleteResultat;
        if (copyNum(temp, resultatMultiplication))
            goto deleteTempRes;
        deleteNumber(temp);
        return resultatMultiplication;
    }

    resultatMultiplication->negatif = 0;
    resultatMultiplication->chiffres = NULL;
    num* resultatInter = malloc(sizeof(num));
    if (!resultatInter)
        goto deleteResultat;

    cell* chiffre2 = nombre2->chiffres;
    int compteur = -1;

    while(chiffre2) {
        resultatInter->negatif = 0;
        resultatInter->chiffres = NULL;
        int i;
        for(i = 0; i < chiffre2->chiffre - '0'; i++) { // On fait des additions successives
            temp = addition(resultatInter, nombre1);
            if (!temp)
                goto deleteInterResultat;
            deleteChiffres(resultatInter);
            if (copyNum(temp, resultatInter))
                goto deleteTempInterRes;
            deleteNumber(temp);
        }
        compteur++;
        for(i = 0; i < compteur; i++)
            if(addHeadNum(resultatInter, '0'))
                return NULL; // A chaque fois que l'on passe au nombre suivant, on rajoute un 0 en poids faible de plus

        temp = addition(resultatMultiplication, resultatInter); // Addition du résultat intermédiaire obtenu avec le résultat final
        if (!temp)
            goto deleteInterResultat;
        deleteChiffres(resultatMultiplication);

        if (copyNum(temp, resultatMultiplication))
            goto deleteTempInterRes;
        deleteChiffres(resultatInter);
        deleteNumber(temp);
        chiffre2 = chiffre2->suivant;
    }
    free(resultatInter);

    if (nombre1->negatif^nombre2->negatif)
        resultatMultiplication->negatif = 1; // Dans le cas ou l'un des nombre serait négatif
    else
        resultatMultiplication->negatif = 0;
    return resultatMultiplication;


    deleteResultat:
    deleteNumber(resultatMultiplication);
    return NULL;

    deleteTempRes:
    deleteNumber(resultatMultiplication);
    deleteNumber(temp);
    return NULL;

    deleteInterResultat:
    deleteNumber(resultatMultiplication);
    deleteNumber(resultatInter);
    return NULL;

    deleteTempInterRes:
    deleteNumber(resultatMultiplication);
    deleteNumber(temp);
    deleteNumber(resultatInter);
    return NULL;


}

void enleverPoidsForts0(num *nombre) {
    cell* tail = checkTailNum(nombre);
    while (tail  && tail->chiffre == '0') {
        deleteTailNum(nombre);
        tail = checkTailNum(nombre);
    }
}

void printRev(cell *chiffre) {
    if (!chiffre)
        return;
    printRev(chiffre->suivant);
    printf("%c", chiffre->chiffre);
} // Méthode récursive afin d'imprimer un nombre du bits de poids le plus fort au plus faible (ex: 10000)

void printNumReverse(num *nombre) {
    if(!nombre->chiffres) {
        printf("0");
        return;
    }
    if(nombre->negatif)
        printf("-");
    printRev(nombre->chiffres);
} // Ajout du signe du nombre, et appel à printRev pour imprimer le nombre.

int findLenNum(num *nombre) {
    int longueur = 0;
    cell* p = nombre->chiffres;
    while(p) {
        longueur++;
        p = p->suivant;
    }
    return longueur;
} // Permet de trouve la longueur d'un nombre en terme de chiffres.

int compareNum(num *nombre1, num *nombre2) { // Renvoie 0 si nombre1 < num2, ou 1 si nombre1 >= num2

    cell* pointeur1 = nombre1->chiffres;
    cell* pointeur2 = nombre2->chiffres;

    if(!pointeur1 && !pointeur2)
        return 1; // Cas 0 et 0
    else if(pointeur1 && !pointeur2) { // cas -13 et 0 et 13 et 0;
        if (nombre1->negatif)
            return 0;
        return 1;
    }
    else if(!pointeur1) { // Cas 0 et -13 OU 0 et 13
        if(nombre2->negatif)
            return 1;
        return 0;
    }

    if(nombre1->negatif && !nombre2->negatif)
        return 0; // -124 et 12 par exemple
    else if (!nombre1->negatif && nombre2->negatif)
        return 1; // 124 et -12 par exemple

    int longueurNum1 = findLenNum(nombre1);
    int longueurNum2 = findLenNum(nombre2);

    if(longueurNum1 > longueurNum2 && nombre1->negatif && nombre2->negatif)
        return 0; // Donc -1344 et -35, donc nombre1 < num2
    else if(longueurNum1 > longueurNum2 && !nombre1->negatif && !nombre2->negatif)
        return 1; // Donc 1344 et 35
    else if (longueurNum1 < longueurNum2 && nombre1->negatif && nombre2->negatif)
        return 1; // Donc -34 et -1344
    else if (longueurNum1 < longueurNum2 && !nombre1->negatif && !nombre2->negatif)
        return 0; // Donc 34 et 1344

    int compteur = 0;
    int i = 0;

    // Les deux nombres sont donc de la même taille en longueur, et de même signe
    while(1) {
        pointeur1 = nombre1->chiffres;
        pointeur2 = nombre2->chiffres;
        while(i < longueurNum1 - 1 - compteur) { // Sinon on a deux chiffres de même longueur. On compare les chiffres 1 à 1 du poids le plus fort au plus faible.
            pointeur1 = pointeur1->suivant;
            pointeur2 = pointeur2->suivant;
            i++;
        }
        if (pointeur1->chiffre - '0' == pointeur2->chiffre - '0') {
            compteur++;
            i = 0;
            if(compteur == longueurNum1)
                return 1; // On atteint la début de notre chaîne et tout est égal, alors 1
        }
        else if (pointeur1->chiffre - '0' > pointeur2->chiffre - '0') {
            if(nombre1->negatif)
                return 0;
            return 1;
        }
        else if (pointeur1->chiffre - '0' < pointeur2->chiffre - '0') {
            if(nombre1->negatif)
                return 1;
            return 0;
        }
    }

} // Renvoie 0 si nombre1 < nombre2, ou 1 si nombre1 >= nombre2

int isEqual(num *nombre1, num *nombre2) {
    if (!nombre1 || !nombre2)
        return 0;

    cell* pointeur1 = nombre1->chiffres;
    cell* pointeur2 = nombre2->chiffres;
    int compteur = 0;
    int i = 0;

    if(!pointeur1 && !pointeur2)
        return 1; // Cas 0 et 0
    else if (!pointeur1 || !pointeur2)
        return 0;

    if (nombre1->negatif != nombre2->negatif)
        return 0;

    int longueurNum1 = findLenNum(nombre1);
    int longueurNum2 = findLenNum(nombre2);

    if (longueurNum1 != longueurNum2)
        return 0;

    while(1) {
        pointeur1 = nombre1->chiffres;
        pointeur2 = nombre2->chiffres;
        while(i < longueurNum1 - 1 - compteur) { // Sinon on a deux chiffres de même longueur. On compare les chiffres 1 à 1 du poids le plus fort au plus faible.
            pointeur1 = pointeur1->suivant;
            pointeur2 = pointeur2->suivant;
            i++;
        }
        if (pointeur1->chiffre - '0' == pointeur2->chiffre - '0') {
            compteur++;
            i = 0;
            if(compteur == longueurNum1)
                return 1; // On atteint la début de notre chaîne et tout est égal, alors 1
        }
        else if (pointeur1->chiffre - '0' != pointeur2->chiffre - '0')
            return 0;
    }
} // Renvoie 1 si num1 == num2,

cell* checkTailNum(num *nombre) {
    cell* temp = nombre->chiffres;
    if(!temp)
        return NULL;
    while(temp->suivant)
        temp = temp->suivant;
    return temp;
} // checkTailNum retourne le bit de poids le plus fort d'un num.

int addHeadNum(num *nombre, char chiffre) {
    cell *nouveau_chiffre = malloc(sizeof(cell));
    if (!nouveau_chiffre)
        return 1;
    nouveau_chiffre->chiffre = chiffre;
    nouveau_chiffre->suivant = nombre->chiffres;
    nombre->chiffres = nouveau_chiffre;
    return 0;
} // Ajoute un chiffre au poids le plus faible du nombre (dans le cas de la multiplication, 0)

int addTailNum(num *nombre, char chiffre) {   // Ajouter un nombre au poid le plus fort.
    cell *nouveau_chiffre = malloc(sizeof(cell));
    if (!nouveau_chiffre)
        return 1; // Out of memory

    nouveau_chiffre->chiffre = chiffre;
    if(!nombre->chiffres) {
        nouveau_chiffre->suivant = nombre->chiffres;  // on pointe le noeud tête
        nombre->chiffres = nouveau_chiffre; // Ajout du noeud dans la liste
    }
    else {
        cell* p = nombre->chiffres;
        while(p->suivant)
            p = p->suivant;
        p->suivant = nouveau_chiffre; // Suivant pointe sur le nouveau chiffre;
        nouveau_chiffre->suivant = NULL;
    }
    return 0;
} // Ajoute un chiffre de poids le plus fort au nombre. Utilisé dans les opérations.

void deleteTailNum(num *nombre) {
    cell* temp;
    cell* p = nombre->chiffres;
    if (!p)
        return;
    else if (!p->suivant) {
        free(p);
        nombre->chiffres = NULL;
    }
    else {
        while(p->suivant->suivant)
            p = p->suivant;
        temp = p->suivant;
        p->suivant = NULL;
        free(temp);
    }
}

void deleteNumber(num *nombre) {
    if (!nombre) return;
    cell* p = nombre->chiffres;
    cell* suivant;
    while (p)
    {
        suivant = p->suivant;
        free(p);
        p = suivant;
    }
    free(nombre);
}

void deleteChiffres(num *nombre) {
    if (!nombre) return;
    cell* p = nombre->chiffres;
    cell* suivant;
    while (p)
    {
        suivant = p->suivant;
        free(p);
        p = suivant;
    }
}

int copyNum(num* srcNum, num* destNum) {
    if (!srcNum)
        return 1; // Out of memory

    destNum->chiffres = NULL;
    destNum->negatif = srcNum->negatif;
    cell *p = srcNum->chiffres;
    while(p) {
        if (addTailNum(destNum, p->chiffre))
            return 1; // Out of memory
        p = p->suivant;
    }
    return 0; // OK
}

num* checkMem(num *nombre, memoire* mem) {
    variable *ptr = mem->tete;
    while(ptr) {
        if (isEqual(nombre, ptr->nombre))
            return ptr->nombre; // On a trouvé un nombre égal à num1 dans la pile
        ptr = ptr->suivant;
    }
    return NULL;
}

variable* rechercherVar(memoire* mem, char var) { // Retourne le noeud contenant la variable si présent
    variable* ptr = mem->tete;
    while (ptr) {
        if (ptr->var == var)
            return ptr;
        ptr = ptr->suivant;
    }
    return ptr;
}

int affecterVal(memoire* mem, char var, num* nombre) { // Trouver la variable 'x' et met la valeur. Si non présente, on ajoute une nouvelle valeur.
    variable *ptr = rechercherVar(mem, var);
    if (ptr) {
        ptr->nombre->compteurRef--;
        if (!ptr->nombre->compteurRef)
            deleteNumber(ptr->nombre);
        ptr->nombre = nombre;
        ptr->nombre->compteurRef++;
        return 0; //OK
    }
    else {
        ptr = malloc(sizeof(variable));
        if (!ptr)
            return 1; // Out of memory

        ptr->var = var;
        ptr->nombre = nombre;
        ptr->nombre->compteurRef++;
        if (!mem->tete) {
            mem->tete = ptr;
            ptr->suivant = NULL;
        }
        else {
            ptr->suivant = mem->tete;
            mem->tete = ptr;
        }
        return 0; //OK
    }
}

int retablirValeurs(memoire* buf, memoire* mem) {  // Rétablir la valeur des variables en memoire a partir du buf si tout est OK.
    variable *ptr = buf->tete;                     // Ajouter les nouvelles variables si nécessaires si elles n,existent pas en memoire.
    int longueur = 0;

    // Afin d'être sûr que le rétablissement de la mémoire se fera sans problème.
    // Si un échec appairaissait lors de l'affectation des valeurs à la mémoire principale, la mémoire principale contiendrait des valeurs
    // issus du buffer, alors que l'on préfère garantir l'intégrite des données en mémoire principale.
    // Ainsi on test qu'il y assez de blocs mémoires disponibles pour le rétablissement des valeurs.
    while(ptr) {
        longueur++;
        ptr = ptr->suivant;
    }

    // Test qu'il n'y aura aucun problème d'allocation au moment du rétablissement des valeurs.
    void *test = malloc(sizeof(variable)*longueur);
    if (!test)
        return 1;
    else free(test);

    ptr = buf->tete;
    if (!ptr)
        return 0; // Il n'y a rien à rétablir à partir du buffer.
    while (ptr) {
        if (affecterVal(mem, ptr->var, ptr->nombre))
            return 1; // Out of memory
        ptr = ptr->suivant;
    }
    return 0;
}

void deleteMem(memoire *mem, num *nombre) {
    variable* ptr;
    ptr = mem->tete;
    variable* temp;
    while (ptr) {
        temp = ptr->suivant;
        ptr->nombre->compteurRef--;
        if (!ptr->nombre->compteurRef && !isEqual(ptr->nombre, nombre))
            deleteNumber(ptr->nombre);
        free(ptr);
        ptr = temp;
    }
    free(mem);
}