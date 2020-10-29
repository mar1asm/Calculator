#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<dos.h>
#include<windows.h>
#include<winbgim.h>
using namespace std;

void prelucrare_initiala_intrebare(string&); //M

void initializare();                                //M
void desparte_intrebarea(string intrebare);         //M
int semnificatie_cuvant(string cuvant);             //M
long long transformare_numar(string numar);         //M
char simbol_operatie(string nume_operatie);         //M
void transforma(long long numar);                   //C
void transforma_ultimele_doua(int numar, int caz);   //C
void transforma_ultima(int numar);                  //C
void pune(char s[]);                                //C
void fisierr();                                     //C

void initializare_en();                             //M
void desparte_intrebarea_en(string intrebare);      //M
long long transformare_numar_en(string numar);      //M
int semnificatie_cuvant_en(string cuvant);          //M
char simbol_operatie_en(string nume_operatie);      //M
string cautare_numar(int numar, int inceput, int sfarsit); //M
void transformare_din_cifre_in_litere_en(long long numar); //M

void adaugare_paranteze(char operatii[], int& numar_operatii, int semnificatie_cuv[], int indice_cuvant, int numar_numere); //M
void calcul(long long numere_in_cifre[], int& indice_numar, char simboluri[], int& indice_operator, int limba);            //C
int corect(long long numere_in_cifre[], int indice_numar, char simboluri[], int indice_operator);                            //C
long long calcul_total(long long numere_in_cifre[], int& indice_numar, char simboluri[], int& indice_operator, int prioritate[]);  //C
void calcul_partial(long long numere_in_cifre[], char simboluri[], int i, int& indice_numar, int& indice_operator, int prioritate[]); //C
void stabileste_prioritate(char simboluri[], int indice_operator, int prioritate[]);                                             //C

void desparte_rezultatul(char sir[100]);               //C
void waitforClick(int& mx, int& my);                     //C
void dreptunghi(int x, int y, int lat);                //C
void butoaneMeniu(int& status, int& culoare);           //C
void butoaneFereastraCalcul(int& status, int& culoare); //C
void citesteInModGrafic(char text[200], int x, int y); //C
void interfataGrafica(int& status, int& culoare);       //C


struct    //structura in care retii numerele in cifre, respectiv litere
{
    int cifre;
    string litere;
} litere_cifre[31];

struct         //structura in care retii cuvintele suta, mie, milion in cifre, respectiv litere
{
    int cifre;
    string litere;
} litere_cifre_multiplicare[7];

struct         //structura in care retii simbolurile operatorilor, respectiv numele lor
{
    char simbol;
    string nume[7];
} operatori[10];


struct
{
    int cifre;
    string litere;
} litere_cifre_en[29];

struct
{
    int cifre;
    string litere;
} litere_cifre_multiplicare_en[3];

struct
{
    char simbol;
    string nume[7];
} operatori_en[10];
char text[200], rezultat_in_litere[100], rezultat_in_litere1[100], rezultat_in_litere2[100], rezultat_in_litere3[100];
int status = 0;
int main()
{

    int culoare = 3;
    initializare_en();
    initializare();
    fisierr();
    interfataGrafica(status, culoare);
}


void prelucrare_initiala_intrebare(string& intrebare)  //elimin caracterele "?.," si transform toate majusculele in minuscule, in plus pentru limba engleza
{
    //elimin si caracterul "-"
    for (unsigned int i = 0; i < intrebare.size(); i++)
        if (isalpha(intrebare[i]))
            intrebare[i] = tolower(intrebare[i]);
    string caractere_de_eliminat = "?,-";
    for (unsigned int i = 0; i < caractere_de_eliminat.size(); i++)
    {
        size_t p = intrebare.find(caractere_de_eliminat[i]);
        while (p != string::npos)
        {
            if ((intrebare[p - 1] == ' ' && intrebare[p + 1] == ' ') || ((int)(intrebare[p + 1]) - '0' >= 0 && (int)(intrebare[p + 1]) - '0' <= 9))
                p++;
            else
                intrebare[p] = ' ';
            p = intrebare.find(caractere_de_eliminat[i], p);
        }
    }
}


void initializare()      //functie care citeste din fisier si pune in structuri
{
    ifstream f("initializare.ro");
    for (int k = 0; k < 31; k++)
        f >> litere_cifre[k].cifre >> litere_cifre[k].litere;
    for (int k = 0; k < 7; k++)
        f >> litere_cifre_multiplicare[k].cifre >> litere_cifre_multiplicare[k].litere;
    char temp = '+';
    int i = 0, j = 0;
    while (!f.eof())
    {
        char c;
        f >> c;
        if (c != temp)
        {
            i++;
            j = 0;
            temp = c;
        }
        operatori[i].simbol = temp;
        f >> operatori[i].nume[j++];
    }
}

int semnificatie_cuvant(string cuvant)  //functie ce returneaza 1 daca cuvantul gasit este parte a unui numar, 2 daca este operatie matematica,
{
    //3 daca cuvantul este "si"(pentru a putea decide dupa daca este parte din numar sau cuvant de legatura), 0 in restul
    if (isdigit(cuvant[0]) || (cuvant[0] == '-' && cuvant.size() != 1) || cuvant == "de")
        return 1;
    if (cuvant == "si")
        return 3;
    if (cuvant == "totul")
        return 2;
    for (int i = 0; i < 31; i++)
        if (cuvant == litere_cifre[i].litere)
            return 1;
    for (int i = 0; i < 7; i++)
        if (cuvant == litere_cifre_multiplicare[i].litere)
            return 1;
    for (int i = 0; i < 9; i++)
    {
        int j = 0;
        while (operatori[i].nume[j].size() != 0)
            if (operatori[i].nume[j++] == cuvant || (operatori[i].simbol == cuvant[0] && cuvant.size() == 1))
                return 2;
    }
    return 0;
}

