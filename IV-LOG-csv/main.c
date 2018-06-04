//
//  main.c
//  IV-LOG-csv
//
//  Created by Takaya Yamazato on 2018/06/01.
//  Copyright © 2018年 Takaya Yamazato. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[] )
{
    //    char fileName[128];
    //    int text;
    unsigned long    c[100000];
    char buff[2048];
    int i,k;
    FILE *fp;
    fpos_t ft;
    long long file_size;
    
    if(argc<2){
        printf("コマンドの後に１つファイルを指定してください。\n");
        printf("使用法： ./output_file <ファイル名>\n");
        exit(1);
    }
    
    if ((fp = fopen(argv[1], "r"))==NULL) {
        printf("ファイルを開くのに失敗しました\n");
        exit(1);
    }
    
    //    ファイルポインタを末尾まで移動
    fseek(fp,0,SEEK_END);
    //ファイルポインタの位置を取得
    fgetpos(fp,&ft);
    file_size = ft;
    printf("SEEK_ENDのファイルポインタの位置は「%lld」です。file_size/128 = %d\n", file_size, (int)file_size/128);
    
    //    ファイルポインタを末尾まで移動
    fseek(fp,0,SEEK_SET);
    //ファイルポインタの位置を取得
    fgetpos(fp,&ft);
    printf("SEEK_SETのファイルポインタの位置は「%lld」です。\n",ft);
    
    for ( k = 0 ; k < (int)file_size/128 ; ++k ){
        //    for ( k = 0 ;  ; k = k+128 ){
        fseek(fp,k*128,SEEK_SET);
        //ファイルポインタの位置を取得
        fgetpos(fp,&ft);
        printf("ファイルポインタの位置は「%lld」です。ｋ＝%d\n",ft, k);
        
        fread(buff, sizeof(char), 128, fp);
        for( i = 0; i < 128 ; ++i ){
            c[k*128+i] = buff[i];
            printf("\nc[%d,%d] %lu\n",k,i, c[k*128+i]);
            if( c[128*k+i] == EOF ){
                //                printf("\nbreak point (k+i=%d)\n",k+i);
                break;
            }
        }/* for( i = 0; i < 128 ; ++i ) */
    }
    /* for ( k = 0 ;  ; k = k+128 ){ */
    for ( k = 0 ;  ; k = k+128 ){
        for( i = 0; i < 128 ; ++i ){
            c[k+i] = fgetc( fp );
            if( c[k+i] == EOF ){
                /*         printf("\nbreak point (k+i=%d)\n",k+i);  */
                break;
            }
        }
        if( c[k+i] == EOF ){
            /*         printf("\nbreak point (k+i=%d)\n",k+i);  */
            break;
        }
        
        if((c[k+38] >> 4) > 9){
            if(c[k+12]==0x02){
                if( (351605520 < ( c[k+28] << 24 | c[k+29] << 16 | c[k+30] << 8 | c[k+31])) && (( c[k+28] << 24 | c[k+29] << 16 | c[k+30] << 8 | c[k+31]) <351680300)){
                    if( (1369562970 <( c[k+32] << 24 | c[k+33] << 16 | c[k+34] << 8 | c[k+35])) && (( c[k+32] << 24 | c[k+33] << 16 | c[k+34] << 8 | c[k+35]) < 136971156)){
                        if((((c[k+78] >> 4) << 4 ) | c[k+78]) == 0x01){
                            /* パケット受信時の PC 日時(JST) 8byte 年/月/日/時:分:ミリ秒 */
                            printf( "%lu/%lu/%lu/%lu:%lu:%lu", c[k+0] << 8 | c[k+1],c[k+2],c[k+3],c[k+4],c[k+5],c[k+6] << 8 | c[k+7]);
                            /* データ長 4byte*/
                            /* printf( ", データ長:%d%d%d%d", c[k+8],c[k+9],c[k+10],c[k+11]); */
                            printf( ",%lu", c[k+8] << 24 | c[k+9] << 16 | c[k+10] << 8 | c[k+11]);
                            /* 車輌種別 1byte 0x00:他車輌，0x02:自車輌，0x01：固定値（路車間）*/
                            /*switch(c[k+12]) {
                             case 0x00 : printf(",0");
                             break;
                             case 0x01 : printf(",1");
                             break;
                             case 0x02 : printf(",2");
                             break;
                             default   : printf(",-");
                             }*/
                            /* タイムスタンプ有無 1byte 0x00:タイムスタンプなし，0x01:タイムスタンプあり*/
                            //        switch(c[k+13]) {
                            //          case 0x00 : printf(",0");
                            //            break;
                            //          case 0x01 : printf(",1");
                            //            break;
                            //          default   : printf(",-");
                            //        }
                            /* 情報フラグ 1byte bit0: 緊急車両フラグ，bit1~bit7: 予約領域*/
                            /*if( c[k+14] == 0x00 ) printf(",0");
                             else{printf(",-");}*/
                            /* 予約 1byte 予約領域(固定値 0x00) */
                            /*if( c[k+15] == 0x00 ) printf(",0");
                             else{printf(",-");}*/
                            
                            /* ここから車車間情報 （共通領域管理情報）*/
                            /* 共通サービス規格ID 3bit，メッセージID 2bit，バージョン情報 3bit =1byte*/
                            /*if( c[k+16] == 0x29 ) printf( ",共通サービス規格ID:%2X", c[k+16]);*/
                            //        if(((c[k+16] >> 5) & 1) == 1) {printf(",1");}
                            //        else{printf(",-");}
                            //        if(((c[k+16] >> 3) & 1) == 1) {printf(",1");}
                            //        else{printf(",-");}
                            //        if(((c[k+16] >> 0) & 1) == 1) {printf(",1");}
                            //        else{printf(",-");}
                            /* 車両ID 4byte */
                            /*    printf( ", 車両ID:%X%X%X%X", c[k+17],c[k+18],c[k+19],c[k+20]); */
                            //             printf( ",%lu", c[k+17] << 24 | c[k+18] << 16 | c[k+19] << 8 | c[k+20]);
                            /* インクリメントカウンタ 1byte　0~255 */
                            //             printf( ",%lu", c[k+21]);
                            /* 共通アプリデータ長 1byte */
                            //             printf( ",%lu", c[k+22]);
                            /* オプションフラグ 位置オプション情報の有無 1bit．状態オプション情報の有無 1bit．位置取得オプション情報 1bit．車両状態オプション情報の有無 1bit．交差点情報の有無 1bit．拡張情報の有無 1bit．拡張オプションフラグの有無 1bit．自由領域の有無 1bit =1byte */
                            /*printf( ",オプションフラグ:%X", c[k+23]);*/
                            /* うるう秒補正情報 1bit + 時刻(時) 7bit = 1byte */
                            /*printf( ",%d", c[k+24]^0x80 );*/
                            /* 時刻(時) 7bit */
                            //        printf(",%lu",(c[k+24] ^ ((c[k+24] >> 7) << 7)));
                            /* 時刻(分) 1byte */
                            //             printf( ",%lu", c[k+25] );
                            /* 時刻(秒) 2byte */
                            /*    printf( ", 時刻(秒):%u.%u", c[k+26],c[k+27] );   */
                            //             printf( ",%f", (c[k+26] << 8 | c[k+27]) * 0.001 );
                        }
                    }
                }
            }
        }
        
        
        
    } //  for ( k = 0 ; k < file_size/128 ; k = k+128 )
    
    
    if( fclose( fp ) == EOF ){
        fputs( "ファイルクローズに失敗しました。\n", stderr );
        exit( EXIT_FAILURE );
    }
    
    return 0;
    
    
    
}  /* main */
