#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "airPdata.h"

void parseLine(char *line, airPdata *apd);
void printData(int length, airPdata *apd);
void deleteStruct(airPdata *apd);
float sexag2decimal(char *str);

#define BUFFER_SIZE 500

int main (int argc, char *argv[]){

	// Declare input buffer and other parameters
	FILE *fid;
	char buffer[BUFFER_SIZE];
	int count = 0;

	
	// Check for command line input and open input file.
	if(argc==2){
		fid = fopen(argv[1], "r");
		if(fid==NULL){
			printf("File %s failed to open. Aborting.\n", argv[1]);
			return 2;
		}
	}
	else{
		printf("Incorect number of input parameters. Please specify the name of the input file only.\n");
		printf("Syntax: ./hw1ecl.exe [input file]\n");
		return 1;
	}
	
	
	// Determine length of the file.
	while(fgets(buffer, BUFFER_SIZE, fid) != NULL){
		count++;
	}
	rewind(fid);
	
	
	// Declare a struct array and allocate memory.
	airPdata *data;

	data = malloc(sizeof(airPdata)*count);
	if(data==NULL){
		printf("Memory allocation for airPdata array failed. Aborting.\n");
		return 2;
	}

	// Read and parse each line of the inputt file.
	for(int i = 0; i<count; i++){
		fgets(buffer, BUFFER_SIZE, fid);
		
		// fgets() includes the New Line delimiter in the output string. 
		// i.e. "This is my string.\n\0"
		// We will truncate the string to drop the '\n' if it is there.
		// Note: There will be no '\n' if the line is longer than the buffer.
		if(buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer)-1] = '\0';
		
		parseLine(buffer, data+i);
	}

	// close the input file.
	fclose(fid);
	
	

	// Output the data to stdout.
	printData(count, data);

	// Free the memory used for fields of the structs.
	for(int i=0; i<count; i++){
		deleteStruct(data+i);
	}

	// Free the memory for the struct array.
	free(data);

	return 0;

}


void parseLine(char *line, airPdata *apd){

	int i=0, j=0, commas=0;
	char *buffer1 = malloc(sizeof(char)*50);
	char *buffer2 = malloc(sizeof(char)*50);
	
	while(commas<15){
		while(*(line+i)!=','){
			i++;
		}
		
		// strncpy does not append a '\0' to the end of the copied sub-string, so we will
		// replace the comma with '\0'. 
		*(line+i) = '\0';

		switch (commas){
		case 0:   //Grab the first "field" - Site Number
	//		apd->siteNumber = malloc(sizeof(char)*(i-j+1));
	//		if(apd->siteNumber==NULL){
	//			printf("malloc failed to initialize airPdata.siteNumber.\n");
	//			exit(-1);
	//		}
	//		strncpy(apd->siteNumber, line+j, i-j+1);
			break;   
		case 1:   //Grab the second "field" - Location ID
			apd->LocID = malloc(sizeof(char)*(i-j+1));
			if(apd->LocID==NULL){
                printf("malloc failed to initialize airPdata.LocID.\n");
                exit(-1);
            }
            strncpy(apd->LocID, line+j, i-j+1);
            break;
		case 2:   //Grab the third "field" - Field Name
			apd->fieldName = malloc(sizeof(char)*(i-j+1));
			if(apd->fieldName==NULL){
				printf("malloc failed to initialize airPdata.fieldName.\n");
				exit(-1);
			}
			strncpy(apd->fieldName, line+j, i-j+1);
			break;
		case 3:   //Grab the fourth "field" - City
			apd->city = malloc(sizeof(char)*(i-j+1));
			if(apd->city==NULL){
				printf("malloc failed to initialize airPdata.city.\n");
				exit(-1);
			}
			strncpy(apd->city, line+j, i-j+1);
			break;
		case 4:   //Grab the fifth "field" - State
	//		apd->state = malloc(sizeof(char)*(i-j+1));
	//		if(apd->state==NULL){
	//			printf("malloc failed to initialize airPdata.state.\n");
	//			exit(-1);
	//		}
	//		strncpy(apd->state, line+j, i-j+1);
			break;
		case 8:   //Grab the ninth "field" - Latitude (sexagesimal string)
			strncpy(buffer1, line+j, i-j+1);
			apd->latitude = sexag2decimal(buffer1);
			break;
		case 9:   //Grab the tenth "field" - Longitude (sexagesimal string)
			strncpy(buffer2, line+j, i-j+1);
			apd->longitude = sexag2decimal(buffer2);
			break;
		case 14:   //Grab the fifteenth "field" - Control Tower (Y or N)
	//		apd->controlTower=*(line+j);
			break;

		case 16:   //Grab the seventeenth "field" - Control Tower on North/South runway (Y or N)
	//		apd->controlTower=*(line+j);
			break;
		}

		j=++i;
		commas++;
	}
	
}

//Untested but is logically sound
float sexag2decimal(char *str)
{
	if(str == NULL)
		return 0.0;
	char ** dms = malloc(sizeof(char*)*2);
	char direction;
	float result = 0;
	int i, j=0;
	
	
	// C stores strings as a pointer to the start and a \0 as the end
	// Every time we find a - or . we change it to \0 to end the string 
	// Then we save a pointer to the next character to be the beginning of the next string
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == '-')
		{
			str[i] = '\0';
			dms[j] = &(str[i+1]);
			j++;
		}
		if(str[i] == 'N' || str[i] == 'E' || str[i] == 'W' || str[i] == 'S')
		{
			direction = str[i];
			str[i] = '\0';
			
		}
	}
	if(atof(str) > 180 || atof(dms[0]) > 59 || atof(dms[1]) >= 60)
		return 0.0;
	
	result += atof(str);
	result += (float)(atof(dms[0])) / 60.00;
	result += (float)(atof(dms[1])) / 3600.00;

	if(direction == 'S' || direction == 'W')
		result *= -1.0;
	
	return result;

}

//Fix this later get format right and print to file

void printData(int length, airPdata *data){
	printf("Code, Name, City, Lat, Lon\n");
	for(int i = 0; i<length; i++)
	{
		printf("%s, %s, %s, %7.4f, %7.4f", (data+i)->LocID, (data+i)->fieldName, (data+i)->city, (data+i)->longitude, (data+i)->latitude);
		printf("\n");
	}
}

void deleteStruct(airPdata *apd){
	free(apd->city);
	free(apd->fieldName);
	free(apd->LocID);
}
