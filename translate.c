// gcc -Wall translate.c
// ./a.out inversion.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NbColons 200
#define NbLines 1000

long decToBin(int val) {
    // Fonction pour traduire un nombre decimal en binaire
    long binarynum = 0;
    int rem, temp = 1;

    while (val!=0)
    {
        rem = val%2;
        val = val / 2;
        binarynum = binarynum + rem*temp;
        temp = temp * 10;
    }
    return binarynum;
}

int tagCollect(int index, char actualLine[], char allTags[NbLines][NbColons]) {
    // Fonction qui parcourt la ligne actuelle et stocke les etiquettes trouves dans le tableau "allTags"
    int tagPres=0;
    for (int i=0; i<strlen(actualLine); i++) {
        if (actualLine[i]==':') {
            tagPres = 1;
        }
    }
    int ind = 0;
    if (tagPres) {
        while (actualLine[ind]!=':') {
            allTags[index][ind]=actualLine[ind];
            ind++;
        }
        ind++;
        return 1;
    }
    return 0;
}

void traduction(int index, char actualLine[], char allTags[NbLines][NbColons], int presTag, char translatedLine[NbColons]) {
    char* codeOpVals[32] = {"add", "sub", "mul", "div", "and", "or", "xor", "shl", "", "", "ldb", "ldw", "stb", "stw", "", "", "", "", "","", "jmp", "jzs", "jzc", "jcs", "jcc", "jns", "jnc", "in", "out", "rnd", "", "hlt"}; // Collection des commande du code opératoire
    int ind = 0; // Indice principale avec laquelle on parcourt la ligne une fois
    if (presTag) { // On parcourt la ligne actuelle jusque la fin de l'etiquette
        while (actualLine[ind]!=':') {
            ind++;
        }
        ind++;
    }
    while (actualLine[ind]==' ') { // On parcourt la ligne actuelle jusqu'a ce qu'on tombe sur une commande
        ind++;
    }
    char command[10]=""; // Chaine de caractere qui contient la commande de la ligne actuelle
    int j=0; // Indice auxiliere pour stocker lettre par lettre la commande actuelle
    while (actualLine[ind]!=' ' && actualLine[ind]!='\0' && actualLine[ind]!='\n') { // stockage de la commande actuelle
        command[j]=actualLine[ind];
        ind++;
        j++;
    }
    
    int i=0; // indice qui va contenir le numero de la commande
    while (i<32 && strcmp(codeOpVals[i], command)) { // la recherche du numero de la commande dans la Collection des commande du code opératoire
        i++;
    }
    
    long binNb = 0; // Variable temporaire pour stocker l'adresse en binaire des commandes ou parametre
    
    if (i<32) { // Si la commande existe et est trouve on la traduit en binaire
        binNb = decToBin(i);
        printf("Command : %s\n", command);
        printf(" trad : %ld\n", binNb);
    } else {
        printf("Error : commande %s n'existe pas\n", command);
    }
    
    
    char parameters[3][4] = {"","",""}; // Tableau de caracteres qui contient tous les arguments de la ligne actuelle
    
    while (actualLine[ind]==' ') {
        ind++;
    }
    
    int col=0; // Indice pour stocker le numero de l'argument qu'on est en train de parcourir
    while (actualLine[ind]!='\n' && actualLine[ind]!='\0') { // On stocker tous les argument de la ligne actuelle
        // !!! Gerer le cas ou c'est une valeur immédiate !!!
        j=0; // Indice auxiliere qui aide a stocker lettre par lettre un parametre qu'on est en train de parcourir
        while (actualLine[ind]!=',' && actualLine[ind]!='\n' && actualLine[ind]!='\0') {
            parameters[col][j]=actualLine[ind];
            ind++;
            j++;
        }
        if (actualLine[ind]!='\n' && actualLine[ind]!='\0') {
            ind+=2;
        }
        col++;
    }
    // "col" contient le nombre d'arguments
    j=0; // Le numero d'argument qu'on est en train de parcourir (j<col)
    int y; // Indice auxiliere pour comparaison des lettres
    int parNb=0; // Variable auxiliere pour recuperer la valeur decimale du numero du parametre ou de l'adrese ou pointe l'etiquette
    int nbAux; // Variable auxiliere qui aide a cacluler le numero du registre
    while (j<col) {
        printf(" parametre : %s\n", parameters[j]);
        parNb=0;
        if (parameters[j][0]=='r') { // Si l'argument est le registre, on traduit son numero en binaire
            // !!! Gerer le cas ou l'etiquette est sous forme "r..." !!!
            y=1;
            while ((parameters[j][y]!='\0')) {
                nbAux = (parameters[j][y]-48)*pow(10, strlen(parameters[j])-1-y);
                parNb += nbAux;
                y++;
            }
            binNb = decToBin(parNb); // Convertion du numero de registre de decimal en binaire
            if (binNb<0 || binNb>11111) {
                printf("Error : le registre %s n'existe pas\n", parameters[j]);
            } else {
                printf(" parametre translated : %ld\n", binNb);
            }
            
        } else { // Si l'argument est une etiquette, on cherche l'adresse ou elle pointe
            i=0; // Indice auxiliere qui va indiquer sur l'indice de l'etiquette dans le tableau des etiquettes
            int equal = 1; // Variable qui =0 si on a trouve l'indice de l'etiquette dans le tableau des etiquettes
            while (i<NbLines && equal) { // On cherche l'indice de l'etiquette dans le tableau des etiquettes
                y=0;
                while (allTags[i][y]!='\0' && parameters[j][y]!='\0' && allTags[i][y]==parameters[j][y]) {
                    y++;
                }
                if (y==strlen(parameters[j]) && y==strlen(allTags[i])) {
                    equal = 0;
                    parNb = i*4; // On a trouve l'indice de l'etiquette et comme chaque instruction fait 32 bits, soit 4 octets, on multiplie l'indice par 4
                }
                i++;
            }
            if (equal) {
                printf("Error : tag %s n'existe pas\n", parameters[j]);
            } else {
                binNb = decToBin(parNb);
                printf(" tag translated : %ld\n", binNb);
            }
        }
        j++;
    }
}

