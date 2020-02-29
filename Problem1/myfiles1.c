#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* defaultName = "myfile.out";


void WriteInFile (int fd, const char *buff, int len){
	//write 
	write(fd,buff,len);	

}

void CopyFile (int fd, const char *file_in){
	//get file contents
	FILE *fp_in = fopen(file_in, "r");
	fseek(fp_in,0,SEEK_END);
	int len = ftell(fp_in);
	rewind(fp_in);
	//printf("len of file is %d\n", len);
	char* buffer = malloc(len+1);
	
	fread(buffer, sizeof(char), len, fp_in);
	fclose(fp_in);
	buffer[len] = '\0';
	//printf("file contents are: %s\n", buffer);

	//write contents to file descriptor
	WriteInFile(fd,buffer,len);
}



int main(int argc, char* argv[1]){


	if (argc <=2) {
		printf("Usage:./myfiles file_in_1 ... file_in_n [file_out]\n");
		return 0;
	}
	
	//get the output file name 
	int numFiles = argc - 1;
	FILE *output = fopen(argv[numFiles], "w");
	int fd_o = fileno(output);

	//iterate through all files and add contents
	int i;
	for (i = 1; i <= numFiles - 1; i++){

		FILE *fp = fopen(argv[i], "r");
	
		//check if exist
		if (!(fp = fopen(argv[i], "r"))){
			printf("%s: No such file or directory\n",argv[i]);
			return 0;
		}
	
		//close the file so it can be used again
		fclose(fp);
	
		//copy file
		CopyFile(fd_o,argv[i]);
	}

	fclose(output);
	return 0;
}
