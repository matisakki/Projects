#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#define MAX_LIMIT 80

//Rakenne:
//structi jossa nimet, ja niistä perustetaan linkitetty lista

struct hahmo {
    char *name;
    int hp;
    int xp;
    char *ase;
    int dmg;
    struct hahmo *next;
};

int compare (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

void lisaaHahmo (struct hahmo *q, char *input) {
    while (q->next) {  // toista, kunnes NULL-osoitin tulee vastaan
        q = q->next;    
    }
    q->next = malloc(sizeof(struct hahmo));

    char *ptr = strtok(input," ");

    ptr = strtok (NULL," "); // ptr on nimi
    if (ptr == NULL) {
        printf("Virheellinen syöte\n");
        return;
    }
    q->next->name = malloc(strlen(ptr)+1);
    strcpy(q->next->name,ptr);

    ptr = strtok (NULL, " "); // ptr on hp
    q->next->hp = atoi(ptr);

    ptr = strtok (NULL, " "); // ptr on ase
    q->next->ase = malloc(strlen(ptr)+1);
    strcpy(q->next->ase,ptr);

    ptr = strtok (NULL, " "); // ptr on aseen dmg
    q->next->dmg = atoi(ptr);

    q->next->xp = 0; //kokemuspisteet alussa 0

    q->next->next = NULL; //asetetaan seuraava hahmo nulliksi
}

void hyokkaa (struct hahmo *q, char *input) {
    struct hahmo *r = q;
    int hit,lefthp;//lyöjän aseen asevahinko ja saajan jäljelle jäävä HP
    int i = 0;
    char *ase; //lyöjän ase

    char *ptr = strtok(input," "); // Liikutaan "H":n ohi
    ptr = strtok (NULL," ");
    if (ptr == NULL) {
        printf("Virheellinen syöte\n");
        return;
    }
    char *attack = malloc(strlen(ptr)+1);
    strcpy(attack,ptr); //tallennetaan hyökkääjä muuttujaan attack

    ptr = strtok (NULL," "); // nyt ptr on iskun saaja
    if (ptr == NULL) {
        return;
    }
    int size = strlen(ptr);
    ptr[size-1] = '\0'; //Laitetaan ptr oikean kokoiseksi että voidaan verrata nimiä

    while (q->next != NULL) { // toista, kunnes NULL-osoitin tulee vastaan
        if (strcmp(q->next->name,attack) == 0) {
            ase = malloc(strlen(q->next->ase)+1);
            strcpy(ase,q->next->ase);// Tallennetaan ase muuttujaan
            hit = q->next->dmg;
            q->next->xp = q->next->xp+hit; //Annetaan lyöjälle xp:t
            i++;
            break;
        }
        q = q->next;
    }
    while (r->next != NULL) { // toista, kunnes NULL-osoitin tulee vastaan
        if (strcmp(r->next->name,ptr) == 0) {
            r->next->hp = r->next->hp-hit; //Vähennetään isku hp:sta
            if(r->next->hp < 0 ) {
                r->next->hp = 0;
            }
            lefthp = r->next->hp;
            i++;
            break;
        }
        r = r->next;    
    }
    if (i == 2) {
        printf("\n%s attacked %s with %s by %d damage.\n",attack,ptr,ase,hit);
        printf("%s has %d hit points remaining. %s gained %d experience points\n",ptr,lefthp,attack,hit);
        free(ase);
        free(attack);
    }
    else {
        printf("\nSinun täytyy lisätä nämä pelaajat ensin peliin!\n");
    }
}

void tulostaHahmot (struct hahmo *q) { //while joka määärällä pelaajien. while
    struct hahmo *r = q;
    int koko = 0;
    int i = 0;
    int j = 0;
    while (q->next) {  // toista, kunnes NULL-osoitin tulee vastaan
        q = q->next;   
        koko++; 
    }
    int *list = malloc(sizeof(int)*koko);
    q = r; //asetetaan pointteri listan alkuun
    while (q->next) {
        list[i] = q->next->xp; //tallenetaan Xp:t listaan
        q = q->next;
        i++;    
    }
    qsort (list, koko, sizeof(int), compare);
    i = 0;
    koko--;
    while (j<(koko+1)) {  // toista, kunnes NULL-osoitin tulee vastaan
        q = r; //asetetaan pointteri listan alkuun
        while (q->next) {
                // printf("\n%d    %d",list[koko-i],q->next->xp);
            if (q->next->xp == list[koko-i] && q->next->hp > 0) {
                printf ("\nNimi %s, osumapisteet %d ja xp on %d\n",q->next->name,q->next->hp,q->next->xp);
                printf("Ase on %s ja sen vahinko on %d\n",q->next->ase,q->next->dmg);
            }
            q = q->next;
        }
        j++;
        i++;
    }
    q = r; //asetetaan pointteri listan alkuun
    while (q->next) {  // toista, kunnes NULL-osoitin tulee vastaan
        if (q->next->hp == 0) {
            printf ("Nimi %s, osumapisteet %d ja xp on %d\n",q->next->name,q->next->hp,q->next->xp);
            printf("Ase on %s ja sen vahinko on %d\n\n",q->next->ase,q->next->dmg);
        }
        q = q->next;
    }
    free(list);
}

void lataa (struct hahmo *q, char *input) {
    FILE *f;
    char *ptr = strtok(input," "); // Liikutaan "H":n ohi
    ptr = strtok (NULL," "); //tiedoston nimi
    if (ptr == NULL) {
        printf("Syöte on virheellinen");
        return;
    }
    int size = strlen(ptr);
    ptr[size-1] = '\0'; //Laitetaan ptr oikean kokoiseksi että voidaan verrata nimiä
    f = fopen(ptr,"r");
    if(f == NULL) {
        printf("Virhe tiedostoa avatessa!");   
        exit(1);             
    }
    char *line = NULL;
    char *s = malloc(sizeof(char)*MAX_LIMIT);
    while (fgets(s, MAX_LIMIT, f) != NULL) {
        line = strtok(s," ");// ptr on nimi
        strcpy(q->next->name,line);
        line = strtok (NULL, " "); // ptr on hp
        q->next->hp = atoi(line);
        line = strtok (NULL, " "); // ptr on xp
        q->next->xp = atoi(line);
        line = strtok (NULL, " "); // ptr on ase
        strcpy(q->next->ase,line);
        line = strtok (NULL, " "); // ptr on dmg
        q->next->dmg = atoi(line);
        q = q->next;
    }
    fclose(f);
    free(s);
}

void tallenna (struct hahmo *q, char *input) {
    FILE *f;
    char *ptr = strtok(input," "); // Liikutaan "H":n ohi
    ptr = strtok (NULL," "); //tiedoston nimi
    if (ptr == NULL) {
        printf("Syöte on virheellinen");
        return;
    }
    int size = strlen(ptr);
    ptr[size-1] = '\0'; //Laitetaan ptr oikean kokoiseksi että voidaan verrata nimiä
    f = fopen(ptr,"w");
    if(f == NULL) {
        printf("Virhe tiedostoa avatessa!");   
        exit(1);             
    }
    while (q->next) {
        fprintf(f,"%s %d %d %s %d\n",q->next->name,q->next->hp,q->next->xp,q->next->ase,q->next->dmg);
        q = q->next;
    }
    fclose(f);
}

void lopetaPeli (struct hahmo *q) {
    struct hahmo *r = q;
    if (!q){
        return;
    }
    while (q->next) {  // toista, kunnes NULL-osoitin tulee vastaan
        r = q->next;
        q->next = q->next->next;
        free(r->name);
        free(r->ase);
        free(r);   
    }
}
int main(void)
{
    struct hahmo p = {NULL};
    char input[MAX_LIMIT];
    printf("Tervetuloa peliin!\n");
    while(1) {
        printf("\nA) Lisää hahmo (syntaksi: A nimi elämät ase iskupisteet)\nH) Hyökkää (syntaksi: H hykkääjä kohde)\nL) Tulosta hahmot (syntaksi: L)\nW) Tallenna hahmot tiedostoon (syntaksi: W tiedostonimi.txt)\nO) Lataa pelin hahmot tiedostosta (syntaksi: O tiedostonimi.txt)\nQ) Poistu ohjelmasta (syntaksi: Q)\n");
        printf("Mitä haluat tehdä: ");
        fgets(input, MAX_LIMIT, stdin);
        if (input[0] == 'A') {
            lisaaHahmo(&p, input);
        }
        else if (input[0] == 'H') {
            hyokkaa(&p, input);
        }
        else if (input[0] == 'L') {
            tulostaHahmot(&p);
        }
        else if (input[0] == 'W') {
            tallenna(&p, input);
        }
        else if (input[0] == 'O') {
            lataa(&p, input);
        }
        else if (input[0] == 'Q') {
            lopetaPeli(&p);
            printf("\nKiitos pelin pelaamisesta!\n");
            break;
        }
        else {
            printf("Virheellinen syöte!\n");
            continue;
        }
    }
    return 0;
}