//
//  main.c
//  IV-LOG-csv
//
//  Created by Takaya Yamazato on 2018/06/01.
//  Copyright © 2018年 Takaya Yamazato. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

/* macros */
#define N 4
        FILE* fp;

int main(int argc, const char * argv[]) {
    int k;
    //    fp = fopen( "/Users/yamazato/Documents/tmp/IV-LOG-csv/IV-LOG-csv/test.bin", "wb" );
    fp = fopen( "/Volumes/yamazato/Documents/tmp/IV-LOG-csv/IV-LOG-csv/test.bin", "wb" );
    if( fp == NULL ){
        fputs( "ファイルオープンに失敗しました。\n", stderr );
        exit( EXIT_FAILURE );
    }


    if (fp != NULL)
    {
        char test[256];

        for (int i=0; i<256; i++)
        { test[i] = i ;}

        fwrite(test, sizeof(char) * 16, 16, fp);
        fclose(fp);

    }
    if (fp !=NULL)
    {
//        fopen("/Users/yamazato/Documents/tmp/IV-LOG-csv/IV-LOG-csv/test.bin", "rb");
        fopen("/Volumes/yamazato/Documents/tmp/IV-LOG-csv/IV-LOG-csv/test.bin", "rb");
        char buff[128];
//                memset(buff,0,sizeof(char)*16);
        for (k=1; k<16; k++){
        fread(buff, sizeof(char)*16, k, fp);
        for (int i = 0; i < 16; i++) {
            printf("%X\n",  buff[i]);
        }
        }

        fclose(fp);
    }


//#include <stdio.h>
//#include <stdlib.h>
//
//int main() {
//    char fileName[128];
//    int fileIndex , text;
//    FILE *fp;
//
//    printf("読み込むファイル名を指定してください>");
//    scanf("%s" , fileName);
//    printf("ファイルを読み出す開始位置をバイト単位で指定してください>");
//    scanf("%d" , &fileIndex);
//
//    if ((fp = fopen(fileName , "rb")) == NULL) {
//        fprintf(stderr , "ファイル操作でエラーが発生");
//        exit(1);
//    }
//
//    fseek(fp , fileIndex , SEEK_SET);
//    printf("///ファイル位置 %ld から読み出します///\n" , ftell(fp));
//
//    while(1) {
//        text = fgetc(fp);
//        if (!feof(fp)) printf("%X" , text);
//        else break;
//    }
//    fclose(fp);
//    return 0;
//}
//
//    // insert code here...
//    printf("Hello, World!\n");
//    return 0;
//


}