char simbol_operatie(string nume_operatie)  //functie ce primeste numele operatiei si returneaza simbolul corespunzator
{
    for (int i = 0; i <= 9; i++)
    {
        int j = 0;
        while (operatori[i].nume[j].size() != 0)
            if (operatori[i].nume[j++] == nume_operatie || operatori[i].simbol == nume_operatie[0])
                return operatori[i].simbol;
    }
    return 0;
}

void desparte_intrebarea(string intrebare)   //functia care primeste intrebarea (string) si o desparte folosind functia de mai sus
{
    stringstream stream_intrebare(intrebare);
    string cuvant_din_intrebare;
    bool numar_negativ = false;

    char simboluri_operatii[20];
    long long numere_in_cifre[10];
    int semnif_cuvant[30], indice_numar = 0, indice_operator = -1, indice_cuvant = -1;
    string numere[10], operatori_din_propozitie[10], ultimul_operator, ultimul_cuvant;
    while (stream_intrebare >> cuvant_din_intrebare)
    {
        switch (semnificatie_cuvant(cuvant_din_intrebare))
        {
        case 1:   // daca cuvantul este parte dintr-un numar
            semnif_cuvant[++indice_cuvant] = 1;
            if (numere[indice_numar].size() == 0)
                numere[indice_numar] += cuvant_din_intrebare;
            else
                numere[indice_numar] += " " + cuvant_din_intrebare;
            break;
        case 2:    // tratez cazurile in care cuvantul minus este parte din numar, nu operatie matematica
            if (cuvant_din_intrebare == "minus" && (semnificatie_cuvant(ultimul_cuvant) == 2 || indice_cuvant == -1 || ultimul_cuvant == "si"))
            {
                numar_negativ = true;
                if (semnif_cuvant[indice_cuvant] == 1)
                    indice_numar++;
                continue;
            }
            if (numar_negativ == true)
            {
                numere[indice_numar] = "minus " + numere[indice_numar];
                numar_negativ = false;
            }
            if (semnif_cuvant[indice_cuvant] == 1)
                indice_numar++;
            semnif_cuvant[++indice_cuvant] = 2;
            if (ultimul_operator == "restul" || ultimul_operator == "catul")
            {
                ultimul_operator = cuvant_din_intrebare;
                continue;
            }
            operatori_din_propozitie[++indice_operator] += cuvant_din_intrebare;
            ultimul_operator = cuvant_din_intrebare;
            break;
        case 0:
            if (numar_negativ == true)
            {
                numere[indice_numar] = "minus " + numere[indice_numar];
                numar_negativ = false;
            }
            if (semnif_cuvant[indice_cuvant] == 1)
                indice_numar++;
            semnif_cuvant[++indice_cuvant] = 0;
            break;
        default:
            if (semnif_cuvant[indice_cuvant] == 1 && numere[indice_numar].find("zeci") != string::npos)  // decid daca cuvantul "si" e parte din numar
            {
                semnif_cuvant[++indice_cuvant] = 1;
                break;
            }
            if (semnif_cuvant[indice_cuvant] == 1)
                indice_numar++;
            semnif_cuvant[++indice_cuvant] = 3;
            break;
        }
        ultimul_cuvant = cuvant_din_intrebare;
    }

    if (numar_negativ == true)  //verific daca ultimul numar este negativ
    {
        numere[indice_numar] = "minus " + numere[indice_numar];
        numar_negativ = false;
    }

    if (semnif_cuvant[indice_cuvant] == 1)
        indice_numar++;
    indice_operator++;

    for (int i = 0; i < indice_numar; i++)  //transform numerele din litere in cifre
        numere_in_cifre[i] = transformare_numar(numere[i]);
    for (int i = 0; i < indice_operator; i++)  //in simboluri_operatii retin simbolurile operatiilor in ordinea in care apar
        simboluri_operatii[i] = simbol_operatie(operatori_din_propozitie[i]);

    adaugare_paranteze(simboluri_operatii, indice_operator, semnif_cuvant, indice_cuvant, indice_numar);
    calcul(numere_in_cifre, indice_numar, simboluri_operatii, indice_operator, 1);
}

long long transformare_numar(string numar)  // functie ce transforma numarul din litere in cifre
{
    stringstream cuv(numar);
    string cuvant;
    long long numar_format = 0;
    long long numar_format_temp = 0;
    bool adunare = false;
    bool numar_negativ = false;
    if (isdigit(numar[0]) || numar[0] == '-')    //cazul in care numarul este deja in cifre
    {
        for (unsigned int i = 0; i < numar.size(); i++)
        {
            if (numar[0] == '-' && !numar_negativ)
            {
                numar_negativ = true;
                continue;
            }
            numar_format = numar_format * 10 + (int)numar[i] - '0';
        }
        if (numar_negativ)
            return -numar_format;
        return numar_format;
    }
    while (cuv >> cuvant)
    {
        if (cuvant == "minus")
        {
            numar_negativ = true;
            continue;
        }
        bool gasit = false;
        for (unsigned int i = 0; i < 7; i++)
        {
            if (litere_cifre_multiplicare[i].litere == cuvant)
            {
                numar_format_temp *= litere_cifre_multiplicare[i].cifre;
                if (i > 2)
                    adunare = true;
                gasit = true;
                break;
            }
        }
        if (!gasit)
            for (unsigned int i = 0; i < 31; i++)
            {
                if (litere_cifre[i].litere == cuvant)
                {
                    if (adunare)
                    {
                        numar_format += numar_format_temp;
                        numar_format_temp = 0;
                        adunare = 0;
                    }
                    numar_format_temp += litere_cifre[i].cifre;
                    break;
                }
            }
    }
    numar_format += numar_format_temp;
    if (numar_negativ)
        numar_format = 0 - numar_format;
    return numar_format;
}