int main(int argc, char* argv[]) {
    char allTags[NbLines][NbColons]; // La collection de tous les etiquettes contenues dans le fichier
    char line[NbColons]; // Ligne actuelle
    char* TabLines[NbLines]; // Tous les lignes du fichier
    
    FILE* fichier_orig = fopen(argv[1],"r");
    if (fichier_orig == NULL) {
        return 1;
    }
    
    int i = 0; // Indice qui aide a remplir le tableau "TabLines" et des que le fichier est parcouru va indiquer le nombre de ligne dans le fichier source
    int tagsPresence; // Variable qui indique la presence d'une nouvelle etiquette sur une ligne ("ici : ..")
    while (fgets(line, NbColons,fichier_orig)!=NULL) { // On parcourt le fichier pour trouver tous les etiquettes
        TabLines[i] = strdup(line);
        tagsPresence = tagCollect(i, TabLines[i], allTags);
        i++;
    }
    // Verification que tous les etiquettes sont bien stockes
    printf("Tags : ");
    int ind = 0;
    while (ind<i) {
        printf("%s\n", allTags[ind]);
        ind++;
    }
    
    fclose(fichier_orig);
    
    // On initialise on fichier qui va conetnir la traduction de langage machine
    FILE* fichierTranlated = NULL;
    fichierTranlated = fopen("newVersion", "w");
    if (fichierTranlated == NULL) {
        return 1;
    }
    
    char translatedLine[NbColons]; // Nouvelle ligne qui est traduit
    int newI = 0; // Indice avec la quelle on va parcourir le tableau "TabLines", qui va indiquer la ligne ou on se trouve
    while (newI<i) {
        tagsPresence = tagCollect(newI, TabLines[newI], allTags); // On verifie si la ligne contient une initialisation d'une etiquette
        traduction(newI, TabLines[newI], allTags, tagsPresence, translatedLine);
        //fprintf(fichierTranlated, "%s\n", TabLines[i]);
        newI++;
    }

    fclose(fichierTranlated);
    
    return 0;
}

