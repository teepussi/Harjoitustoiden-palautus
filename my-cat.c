//Tekija: Timi Marviala

#include <stdio.h>
#include <stdlib.h>
#define KOKO 128

int main(int argc, char* argv[]) {
	int i;
	char buf[KOKO];	
	if (argc == 1) { //Jos ei parametreja
		exit(0);
	}
	for (i=1; i<argc; i++) { //Printtaa kaikki annetut tiedostot
		FILE *tied = fopen(argv[i], "r");
		if (tied == NULL) { //Jos tiedosto ei aukea
			printf("my-cat: cannot open file\n");
			exit(1);
		}
		while (fgets(buf, KOKO, tied) != NULL) { //Printtaa tiedoston
			printf("%s",buf);
		}
		fclose(tied);
	}
	return 0;
}