struct
{
    int cifre;
    char litere[20];
} litere_cifre_char[31];
char s[100];

void pune(char s[])  // va pune sirul dat la inceputul sirului cu rezultatul
{
    strcat(s, " ");
    strcat(s, rezultat_in_litere);
    strcpy(rezultat_in_litere, s);
}

void fisierr()
{
    ifstream f("initializare.ro");
    for (int k = 0; k < 31; k++)
        f >> litere_cifre_char[k].cifre >> litere_cifre_char[k].litere;
}
void transforma_ultima(int numar) //functie folosita pentru a scrie prima cifra din fiecare grupa de cate 3 cifre din care se poate forma numarul
{
    if (numar % 10 != 0);
    {
        int i, x = numar % 10, ok = 0;

        if (x == 1)
        {
            strcpy(s, "suta");
            pune(s);
            strcpy(s, "o");
            pune(s);
        }
        if (x == 2)
        {
            strcpy(s, "sute");
            pune(s);
            strcpy(s, "doua");
            pune(s);
        }
        if (x != 1 && x != 2)
            for (i = 6; i <= 12; i++)
                if (litere_cifre_char[i].cifre == x)
                {
                    ok = 1;
                    break;
                }
        if (ok)
        {
            strcpy(s, "sute");
            pune(s);
            strcpy(s, litere_cifre_char[i].litere);
            pune(s);
        }

    }
}

void transforma_ultimele_doua(long long numar, int caz)
{
    if (caz == 2 && numar % 1000 == 1)
    {
        strcpy(s, "o mie");
        pune(s);
    }
    else if (numar != 0) //daca numarul este 0 atunci functia nu scrie nimic
    {
        int ok1 = 0, ok2 = 0, i;
        int x = numar % 100;
        for (i = 13; i <= 30; i++)  //numarele de la 10 la 19 plus numerele 20,30,40,50,60,70,80,90
            if (litere_cifre_char[i].cifre == x)
            {
                ok1 = 1;
                break;  //dam break pentru a retine i atunci cand litere_cifre_char[i].cifre==x
            }
        if (ok1)
        {
            if (caz == 2)
            {
                if (i < 23)  //daca numarul apartine 10 , 19
                {
                    strcpy(s, "mii");
                    pune(s);
                }
                else  //daca nuumarul este 20,30,40,50,60,70,80 sau 90
                    if (x != 0)
                    {
                        strcpy(s, "de mii");
                        pune(s);
                    }
            }
            else if (caz == 3)
            {
                if (i < 23)
                {
                    strcpy(s, "milioane");
                    pune(s);
                }
                else                          //aici nu mai verificam daca ultimele doua numere din grupa sunt diferite de 0
                {
                    //daca toate numerele din grupa ar fi fost 0 atunci functia s-ar fi oprit la linia 71
                    strcpy(s, "de milioane");  // daca ultimele2 cifre din grupa sunt 0 inseamna ca prima este diferita de 0, caz in
                    pune(s);                  //care va trebui sa afisam "de milioane"
                }
            }
            strcpy(s, litere_cifre_char[i].litere);
            pune(s);
        }
        if (ok1 == 0)  //daca ultimele doua cifre nu se scriu legat atunci va trebui sa le luam separat
        {
            x = numar % 10;  //ultima cifra a numarului
            for (i = 6; i <= 12; i++)  // comparam cu cifrele de la 3 la 9
                if (litere_cifre_char[i].cifre == x)
                {
                    ok2 = 1;
                    break;
                }
            if (ok2) //daca avem ultima cifra de la 3 la 9
            {
                if (caz == 2)
                {
                    if (numar % 100 > 10)
                        strcpy(s, "de mii"); //daca cifra zecilor este mai mare decat 1 atunci vom pune "de mii"
                    else
                        strcpy(s, "mii");
                    pune(s);
                }
                else
                {
                    if (caz == 3)
                    {
                        if (numar % 100 > 10)
                            strcpy(s, "de milioane"); //la fel ca mai sus
                        else
                            strcpy(s, "milioane");
                        pune(s);
                    }
                }
                strcpy(s, litere_cifre_char[i].litere);
                pune(s);
            }
            else //daca ultima cifra este 1,2 sau 0(daca este 0 nu afisam nimic)
            {
                if (caz == 2)
                {
                    if (numar % 100 > 10 || numar % 1000 > 99)
                    {
                        strcpy(s, "de mii");
                        pune(s);
                    } //daca cifra zecilor este mai mare decat 1 atunci vom pune "de mii"
                    else if (numar % 1000 != 0)
                    {
                        strcpy(s, "mii");
                        pune(s);
                    }
                }
                else
                {
                    if (caz == 3)
                    {
                        if (numar % 100 > 10 || numar % 1000 > 99)
                            strcpy(s, "de milioane");
                        else
                            strcpy(s, "milion");
                        pune(s);
                    }
                }
                if (x == 1)
                {
                    if (caz == 3 && numar == 1)
                        strcpy(s, "un"); //atunci cand avem un milion...
                    else
                        strcpy(s, "unu");
                    pune(s);
                }
                if (x == 2 && caz == 1)
                {
                    strcpy(s, "doi");
                    pune(s);
                }
                if (x == 2 && (caz == 2 || caz == 3))
                {
                    strcpy(s, "doua");
                    pune(s);
                }
            }
            x = numar % 100 / 10 * 10; // cifra zecilor
            for (i = 23; i < 31; i++)
                if (litere_cifre_char[i].cifre == x)
                {
                    strcpy(s, "si");  //intre cifra zecilor si cifra unitatilor dintr-o grupa vom pune "si"
                    pune(s);
                    strcpy(s, litere_cifre_char[i].litere);
                    pune(s);
                }
        }


    }
}

