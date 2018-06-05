//
//  main.c
//  IV-LOG-csv
//
//  Created by Takaya Yamazato on 2018/06/05.
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
    unsigned long long file_size;
    
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

    //    ファイルポインタを先頭まで移動
    fseek(fp,0,SEEK_SET);
//    //ファイルポインタの位置を取得
//    fgetpos(fp,&ft);
//    printf("SEEK_SETのファイルポインタの位置は「%lld」です。\n",ft);
    
    for ( k = 0 ; k < (unsigned long long)file_size/128 ; ++k ){
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
        
/* ここから三谷くんのソースファイルからコピー */
        if((c[38] >> 4) > 9){
            //        if(c[12]==0x02){
            //            if( (351605520 < ( c[28] << 24 | c[29] << 16 | c[30] << 8 | c[31])) && (( c[28] << 24 | c[29] << 16 | c[30] << 8 | c[31]) <351680300)){
            //                if( (1369562970 <( c[32] << 24 | c[33] << 16 | c[34] << 8 | c[35])) && (( c[32] << 24 | c[33] << 16 | c[34] << 8 | c[35]) < 136971156)){
            //                    if((((c[78] >> 4) << 4 ) | c[78]) == 0x01){
            /* パケット受信時の PC 日時(JST) 8byte 年/月/日/時:分:ミリ秒 */
                    printf( "%lu/%lu/%lu/%lu:%lu:%5lu", c[0] << 8 | c[1],c[2],c[3],c[4],c[5],c[6] << 8 | c[7]);
            /* データ長 4byte*/
            /* printf( ", データ長:%d%d%d%d", c[8],c[9],c[10],c[11]); */
                    printf( ",%lu", c[8] << 24 | c[9] << 16 | c[10] << 8 | c[11]);
            /* 車輌種別 1byte 0x00:他車輌，0x02:自車輌，0x01：固定値（路車間）*/
            switch(c[12]) {
             case 0x00 : printf(",0 他車輌");
             break;
             case 0x01 : printf(",1　路車間");
             break;
             case 0x02 : printf(",2　自車輌");
             break;
             default   : printf(",-");
             }
            /* タイムスタンプ有無 1byte 0x00:タイムスタンプなし，0x01:タイムスタンプあり*/
                    switch(c[13]) {
                      case 0x00 : printf(",0　タイムスタンプ無し");
                        break;
                      case 0x01 : printf(",1　タイムスタンプあり");
                        break;
                      default   : printf(",-");
                    }
            /* 情報フラグ 1byte bit0: 緊急車両フラグ，bit1~bit7: 予約領域*/
            if( c[14] == 0x00 ) printf(",0　緊急車両フラグ");
             else{printf(",-");}
            /* 予約 1byte 予約領域(固定値 0x00) */
            // if( c[15] == 0x00 ) printf(",0");
            //   else{printf(",-");}
            
            /* ここから車車間情報 （共通領域管理情報）*/
            /* 共通サービス規格ID 3bit，メッセージID 2bit，バージョン情報 3bit =1byte*/
            /*if( c[16] == 0x29 ) printf( ",共通サービス規格ID:%2X", c[16]);*/
            //        if(((c[16] >> 5) & 1) == 1) {printf(",1");}
            //        else{printf(",-");}
            //        if(((c[16] >> 3) & 1) == 1) {printf(",1");}
            //        else{printf(",-");}
            //        if(((c[16] >> 0) & 1) == 1) {printf(",1");}
            //        else{printf(",-");}
            /* 車両ID 4byte */
            /*    printf( ", 車両ID:%X%X%X%X", c[17],c[18],c[19],c[20]); */
            //             printf( ",%lu", c[17] << 24 | c[18] << 16 | c[19] << 8 | c[20]);
            /* インクリメントカウンタ 1byte　0~255 */
            //             printf( ",%lu", c[21]);
            /* 共通アプリデータ長 1byte */
            //             printf( ",%lu", c[22]);
            /* オプションフラグ 位置オプション情報の有無 1bit．状態オプション情報の有無 1bit．位置取得オプション情報 1bit．車両状態オプション情報の有無 1bit．交差点情報の有無 1bit．拡張情報の有無 1bit．拡張オプションフラグの有無 1bit．自由領域の有無 1bit =1byte */
            /*printf( ",オプションフラグ:%X", c[23]);*/
            /* うるう秒補正情報 1bit + 時刻(時) 7bit = 1byte */
            /*printf( ",%d", c[24]^0x80 );*/
            /* 時刻(時) 7bit */
            //        printf(",%lu",(c[24] ^ ((c[24] >> 7) << 7)));
            /* 時刻(分) 1byte */
            //             printf( ",%lu", c[25] );
            /* 時刻(秒) 2byte */
            /*    printf( ", 時刻(秒):%u.%u", c[26],c[27] );   */
            //             printf( ",%f", (c[26] << 8 | c[27]) * 0.001 );
            
            /* GPSデータ変換は以下を参考に
             https://stackoverflow.com/questions/17355604/how-to-convert-gps-longitude-and-latitude-from-hex */
            /* 緯度 4byte */
            printf(",GPSデータ:");
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
            /* 高度取得情報 (下位4bit)*/
            //        if((c[38] >> 3) == 1){
            //            /*11xx*/
            //            if((c[38] >> 2) == 3){
            //                /*111x*/
            //                if((c[38] >> 1) == 7){
            //                    if((c[38] >> 0) == 15){printf(",15");}/*1111*/
            //                    else{printf(",14");}/*1110*/
            //                }
            /*110x*/
            //                else{
            //                    if((c[38] >> 0) == 6){printf(",13");}/*1101*/
            //                    else{printf(",12");}/*1100*/
            //                }
            //            }
            /*10xx*/
            //            else{
            /*101x*/
            //                if((c[38] >> 1) == 5){
            //                    if((c[38] >> 0) == 11){printf(",11");}/*1011*/
            //                    else{printf(",10");}/*1010*/
            //                }
            /*100x*/
            //                else{
            //                    if((c[38] >> 0) == 4){printf(",9");}/*1001*/
            //                    else{printf(",8");}/*1000*/
            //                }
            //            }
            //        }
            //        else{
            /*01xx*/
            //            if((c[38] >> 2) == 1){
            /*011x*/
            //                if((c[38] >> 1) == 3){
            //                    if((c[38] >> 0) == 7){printf(",7");}/*0111*/
            //                    else{printf(",6");}/*0110*/
            //                }
            /*010x*/
            //                else{
            //                    if((c[38] >> 0) == 5){printf(",5");}/*0101*/
            //                    else{printf(",4");}/*0100*/
            //                }
            //            }
            /*00xx*/
            //            else{
            /*001x*/
            //                if((c[38] >> 1) == 1){
            //                    if((c[38] >> 0) == 3){printf(",3");}/*0011*/
            //                    else{printf(",2");}/*0010*/
            //                }
            /*000x*/
            //                else{
            //                    if((c[38] >> 4) == 1){printf(",1");}/*0001*/
            //                    else{printf(",0");}/*0000*/
            //                }
            //            }
            //        }
            /* 車速 16bit 2byte */
            //             printf( ",%f", ( c[39] << 8 | c[40] ) * 0.01 );
            /* 車両方位角 2byte */
            //             printf( ",%f", ( c[41] << 8 | c[42] ) * 0.0125 );
            /* 前後加速度 2byte */
            //             if( ( c[43] << 8 | c[44] ) == 0x8000 ) { printf( ",-" ); }
            //             else { printf( ",%f", ( c[43] << 8 | c[44] ) * 0.01 );  }
            /* 車速取得情報 3bit，車両方位角取得情報 3bit，前後加速度取得情報 3bit, シフトポジション 3bit, ステアリング角度 12bit = 3byte */
            /*printf( ", 車輌挙動情報:%X%X%X", c[45], c[46], c[47] );*/
            /*車速取得情報*/
            //            if((c[45] >> 7) == 1){
            //                /*11x*/
            //                if((c[45] >> 6) == 3){
            //                    if((c[45] >> 5) == 7){printf(",7");}/*111*/
            //                    else{printf(",6");}/*110*/
            //                }
            /*10x*/
            //                else{
            //                    if((c[45] >> 5) == 5){printf(",5");}/*101*/
            //                    else{printf(",4");}/*100*/
            //                }
            //            }
            /*0xx*/
            //            else{
            /*01x*/
            //                if((c[45] >> 6) == 1){
            //                    if((c[45] >> 5) == 3){printf(",3");}/*011*/
            //                    else{printf(",2");}/*010*/
            //                }
            /*00x*/
            //                else{
            //                    if((c[45] >> 5) == 1){printf(",1");}/*001*/
            //                    else{printf(",0");}/*000*/
            //                }
            //            }
            /*車両方位角取得情報*/
            //            if((c[45] >> 4) % 2 == 1){
            /*11x*/
            //                if((c[45] >> 3) %2 == 1){
            //                    if((c[45] >> 2) %2 == 1){printf(",7");}/*111*/
            //                    else{printf(",6");}/*110*/
            //                }
            /*10x*/
            //                else{
            //                    if((c[45] >> 2) == 5){printf(",5");}/*101*/
            //                    else{printf(",4");}/*100*/
            //                }
            //            }
            /*0xx*/
            //            else{
            /*01x*/
            //                if((c[45] >> 3) %2 == 1){
            //                    if((c[45] >> 2) %2 == 1){printf(",3");}/*011*/
            //                    else{printf(",2");}/*010*/
            //                }
            /*00x*/
            //                else{
            //                    if((c[45] >> 2) %2 == 1){printf(",1");}/*001*/
            //                    else{printf(",0");}/*000*/
            //                }
            //            }
            /*前後加速度取得情報*/
            //            if((c[45] >> 1) % 2 == 1){
            //                /*11x*/
            //                if((c[45] >> 0) %2 == 1){
            //                    if((c[46] >> 7) %2 == 1){printf(",7");}/*111*/
            //                    else{printf(",6");}/*110*/
            //                }
            //                /*10x*/
            //                else{
            //                    if((c[46] >> 7) == 5){printf(",5");}/*101*/
            //                    else{printf(",4");}/*100*/
            //                }
            //            }
            //            /*0xx*/
            //            else{
            //                /*01x*/
            //                if((c[45] >> 0) %2 == 1){
            //                    if((c[46] >> 7) %2 == 1){printf(",3");}/*011*/
            //                    else{printf(",2");}/*010*/
            //                }
            //                /*00x*/
            //                else{
            //                    if((c[46] >> 7) %2 == 1){printf(",1");}/*001*/
            //                    else{printf(",0");}/*000*/
            //                }
            //            }
            //            /*シフトポジション*/
            //            if((c[46] >> 6) % 2 == 1){
            //                /*11x*/
            //                if((c[46] >> 5) %2 == 1){
            //                    if((c[46] >> 4) %2 == 1){printf(",7");}/*111*/
            //                    else{printf(",6");}/*110*/
            //                }
            //                /*10x*/
            //                else{
            //                    if((c[46] >> 4) == 5){printf(",5");}/*101*/
            //                    else{printf(",4");}/*100*/
            //                }
            //            }
            //            /*0xx*/
            //            else{
            //                /*01x*/
            //                if((c[46] >> 5) %2 == 1){
            //                    if((c[46] >> 4) %2 == 1){printf(",3");}/*011*/
            //                    else{printf(",2");}/*010*/
            //                }
            //                /*00x*/
            //                else{
            //                    if((c[46] >> 4) %2 == 1){printf(",1");}/*001*/
            //                    else{printf(",0");}/*000*/
            //                }
            //            }
            //             /* 車両サイズ種別 4bit，車両用途種別 4bit = 1byte */
            //             /*printf( ", 車両サイズ・用途:%X", c[48] );             */
            //             /* 車両サイズ種別 (上位4bit)*/
            //        if((c[48] >> 7) == 1){
            //            /*11xx*/
            //            if((c[48] >> 6) == 3){
            //                /*111x*/
            //                if((c[48] >> 5) == 7){
            //                    if((c[48] >> 4) == 15){printf(",15");}/*1111*/
            //                    else{printf(",14");}/*1110*/
            //                }
            //                /*110x*/
            //                else{
            //                    if((c[48] >> 4) == 6){printf(",13");}/*1101*/
            //                    else{printf(",12");}/*1100*/
            //                }
            //            }
            //            /*10xx*/
            //            else{
            //                /*101x*/
            //                if((c[48] >> 5) == 5){
            //                    if((c[48] >> 4) == 11){printf(",11");}/*1011*/
            //                    else{printf(",10");}/*1010*/
            //                }
            //                /*100x*/
            //                else{
            //                    if((c[48] >> 4) == 4){printf(",9");}/*1001*/
            //                    else{printf(",8");}/*1000*/
            //                }
            //            }
            //        }
            //        else{
            //            /*01xx*/
            //            if((c[48] >> 6) == 1){
            //                /*011x*/
            //                if((c[48] >> 5) == 3){
            //                    if((c[48] >> 4) == 7){printf(",7");}/*0111*/
            //                    else{printf(",6");}/*0110*/
            //                }
            //                /*010x*/
            //                else{
            //                    if((c[48] >> 4) == 5){printf(",5");}/*0101*/
            //                    else{printf(",4");}/*0100*/
            //                }
            //            }
            //            /*00xx*/
            //            else{
            //                /*001x*/
            //                if((c[48] >> 5) == 1){
            //                    if((c[48] >> 4) == 3){printf(",3");}/*0011*/
            //                    else{printf(",2");}/*0010*/
            //                }
            //                /*000x*/
            //                else{
            //                    if((c[48] >> 4) == 1){printf(",1");}/*0001*/
            //                    else{printf(",0");}/*0000*/
            //                }
            //            }
            //        }
            /* 車両用途種別 (下位4bit)*/
            //        if((c[48] >> 3) == 1){
            //            /*11xx*/
            //            if((c[48] >> 2) == 3){
            //                /*111x*/
            //                if((c[48] >> 1) == 7){
            //                    if((c[48] >> 0) == 15){printf(",15");}/*1111*/
            //                    else{printf(",14");}/*1110*/
            //                }
            /*110x*/
            //                else{
            //                    if((c[48] >> 0) == 6){printf(",13");}/*1101*/
            //                    else{printf(",12");}/*1100*/
            //                }
            //            }
            /*10xx*/
            //            else{
            //                /*101x*/
            //                if((c[48] >> 1) == 5){
            //                    if((c[48] >> 0) == 11){printf(",11");}/*1011*/
            //                    else{printf(",10");}/*1010*/
            //                }
            //                /*100x*/
            //                else{
            //                    if((c[48] >> 0) == 4){printf(",9");}/*1001*/
            //                    else{printf(",8");}/*1000*/
            //                }
            //            }
            //        }
            //        else{
            //            /*01xx*/
            //            if((c[48] >> 2) == 1){
            //                /*011x*/
            //                if((c[48] >> 1) == 3){
            //                    if((c[48] >> 0) == 7){printf(",7");}/*0111*/
            //                    else{printf(",6");}/*0110*/
            //                }
            /*010x*/
            //                else{
            //                    if((c[48] >> 0) == 5){printf(",5");}/*0101*/
            //                    else{printf(",4");}/*0100*/
            //                }
            //            }
            /*00xx*/
            //            else{
            /*001x*/
            //                if((c[48] >> 1) == 1){
            //                    if((c[48] >> 0) == 3){printf(",3");}/*0011*/
            //                    else{printf(",2");}/*0010*/
            //                }
            /*000x*/
            //                else{
            //                    if((c[48] >> 4) == 1){printf(",1");}/*0001*/
            //                    else{printf(",0");}/*0000*/
            //                }
            //            }
            //        }
            /* 車幅 10bit，車長 14bit = 3byte */
            /*printf( ", 車幅・車長:%X%X%X", c[49], c[50], c[51] );*/
            /*車幅*/
            //        printf( ",%f", ( c[49] << 2 | c[50] >> 6) * 0.01 );
            //        /*車長*/
            //        printf( ",%f", ((c[50] ^ ( c[50] >> 6 ) << 6) << 8| c[51] ) * 0.01 );
            /* 位置情報遅れ時間 5bit，リビジョンカウンタ 5bit，道路施設情報 3bit，道路区分情報 3bit = 2byte */
            /*printf( ", 道路区分情報途:%X%X", c[52], c[53] );*/
            //        if((c[52] >> 3) == 31){printf(",-");}
            //        else{printf( ",%f", ( c[52] >> 3 )* 0.1 );}
            /*リビジョンカウンタ*/
            //        if((((((c[52] >> 3) << 3) ^ c[52]) << 2) | c[53] >> 6) == 31){printf(",-");}
            //        else{printf( ",%f", (((((c[52] >> 3) << 3) ^ c[52]) << 2) | c[53] >> 6)* 0.1 );}
            /*道路施設情報*/
            /*何故かうまく出力されない*/
            /*printf(",道路%d",((((c[53] >> 5) << 5) >> 2) ^ c[53]));
             if((c[53] >> 5) %2 == 1){*/
            /*11x*/
            /*if((c[53] >> 4) %2 == 1){
             if((c[53] >> 3) %2 == 1){printf(",7");}*//*111*/
            /*else{printf(",6");}*//*110*/
            /*}*/
            /*10x*/
            /*else{
             if((c[53] >> 3) %2 == 1){printf(",5");}*//*101*/
            /*else{printf(",4");}*//*100*/
            /*}
             }*/
            /*0xx*/
            /*else{*/
            /*01x*/
            /*if((c[53] >> 4) %2 == 1){
             if((c[46] >> 3) %2 == 1){printf(",3");}*//*011*/
            /*else{printf(",2");}*//*010*/
            /*}*/
            /*00x*/
            /*else{
             if((c[46] >> 3) %2 == 1){printf(",1");}*//*001*/
            /*else{printf(",-");}*//*000*/
            /*}*/
            /*}*/
            /*道路区分情報*/
            /*if((c[53] >> 2) %2 == 1){*/
            /*11x*/
            /*if((c[53] >> 1) %2 == 1){*/
            /*if((c[53] >> 0) %2 == 1){printf(",7");}*//*111*/
            /*else{printf(",6");}*//*110*/
            /*}*/
            /*10x*/
            /*else{*/
            /*if((c[53] >> 0) == 5){printf(",5");}*//*101*/
            /*else{printf(",4");}*//*100*/
            /*}*/
            /*}*/
            /*0xx*/
            /*else{*/
            /*01x*/
            /*if((c[53] >> 4) %2 == 1){*/
            /*if((c[46] >> 3) %2 == 1){printf(",3");}*//*011*/
            /*else{printf(",2");}*//*010*/
            /*}*/
            /*00x*/
            /*else{*/
            /*if((c[46] >> 3) %2 == 1){printf(",1");}*//*001*/
            /*else{printf(",-");}*//*000*/
            /*}
             }*/
            /* 位置情報誤差楕円長半径 1byte */
            /*if( c[54] == 0xFF ) { printf( ",-" ); }
             else {                  printf( ",%f", c[54] * 0.5 ); }*/
            /* 位置情報誤差楕円短半径 1byte */
            /*if( c[55] == 0xFF ) { printf( ",-" ); }
             else {                  printf( ",%f", c[55] * 0.5 );    }*/
            /* 位置情報誤差楕円回転角 2byte */
            /*if( ( c[56] << 8 | c[57] ) == 0xFFFF ) { printf( ",-" ); }
             else {                  printf( ",%f", ( c[56] << 8 | c[57] ) * 0.0125 );   }*/
            
            /* GNSS測位モード 2bit，GNSS位置精度低下率 6bit = 1byte */
            /*printf( ", GNSS 測位モード・位置精度低下率:%X", c[58] );      */
            /* GNSS捕捉衛星数 4bit，GNSSマルチパス検出 2bit, 自律航法機能情報 1bit, マップマッチング機能情報 1bit = 1byte */
            /*printf( ", GNSS捕捉衛星数等:%X", c[59] );                 */
            
            
            /* ヨーレート 2byte */
            //             if( ( c[60] << 8 | c[61] ) == 0x8000 ) { printf( ",-" ); }
            //             else {                  printf( ",%f", ( c[60] << 8 | c[61] ) * 0.01 );   }
            /* ブレーキ状態 6bit，補助ブレーキ状態 2bit = 1byte */
            /*printf( ", ブレーキ・補助ブレーキ状態:%X", c[62] );      */
            //            if((c[62] >> 2) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[62] >> 3) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[62] >> 4) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[62] >> 5) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[62] >> 6) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[62] >> 7) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            /*補助ブレーキ状態*/
            //            if((c[62]%2) == 0 ){
            //                if((c[62] >> 1) %2 == 0){printf(",-");}
            //                else{printf(",2");}
            //            }
            //            else{
            //                if(c[62] %2 == 0){printf(",1");}
            //                else{printf(",3");}
            //            }
            
            /* アクセルペダル開度 1byte */
            /*printf( ",%f", c[63] * 0.005 );*/
            /* 灯火類状態 1byte */
            /*printf( ", 灯火類状態:%X", c[64] );      */
            //            if((c[64] ) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[64] >> 1) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[64] >> 2) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[64] >> 3) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[64] >> 4) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[64] >> 5) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            //            if((c[64] >> 6) %2 == 1){printf(",1");}
            //            else{printf(",0");}
            
            /* ACC 作動状態 2bit, C-ACC 作動状態 2bit, PCS 作動状態 2bit, ABS 作動状態 2bit = 1byte */
            /* printf( ", ACC, C-ACC, PCS, ABS作動状態:%X", c[65] );*/
            /* TRC 作動状態 2bit, ESC 作動状態 2bit, LKA 作動状態 2bit, LDW 作動状態 2bit = 1byte */
            /*printf( ", TRC, ESC, LKA, LWD作動状態:%X", c[66] );*/
            
            /* 交差点距離取得情報 3bit，交差点距離 10bit, 交差点位置取得情報 3bit,  = 2byte */
            /*printf( ", ブレーキ・補助ブレーキ状態:%X", c[67] << 8 | c[68] );*/
            /* 交差点緯度 4byte */
            //             if( ( c[69] << 24 | c[70] << 16 | c[71] << 8 | c[72] ) == 0x80000000 ) {
            //               printf( ",-"); }
            //             else {
            //               printf( ",%lf", ( c[69] << 24 | c[70] << 16 | c[71] << 8 | c[72] ) * 0.0000001 ); }
            /* 交差点経度 4byte */
            //             if( ( c[73] << 24 | c[74] << 16 | c[75] << 8 | c[76] ) == 0x80000000 ) {
            //               printf( ",-"); }
            //             else {
            //               printf( ",%lf", ( c[73] << 24 | c[74] << 16 | c[75] << 8 | c[76] ) * 0.0000001 ); }
            
            
            /* 自家用自動車用拡張情報 1byte */
            /*自家用自動車のための拡張情報。車両用途種別が「自家用自動車」の場合にセットする。*/
            /*printf( ", 自家用自動車用拡張情報:%X", c[77] );*/
            //             if((((c[48] >> 4) << 4) ^ c[48] )== 0){
            /* 運転運行情報 (上位4bit)*/
            //        if((c[77] >> 7) == 1){
            //            /*11xx*/
            //            if((c[77] >> 6) == 3){
            //                /*111x*/
            //                if((c[77] >> 5) == 7){
            //                    if((c[77] >> 4) == 15){printf(",15");}/*1111*/
            //                    else{printf(",14");}/*1110*/
            //                }
            //                /*110x*/
            //                else{
            //                    if((c[77] >> 4) == 6){printf(",13");}/*1101*/
            //                    else{printf(",12");}/*1100*/
            //                }
            //            }
            /*10xx*/
            //            else{
            //                /*101x*/
            //                if((c[77] >> 5) == 5){
            //                    if((c[77] >> 4) == 11){printf(",11");}/*1011*/
            //                    else{printf(",10");}/*1010*/
            //                }
            /*100x*/
            //                else{
            //                    if((c[77] >> 4) == 4){printf(",9");}/*1001*/
            //                    else{printf(",8");}/*1000*/
            //                }
            //            }
            //        }
            //        else{
            //            /*01xx*/
            //            if((c[77] >> 6) == 1){
            //                /*011x*/
            //                if((c[77] >> 5) == 3){
            //                    if((c[77] >> 4) == 7){printf(",7");}/*0111*/
            //                    else{printf(",6");}/*0110*/
            //                }
            //                /*010x*/
            //                else{
            //                    if((c[77] >> 4) == 5){printf(",5");}/*0101*/
            //                    else{printf(",4");}/*0100*/
            //                }
            //            }
            //            /*00xx*/
            //            else{
            //                /*001x*/
            //                if((c[77] >> 5) == 1){
            //                    if((c[77] >> 4) == 3){printf(",3");}/*0011*/
            //                    else{printf(",2");}/*0010*/
            //                }
            //                /*000x*/
            //                else{
            //                    if((c[77] >> 4) == 1){printf(",1");}/*0001*/
            //                    else{printf(",0");}/*0000*/
            //                }
            //            }
            //        }
            /* 状態情報 (下位4bit)*/
            //        if((c[77] >> 3) == 1){
            //            /*11xx*/
            //            if((c[77] >> 2) == 3){
            //                /*111x*/
            //                if((c[77] >> 1) == 7){
            //                    if((c[77] >> 0) == 15){printf(",15");}/*1111*/
            //                    else{printf(",14");}/*1110*/
            //                }
            /*110x*/
            //                else{
            //                    if((c[77] >> 0) == 6){printf(",13");}/*1101*/
            //                    else{printf(",12");}/*1100*/
            //                }
            //            }
            /*10xx*/
            //            else{
            //                /*101x*/
            //                if((c[77] >> 1) == 5){
            //                    if((c[77] >> 0) == 11){printf(",11");}/*1011*/
            //                    else{printf(",10");}/*1010*/
            //                }
            /*100x*/
            //                else{
            //                    if((c[77] >> 0) == 4){printf(",9");}/*1001*/
            //                    else{printf(",8");}/*1000*/
            //                }
            //            }
            //        }
            //        else{
            //            /*01xx*/
            //            if((c[77] >> 2) == 1){
            //                /*011x*/
            //                if((c[77] >> 1) == 3){
            //                    if((c[77] >> 0) == 7){printf(",7");}/*0111*/
            //                    else{printf(",6");}/*0110*/
            //                }
            /*010x*/
            //                else{
            //                    if((c[77] >> 0) == 5){printf(",5");}/*0101*/
            //                    else{printf(",4");}/*0100*/
            //                }
            //            }
            /*00xx*/
            //            else{
            /*001x*/
            //                if((c[77] >> 1) == 1){
            //                    if((c[77] >> 0) == 3){printf(",3");}/*0011*/
            //                    else{printf(",2");}/*0010*/
            //                }
            /*000x*/
            //                else{
            //                    if((c[77] >> 4) == 1){printf(",1");}/*0001*/
            //                    else{printf(",0");}/*0000*/
            //                }
            //            }
            //        }
            //             }
            /* 緊急自動車用拡張情報 1byte */
            /*緊急自動車のための拡張情報。車両用途種別が「緊急自動車」の場合にセットする。*/
            /*printf( ", 緊急自動車用拡張情報:%X", c[78] );*/
            //             if((((c[48] >> 4) << 4) ^ c[48] )== 1){
            /* 運転運行情報 (上位4bit)*/
            //        if((c[78] >> 7) == 1){
            /*11xx*/
            //            if((c[78] >> 6) == 3){
            /*111x*/
            //                if((c[78] >> 5) == 7){
            //                    if((c[78] >> 4) == 15){printf(",15");}/*1111*/
            //                    else{printf(",14");}/*1110*/
            //                }
            /*110x*/
            //                else{
            //                    if((c[78] >> 4) == 6){printf(",13");}/*1101*/
            //                    else{printf(",12");}/*1100*/
            //                }
            //            }
            /*10xx*/
            //            else{
            /*101x*/
            //                if((c[78] >> 5) == 5){
            //                    if((c[78] >> 4) == 11){printf(",11");}/*1011*/
            //                    else{printf(",10");}/*1010*/
            //                }
            /*100x*/
            //                else{
            //                    if((c[78] >> 4) == 4){printf(",9");}/*1001*/
            //                    else{printf(",8");}/*1000*/
            //                }
            //            }
            //        }
            //        else{
            /*01xx*/
            //            if((c[78] >> 6) == 1){
            //                /*011x*/
            //                if((c[78] >> 5) == 3){
            //                    if((c[78] >> 4) == 7){printf(",7");}/*0111*/
            //                    else{printf(",6");}/*0110*/
            //                }
            /*010x*/
            //                else{
            //                    if((c[78] >> 4) == 5){printf(",5");}/*0101*/
            //                    else{printf(",4");}/*0100*/
            //                }
            //            }
            /*00xx*/
            //            else{
            /*001x*/
            //                if((c[78] >> 5) == 1){
            //                    if((c[78] >> 4) == 3){printf(",3");}/*0011*/
            //                    else{printf(",2");}/*0010*/
            //                }
            /*000x*/
            //                else{
            //                    if((c[78] >> 4) == 1){printf(",1");}/*0001*/
            //                    else{printf(",0");}/*0000*/
            //                }
            //            }
            //        }
            /* 状態情報 (下位4bit)*/
            //        if((c[78] >> 3) == 1){
            //            /*11xx*/
            //            if((c[78] >> 2) == 3){
            //                /*111x*/
            //                if((c[78] >> 1) == 7){
            //                    if((c[78] >> 0) == 15){printf(",15");}/*1111*/
            //                    else{printf(",14");}/*1110*/
            //                }
            //                /*110x*/
            //                else{
            //                    if((c[78] >> 0) == 6){printf(",13");}/*1101*/
            //                    else{printf(",12");}/*1100*/
            //                }
            //            }
            //            /*10xx*/
            //            else{
            //                /*101x*/
            //                if((c[78] >> 1) == 5){
            //                    if((c[78] >> 0) == 11){printf(",11");}/*1011*/
            //                    else{printf(",10");}/*1010*/
            //                }
            //                /*100x*/
            //                else{
            //                    if((c[78] >> 0) == 4){printf(",9");}/*1001*/
            //                    else{printf(",8");}/*1000*/
            //                }
            //            }
            //        }
            //        else{
            //            /*01xx*/
            //            if((c[78] >> 2) == 1){
            //                /*011x*/
            //                if((c[78] >> 1) == 3){
            //                    if((c[78] >> 0) == 7){printf(",7");}/*0111*/
            //                    else{printf(",6");}/*0110*/
            //                }
            //                /*010x*/
            //                else{
            //                    if((c[78] >> 0) == 5){printf(",5");}/*0101*/
            //                    else{printf(",4");}/*0100*/
            //                }
            //            }
            //            /*00xx*/
            //            else{
            //                /*001x*/
            //                if((c[78] >> 1) == 1){
            //                    if((c[78] >> 0) == 3){printf(",3");}/*0011*/
            //                    else{printf(",2");}/*0010*/
            //                }
            //                /*000x*/
            //                else{
            //                    if((c[78] >> 4) == 1){printf(",1");}/*0001*/
            //                    else{printf(",0");}/*0000*/
            //                }
            //            }
            //        }
            /* 道路維持作業用自動車用拡張情報 1byte */
            /*printf( ", 道路維持作業用自動車用拡張情報:%X", c[79] );*/
            printf("\n");
            //                    }
            //                }
            //            }
            //        }
        }
/* ここまで三谷くんのソースファイルからコピー */
        

    } //  for ( k = 0 ; k < file_size/128 ; k = k+128 )
    

    
    if( fclose( fp ) == EOF ){
        fputs( "ファイルクローズに失敗しました。\n", stderr );
        exit( EXIT_FAILURE );
    }
    
    return 0;
    
    
    
}  /* main */
