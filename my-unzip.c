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
		printf("my-unzip: file1 [file2 ...]\n");
		exit(1);
	}
	for (i=1; i<argc; i++) {
		FILE *tied = fopen(argv[i], "r");
		if (tied == NULL) { //Jos tiedosto ei aukea
			printf("my-unzip: cannot open file\n");
			exit(1);
		}
		while (fgets(buf, KOKO, tied) != NULL) {
			char *os = buf;
			unsigned int yht = 0;
			while (*os != '\0') {
				if (*os <= '9' && *os >='0') {
					yht = yht*10+(*os-'0');
				} else {
					while (yht != 0) {
						printf("%c", *os);
						yht--;
					}
				}
				os++;
			}
		}
		fclose(tied);
		printf("\n");
	}
	return 0;
}
