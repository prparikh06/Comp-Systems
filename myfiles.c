#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		printf("Usage:./myfiles file_in_1 file_in_2 [file_out (default:myfile.out)]\n");
		return 0;
	}
	
	char* file_names[3];
	file_names[0] = argv[1];
	file_names[1] = argv[2];

	//if output file name specified
	if (argc == 4)
		file_names[2] = argv[3];
	else
		file_names[2] = defaultName;
		
	FILE *fp1, *fp2, *output;

	//check if exist
	if (!(fp1 = fopen(file_names[0], "r"))){
		printf("%s: No such file or directory\n",file_names[0]);
		return 0;
	}
	else if (!(fp2 = fopen(file_names[1], "r"))){
		printf("%s: No such file or directoy\n", file_names[1]);
		return 0;
	}
	
	output = fopen(file_names[2], "w");
	
	//close the files so they can be used again
	fclose(fp1);
	fclose(fp2);
	
	int fd_o = fileno(output);
	
	//copy files
	CopyFile(fd_o,file_names[0]);
	CopyFile(fd_o,file_names[1]);
	

	fclose(output);

}
