#include "ReadFile.h"

char* loadFile(FILE* fp)
{
    long lSize;
    char* buffer;
    if(!fp){
	fprintf(stderr, "Invalid file pointer when trying to load file");
	exit(1);
    }
    
    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = calloc(1, lSize);
    
    if(!buffer){
	fclose(fp);
	free(buffer);
	fprintf(stderr,"memory alloc fails when trying to load file");
	exit(1);
    }
    
    int result = fread(buffer, lSize-1, 1, fp);

    if( result != 1){
	fclose(fp);
	free(buffer);
	fprintf(stderr,"Failed to read entire file");
	exit(1);
    }
    return buffer;
}