void transforma(long long numar)
{
    rezultat_in_litere[0] = '\000';
    int numarulEsteNegativ = 0;
    if (numar < 0)
    {
        numarulEsteNegativ = 1;
    }
    numar = abs(numar);
    int caz = 1; // caz este o variabila care ne spune in ce grupa din cele trei ale numarului suntem (am impartit numarul in grupe de cate 3 cifre)
    if (numar == 0)
    {
        strcpy(s, "zero");
        pune(s);
        caz == 4;
    }
    while (caz <= 3)
    {
        transforma_ultimele_doua(numar, caz);
        numar = numar / 100;
        if (numar != 0)
            transforma_ultima(numar % 10);
        numar = numar / 10;
        caz++;
    }

    if (numarulEsteNegativ)
    {
        strcpy(s, "minus");
        pune(s);
    }
}

//aici sunt functiile pentru intrebarile in limba engleza

void initializare_en()
{
    ifstream f("initializare.en");
    for (int k = 0; k < 29; k++)
        f >> litere_cifre_en[k].cifre >> litere_cifre_en[k].litere;
    for (int k = 0; k < 3; k++)
        f >> litere_cifre_multiplicare_en[k].cifre >> litere_cifre_multiplicare_en[k].litere;
    char temp = '+';
    int i = 0, j = 0;
    while (!f.eof())
    {
        char c;
        f >> c;
        if (c != temp)
        {
            i++;
            j = 0;
            temp = c;
        }
        operatori_en[i].simbol = temp;
        f >> operatori_en[i].nume[j++];
    }
}

long long transformare_numar_en(string numar)
{
    string cuv;
    long long numar_format = 0;
    long long numar_format_temp = 0;
    bool adunare = false;
    bool numar_negativ = false;
    stringstream cuvinte(numar);
    if (isdigit(numar[0]) || numar[0] == '-')    //cazul in care numarul este deja in cifre
    {
        for (unsigned int i = 0; i < numar.size(); i++)
        {
            if (numar[0] == '-' && !numar_negativ)
            {
                numar_negativ = true;
                continue;
            }
            numar_format = numar_format * 10 + (int)numar[i] - '0';
        }
        if (numar_negativ)
            return -numar_format;
        return numar_format;
    }
    while (cuvinte >> cuv)
    {
        if (cuv == "minus")
        {
            numar_negativ = true;
            continue;
        }
        bool gasit = 0;
        for (unsigned int i = 0; i < 3; i++)
        {
            if (litere_cifre_multiplicare_en[i].litere == cuv)
            {
                numar_format_temp *= litere_cifre_multiplicare_en[i].cifre;
                if (i)
                    adunare = true;
                gasit = 1;
                break;
            }
        }
        if (!gasit)
            for (unsigned int i = 0; i < 29; i++)
            {
                if (litere_cifre_en[i].litere == cuv)
                {
                    if (adunare)
                    {
                        numar_format += numar_format_temp;
                        numar_format_temp = 0;
                        adunare = 0;
                    }
                    numar_format_temp += litere_cifre_en[i].cifre;
                    break;
                }
            }
    }
    numar_format += numar_format_temp;
    if (numar_negativ)
        numar_format = 0 - numar_format;
    return numar_format;
}

int semnificatie_cuvant_en(string cuvant)
{
    if (isdigit(cuvant[0]) || (cuvant[0] == '-' && cuvant.size() != 1))
        return 1;
    if (cuvant == "all")
        return 2;
    if (cuvant == "and")
        return 3;
    for (int i = 0; i < 29; i++)
        if (cuvant == litere_cifre_en[i].litere)
            return 1;
    for (int i = 0; i < 3; i++)
        if (cuvant == litere_cifre_multiplicare_en[i].litere)
            return 1;
    for (int i = 0; i < 6; i++)
    {
        int j = 0;
        while (operatori_en[i].nume[j].size() != 0)
            if (operatori_en[i].nume[j++] == cuvant || (operatori_en[i].simbol == cuvant[0] && cuvant.size() == 1))
                return 2;
    }
    return 0;
}

void desparte_intrebarea_en(string intrebare)   //functia care primeste intrebarea (string) si o desparte folosind functia de mai sus
{
    stringstream stream_intrebare(intrebare);
    string cuvant_din_intrebare;
    bool numar_negativ = false;

    char simboluri_operatii[20];
    long long numere_in_cifre[10];
    int semnif_cuvant[30], indice_numar = 0, indice_operator = -1, indice_cuvant = -1;
    string numere[10], operatori_din_propozitie[10], ultimul_operator, ultimul_cuvant;
    while (stream_intrebare >> cuvant_din_intrebare)
    {
        switch (semnificatie_cuvant_en(cuvant_din_intrebare))
        {
        case 1:   // daca cuvantul este parte dintr-un numar
            semnif_cuvant[++indice_cuvant] = 1;
            if (numere[indice_numar].size() == 0)
                numere[indice_numar] += cuvant_din_intrebare;
            else
                numere[indice_numar] += " " + cuvant_din_intrebare;
            break;
        case 2:    // tratez cazurile in care cuvantul minus este parte din numar, nu operatie matematica
            if (cuvant_din_intrebare == "minus" && (semnificatie_cuvant_en(ultimul_cuvant) == 2 || indice_cuvant == -1 || ultimul_cuvant == "and"))
            {
                numar_negativ = true;
                if (semnif_cuvant[indice_cuvant] == 1)
                    indice_numar++;
                continue;
            }
            if (numar_negativ == true)
            {
                numere[indice_numar] = "minus " + numere[indice_numar];
                numar_negativ = false;
            }
            if (semnif_cuvant[indice_cuvant] == 1)
                indice_numar++;
            semnif_cuvant[++indice_cuvant] = 2;
            if (ultimul_operator == "remainder" || ultimul_operator == "modulo")
            {
                ultimul_operator = cuvant_din_intrebare;
                continue;
            }
            operatori_din_propozitie[++indice_operator] += cuvant_din_intrebare;
            ultimul_operator = cuvant_din_intrebare;
            break;
        case 0:
            if (numar_negativ == true)
            {
                numere[indice_numar] = "minus " + numere[indice_numar];
                numar_negativ = false;
            }
            if (semnif_cuvant[indice_cuvant] == 1)
                indice_numar++;
            semnif_cuvant[++indice_cuvant] = 0;
            break;
        default:
            if (semnif_cuvant[indice_cuvant] == 1)
                indice_numar++;
            semnif_cuvant[++indice_cuvant] = 3;
            break;
        }
        ultimul_cuvant = cuvant_din_intrebare;
    }
    if (numar_negativ == true)  //verific daca ultimul numar este negativ
    {
        numere[indice_numar] = "minus " + numere[indice_numar];
        numar_negativ = false;
    }
    if (semnif_cuvant[indice_cuvant] == 1)
        indice_numar++;
    indice_operator++;

    for (int i = 0; i < indice_numar; i++)  //transform numerele din litere in cifre
        numere_in_cifre[i] = transformare_numar_en(numere[i]);
    for (int i = 0; i < indice_operator; i++)  //in simboluri_operatii retin simbolurile operatiilor in ordinea in care apar
        simboluri_operatii[i] = simbol_operatie_en(operatori_din_propozitie[i]);

    adaugare_paranteze(simboluri_operatii, indice_operator, semnif_cuvant, indice_cuvant, indice_numar);
    calcul(numere_in_cifre, indice_numar, simboluri_operatii, indice_operator, 2);
}

