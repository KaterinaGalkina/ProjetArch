// gcc -Wall translate.c
// ./a.out inversion.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NbColons 200
#define NbLines 1000

int tagCollect(int index, char actualLine[], char allTags[NbLines][NbColons]) {
    // Fonction qui parcourt la ligne actuelle et stocke les etiquettes trouves dans le tableau "allTags"
    int tagPres=0;
    for (int i = 0; i < strlen(actualLine); i++) {
        if (actualLine[i] == ':') {
            tagPres = 1;
        }
    }
    int ind = 0;
    if (tagPres) {
        while (actualLine[ind] != ':') {
            int carVal = actualLine[ind];
            if (carVal < 48 || (carVal > 57 && carVal < 65) || (carVal > 90 && carVal < 97) || carVal > 122){
                printf("Error : le format de l'etiquette n'est pas valide le caractere %c n'est pas accepte\n", carVal);
                return 0;
            }
            else {
                allTags[index][ind] = actualLine[ind];
                ind++;
            }
        }
        ind++;
        return 1;
    }
    return 0;
}

void traduction(int index, char actualLine[], char allTags[NbLines][NbColons], int presTag, char translatedLine[NbColons]) {
    char* codeOpVals[32] = {"add", "sub", "mul", "div", "and", "or", "xor", "shl", "", "", "ldb", "ldw", "stb", "stw", "", "", "", "", "","", "jmp", "jzs", "jzc", "jcs", "jcc", "jns", "jnc", "in", "out", "rnd", "", "hlt"}; // Collection des commande du code opératoire
    long translatedLineBin[5]={0,0,0,0,0};
    int ind = 0; // Indice principal avec lequel on parcourt la ligne une fois
    if (presTag) { // On parcourt la ligne actuelle jusque la fin de l'etiquette
        while (actualLine[ind] != ':') {
            ind++;
        }
        ind++;
    }
    while (actualLine[ind] == ' '|| actualLine[ind] == '0' || actualLine[ind] == '1' || actualLine[ind] == '\t') { // On parcourt la ligne actuelle jusqu'a ce qu'on tombe sur une commande
        ind++;
    }
    char command[10] = ""; // Chaine de caractere qui contient la commande de la ligne actuelle
    int j = 0; // Indice auxiliaire pour stocker lettre par lettre la commande actuelle
    while (actualLine[ind] != ' ' && actualLine[ind] != '\0' && actualLine[ind] != '\n') { // stockage de la commande actuelle
        command[j] = actualLine[ind];
        ind++;
        j++;
    }
    
    int i = 0; // indice qui va contenir le numero de la commande
    while (i < 32 && strcmp(codeOpVals[i], command)) { // la recherche du numero de la commande dans la Collection des commande du code opératoire
        i++;
    }
   
    if (i<32) { // Si la commande existe et est trouve on la stocke
        translatedLineBin[0] = i; // GERER LE CAS OU ON A PAS CETTE COMMANDE MAIS ELLE EST QUAND MEME DANS L'INTERVALLE
        
    } else {
        printf("Error : commande %s n'existe pas\n", command);
    }
    
    
    char parameters[3][15] = {"","",""}; // Tableau de caracteres qui contient tous les arguments de la ligne actuelle
    
    while (actualLine[ind] == ' ') {
        ind++;
    }
    int col = 0; // Indice pour stocker le numero de l'argument qu'on est en train de parcourir
    if (translatedLineBin[0] >=10 && translatedLineBin[0] <=11) {
        j = 0; // Indice auxiliaire qui aide a stocker lettre par lettre un parametre qu'on est en train de parcourir
        while (actualLine[ind] != ',' && actualLine[ind] != '\n' && actualLine[ind] != '\0') {
            parameters[col][j] = actualLine[ind];
            ind++;
            j++;
        }
        if (actualLine[ind] != '\n' && actualLine[ind] != '\0') {
            ind += 2;
        }
        col++;
        ind++; // On passe le "("
        j=0;
        while (actualLine[ind] != ')') {
            parameters[col][j] = actualLine[ind];
            ind++;
            j++;
        }
        col++;
        ind++; // On passe le ")"
        j=0;
        while (actualLine[ind] != '\0' && actualLine[ind] != '\n') {
            parameters[col][j] = actualLine[ind];
            ind++;
            j++;
        }
        col++;

        int indiccee = 0;
        while (indiccee<col) {
            printf("param : %s\n", parameters[indiccee]);
            indiccee++;
        }
    }
    else if (translatedLineBin[0] >=12 && translatedLineBin[0] <=13) { // GERER LE CAS OU LES PARANTHESES SONT AU MAUVAIS ENDROIT ; stb (S)rd, rn
    // GERER LE CAS OU PLUSIEURS ESPACES SONT ENTRE PARAMETRES!!!!
        ind++; // On passe le "("
        j=0;
        while (actualLine[ind] != ')') {
            parameters[0][j] = actualLine[ind];
            ind++;
            j++;
        }
        col++;
        ind++; // On passe le ")"
        j=0;
        while (actualLine[ind] != '\0' && actualLine[ind] != '\n' && actualLine[ind] != ',') {
            parameters[2][j] = actualLine[ind];
            ind++;
            j++;
        }
        col++;
        ind+=2; // On passe les caracteres ", "
        j = 0; // Indice auxiliaire qui aide a stocker lettre par lettre un parametre qu'on est en train de parcourir
        while (actualLine[ind] != ',' && actualLine[ind] != '\n' && actualLine[ind] != '\0') {
            parameters[1][j] = actualLine[ind];
            ind++;
            j++;
        }
        if (actualLine[ind] != '\n' && actualLine[ind] != '\0') {
            ind += 2;
        }
        col++;

        int indiccee = 0;
        while (indiccee<col) {
            printf("param : %s\n", parameters[indiccee]);
            indiccee++;
        }
    } else {
        while (actualLine[ind] != '\n' && actualLine[ind] != '\0') { // On stocker tous les argument de la ligne actuelle
            j = 0; // Indice auxiliaire qui aide a stocker lettre par lettre un parametre qu'on est en train de parcourir
            while (actualLine[ind] != ',' && actualLine[ind] != '\n' && actualLine[ind] != '\0') {
                parameters[col][j] = actualLine[ind];
                ind++;
                j++;
            }
            if (actualLine[ind] != '\n' && actualLine[ind] != '\0') {
                ind += 2;
            }
            col++;
        }
    }

    if (((translatedLineBin[0]>=10 && translatedLineBin[0]<=13) || (translatedLineBin[0]>=0 && translatedLineBin[0]<=7) || translatedLineBin[0]==29 || ((translatedLineBin[0]>=10 && translatedLineBin[0]<=13))) && col!=3) {
        printf("Error : La commande %s prend 3 parametres\n", command);
    }
    if ((translatedLineBin[0]>=20 && translatedLineBin[0]<=28) && col!=1) {
        printf("Error : La commande %s prend 1 parametre\n", command);
    }
    if (translatedLineBin[0]==31  && col!=0) {
        printf("Error : La commande %s prend 0 parametres\n", command);
    }
    // "col" contient le nombre d'arguments
    j = 0; // Le numero d'argument qu'on est en train de parcourir (j<col)

    while (j < col) {
        int y; // Indice auxiliaire pour comparaison des lettres
        int parNb = 0; // Variable auxiliaire pour recuperer la valeur decimale du numero du parametre ou de l'adrese ou pointe l'etiquette
        int nbAux; // Variable auxiliaire qui aide a calculer la valeur immediate
        if (parameters[j][0] == '#'){
            if (j!=2) {
                printf("Error : valeur immediate doit etre passe en 2e source \n");
            }
            y = 1;
            while ((parameters[j][y] != '\0')) {
                nbAux = (parameters[j][y] - 48) * pow(10, strlen(parameters[j])- 1 - y);    // convertir le caractere ascii en entier (0 car = 48 en decimal)
                parNb += nbAux;
                y++;
            }
            printf(" immediate value translated : %d\n", parNb);
            translatedLineBin[3] = 1;
            translatedLineBin[4] = parNb;
        }
        else if (parameters[j][0]=='r') { // Si l'argument est le registre, on traduit son numero en binaire
            // !!! Gerer le cas ou l'etiquette est sous forme "r..." !!!
            y = 1;
            while ((parameters[j][y]!='\0')) {
                nbAux = (parameters[j][y]-48)*pow(10, strlen(parameters[j])-1-y);    // convertir le caractere ascii en entier (0 car = 48 en decimal)
                parNb += nbAux;
                y++;
            }
            if (j!=2) {
                translatedLineBin[j+1] = parNb;
            } else {
                translatedLineBin[3] = 0;
                translatedLineBin[4] = parNb;
            }
            if (parNb < 0 || parNb > 31) {
                printf("Error : le registre %s n'existe pas\n", parameters[j]);
            } 
            
        } else { // Si l'argument est une etiquette, on cherche l'adresse ou elle pointe
            i = 0; // Indice auxiliaire qui va indiquer sur l'indice de l'etiquette dans le tableau des etiquettes
            int equal = 1; // Variable qui =0 si on a trouve l'indice de l'etiquette dans le tableau des etiquettes
            while (i < NbLines && equal) { // On cherche l'indice de l'etiquette dans le tableau des etiquettes
                y = 0;
                while (allTags[i][y]!='\0' && parameters[j][y]!='\0' && allTags[i][y]==parameters[j][y]) {
                    y++;
                }
                if (y == strlen(parameters[j]) && y == strlen(allTags[i])) {
                    equal = 0;
                    parNb = i*4; // On a trouve l'indice de l'etiquette et comme chaque instruction fait 32 bits, soit 4 octets, on multiplie l'indice par 4
                }
                i++;
            }
            if (equal) {
                printf("Error : tag %s n'existe pas\n", parameters[j]);
            } else {
                translatedLineBin[3] = 1;
                translatedLineBin[4] = parNb;
            }
        }
        j++;
    }
    int nbAux2 = (translatedLineBin[0]<<27) + (translatedLineBin[1]<<22) + (translatedLineBin[2]<<17) + (translatedLineBin[3]<<16) + translatedLineBin[4];
    printf("Traduction : %x\n", nbAux2);

    int indxx = 0;
    while (indxx < 5) {
        printf("%ld\n", translatedLineBin[indxx]);
        indxx++;
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
    
    int nbLines = 0; // Indice qui aide a remplir le tableau "TabLines" et des que le fichier est parcouru va indiquer le nombre de ligne dans le fichier source
    int tagsPresence; // Variable qui indique la presence d'une nouvelle etiquette sur une ligne ("ici : ..")
    while (fgets(line, NbColons,fichier_orig)!=NULL) { // On parcourt le fichier pour trouver tous les etiquettes
        TabLines[nbLines] = strdup(line);
        tagsPresence = tagCollect(nbLines, TabLines[nbLines], allTags);
        nbLines++;
    }
    // Verification que tous les etiquettes sont bien stockes
    printf("Tags : ");
    int ind = 0;
    while (ind < nbLines) {
        printf("%s\n", allTags[ind]);
        ind++;
    }
    
    fclose(fichier_orig);
    
    // On initialise on fichier qui va conetnir la traduction de langage machine
    FILE* fichierTranlated = NULL;
    fichierTranlated = fopen("newVersion.txt", "w");
    if (fichierTranlated == NULL) {
        return 1;
    }
    
    char translatedLine[NbColons]; // Nouvelle ligne qui est traduit
    int newI = 0; // Indice avec la quelle on va parcourir le tableau "TabLines", qui va indiquer la ligne ou on se trouve
    while (newI < nbLines) {
        tagsPresence = tagCollect(newI, TabLines[newI], allTags); // On verifie si la ligne contient une initialisation d'une etiquette
        traduction(newI, TabLines[newI], allTags, tagsPresence, translatedLine);
        //fprintf(fichierTranlated, "%s\n", TabLines[nbLines]);
        newI++;
    }

    fclose(fichierTranlated);
    
    return 0;
}





