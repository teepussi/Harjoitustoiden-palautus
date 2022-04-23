//Tekija: Timi Marviala
//Lahteet:
//	Henrik Kauppi (Opiskelukaveri)
//	https://www.ibm.com/docs/en/aix/7.2?topic=types-signed-unsigned-integers

#include <stdio.h>
#include <stdlib.h>
#define KOKO 128

int main (int argc, char* argv[]) {
	int i;
	char buf[KOKO];
	if (argc == 1) { //Jos ei parametreja
		printf("my-zip: file1 [file2 ...]\n");
		exit(1);
	}
	for (i=1; i<argc; i++) {
		FILE *tied = fopen(argv[i], "r");
		if (tied == NULL) { //Jos tiedosto ei aukea
			printf("my-zip: cannot open file\n");
			exit(1);
		}
		while (fgets(buf, KOKO, tied) != NULL) {
			char *os = buf;
			char nyt = *os;
			unsigned int yht = 0;
			while (*os != '\0') {
				if (*os == nyt) {
					yht++;
					os++;
					continue;
				} else if (yht == 4294967295) {
					printf("%d%c", yht, nyt);
					nyt = '\0';
					yht = 0;
				} else {
					printf("%d%c", yht, nyt);
					nyt = *os;
					yht = 0;
				}
			}
		}
		printf("\n");
		fclose(tied);
	}
	return 0;
}
			
	
