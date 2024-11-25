#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;

const int MAX_L = 2, MAX_C = 4, MAX_TRIES = 12, MIN_TRIES = 8;

int c_params = 0, params, tableau[MAX_L][MAX_C];
clock_t time_begin;

void initLine();
void showLine(int v_tableau[MAX_C]);
int convertColor(char charac);
void checkLine();
void resetColor();
void endGame(bool winner);
void playerRound();

void textcolor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,color);
}

int main()
{
    /* Initialisation Console + Horloge */
    srand(time(NULL));
    system("color 87");

    cout << "****************[ MASTER MIND ]****************" << endl;
    do // Contrôle de saisie
    {
        cout << "Entrez le nombre d'essais auxquels vous aurez droit (" << MIN_TRIES << " - " << MAX_TRIES << ") : ";
        cin >> params;
        cout << endl;
    }
    while(params < MIN_TRIES || params > MAX_TRIES);
    time_begin = clock(); // On initialise l'horloge pour le temps de jeu à la fin
    c_params = params;
    initLine();
    playerRound();
}
void initLine()
{
    /*
    Procédure initialisant le tableau de couleur
    */
    for(int j = 0; j < MAX_C; j++)
    {
        do
        {
            tableau[0][j] = rand() % 8;
        }
        while(tableau[0][j] == 3 || tableau[0][j] == 5);
    }
    int* a = new int[16];

    delete[] a;
}
void showLine(int v_tableau[MAX_C])
{
    /*
    Procédure affichant une ligne : celle de l'ordinateur ou celle jouée
    */
    for(int i = 0; i < MAX_C; i++)
    {
        textcolor(v_tableau[i] * 16);
        cout << "       ";
        textcolor(8 * 16 + 7);
        cout << " ";
    }
    cout << endl;
}
void endGame(bool winner)
{
    /*
    Procédure terminant le jeu
    */
    if(winner)
    {
        system("color 27");
        system("cls");
        cout << "           ***** Vous remportez la partie ! ******             " << endl;
        cout << "Temps : " << (float) (clock() - time_begin) / CLOCKS_PER_SEC << "s" << endl;
        cout << "Nombre de coups : " << c_params - params + 1 << "/" << c_params << endl;
    }
    else
    {
        system("color 47");
        system("cls");
        cout << "           ***** Vous avez perdu la partie ! ******             " << endl;
        cout << "Temps : " << (float) (clock() - time_begin) / CLOCKS_PER_SEC << "s" << endl;
    }
}
void checkLine()
{
    /*
    Procédure vérifiant la saisie de la ligne en fonction de celle de l'ordinateur
    */
    int count_right = 0,count_bad =0, pos[MAX_C];
    for(int i = 0; i < MAX_C; i++)
    {
        /*
        On vérifie d'abord les couleurs bien placées

        Le tableau pos :
            Il sert à stocker dans un tableau à 4 cases la couleur en fonction de son placement
            Vert => 10  Bien placée
            Bleu => 9 Mal placée
            Rouge => 12 Absente

            Ainsi on pourra vérifier quelles cases ont été vérifiées dans la deuxième boucle
        */
        pos[i] = 0;
        if(tableau[0][i] == tableau[1][i]) // Si c'est bon
        {
            count_right ++;
            pos[i] = 10;
        }

    }
    if(count_right < 4) // Vérification si jamais il n'y a pas tout juste
    {
        for(int i = 0; i < MAX_C; i++)
        {
            /*
            Entrée dans la deuxième boucle qui vérifie le placement de la couleur (absente ou mal placée)
            */
            if(pos[i] == 0) // Si jamais la case n'a pas été vérifiée
            {
                int j = 0;
                while((tableau[1][i] != tableau[0][j] || (pos[j] == 10 || pos[j] == 9)) && j < MAX_C)
                    j++;
                if(j < MAX_C) // Couleur mal placée
                {
                    count_bad ++;
                    pos[i] = 9;
                }
                else // Couleur absente
                    pos[i] = 12;

            }
        }
        params --;
        textcolor(7 * 16);
        cout << "Il y a "<<count_right<< " bonne(s) couleur(s), bien placée(s)"<<endl;
        textcolor(7);
        cout << "Il y a "<<count_bad<< " bonne(s) couleur(s), mal placée(s)"<<endl;
        textcolor(8 * 16 + 7);
        playerRound();
    }
    else
        endGame(true);


}
int convertColor(char charac)
{
    /*
        Procédure convertissant le caractère entré en un entier qu'exploite la console
    */
    switch(charac)
    {
        case 'N':
            return 0;
        case 'C':
            return 1;
        case 'V':
            return 2;
        case 'R':
            return 4;
        case 'J':
            return 6;
        case 'B':
            return 7;
        case 'T':
            return -1;
        default:
            return 3;
    }
}
void playerRound()
{
    char saisie;
    int valid = 0;
    if(params != 0)
    {
        cout << "|-------- Couleurs --------|" << endl;
        cout << "N : Noir" << endl << "C : Cyan" << endl << "V : Vert" << endl << "R : Rouge" << endl << "J : Jaune " << endl << "B : Blanc" << endl << "T : Triche" <<endl;
        do // Bouclage tant que la ligne n'a pas été validée
        {
            for(int i = 0; i < MAX_C; i++) // Chaque couleur de la ligne
            {
                do // Contrôle de saisie
                {
                    cout << "Quelle couleur choisir pour la case n°" << i+1 << " ?" << endl;
                    cin >> saisie;
                    tableau[1][i] = convertColor(saisie);
                    if(tableau[1][i] == -1) // Si on entre le code Triche !
                    {
                        showLine(tableau[0]);
                    }
                }
                while(tableau[1][i] < 0 || tableau[1][i] > 7 || tableau[1][i] == 3 || tableau[1][i] == 5 || tableau[1][i] == -1);
            }
            showLine(tableau[1]);
            cout << "Valider la ligne ? (1 - oui | 0 - non) "<< endl;
            cin >> valid;
        }
        while(valid == 0);
        textcolor(2 * 16 + 7);
        cout << "             Nombre d'essais restants :" << params - 1 << "                 " << endl << endl;
        showLine(tableau[1]);
        cout << "                                               " << endl;
        checkLine();
    }
    else
        endGame(false);
}
