//for linux

//#define _CRT_SECURE_NO_WARNINGS
//#include<Windows.h>
#include<stdio.h>
#include<io.h>
#include <math.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#define Xlength (Xcell*Xmax)
#define Ylength (Ycell*Ymax)

int Xmax = 2000, Ymax = 100, loB = 4, magdir = 1;
double Ms = 800e3, Xcell = 1e-9, Ycell = 1e-9;
char SearchAddr[255] =".";

void trace(char* filename,int MagDir) {
	printf("%s\n", filename);	//0070--Oxs_TimeDriver-Magnetization-00-0025000.omf
	char fout[255];
	char fin[255];
	FILE *fpin, *fpout;
	double mx, my, mz;
	int i, n = 0;
	double x = 0, y = 0, z = 0;
	strcpy(fin, SearchAddr);
	strcat(fin, filename);

	strncpy(fout, filename, loB);
	fout[loB] = '\0';
	strcat(fout, ".txt");

	fpin = fopen(fin, "r");
	fpout = fopen(fout, "a");

	while (1) {
		if (fgetc(fpin) != 't')
			continue;
		if (fgetc(fpin) != 'i')
			continue;
		if (fgetc(fpin) != 'm')
			continue;
		if (fgetc(fpin) != 'e')
			continue;
		if (fgetc(fpin) == ':')
			break;
	}
	double t;
	fscanf(fpin, "%lf", &t);
	while (1) {
		if (fgetc(fpin) != 'x')
			continue;
		if (fgetc(fpin) == 't')
			break;
	}
	for (i = 0; i<Xmax*Ymax; i++) {
		fscanf(fpin, "%lf", &mx);
		fscanf(fpin, "%lf", &my);
		fscanf(fpin, "%lf", &mz);
		x += mx;
		y += my;
		z += mz;
	}
	switch (MagDir) {
	case 0:
		printf("%e\t%e\n", t, (x / Xmax / Ymax / 2 / Ms + 0.5)*Xlength);
		fprintf(fpout, "%e\t%e\n", t, (x / Xmax / Ymax / 2 / Ms + 0.5)*Xlength); break;
	case 1:
		printf("%e\t%e\n", t, (y / Xmax / Ymax / 2 / Ms + 0.5)*Xlength);
		fprintf(fpout, "%e\t%e\n", t, (y / Xmax / Ymax / 2 / Ms + 0.5)*Xlength); break;
	case 2:
		printf("%e\t%e\n", t, (z / Xmax / Ymax / 2 / Ms + 0.5)*Xlength);
		fprintf(fpout, "%e\t%e\n", t, (z / Xmax / Ymax / 2 / Ms + 0.5)*Xlength); break;
	default:
		printf("error in MagDir\n");
		break;
	}
	fclose(fpin);
	fclose(fpout);
}

void usage(char* program) {
	printf("\nProgram Usage:\n%s\n", program);
	printf("     [ -a  2000 ]    Xmax\n");
	printf("     [ -b  100 ]    Ymax\n");
	printf("     [ -x  1e-9 ]    Xcell\n");
	printf("     [ -y  1e-9 ]    Ycell\n");
	printf("     [ -m  8003e ]    Ms\n");
	printf("     [ -d  1 ]    Mag Direction 0:x 1:y 2:z\n");
	printf("     [ -l  4 ]    length of B in filename\n");
	printf("     [ -f  ]  the file address\n");
	printf("     [ -h ]  display this information\n");
	printf("\n");
	exit(1);
}

void getMyArgs(int argc, char** argv) {
	char ch;
	while ((ch = getopt(argc, argv, "a:b:x:y:m:d:l:f:h")) != -1) {
		switch (ch) {
		case 'a':
			Xmax = atoi(optarg); break;
		case 'b':
			Ymax = atoi(optarg); break;
		case 'x':
			Xcell = atoi(optarg); break;
		case 'y':
			Ycell = atoi(optarg); break;
		case 'm':
			Ms = atoi(optarg); break;
		case 'd':
			magdir = atoi(optarg); break;
		case 'l':
			loB = atoi(optarg); break;
		case 'f':
			strcpy(SearchAddr, optarg); break;
		case 'h':
			usage(argv[0]);
		case '?':
			usage(argv[0]);
		}
	}
}
/*
int main(int argc, char *argv[]){

	long Handle;
	struct _finddata_t FileInfo;
//	getMyArgs(argc, argv);
	strcat(SearchAddr, "*.omf");
	Handle = _findfirst(SearchAddr, &FileInfo);
	if (-1 == Handle)
		return -1;
	trace(FileInfo.name, magdir);

	while (!_findnext(Handle, &FileInfo)){
		trace(FileInfo.name, magdir);

	}
	_findclose(Handle);
	system("pause");
	return 0;
}
*/

int main(int argc, char * argv[]){
    struct dirent *ptr;    
    DIR *dir;
    getMyArgs(argc, argv);
    dir=opendir(SearchAddr);
    
    char pattern[] = {'.', 'o', 'm', 'f'};
    int len_pattern = strlen(pattern);

    while((ptr=readdir(dir))!=NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        if (memcmp(ptr->d_name + strlen(ptr->d_name) - len_pattern, pattern, len_pattern) == 0) {
            printf("%s\n",ptr->d_name);
	    trace(ptr->d_name, magdir);
        }
    }
    closedir(dir);
    return 0;
}