char simbol_operatie_en(string nume_operatie)
{
    for (int i = 0; i <= 5; i++)
    {
        int j = 0;
        while (operatori_en[i].nume[j].size() != 0)
            if (operatori_en[i].nume[j++] == nume_operatie || operatori_en[i].simbol == nume_operatie[0])
                return operatori_en[i].simbol;
    }
    return 0;
}

string cautare_numar(int numar, int inceput, int sfarsit)  //functie auxiliara pentru functia de mai jos, returneaza echivalentul in litere al unei cifre
{
    for (int i = inceput; i <= sfarsit; i++)
        if (litere_cifre_en[i].cifre == numar)
            return litere_cifre_en[i].litere;
    return 0;
}

void transformare_din_cifre_in_litere_en(long long numar)  //functie ce transforma rezultatul din cifre in litere
{
    int numarulEsteNegativ = 0;
    if (numar < 0)
    {
        numarulEsteNegativ = 1;
        numar = abs(numar);
    }
    if (!numar)
    {
        strcpy(s, "zero");
        pune(s);
    }
    else
    {
        string numar_format, numar_format_partial;
        int ordin = 0;
        while (numar)
        {
            int temp = numar % 1000;        //pentru a transforma numarul din cifre in litere il impart in grupuri de cate 3 cifre si adaug, in functie de caz
            numar /= 1000;                //cuvintele "thousand" sau "million"
            if (temp / 100)
            {
                if (numar_format_partial.back() != ' ' && numar_format_partial.size() != 0)
                    numar_format_partial += " ";
                numar_format_partial += cautare_numar(temp / 100, 1, 10);
                numar_format_partial = numar_format_partial + " " + "hundred";
            }
            temp %= 100;
            if (temp && temp < 20)
            {
                if (numar_format_partial.back() != ' ' && numar_format_partial.size() != 0)
                    numar_format_partial += " ";
                numar_format_partial += cautare_numar(temp, 1, 20);
            }
            else if (temp && temp >= 20)
            {
                int zeci = temp / 10 * 10;
                if (numar_format_partial.back() != ' ' && numar_format_partial.size() != 0)
                    numar_format_partial += " ";
                numar_format_partial += cautare_numar(zeci, 21, 28);
                temp %= 10;
                if (temp)
                {
                    if (numar_format_partial.back() != ' ' && numar_format_partial.size() != 0)
                        numar_format_partial += "-";
                    numar_format_partial += cautare_numar(temp, 1, 10);

                }
            }
            switch (ordin)
            {
            case 1:
                numar_format_partial += " thousand";
                break;
            case 2:
                numar_format_partial += " million";
                break;
            default:
                break;
            }
            ordin++;
            if (numar_format_partial.back() != ' ' && numar_format_partial.size() != 0 && numar_format.size() != 0)
                numar_format_partial += " ";
            numar_format = numar_format_partial + numar_format;
            numar_format_partial = "";
        }
        strcpy(rezultat_in_litere, numar_format.c_str());
        if (numarulEsteNegativ)
        {
            strcpy(s, "minus");
            pune(s);
        }

    }
}


