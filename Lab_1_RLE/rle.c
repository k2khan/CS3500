#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void compress(int inFD, int outFD, int bufferSize);
void decompress(int inFD, int outFD, int bufferSize);

int main(int argc, char* argv[]) {
	char* inputFile;
	char* outputFile;
	char* bufferSize;
	char* mode;
	
	//Read in the arguments
	inputFile = argv[1];
	outputFile = argv[2];
	bufferSize = argv[3];
	mode = argv[4];

	//Make mode an integer
	int mode_as_int = atoi(mode);

	//Useage statement
	if (argc == 1) {
		printf("rle <input file> <output file> <compression length> <mode bit: 0 compress, 1 extract> \n");
		exit(0);
	}
	
	//Quit if more than 4 args
	if (argc != 5) {
		printf("Incorrect useage! Please use ./rle to find the useage statement! \n");
		exit(0);
	}

	if (atoi(bufferSize) < 1) {
		printf("Please use a buffersize >= 1! \n");
		exit(0);
	}

	//Open the input file descriptor
	int inFD = open( inputFile, O_RDONLY );
	if( inFD == -1 ){
		perror("Could not open input file");
		exit(-1);
	} 

	//Open the output file descriptor
	int outFD = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  	if (outFD == -1) {
    	perror("Could not open output file");
    	exit(-1);
  	}
	
	
	if (mode_as_int == 0) {
		compress(inFD, outFD, atoi(bufferSize));
	}
	else if (mode_as_int == 1) {
		decompress(inFD, outFD, atoi(bufferSize));
	}
	else {
		perror("Not a valid mode!\n\n");
		exit(-1);
	}
											
	return 0; 
}

void compress(int inFD, int outFD, int bufferSize) {
	unsigned char counter = 1;
	char buffer[bufferSize];
 	ssize_t retVal = read(inFD, buffer, bufferSize);
	
	if (retVal == -1) {
    	perror("Could not read from file");
    	exit(-1);
  	}
	
	//Give end of line character	
	buffer[retVal] = '\0';

		
	while (1) {
		char prev_byte[bufferSize];
		memcpy(prev_byte, buffer, strlen(buffer)+1);
		
		//read in next value	
		ssize_t retVal = read(inFD, buffer, bufferSize);

		if (retVal == -1) {
			perror("Could not read from file");
			exit(-1);
		}
		
		//If we get to the end of file, break
		if (retVal == 0) {
			break;
		}
		
		char* next_byte = buffer;
		
		//compare strings
		int ret = strncmp(prev_byte, next_byte, bufferSize);
			
		//If we hit a length of 255
		if (counter == 255) {
			retVal = write(outFD, &counter, 1);
			if (retVal == -1) {
				perror("Could not write to file");
				exit(-1);
			}

			retVal = write(outFD, &prev_byte, strlen(prev_byte));
			if (retVal == -1) {
				perror("Could not write to file");
				exit(-1);
			}
			counter = 1;
		}	
		
		//If the strings are the same
		else if (ret == 0) {
			counter++;
		} 

		//If strings are different	
		else if (ret != 0) {
			retVal = write(outFD, &counter, 1);
			if (retVal == -1) {
				perror("Could not write to file");
				exit(-1);
			}

			retVal = write(outFD, &prev_byte, strlen(prev_byte));
			if (retVal == -1) {
				perror("Could not write to file");
				exit(-1);
			}
			counter = 1;
		}	
	}
	return;
}

void decompress(int inFD, int outFD, int bufferSize) {
	while (1) {
		unsigned char counter;
		ssize_t retVal = read(inFD, &counter, 1); //Reat in the counter to the appropriate value
		if (retVal == -1) {
			perror("Could not read from file");
			exit(-1);
		}

		if (retVal == 0) {
				break;
			}

		char str[2] = {0}; //Stack Overflow
		str[0] = counter; //Stack Overflow


		char buffer[bufferSize]; //Read in the actual character
		retVal = read(inFD, buffer, bufferSize);
		if (retVal == -1) {
			perror("Could not write to file");
			exit(-1);
		}

		for (int i = 0; i < counter; i++) {
			retVal = write(outFD, buffer, bufferSize);
			if (retVal == -1) {
				perror("Could not write to file");
				exit(-1);
			}
		}
	}
	return;
}

