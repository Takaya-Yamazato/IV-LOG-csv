#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    
	FILE* fp;
	int num = 900;
	double d = 7.85;
	char str[] = "xyz\nxyz";


	fp = fopen( "test.bin", "wb" );
	if( fp == NULL ){
		fputs( "ファイルオープンに失敗しました。\n", stderr );
		exit( EXIT_FAILURE );
	}


	fwrite( &num, sizeof(num), 1, fp );
	fwrite( &d, sizeof(d), 1, fp );
	fwrite( str, sizeof(char), sizeof(str), fp );


	if( fclose( fp ) == EOF ){
		fputs( "ファイルクローズに失敗しました。\n", stderr );
		exit( EXIT_FAILURE );
	}

	return 0;
	}