// aici incepe partea de calcul
void adaugare_paranteze(char operatii[], int& numar_operatii, int semnif_cuv[], int numar_cuvinte, int numar_numere)
{

    int operatie_termeni[30][2] = { 0 }, operatie_termeni_indice = -1, semnificatie_cuv[30], numar_cuvinte_final = -1, indicele_operatiei = -1;
    char operatii_final[30];
    int indice_operatii_final = -1;
    int numar_de_operatii = 0;
    for (int i = 0; i < numar_operatii; i++)
        if (operatii[i] == '+' || operatii[i] == '-' || operatii[i] == '*' || operatii[i] == '/' || operatii[i] == '%')
            numar_de_operatii++;
    if (numar_operatii >= numar_numere)
        return;
    for (int i = 0; i <= numar_cuvinte; i++)    //elimin toate celelalte cuvinte in afara de numere si operatori si cuvantul "si/and"
    {
        //si consider numerele compuse ca fiind un singur cuvant
        switch (semnif_cuv[i])
        {
        case 1:
            semnificatie_cuv[++numar_cuvinte_final] = 1;
            while (semnif_cuv[i + 1] == 1)
                i++;
            break;
        case 2:
            semnificatie_cuv[++numar_cuvinte_final] = 2;
            break;
        case 3:
            semnificatie_cuv[++numar_cuvinte_final] = 3;
            break;
        default:
            break;

        }
    }
    if (numar_numere - 1 > numar_operatii&& semnificatie_cuv[0] == 2 && numar_operatii == 1)   //daca este o intrebare de tipul "suma dintre a, b, c si d"
    {
        for (int i = 1; i < numar_numere - 1; i++)
            operatii[i] = operatii[i - 1];
        numar_operatii = numar_numere - 1;
        return;
    }

    numar_cuvinte_final++;
    for (int i = 0; i < numar_cuvinte_final; i++)   //adaug paranteze deschide pentru operatorii care au ambele numere dupa ei. ex: suma dintre 5 si 4
    {
        switch (semnificatie_cuv[i])
        {
        case 1:
            if (semnificatie_cuv[i + 1] == 2 && semnificatie_cuv[i + 2] == 1 && operatie_termeni[operatie_termeni_indice][1] != 1)
            {
                operatii_final[++indice_operatii_final] = operatii[++indicele_operatiei];
                operatie_termeni[operatie_termeni_indice + 1][1]++;
                i += 2;
                break;
            }
            if ((i < numar_cuvinte_final - 1) && (semnificatie_cuv[i + 1] == 2) && (operatie_termeni[operatie_termeni_indice][1] == 0 || !i))
            {
                operatie_termeni[operatie_termeni_indice + 1][1]++;
            }
            else
            {
                operatie_termeni[operatie_termeni_indice][1]++;
                while (operatie_termeni[operatie_termeni_indice][1] == 2)
                {
                    operatii_final[++indice_operatii_final] = operatii[operatie_termeni[operatie_termeni_indice][0]];
                    operatie_termeni[operatie_termeni_indice][1] = 0;
                    operatie_termeni_indice--;
                    if (semnificatie_cuv[i + 1] == 2)
                        operatie_termeni[operatie_termeni_indice + 1][1]++;
                    operatie_termeni[operatie_termeni_indice][1]++;
                }
            }
            break;

        case 2:
            if (operatii[indicele_operatiei + 1] == ')')
            {
                ++indicele_operatiei;
                operatii_final[++indice_operatii_final] = ')';
                break;
            }
            operatie_termeni[++operatie_termeni_indice][0] = ++indicele_operatiei;
            if (operatie_termeni[operatie_termeni_indice][1] == 0)
                operatii_final[++indice_operatii_final] = '(';
            break;

        default:
            operatie_termeni[operatie_termeni_indice][1]++;
            while (operatie_termeni[operatie_termeni_indice][1] == 2)
            {
                operatii_final[++indice_operatii_final] = operatii[operatie_termeni[operatie_termeni_indice][0]];
                operatie_termeni[operatie_termeni_indice][1] = 0;
                operatie_termeni_indice--;
                operatie_termeni[operatie_termeni_indice][1]++;
            }
            break;

        }
    }
    numar_operatii = indice_operatii_final + 1;
    for (int i = 0; i < numar_operatii; i++)
        operatii[i] = operatii_final[i];
}

void stabileste_prioritate(char simboluri[], int indice_operator, int prioritate[])
{
    for (int i = 0; i < indice_operator; i++)
        if (simboluri[i] == '+' || simboluri[i] == '-')
            prioritate[i] = 1;
        else if (simboluri[i] == '*' || simboluri[i] == '/' || simboluri[i] == '%')
            prioritate[i] = 2;
}
void calcul_partial(long long numere_in_cifre[], char simboluri[], int i, int& indice_numar, int& indice_operator, int prioritate[])
{
    int z = i - 1;
    int k = 0;
    while (z >= 0)
    {
        if (simboluri[z] == '+' || simboluri[z] == '-' || simboluri[z] == '*' || simboluri[z] == '/' || simboluri[z] == '%')
        {
            k++;
        }  //numaram cati sunt inaintea celui la care suntem pentru a sti intre ce numere se efectueaza operatia
        z--;
    }
    if (simboluri[i] == '+')
    {
        numere_in_cifre[k] = numere_in_cifre[k] + numere_in_cifre[k + 1]; //rezultatul va fi scris in locul primului dintre cele doua numere
    }
    if (simboluri[i] == '-')
    {
        numere_in_cifre[k] = numere_in_cifre[k] - numere_in_cifre[k + 1];
    }
    if (simboluri[i] == '*')
    {
        numere_in_cifre[k] = numere_in_cifre[k] * numere_in_cifre[k + 1];
    }
    if (simboluri[i] == '/')
    {
        numere_in_cifre[k] = numere_in_cifre[k] / numere_in_cifre[k + 1];
    }
    if (simboluri[i] == '%')
    {
        numere_in_cifre[k] = numere_in_cifre[k] % numere_in_cifre[k + 1];
    }
    for (int j = k + 1; j < indice_numar - 1; j++)
        numere_in_cifre[j] = numere_in_cifre[j + 1]; //mutam doate numerele de dupa cu o pozitie inapoi pentru a elimina al doilea numar
    numere_in_cifre[indice_numar] = 0;
    indice_numar--;  //miscoram numarul total de numere deoarece avem cu un numar mai putin

    if (simboluri[i - 1] == '(') //daca un operator are '(' inainte va trebui sa eliminam si operatorul si '('
    {
        for (int j = i - 1; j < indice_operator - 2; j++)
        {
            simboluri[j] = simboluri[j + 2];
            prioritate[j] = prioritate[j + 2];
        }
        simboluri[indice_operator] = simboluri[indice_operator - 1] = (char)1024;
        indice_operator = indice_operator - 2;
    }
    else  // altfel va trebui sa eliminam doar operatorul
    {
        for (int j = i; j < indice_operator - 1; j++)
        {
            simboluri[j] = simboluri[j + 1];
            prioritate[j] = prioritate[j + 1];
        }
        simboluri[indice_operator] = (char)1024;
        indice_operator = indice_operator - 1;
    }
}

