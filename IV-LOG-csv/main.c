//
//  main.c
//  IV-LOG-csv
//
//  Created by Takaya Yamazato on 2018/06/01.
//  Copyright © 2018年 Takaya Yamazato. All rights reserved.
//
//書式    size_t fread( void *buf, size_t size, size_t num, FILE *fp )
//機能    バイナリファイルを読み込む
//引数    void *buf : バイナリデータを受け取るバッファ
//size_t size : データ要素1個のサイズ
//size_t num : データの個数
//FILE *fp : ファイル(ストリーム)ポインタ
//戻り値    成功すると、読み込んだデータの個数を返し、データが無いと、0を返します。

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
        char test[2048];

        for (int i=0; i<2048; i++)
        { test[i] = i ;}

        fwrite(test, sizeof(char) * 128, 16, fp);
        fclose(fp);

    }
        fpos_t ft;
  
    
    if (fp !=NULL)
    {
//        fopen("/Users/yamazato/Documents/tmp/IV-LOG-csv/IV-LOG-csv/test.bin", "rb");
        fopen("/Volumes/yamazato/Documents/tmp/IV-LOG-csv/IV-LOG-csv/test.bin", "rb");
        char buff[2048];
//                memset(buff,0,sizeof(char)*16);
        //    ファイルポインタを末尾まで移動
        fseek(fp,0,SEEK_SET);
        //ファイルポインタの位置を取得
        fgetpos(fp,&ft);
        printf("SEEK_SETのファイルポインタの位置は「%lld」です。\n",ft);
        //    ファイルポインタを末尾まで移動
        fseek(fp,0,SEEK_END);
        //ファイルポインタの位置を取得
        fgetpos(fp,&ft);
        printf("SEEK_ENDのファイルポインタの位置は「%lld」です。\n",ft);
        
        for (k=0; k<16; k++){
            //    ファイルポインタを末尾まで移動
            fseek(fp,k*128,SEEK_SET);
            //ファイルポインタの位置を取得
            fgetpos(fp,&ft);
            printf("現在のファイルポインタの位置は「%lld」です。\n",ft);

        fread(buff, sizeof(char)*128, 16, fp);
        for (int i = 0; i < 128; i++) {
            printf("%d-%d %lu %X\n", fgetpos(fp,&ft), i, sizeof(char)*128, buff[i]);
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
//#include <stdio.h>
//
//    int main(void)
//    {
//        fpos_t ft;
//        FILE *fp;
//
//
//        fp = fopen("test.txt","r");
//
//        //ファイルポインタを末尾まで移動
//        fseek(fp,0,SEEK_END);
//
//        //ファイルポインタの位置を取得
//        fgetpos(fp,&ft);
//
//        printf("現在のファイルポインタの位置は「%d」です。\n",ft);
//
//        fclose(fp);
//
//        return 0;
//    }
//    // insert code here...
//    printf("Hello, World!\n");
//    return 0;
//


}
