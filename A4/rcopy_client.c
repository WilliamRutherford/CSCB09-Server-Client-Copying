#include <stdlib.h>
#include<stdio.h>

#include "ftree.h"

int main( int argc, char **argv){

    if( argc == 4 ){

	return fcopy_client(argv[1], argv[2], argv[3], PORT);

    } else {

	perror("incorrect arguments");

    }
}