int corect(long long numere_in_cifre[], int indice_numar, char simboluri[], int indice_operator)
{
    for (int i = 0; i < indice_operator; i++)
    {
        int z = i - 1;
        int k = 0;
        while (z >= 0)
        {
            if (simboluri[z] == '+' || simboluri[z] == '-' || simboluri[z] == '*' || simboluri[z] == '/' || simboluri[z] == '%')
            {
                k++;
            }  //numaram cati sunt inaintea celui la care suntem pentru a sti intre ce numere se efectueaza operatia
            z--;
        }
        if (numere_in_cifre[k + 1] == 0 && simboluri[i] == '/')
            return 2;
    }
    int numar_de_operatii = 0;
    for (int i = 0; i < indice_operator; i++)
        if (simboluri[i] == '+' || simboluri[i] == '-' || simboluri[i] == '*' || simboluri[i] == '/' || simboluri[i] == '%')
            numar_de_operatii++;
    if (indice_numar - numar_de_operatii == 1) //daca numarul de operatii este cu 1 mai mic decat numarul de numere atunci intrebarea este corecta
        return 1;
    return 0;
}

long long calcul_total(long long numere_in_cifre[], int& indice_numar, char simboluri[], int& indice_operator, int prioritate[])
{
    {
        int i;
        for (i = indice_operator - 1; i >= 0; i--)
            if (simboluri[i] == '(') // prima data efectuam operatiile care au '(' inainte, apoi vom calcula in functie de prioritatea operatiilor
            {
                calcul_partial(numere_in_cifre, simboluri, i + 1, indice_numar, indice_operator, prioritate);
            }
        simboluri[indice_operator] = ')'; // punem ')' pe ultima pozitie deoarece secventa de mai jos calculeaza doar pana la ultima ')'
        indice_operator++;
        for (int t = 0; t < indice_operator; t++)
        {
            if (simboluri[t] == ')')  //vom lua pe rand fiecare ')' de la prima la ultima
            {
                for (i = 0; i < t; i++)
                {
                    if (prioritate[i] == 2) //calculam prima data pentru toate operatiile cu prioritatea 2
                    {
                        calcul_partial(numere_in_cifre, simboluri, i, indice_numar, indice_operator, prioritate);
                        t--;  //dupa apelarea functiei calcul_partial(...) vectorii se micsoreaza asa ca trebuie sa micsoram i si t pentru a nu
                        i--;  //sari peste numere si operatori
                    }
                }
                for (i = 0; i < t; i++)
                {
                    if (prioritate[i] == 1)  //apoi pentru uperatiile cu prioritatea 1
                    {
                        calcul_partial(numere_in_cifre, simboluri, i, indice_numar, indice_operator, prioritate);
                        t--;
                        i--;
                    }

                }
            }
        }
        long long rezultat = numere_in_cifre[0];
        return rezultat;
    }
}



void calcul(long long numere_in_cifre[], int& indice_numar, char simboluri[], int& indice_operator, int limba)
{
    int prioritate[100];
    stabileste_prioritate(simboluri, indice_operator, prioritate);
    if (corect(numere_in_cifre, indice_numar, simboluri, indice_operator) == 0)
    {
        if (status == 1)
            strcpy(s, "Intrebarea este gresita");
        else
            strcpy(s, "The question is wrong!");
        pune(s);
    }
    else if (corect(numere_in_cifre, indice_numar, simboluri, indice_operator) == 2)
    {
        if (status == 1)
            strcpy(s, "Nu se poate realiza impartirea la zero!");
        else
            strcpy(s, "Numbers cannot be divided by zero!");
        pune(s);
    }

    else
    {
        long long rezultat = calcul_total(numere_in_cifre, indice_numar, simboluri, indice_operator, prioritate);
        if (limba == 1)
            transforma(rezultat);
        else
            transformare_din_cifre_in_litere_en(rezultat);
    }

}
void desparte_rezultatul(char sir[100])
{
    int i;
    if (strlen(sir) > 45)
    {
        for (i = 44; i >= 0; i--)
            if (sir[i] == ' ')
                break;
        i++;
        strncpy(rezultat_in_litere1, sir, i);
        strcpy(sir, sir + i);
        int j;
        if (strlen(sir) > 45)
        {
            for (j = 44; j >= 0; j--)
                if (sir[j] == ' ')
                    break;
            j++;
            strncpy(rezultat_in_litere2, sir, j);
            strcpy(sir, sir + j);
            if (strlen(sir) != 0)
                strcpy(rezultat_in_litere3, sir);

        }
        else
            strcpy(rezultat_in_litere2, sir);
    }
    else
        strcpy(rezultat_in_litere1, sir);


}
void waitforClick(int& mx, int& my)
{
    mx = my = -1;
    while (mx == -1)
        getmouseclick(WM_LBUTTONUP, mx, my);
}

void dreptunghi(int x, int y, int lat)
{
    line(x - lat, y - lat / 2, x - lat, y + lat / 2);
    line(x - lat, y + lat / 2, x + lat, y + lat / 2);
    line(x + lat, y + lat / 2, x + lat, y - lat / 2);
    line(x + lat, y - lat / 2, x - lat, y - lat / 2);
}

