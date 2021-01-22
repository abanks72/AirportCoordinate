#ifndef AIR_PDATA
#define AIR_PDATA

typedef struct airPdata{
	char *LocID; //Airport’s ‘‘Short Name’’, ie MCO
	char *fieldName; //Airport Name
	char *city; //Associated City
	float longitude; //Longitude
	float latitude; //Latitude
} airPdata;

#endif