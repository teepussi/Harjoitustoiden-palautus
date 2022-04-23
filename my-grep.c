//Tekija: Timi Marviala

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define KOKO 1024

int main(int argc, char* argv[]) {
	if (argc > 2) { //Jos tiedosto ja hakutermi annettu
		int i;
		char* haku = argv[1];
		char buf[KOKO];
		for (i=2; i<argc; i++) {
			FILE *tied = fopen(argv[i], "r");
			if (tied == NULL) {
				printf("my-grep: cannot open file\n");
				exit(1);
			}
			while (fgets(buf, KOKO, tied) != NULL) {
				char *loytyi = strstr(buf, haku);
				if (loytyi != NULL) {
					printf("%s", buf);
				}
			}
			fclose(tied);
		}
	} else if (argc == 2) { //Jos tiedostoa ei annettu
		char* haku = argv[1];
		while (1) {
			char rivi[KOKO];
			scanf("%s", rivi);
			char *loytyi = strstr(rivi, haku);
			if (loytyi != NULL) {
				printf("%s\n", rivi);
				exit(0);
			}
		}
	} else if (argc == 1){ //Jos ei parametreja
		printf("my-grep: searchterm [file ...]\n");
		exit(1);
	}
	return 0;
}