void butoaneMeniu(int& status, int& culoare)
{
    settextstyle(0, HORIZ_DIR, 0);
    outtextxy(250, 50, "Calculator");
    outtextxy(120, 180, "Alegeti limba / Choose language:");
    dreptunghi(300, 250, 50);
    dreptunghi(300, 325, 50);
    int culoareaUrmatoare;
    if (culoare < 6)
        culoareaUrmatoare = culoare + 1;
    else
        culoareaUrmatoare = 1;
    setfillstyle(SOLID_FILL, culoareaUrmatoare);
    bar(35, 480, 115, 520);
    setfillstyle(SOLID_FILL, WHITE);
    outtextxy(265, 240, "Romana");
    outtextxy(260, 315, "English");
    int mx = 0, my = 0;
    status = 0;
    while (status == 0)
    {
        waitforClick(mx, my);
        if (mx > 250 && mx < 350 && my>225 && my < 275)
            status = 1;
        if (mx > 250 && mx < 350 && my>300 && my < 350)
            status = 2;
        if (mx > 35 && mx < 115 && my>480 && my < 520)
        {
            if (culoare < 6)
                culoare++;
            else
                culoare = 2;
            break;
        }
    }
}
void butoaneFereastraCalcul(int& status, int& culoare)
{
    int mx = -1, my = -1;
    if (status == 2)
    {
        setcolor(culoare);
        dreptunghi(520, 500, 40);
        dreptunghi(520, 260, 40);
        outtextxy(499, 490, "Back");
        outtextxy(493, 250, "Clear");
        while (status == 2)
        {
            waitforClick(mx, my);
            if (mx > 480 && mx < 560 && my>480 && my < 520)
                status = 0;
            if (mx > 480 && mx < 560 && my>240 && my < 280)
            {
                status = 2;
                break;
            }
        }
    }
    else
    {
        setcolor(culoare);
        dreptunghi(520, 500, 40);
        dreptunghi(520, 260, 40);
        outtextxy(485, 490, "Inapoi");
        outtextxy(485, 250, "Curata");
        while (status == 1)
        {
            waitforClick(mx, my);
            if (mx > 480 && mx < 560 && my>480 && my < 520)
                status = 0;
            if (mx > 480 && mx < 560 && my>240 && my < 280)
            {
                status = 1;
                break;
            }
        }
    }
}

void citesteInModGrafic(char text[200], int x, int y)
{
    char tasta, sir[2];
    sir[1] = '\0';
    char text_[200];
    strcpy(text, "");
    strcpy(text_, "_");
    outtextxy(x, y, text_);
    do
    {
        tasta = getch();
        if (tasta == 8 && strlen(text) > 0)
        {
            strcpy(text_, text);
            strcat(text_, "_");
            outtextxy(x, y, text_);
            text[strlen(text) - 1] = '\0';
            strcpy(text_, text);
            strcat(text_, "_");
            if (strlen(text) > 45)
                x = x + 12;
            outtextxy(x, y, text_);
            setfillstyle(SOLID_FILL, WHITE);
            bar(0, 0, 25, 550);



        }
        if (tasta >= 32 && tasta <= 127)
        {
            strcpy(text_, text);
            strcat(text_, "_");
            outtextxy(x, y, text_);
            text_[strlen(text_) - 1] = '\0';
            sir[0] = tasta;
            strcat(text, sir);
            strcpy(text_, text);
            strcat(text_, "_");
            if (strlen(text) > 45)
                x = x - 12;
            outtextxy(x, y, text_);
            setfillstyle(SOLID_FILL, WHITE);
            bar(0, 0, 25, 550);
        }
    } while (tasta != 13 && tasta != 27);
    setcolor(MAGENTA);
    outtextxy(x, y, text_);
    if (tasta == 27)
        strcpy(text, "");
}
int fereastra = 0;
void interfataGrafica(int& status, int& culoare)//(char text[200])
{
    if (status == 0)
    {
        if (fereastra == 0)
        {
            initwindow(600, 550, "Calculator");
            fereastra = 1;
        }
        setcolor(culoare);
        setfillstyle(SOLID_FILL, WHITE);
        bar(0, 0, 600, 550);
        setbkcolor(WHITE);
        setlinestyle(0, 0, 3);
        butoaneMeniu(status, culoare);
    }
    if (status == 1)
    {
        bar(0, 0, 600, 550);
        outtextxy(25, 150, "Intreaba:");
        outtextxy(25, 300, "Raspunsul este:");
        setlinestyle(3, 0, 3);
        line(25, 222, 575, 222);
        setlinestyle(0, 0, 1);
        citesteInModGrafic(text, 25, 200);
        string intrebare = text;
        prelucrare_initiala_intrebare(intrebare);
        desparte_intrebarea(intrebare);
        desparte_rezultatul(rezultat_in_litere);
        outtextxy(25, 325, rezultat_in_litere1);
        outtextxy(25, 350, rezultat_in_litere2);
        outtextxy(25, 375, rezultat_in_litere3);
        strcpy(rezultat_in_litere, "");
        strcpy(rezultat_in_litere1, "");
        strcpy(rezultat_in_litere2, "");
        strcpy(rezultat_in_litere3, "");
    }
    if (status == 2)
    {
        bar(0, 0, 600, 550);
        outtextxy(25, 150, "Ask:");
        outtextxy(25, 300, "The answer is:");
        setlinestyle(3, 0, 3);
        line(25, 222, 575, 222);
        setlinestyle(0, 0, 1);
        citesteInModGrafic(text, 25, 200);
        string intrebare = text;
        prelucrare_initiala_intrebare(intrebare);
        desparte_intrebarea_en(intrebare);
        desparte_rezultatul(rezultat_in_litere);
        outtextxy(25, 325, rezultat_in_litere1);
        outtextxy(25, 350, rezultat_in_litere2);
        outtextxy(25, 375, rezultat_in_litere3);
        strcpy(rezultat_in_litere, "");
        strcpy(rezultat_in_litere1, "");
        strcpy(rezultat_in_litere2, "");
        strcpy(rezultat_in_litere3, "");
    }
    if (status != 0)
        butoaneFereastraCalcul(status, culoare);
    interfataGrafica(status, culoare);
}
