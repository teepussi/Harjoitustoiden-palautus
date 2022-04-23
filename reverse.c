//Tekija: Timi Marviala
//Lahteet: 
//	Henrik Kauppi (Opiskelukaveri)
//	https://stackoverflow.com/questions/14598730/reverse-order-of-lines-wihout-reversing-words-in-c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LKOK 250
#define RKOK 10

int main (int argc, char *argv[]){
	char rivi[RKOK][LKOK];
	char* nimi1;
	char* nimi2;
	int i = 0;
	int tot = 0;
	int n;
	if (argc != 3) { //Jos parametreja ei ole oikea maara
		fprintf(stderr, "usage: ./reverse <input> <output>\n");
		exit(1);
	}
	nimi1 = argv[1];
	nimi2 = argv[2];
	if (strcmp(nimi1, nimi2) == 0){ //Jos annetut tiedostot ovat samat
		fprintf(stderr, "Input and output file must differ.\n");
		exit(1);
	}
	FILE* tied1 = fopen(nimi1, "r");
	if  (tied1 == NULL) { //Jos tiedosto ei aukea
		fprintf(stderr, "error: cannot open file '%s'\n", nimi1);
		exit(1);
	}
	while (fgets(rivi[i], LKOK, tied1)){ 
		rivi[i][strlen(rivi[i])-1]= '\0';
		i++;
	}
	tot = i;
	FILE* tied2 = fopen(nimi2, "w");
	if  (tied2 == NULL) { //Jos tiedosto ei aukea
		fprintf(stderr, "error: cannot open file '%s'\n", nimi2);
		exit(1);
	}
	for (n = tot-1; n>=0;n--){
		fprintf(tied2, "%s\n", rivi[n]);
	}
	fclose(tied1);
	fclose(tied2);	
	printf("Reverse successful!\n");
	return 0;
}
