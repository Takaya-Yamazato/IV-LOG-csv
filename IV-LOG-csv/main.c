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
    unsigned long    c[100000], count;
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
//    printf("SEEK_ENDのファイルポインタの位置は「%lld」です。file_size/128 = %d\n", file_size, (int)file_size/128);
//    file_size = 128;
    //    ファイルポインタを先頭まで移動
    fseek(fp,0,SEEK_SET);
//    //ファイルポインタの位置を取得
//    fgetpos(fp,&ft);
//    printf("SEEK_SETのファイルポインタの位置は「%lld」です。\n",ft);
    
    for ( k = 0 ; k < (int)file_size/128 ; ++k ){
        //    for ( k = 0 ;  ; k = k+128 ){
        fseek(fp,k*128,SEEK_SET);
        //ファイルポインタの位置を取得
        fgetpos(fp,&ft);
//        printf("ファイルポインタの位置は「%lld」です。ｋ＝%d\n",ft, k);
        
        count = fread(buff, sizeof(char), 128, fp);
        
        for( i = 0; i < 128 ; ++i ){
            c[i] = buff[i];
            if (c[i] > 0xFFFFFFFFFFFFFF00 ){ c[i] = c[i] ^ 0xFFFFFFFFFFFFFF00 ;}
//            c[128*k+i] = fgetc( fp );
//            printf("\nd[%d,%d] %lX %lu %lu\n",k,i, c[i], c[i], count);
            if( c[i] == EOF ){
                break;
            }
            if( c[i] == EOF ){
                break;
            }
        }/* for( i = 0; i < 128 ; ++i ) */
        
    
        if((c[38] >> 4) > 9){
//            if(c[12]==0x02){
//                if( (351605520 < ( c[28] << 24 | c[29] << 16 | c[30] << 8 | c[31])) && (( c[28] << 24 | c[29] << 16 | c[30] << 8 | c[31]) <351680300)){
//                    if( (1369562970 <( c[32] << 24 | c[33] << 16 | c[34] << 8 | c[35])) && (( c[32] << 24 | c[33] << 16 | c[34] << 8 | c[35]) < 136971156)){
//                        if((((c[78] >> 4) << 4 ) | c[78]) == 0x01){
//                            /* パケット受信時の PC 日時(JST) 8byte 年/月/日/時:分:ミリ秒 */
//                            printf( "%lu/%lu/%lu/%lu:%lu:%lu", c[0] << 8 | c[1],c[2],c[3],c[4],c[5],c[6] << 8 | c[7]);
//                            /* データ長 4byte*/
//                            /* printf( ", データ長:%d%d%d%d", c[8],c[9],c[10],c[11]); */
//                            printf( ",%lu", c[8] << 24 | c[9] << 16 | c[10] << 8 | c[11]);
//                            /* 車輌種別 1byte 0x00:他車輌，0x02:自車輌，0x01：固定値（路車間）*/
//                            /*switch(c[12]) {
//                             case 0x00 : printf(",0");
//                             break;
//                             case 0x01 : printf(",1");
//                             break;
//                             case 0x02 : printf(",2");
//                             break;
//                             default   : printf(",-");
//                             }*/
//                            /* タイムスタンプ有無 1byte 0x00:タイムスタンプなし，0x01:タイムスタンプあり*/
//                            //        switch(c[13]) {
//                            //          case 0x00 : printf(",0");
//                            //            break;
//                            //          case 0x01 : printf(",1");
//                            //            break;
//                            //          default   : printf(",-");
//                            //        }
//                            /* 情報フラグ 1byte bit0: 緊急車両フラグ，bit1~bit7: 予約領域*/
//                            /*if( c[14] == 0x00 ) printf(",0");
//                             else{printf(",-");}*/
//                            /* 予約 1byte 予約領域(固定値 0x00) */
//                            /*if( c[15] == 0x00 ) printf(",0");
//                             else{printf(",-");}*/
//
//                            /* ここから車車間情報 （共通領域管理情報）*/
//                            /* 共通サービス規格ID 3bit，メッセージID 2bit，バージョン情報 3bit =1byte*/
//                            /*if( c[16] == 0x29 ) printf( ",共通サービス規格ID:%2X", c[16]);*/
//                            //        if(((c[16] >> 5) & 1) == 1) {printf(",1");}
//                            //        else{printf(",-");}
//                            //        if(((c[16] >> 3) & 1) == 1) {printf(",1");}
//                            //        else{printf(",-");}
//                            //        if(((c[16] >> 0) & 1) == 1) {printf(",1");}
//                            //        else{printf(",-");}
//                            /* 車両ID 4byte */
//                            /*    printf( ", 車両ID:%X%X%X%X", c[17],c[18],c[19],c[20]); */
//                            //             printf( ",%lu", c[17] << 24 | c[18] << 16 | c[19] << 8 | c[20]);
//                            /* インクリメントカウンタ 1byte　0~255 */
//                            //             printf( ",%lu", c[21]);
//                            /* 共通アプリデータ長 1byte */
//                            //             printf( ",%lu", c[22]);
//                            /* オプションフラグ 位置オプション情報の有無 1bit．状態オプション情報の有無 1bit．位置取得オプション情報 1bit．車両状態オプション情報の有無 1bit．交差点情報の有無 1bit．拡張情報の有無 1bit．拡張オプションフラグの有無 1bit．自由領域の有無 1bit =1byte */
//                            /*printf( ",オプションフラグ:%X", c[23]);*/
//                            /* うるう秒補正情報 1bit + 時刻(時) 7bit = 1byte */
//                            /*printf( ",%d", c[24]^0x80 );*/
//                            /* 時刻(時) 7bit */
//                                    printf(",%lu",(c[24] ^ ((c[24] >> 7) << 7)));
//                            /* 時刻(分) 1byte */
//                                         printf( ",%lu", c[25] );
//                            /* 時刻(秒) 2byte */
//                            /*    printf( ", 時刻(秒):%u.%u", c[26],c[27] );   */
//                                         printf( ",%f", (c[26] << 8 | c[27]) * 0.001 );

        /* GPSデータ変換は以下を参考に
         https://stackoverflow.com/questions/17355604/how-to-convert-gps-longitude-and-latitude-from-hex */
        /* 緯度 4byte */
        printf( "%lf", ( c[28] << 24 | c[29] << 16 | c[30] << 8 | c[31]) * 0.0000001 );
        /* 経度 4byte */
        printf( ",%lf", ( c[32] << 24 | c[33] << 16 | c[34] << 8 | c[35]) * 0.0000001 );
        /* 高度 2byte */
        //             printf( ",%f", ( c[36] << 8 | c[37] ) * 0.1 );
        /* 位置取得情 4bit，高度取得情報 4bit = 1byte */
        /*printf( ",%X", c[38] );             */
        /* 位置取得情報 (上位4bit)*/
        if((c[38] >> 7) == 1){
            /*11xx*/
            if((c[38] >> 6) == 3){
                /*111x*/
                if((c[38] >> 5) == 7){
                    if((c[38] >> 4) == 15){printf(",15");}/*1111*/
                    else{printf(",14");}/*1110*/
                }
                /*110x*/
                else{
                    if((c[38] >> 4) == 13){printf(",13");}/*1101*/
                    else{printf(",12");}/*1100*/
                }
            }
            /*10xx*/
            else{
                /*101x*/
                if((c[38] >> 5) == 5){
                    if((c[38] >> 4) == 11){printf(",11");}/*1011*/
                    else{printf(",10");}/*1010*/
                }
                /*100x*/
                else{
                    if((c[38] >> 4) == 9){printf(",9");}/*1001*/
                    else{printf(",8");}/*1000*/
                }
            }
        }
        else{
            /*01xx*/
            if((c[38] >> 6) == 1){
                /*011x*/
                if((c[38] >> 5) == 3){
                    if((c[38] >> 4) == 7){printf(",7");}/*0111*/
                    else{printf(",6");}/*0110*/
                }
                /*010x*/
                else{
                    if((c[38] >> 4) == 5){printf(",5");}/*0101*/
                    else{printf(",4");}/*0100*/
                }
            }
            /*00xx*/
            else{
                /*001x*/
                if((c[38] >> 5) == 1){
                    if((c[38] >> 4) == 3){printf(",3");}/*0011*/
                    else{printf(",2");}/*0010*/
                }
                /*000x*/
                else{
                    if((c[38] >> 4) == 1){printf(",1");}/*0001*/
                    else{printf(",0");}/*0000*/
                }
            }
        }
    printf("\n");
            
        }
//                    }
//                }
//            }
//        }
        
        

    } //  for ( k = 0 ; k < file_size/128 ; k = k+128 )
    

    
    if( fclose( fp ) == EOF ){
        fputs( "ファイルクローズに失敗しました。\n", stderr );
        exit( EXIT_FAILURE );
    }
    
    return 0;
    
    
    
}  /* main */
