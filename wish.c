//Tekija: Timi Marviala
//Lahteet: 
//	Henrik Kauppi (Opiskelukaveri)
//	Jyri Marviala
//	https://medium.com/swlh/tutorial-to-code-a-simple-shell-in-c-9405b2d3533e
//	https://www.tutorialspoint.com/c_standard_library/c_function_fflush.htm
//	https://gist.github.com/parse/966049

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<ctype.h>

int KOKO = 1024;
int batch = 0;
int polkuMuut = 0;
int sulj = 0;
int polkuTyhj = 0;
char error_message[30] = "An error has occurred\n";
char *path;
char polku[512][512];
int polkuNum = 0;

int tarkista(char* buf) {
	int f = 0;
	for (int i=0;i<strlen(buf);i++) {
		if (isspace(buf[i])==0) {
			f = 1;
			break;
		}
	}        
	return f;
}
int uusiProsessi(char *argss[]) {
	int rc=fork();
	if (rc<0) {
		write(STDERR_FILENO, error_message, strlen(error_message));
		exit(1);
	} else if (rc == 0 && polkuTyhj != 1) {
		if (polkuMuut == 0) {
			path=strdup("/bin/");
			path=strcat(path, argss[0]);
			if (access(path,X_OK) !=0 && polkuMuut == 0) {
				path = strdup("/usr/bin/");
				path = strcat(path, argss[0]);
				if (access(path, X_OK)!=0) {
					write(STDERR_FILENO, error_message, strlen(error_message));
					exit(0);
				}
			}
		} else if (polkuMuut == 1 && polkuNum == 0) {
			path = strcat(path, argss[0]);
			if (access(path, X_OK) != 0 ) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(0);
			}
		} else {
			for (int x=0; x<polkuNum; x++) {
				strcat(polku[x], argss[0]);
				if (access(polku[x], X_OK)==0) {
					strcpy(path, polku[x]);
					break;
				}
			}
		}
		if (execv(path, argss)==-1) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(1);
		}
	} else {
		int returnStatus=0;
	}
	return rc;
}
int prosessi(char *buf) {
	int stdout_kopio = 0;
	int rc;
	if (strstr(buf,">")!=NULL) {
		int a=0;
		char* ohjaus[sizeof(char)*512];
		ohjaus[0] = strtok(strdup(buf)," \n\t>");
		while (ohjaus[a] != NULL) {
			a++;
			ohjaus[a] = strtok(NULL," \n\t>");
		}
		if (a==1) {
			write(STDERR_FILENO, error_message, strlen(error_message)); 
			exit(0);
		}
		int i=0;
		char* argss[sizeof(buf)];
		argss[0]= strtok(buf,"\n\t>");
		while (argss[i] != NULL) {
			i++;
			argss[i] = strtok(NULL," \n\t>");
		}
		if (i>2) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(0);
		}
		int y = 0;
		char* token[sizeof(argss[1])];
		token[0] = strtok(argss[1]," \n\t");
		while(token[y] != NULL){
			y++;
			token[y] = strtok(NULL," \n\t");
		}
		char *output = strdup(token[0]);
		stdout_kopio = dup(1);
		int out = open(output, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		int error = open(output, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		fflush(stdout);
		dup2(out, STDOUT_FILENO);
		dup2(out, STDERR_FILENO);
		close(out);
		sulj=1;
		if (out==-1 || error==-1 || y>1 || i>2) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(0);
		}
		argss[i+1] = NULL;
		token[y+1] = NULL;
		strcpy(buf, argss[0]);
	}
	if (buf[0] != '\0' && buf[0] != '\n') {
		char *komento[sizeof(buf)];
		komento[0] = strtok(buf, " \t\n");
		int p = 0;
		while (komento[p] != NULL) {
			p++;
			komento[p] = strtok(NULL, " \n\t");
		}
		komento[p+1] = NULL;
		if (strcmp(komento[0],"cd") == 0) {
			if (p==2) {
				if (chdir(komento[1]) != 0) {
					write(STDERR_FILENO, error_message, strlen(error_message));
				}
			} else {
				write(STDERR_FILENO, error_message, strlen(error_message));
			}
		} else if (strcmp(komento[0], "path") == 0) {
			polkuMuut = 1;
			if (p==2) {
				polkuTyhj = 0;
				path = strdup(komento[1]);
				if (path[strlen(path)-1]!='/') {
					strcat(path,"/");
				}
			} else if (p==1) {
				polkuTyhj = 1;
			} else {
				polkuTyhj = 0;
				for (int i=1; i<p; i++) {
					char *temp = strdup(komento[i]);
					if (temp[strlen(temp)-1] != '/') {
						strcat(temp,"/");
					}
					strcpy(polku[i-1],temp);
					polkuNum++;
				}
			}
		} else if(strcmp(komento[0],"exit") == 0) {
			if(p==1){
				exit(0);
			} else {
				write(STDERR_FILENO, error_message, strlen(error_message));
			}
		} else {
			if (polkuTyhj==1) {
				write(STDERR_FILENO, error_message, strlen(error_message));
			} else {
				rc=uusiProsessi(komento);
			}
		}
	}
	if (sulj==1) {
		dup2(stdout_kopio,1);
		close(stdout_kopio);
	}
	return rc;
}
int main (int argc, char* argv[]) {
	FILE *tied = NULL;
	path = (char*) malloc(KOKO);
	char buf[KOKO];
	if (argc==1) { //Ei batch
		tied = stdin;
		write(STDOUT_FILENO, "wish> ", strlen("wish> ")); 
	} else if (argc==2) { //Batch
		char *tied2 = strdup(argv[1]);
		tied = fopen(tied2, "r");
		if (tied == NULL) {
			write(STDERR_FILENO, error_message, strlen(error_message));
			exit(1);
		}
		batch = 1;
	} else {
		write(STDERR_FILENO, error_message, strlen(error_message));
		exit(1);
	}
	while (fgets(buf, KOKO, tied)) {
		sulj = 0;
		int f = 0;
		for (int i=0; i<strlen(buf); i++) {
			if (isspace(buf[i])==0) {
				f = 1;
				break;
			}
		}        
		if (f==0) { //Tarkistaa onko buf tyhja
			continue;
		}
		if (strstr(buf,"&")!=NULL) {
			int t = 0;
			char *argss[sizeof(buf)];
			argss[0] = strtok(buf,"\n\t&");
			while(argss[t] != NULL){
				t++;
				argss[t] = strtok(NULL,"\n\t&");
			}
			argss[t+1] = NULL;
			int pid[t];
			for (int i=0; i<t; i++) {
				pid[i] = prosessi(argss[i]);
				for (int x=0; x<t; i++) {
					int returnStatus = 0;
					waitpid(pid[x],&returnStatus,0);
					if (returnStatus == 1) {
						write(STDERR_FILENO, error_message, strlen(error_message));
						exit(1);
					}
				}
			}
		} else {
			prosessi(buf);
		}
		if (argc == 1) {
			write(STDOUT_FILENO, "wish> ", strlen("wish> ")); 
		}
	}
}

