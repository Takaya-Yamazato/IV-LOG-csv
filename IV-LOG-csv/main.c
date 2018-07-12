//
//  main.c
//  IV-LOG-csv
//
//  Created by Takaya Yamazato on 2018/06/05.
//  Copyright © 2018年 Takaya Yamazato. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int Header_Information(unsigned long d[100000]); /* ヘッダ情報 16 Byte （車車間，路車間で共通）*/
int V2V_Dataframe(unsigned long    d[100000]); /* 車車間通信のデータフレーム */
int I2V_Dataframe(unsigned long    d[100000]); /* 路車間通信のデータフレーム */

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

        }/* for( i = 0; i < 128 ; ++i ) */
        
        /* ここから三谷くんのソースファイルからコピー */
        
        Header_Information(c);  /* 共通ヘッダ情報 */
        
        if((c[38] >> 4) > 9){
            //        if(c[12]==0x02){
            //            if( (351605520 < ( c[28] << 24 | c[29] << 16 | c[30] << 8 | c[31])) && (( c[28] << 24 | c[29] << 16 | c[30] << 8 | c[31]) <351680300)){
            //                if( (1369562970 <( c[32] << 24 | c[33] << 16 | c[34] << 8 | c[35])) && (( c[32] << 24 | c[33] << 16 | c[34] << 8 | c[35]) < 136971156)){
            //                    if((((c[78] >> 4) << 4 ) | c[78]) == 0x01){
            
//                        V2V_Dataframe(c);       /* 車車間通信のデータフレーム */
        
            //                    }
            //                }
            //            }
            //        }
        }
        
        I2V_Dataframe(c);       /* 路車間通信のデータフレーム */
        
        
        /* ここまで三谷くんのソースファイルからコピー */
        
    printf("\n");
    
    } //  for ( k = 0 ; k < file_size/128 ; k = k+128 )
    

    
    if( fclose( fp ) == EOF ){
        fputs( "ファイルクローズに失敗しました。\n", stderr );
        exit( EXIT_FAILURE );
    }


    return 0;
    
}  /* main */

int Header_Information(unsigned long *d){
    /* ヘッダ情報（16 byte）*/
    /* パケット受信時の PC 日時(JST) 8byte 年/月/日/時:分:ミリ秒 */
    printf( "%lu/%lu/%lu/%lu:%lu:%5lu", d[0] << 8 | d[1],d[2],d[3],d[4],d[5],d[6] << 8 | d[7]);
    /* データ長 4byte*/
    /* printf( ", データ長:%d%d%d%d", d[8],d[9],d[10],d[11]); */
    printf( ",%lu", d[8] << 24 | d[9] << 16 | d[10] << 8 | d[11]);
    /* 車輌種別 1byte 0x00:他車輌，0x02:自車輌，0x01：固定値（路車間）*/
    switch(d[12]) {
        case 0x00 : printf(",0 他車輌");
            break;
        case 0x01 : printf(",1　路車間");
            break;
        case 0x02 : printf(",2　自車輌");
            break;
        default   : printf(",-");
    }
    /* タイムスタンプ有無 1byte 0x00:タイムスタンプなし，0x01:タイムスタンプあり*/
    switch(d[13]) {
        case 0x00 : printf(",0　タイムスタンプ無し");
            break;
        case 0x01 : printf(",1　タイムスタンプあり");
            break;
        default   : printf(",-");
    }
    /* 情報フラグ 1byte bit0: 緊急車両フラグ，bit1~bit7: 予約領域*/
    if( d[14] == 0x00 ) printf(",0　緊急車両フラグ");
    else{printf(",-");}
    /* 予約 1byte 予約領域(固定値 0x00) */
    // if( d[15] == 0x00 ) printf(",0");
    //   else{printf(",-");}
    
    return 0;
}

int V2V_Dataframe(unsigned long *d){
    
    /* ここから車車間情報 （共通領域管理情報）*/
    /* 共通サービス規格ID 3bit，メッセージID 2bit，バージョン情報 3bit =1byte*/
    if( d[16] == 0x29 ) printf( ",共通サービス規格ID:%2lX", d[16]);
    if(((d[16] >> 5) & 1) == 1) {printf(",1");}
           else{printf(",-");}
    if(((d[16] >> 3) & 1) == 1) {printf(",1");}
           else{printf(",-");}
    if(((d[16] >> 0) & 1) == 1) {printf(",1");}
           else{printf(",-");}
    /* 車両ID 4byte */
    /*    printf( ", 車両ID:%X%X%X%X", d[17],d[18],d[19],d[20]); */
    //             printf( ",%lu", d[17] << 24 | d[18] << 16 | d[19] << 8 | d[20]);
    /* インクリメントカウンタ 1byte　0~255 */
    //             printf( ",%lu", d[21]);
    /* 共通アプリデータ長 1byte */
    //             printf( ",%lu", d[22]);
    /* オプションフラグ 位置オプション情報の有無 1bit．状態オプション情報の有無 1bit．位置取得オプション情報 1bit．車両状態オプション情報の有無 1bit．交差点情報の有無 1bit．拡張情報の有無 1bit．拡張オプションフラグの有無 1bit．自由領域の有無 1bit =1byte */
    /*printf( ",オプションフラグ:%X", d[23]);*/
    /* うるう秒補正情報 1bit + 時刻(時) 7bit = 1byte */
    /*printf( ",%d", d[24]^0x80 );*/
    /* 時刻(時) 7bit */
    //        printf(",%lu",(d[24] ^ ((d[24] >> 7) << 7)));
    /* 時刻(分) 1byte */
    //             printf( ",%lu", d[25] );
    /* 時刻(秒) 2byte */
    /*    printf( ", 時刻(秒):%u.%u", d[26],d[27] );   */
    //             printf( ",%f", (d[26] << 8 | d[27]) * 0.001 );
    
    /* GPSデータ変換は以下を参考に
     https://stackoverflow.com/questions/17355604/how-to-convert-gps-longitude-and-latitude-from-hex */
    /* 緯度 4byte */
    printf(",GPSデータ:");
    printf( "%lf", ( d[28] << 24 | d[29] << 16 | d[30] << 8 | d[31]) * 0.0000001 );
    /* 経度 4byte */
    printf( ",%lf", ( d[32] << 24 | d[33] << 16 | d[34] << 8 | d[35]) * 0.0000001 );
    /* 高度 2byte */
    //             printf( ",%f", ( d[36] << 8 | d[37] ) * 0.1 );
    /* 位置取得情 4bit，高度取得情報 4bit = 1byte */
    /*printf( ",%X", d[38] );             */
    /* 位置取得情報 (上位4bit)*/
    if((d[38] >> 7) == 1){
        /*11xx*/
        if((d[38] >> 6) == 3){
            /*111x*/
            if((d[38] >> 5) == 7){
                if((d[38] >> 4) == 15){printf(",15");}/*1111*/
                else{printf(",14");}/*1110*/
            }
            /*110x*/
            else{
                if((d[38] >> 4) == 13){printf(",13");}/*1101*/
                else{printf(",12");}/*1100*/
            }
        }
        /*10xx*/
        else{
            /*101x*/
            if((d[38] >> 5) == 5){
                if((d[38] >> 4) == 11){printf(",11");}/*1011*/
                else{printf(",10");}/*1010*/
            }
            /*100x*/
            else{
                if((d[38] >> 4) == 9){printf(",9");}/*1001*/
                else{printf(",8");}/*1000*/
            }
        }
    }
    else{
        /*01xx*/
        if((d[38] >> 6) == 1){
            /*011x*/
            if((d[38] >> 5) == 3){
                if((d[38] >> 4) == 7){printf(",7");}/*0111*/
                else{printf(",6");}/*0110*/
            }
            /*010x*/
            else{
                if((d[38] >> 4) == 5){printf(",5");}/*0101*/
                else{printf(",4");}/*0100*/
            }
        }
        /*00xx*/
        else{
            /*001x*/
            if((d[38] >> 5) == 1){
                if((d[38] >> 4) == 3){printf(",3");}/*0011*/
                else{printf(",2");}/*0010*/
            }
            /*000x*/
            else{
                if((d[38] >> 4) == 1){printf(",1");}/*0001*/
                else{printf(",0");}/*0000*/
            }
        }
    }
    /* 高度取得情報 (下位4bit)*/
    //        if((d[38] >> 3) == 1){
    //            /*11xx*/
    //            if((d[38] >> 2) == 3){
    //                /*111x*/
    //                if((d[38] >> 1) == 7){
    //                    if((d[38] >> 0) == 15){printf(",15");}/*1111*/
    //                    else{printf(",14");}/*1110*/
    //                }
    /*110x*/
    //                else{
    //                    if((d[38] >> 0) == 6){printf(",13");}/*1101*/
    //                    else{printf(",12");}/*1100*/
    //                }
    //            }
    /*10xx*/
    //            else{
    /*101x*/
    //                if((d[38] >> 1) == 5){
    //                    if((d[38] >> 0) == 11){printf(",11");}/*1011*/
    //                    else{printf(",10");}/*1010*/
    //                }
    /*100x*/
    //                else{
    //                    if((d[38] >> 0) == 4){printf(",9");}/*1001*/
    //                    else{printf(",8");}/*1000*/
    //                }
    //            }
    //        }
    //        else{
    /*01xx*/
    //            if((d[38] >> 2) == 1){
    /*011x*/
    //                if((d[38] >> 1) == 3){
    //                    if((d[38] >> 0) == 7){printf(",7");}/*0111*/
    //                    else{printf(",6");}/*0110*/
    //                }
    /*010x*/
    //                else{
    //                    if((d[38] >> 0) == 5){printf(",5");}/*0101*/
    //                    else{printf(",4");}/*0100*/
    //                }
    //            }
    /*00xx*/
    //            else{
    /*001x*/
    //                if((d[38] >> 1) == 1){
    //                    if((d[38] >> 0) == 3){printf(",3");}/*0011*/
    //                    else{printf(",2");}/*0010*/
    //                }
    /*000x*/
    //                else{
    //                    if((d[38] >> 4) == 1){printf(",1");}/*0001*/
    //                    else{printf(",0");}/*0000*/
    //                }
    //            }
    //        }
    /* 車速 16bit 2byte */
    //             printf( ",%f", ( d[39] << 8 | d[40] ) * 0.01 );
    /* 車両方位角 2byte */
    //             printf( ",%f", ( d[41] << 8 | d[42] ) * 0.0125 );
    /* 前後加速度 2byte */
    //             if( ( d[43] << 8 | d[44] ) == 0x8000 ) { printf( ",-" ); }
    //             else { printf( ",%f", ( d[43] << 8 | d[44] ) * 0.01 );  }
    /* 車速取得情報 3bit，車両方位角取得情報 3bit，前後加速度取得情報 3bit, シフトポジション 3bit, ステアリング角度 12bit = 3byte */
    /*printf( ", 車輌挙動情報:%X%X%X", d[45], d[46], d[47] );*/
    /*車速取得情報*/
    //            if((d[45] >> 7) == 1){
    //                /*11x*/
    //                if((d[45] >> 6) == 3){
    //                    if((d[45] >> 5) == 7){printf(",7");}/*111*/
    //                    else{printf(",6");}/*110*/
    //                }
    /*10x*/
    //                else{
    //                    if((d[45] >> 5) == 5){printf(",5");}/*101*/
    //                    else{printf(",4");}/*100*/
    //                }
    //            }
    /*0xx*/
    //            else{
    /*01x*/
    //                if((d[45] >> 6) == 1){
    //                    if((d[45] >> 5) == 3){printf(",3");}/*011*/
    //                    else{printf(",2");}/*010*/
    //                }
    /*00x*/
    //                else{
    //                    if((d[45] >> 5) == 1){printf(",1");}/*001*/
    //                    else{printf(",0");}/*000*/
    //                }
    //            }
    /*車両方位角取得情報*/
    //            if((d[45] >> 4) % 2 == 1){
    /*11x*/
    //                if((d[45] >> 3) %2 == 1){
    //                    if((d[45] >> 2) %2 == 1){printf(",7");}/*111*/
    //                    else{printf(",6");}/*110*/
    //                }
    /*10x*/
    //                else{
    //                    if((d[45] >> 2) == 5){printf(",5");}/*101*/
    //                    else{printf(",4");}/*100*/
    //                }
    //            }
    /*0xx*/
    //            else{
    /*01x*/
    //                if((d[45] >> 3) %2 == 1){
    //                    if((d[45] >> 2) %2 == 1){printf(",3");}/*011*/
    //                    else{printf(",2");}/*010*/
    //                }
    /*00x*/
    //                else{
    //                    if((d[45] >> 2) %2 == 1){printf(",1");}/*001*/
    //                    else{printf(",0");}/*000*/
    //                }
    //            }
    /*前後加速度取得情報*/
    //            if((d[45] >> 1) % 2 == 1){
    //                /*11x*/
    //                if((d[45] >> 0) %2 == 1){
    //                    if((d[46] >> 7) %2 == 1){printf(",7");}/*111*/
    //                    else{printf(",6");}/*110*/
    //                }
    //                /*10x*/
    //                else{
    //                    if((d[46] >> 7) == 5){printf(",5");}/*101*/
    //                    else{printf(",4");}/*100*/
    //                }
    //            }
    //            /*0xx*/
    //            else{
    //                /*01x*/
    //                if((d[45] >> 0) %2 == 1){
    //                    if((d[46] >> 7) %2 == 1){printf(",3");}/*011*/
    //                    else{printf(",2");}/*010*/
    //                }
    //                /*00x*/
    //                else{
    //                    if((d[46] >> 7) %2 == 1){printf(",1");}/*001*/
    //                    else{printf(",0");}/*000*/
    //                }
    //            }
    //            /*シフトポジション*/
    //            if((d[46] >> 6) % 2 == 1){
    //                /*11x*/
    //                if((d[46] >> 5) %2 == 1){
    //                    if((d[46] >> 4) %2 == 1){printf(",7");}/*111*/
    //                    else{printf(",6");}/*110*/
    //                }
    //                /*10x*/
    //                else{
    //                    if((d[46] >> 4) == 5){printf(",5");}/*101*/
    //                    else{printf(",4");}/*100*/
    //                }
    //            }
    //            /*0xx*/
    //            else{
    //                /*01x*/
    //                if((d[46] >> 5) %2 == 1){
    //                    if((d[46] >> 4) %2 == 1){printf(",3");}/*011*/
    //                    else{printf(",2");}/*010*/
    //                }
    //                /*00x*/
    //                else{
    //                    if((d[46] >> 4) %2 == 1){printf(",1");}/*001*/
    //                    else{printf(",0");}/*000*/
    //                }
    //            }
    //             /* 車両サイズ種別 4bit，車両用途種別 4bit = 1byte */
    //             /*printf( ", 車両サイズ・用途:%X", d[48] );             */
    //             /* 車両サイズ種別 (上位4bit)*/
    //        if((d[48] >> 7) == 1){
    //            /*11xx*/
    //            if((d[48] >> 6) == 3){
    //                /*111x*/
    //                if((d[48] >> 5) == 7){
    //                    if((d[48] >> 4) == 15){printf(",15");}/*1111*/
    //                    else{printf(",14");}/*1110*/
    //                }
    //                /*110x*/
    //                else{
    //                    if((d[48] >> 4) == 6){printf(",13");}/*1101*/
    //                    else{printf(",12");}/*1100*/
    //                }
    //            }
    //            /*10xx*/
    //            else{
    //                /*101x*/
    //                if((d[48] >> 5) == 5){
    //                    if((d[48] >> 4) == 11){printf(",11");}/*1011*/
    //                    else{printf(",10");}/*1010*/
    //                }
    //                /*100x*/
    //                else{
    //                    if((d[48] >> 4) == 4){printf(",9");}/*1001*/
    //                    else{printf(",8");}/*1000*/
    //                }
    //            }
    //        }
    //        else{
    //            /*01xx*/
    //            if((d[48] >> 6) == 1){
    //                /*011x*/
    //                if((d[48] >> 5) == 3){
    //                    if((d[48] >> 4) == 7){printf(",7");}/*0111*/
    //                    else{printf(",6");}/*0110*/
    //                }
    //                /*010x*/
    //                else{
    //                    if((d[48] >> 4) == 5){printf(",5");}/*0101*/
    //                    else{printf(",4");}/*0100*/
    //                }
    //            }
    //            /*00xx*/
    //            else{
    //                /*001x*/
    //                if((d[48] >> 5) == 1){
    //                    if((d[48] >> 4) == 3){printf(",3");}/*0011*/
    //                    else{printf(",2");}/*0010*/
    //                }
    //                /*000x*/
    //                else{
    //                    if((d[48] >> 4) == 1){printf(",1");}/*0001*/
    //                    else{printf(",0");}/*0000*/
    //                }
    //            }
    //        }
    /* 車両用途種別 (下位4bit)*/
    //        if((d[48] >> 3) == 1){
    //            /*11xx*/
    //            if((d[48] >> 2) == 3){
    //                /*111x*/
    //                if((d[48] >> 1) == 7){
    //                    if((d[48] >> 0) == 15){printf(",15");}/*1111*/
    //                    else{printf(",14");}/*1110*/
    //                }
    /*110x*/
    //                else{
    //                    if((d[48] >> 0) == 6){printf(",13");}/*1101*/
    //                    else{printf(",12");}/*1100*/
    //                }
    //            }
    /*10xx*/
    //            else{
    //                /*101x*/
    //                if((d[48] >> 1) == 5){
    //                    if((d[48] >> 0) == 11){printf(",11");}/*1011*/
    //                    else{printf(",10");}/*1010*/
    //                }
    //                /*100x*/
    //                else{
    //                    if((d[48] >> 0) == 4){printf(",9");}/*1001*/
    //                    else{printf(",8");}/*1000*/
    //                }
    //            }
    //        }
    //        else{
    //            /*01xx*/
    //            if((d[48] >> 2) == 1){
    //                /*011x*/
    //                if((d[48] >> 1) == 3){
    //                    if((d[48] >> 0) == 7){printf(",7");}/*0111*/
    //                    else{printf(",6");}/*0110*/
    //                }
    /*010x*/
    //                else{
    //                    if((d[48] >> 0) == 5){printf(",5");}/*0101*/
    //                    else{printf(",4");}/*0100*/
    //                }
    //            }
    /*00xx*/
    //            else{
    /*001x*/
    //                if((d[48] >> 1) == 1){
    //                    if((d[48] >> 0) == 3){printf(",3");}/*0011*/
    //                    else{printf(",2");}/*0010*/
    //                }
    /*000x*/
    //                else{
    //                    if((d[48] >> 4) == 1){printf(",1");}/*0001*/
    //                    else{printf(",0");}/*0000*/
    //                }
    //            }
    //        }
    /* 車幅 10bit，車長 14bit = 3byte */
    /*printf( ", 車幅・車長:%X%X%X", d[49], d[50], d[51] );*/
    /*車幅*/
    //        printf( ",%f", ( d[49] << 2 | d[50] >> 6) * 0.01 );
    //        /*車長*/
    //        printf( ",%f", ((d[50] ^ ( d[50] >> 6 ) << 6) << 8| d[51] ) * 0.01 );
    /* 位置情報遅れ時間 5bit，リビジョンカウンタ 5bit，道路施設情報 3bit，道路区分情報 3bit = 2byte */
    /*printf( ", 道路区分情報途:%X%X", d[52], d[53] );*/
    //        if((d[52] >> 3) == 31){printf(",-");}
    //        else{printf( ",%f", ( d[52] >> 3 )* 0.1 );}
    /*リビジョンカウンタ*/
    //        if((((((d[52] >> 3) << 3) ^ d[52]) << 2) | d[53] >> 6) == 31){printf(",-");}
    //        else{printf( ",%f", (((((d[52] >> 3) << 3) ^ d[52]) << 2) | d[53] >> 6)* 0.1 );}
    /*道路施設情報*/
    /*何故かうまく出力されない*/
    /*printf(",道路%d",((((d[53] >> 5) << 5) >> 2) ^ d[53]));
     if((d[53] >> 5) %2 == 1){*/
    /*11x*/
    /*if((d[53] >> 4) %2 == 1){
     if((d[53] >> 3) %2 == 1){printf(",7");}*//*111*/
    /*else{printf(",6");}*//*110*/
    /*}*/
    /*10x*/
    /*else{
     if((d[53] >> 3) %2 == 1){printf(",5");}*//*101*/
    /*else{printf(",4");}*//*100*/
    /*}
     }*/
    /*0xx*/
    /*else{*/
    /*01x*/
    /*if((d[53] >> 4) %2 == 1){
     if((d[46] >> 3) %2 == 1){printf(",3");}*//*011*/
    /*else{printf(",2");}*//*010*/
    /*}*/
    /*00x*/
    /*else{
     if((d[46] >> 3) %2 == 1){printf(",1");}*//*001*/
    /*else{printf(",-");}*//*000*/
    /*}*/
    /*}*/
    /*道路区分情報*/
    /*if((d[53] >> 2) %2 == 1){*/
    /*11x*/
    /*if((d[53] >> 1) %2 == 1){*/
    /*if((d[53] >> 0) %2 == 1){printf(",7");}*//*111*/
    /*else{printf(",6");}*//*110*/
    /*}*/
    /*10x*/
    /*else{*/
    /*if((d[53] >> 0) == 5){printf(",5");}*//*101*/
    /*else{printf(",4");}*//*100*/
    /*}*/
    /*}*/
    /*0xx*/
    /*else{*/
    /*01x*/
    /*if((d[53] >> 4) %2 == 1){*/
    /*if((d[46] >> 3) %2 == 1){printf(",3");}*//*011*/
    /*else{printf(",2");}*//*010*/
    /*}*/
    /*00x*/
    /*else{*/
    /*if((d[46] >> 3) %2 == 1){printf(",1");}*//*001*/
    /*else{printf(",-");}*//*000*/
    /*}
     }*/
    /* 位置情報誤差楕円長半径 1byte */
    /*if( d[54] == 0xFF ) { printf( ",-" ); }
     else {                  printf( ",%f", d[54] * 0.5 ); }*/
    /* 位置情報誤差楕円短半径 1byte */
    /*if( d[55] == 0xFF ) { printf( ",-" ); }
     else {                  printf( ",%f", d[55] * 0.5 );    }*/
    /* 位置情報誤差楕円回転角 2byte */
    /*if( ( d[56] << 8 | d[57] ) == 0xFFFF ) { printf( ",-" ); }
     else {                  printf( ",%f", ( d[56] << 8 | d[57] ) * 0.0125 );   }*/
    
    /* GNSS測位モード 2bit，GNSS位置精度低下率 6bit = 1byte */
    /*printf( ", GNSS 測位モード・位置精度低下率:%X", d[58] );      */
    /* GNSS捕捉衛星数 4bit，GNSSマルチパス検出 2bit, 自律航法機能情報 1bit, マップマッチング機能情報 1bit = 1byte */
    /*printf( ", GNSS捕捉衛星数等:%X", d[59] );                 */
    
    
    /* ヨーレート 2byte */
    //             if( ( d[60] << 8 | d[61] ) == 0x8000 ) { printf( ",-" ); }
    //             else {                  printf( ",%f", ( d[60] << 8 | d[61] ) * 0.01 );   }
    /* ブレーキ状態 6bit，補助ブレーキ状態 2bit = 1byte */
    /*printf( ", ブレーキ・補助ブレーキ状態:%X", d[62] );      */
    //            if((d[62] >> 2) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[62] >> 3) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[62] >> 4) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[62] >> 5) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[62] >> 6) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[62] >> 7) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    /*補助ブレーキ状態*/
    //            if((d[62]%2) == 0 ){
    //                if((d[62] >> 1) %2 == 0){printf(",-");}
    //                else{printf(",2");}
    //            }
    //            else{
    //                if(d[62] %2 == 0){printf(",1");}
    //                else{printf(",3");}
    //            }
    
    /* アクセルペダル開度 1byte */
    /*printf( ",%f", d[63] * 0.005 );*/
    /* 灯火類状態 1byte */
    /*printf( ", 灯火類状態:%X", d[64] );      */
    //            if((d[64] ) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[64] >> 1) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[64] >> 2) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[64] >> 3) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[64] >> 4) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[64] >> 5) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    //            if((d[64] >> 6) %2 == 1){printf(",1");}
    //            else{printf(",0");}
    
    /* ACC 作動状態 2bit, C-ACC 作動状態 2bit, PCS 作動状態 2bit, ABS 作動状態 2bit = 1byte */
    /* printf( ", ACC, C-ACC, PCS, ABS作動状態:%X", d[65] );*/
    /* TRC 作動状態 2bit, ESC 作動状態 2bit, LKA 作動状態 2bit, LDW 作動状態 2bit = 1byte */
    /*printf( ", TRC, ESC, LKA, LWD作動状態:%X", d[66] );*/
    
    /* 交差点距離取得情報 3bit，交差点距離 10bit, 交差点位置取得情報 3bit,  = 2byte */
    /*printf( ", ブレーキ・補助ブレーキ状態:%X", d[67] << 8 | d[68] );*/
    /* 交差点緯度 4byte */
    //             if( ( d[69] << 24 | d[70] << 16 | d[71] << 8 | d[72] ) == 0x80000000 ) {
    //               printf( ",-"); }
    //             else {
    //               printf( ",%lf", ( d[69] << 24 | d[70] << 16 | d[71] << 8 | d[72] ) * 0.0000001 ); }
    /* 交差点経度 4byte */
    //             if( ( d[73] << 24 | d[74] << 16 | d[75] << 8 | d[76] ) == 0x80000000 ) {
    //               printf( ",-"); }
    //             else {
    //               printf( ",%lf", ( d[73] << 24 | d[74] << 16 | d[75] << 8 | d[76] ) * 0.0000001 ); }
    
    
    /* 自家用自動車用拡張情報 1byte */
    /*自家用自動車のための拡張情報。車両用途種別が「自家用自動車」の場合にセットする。*/
    /*printf( ", 自家用自動車用拡張情報:%X", d[77] );*/
    //             if((((d[48] >> 4) << 4) ^ d[48] )== 0){
    /* 運転運行情報 (上位4bit)*/
    //        if((d[77] >> 7) == 1){
    //            /*11xx*/
    //            if((d[77] >> 6) == 3){
    //                /*111x*/
    //                if((d[77] >> 5) == 7){
    //                    if((d[77] >> 4) == 15){printf(",15");}/*1111*/
    //                    else{printf(",14");}/*1110*/
    //                }
    //                /*110x*/
    //                else{
    //                    if((d[77] >> 4) == 6){printf(",13");}/*1101*/
    //                    else{printf(",12");}/*1100*/
    //                }
    //            }
    /*10xx*/
    //            else{
    //                /*101x*/
    //                if((d[77] >> 5) == 5){
    //                    if((d[77] >> 4) == 11){printf(",11");}/*1011*/
    //                    else{printf(",10");}/*1010*/
    //                }
    /*100x*/
    //                else{
    //                    if((d[77] >> 4) == 4){printf(",9");}/*1001*/
    //                    else{printf(",8");}/*1000*/
    //                }
    //            }
    //        }
    //        else{
    //            /*01xx*/
    //            if((d[77] >> 6) == 1){
    //                /*011x*/
    //                if((d[77] >> 5) == 3){
    //                    if((d[77] >> 4) == 7){printf(",7");}/*0111*/
    //                    else{printf(",6");}/*0110*/
    //                }
    //                /*010x*/
    //                else{
    //                    if((d[77] >> 4) == 5){printf(",5");}/*0101*/
    //                    else{printf(",4");}/*0100*/
    //                }
    //            }
    //            /*00xx*/
    //            else{
    //                /*001x*/
    //                if((d[77] >> 5) == 1){
    //                    if((d[77] >> 4) == 3){printf(",3");}/*0011*/
    //                    else{printf(",2");}/*0010*/
    //                }
    //                /*000x*/
    //                else{
    //                    if((d[77] >> 4) == 1){printf(",1");}/*0001*/
    //                    else{printf(",0");}/*0000*/
    //                }
    //            }
    //        }
    /* 状態情報 (下位4bit)*/
    //        if((d[77] >> 3) == 1){
    //            /*11xx*/
    //            if((d[77] >> 2) == 3){
    //                /*111x*/
    //                if((d[77] >> 1) == 7){
    //                    if((d[77] >> 0) == 15){printf(",15");}/*1111*/
    //                    else{printf(",14");}/*1110*/
    //                }
    /*110x*/
    //                else{
    //                    if((d[77] >> 0) == 6){printf(",13");}/*1101*/
    //                    else{printf(",12");}/*1100*/
    //                }
    //            }
    /*10xx*/
    //            else{
    //                /*101x*/
    //                if((d[77] >> 1) == 5){
    //                    if((d[77] >> 0) == 11){printf(",11");}/*1011*/
    //                    else{printf(",10");}/*1010*/
    //                }
    /*100x*/
    //                else{
    //                    if((d[77] >> 0) == 4){printf(",9");}/*1001*/
    //                    else{printf(",8");}/*1000*/
    //                }
    //            }
    //        }
    //        else{
    //            /*01xx*/
    //            if((d[77] >> 2) == 1){
    //                /*011x*/
    //                if((d[77] >> 1) == 3){
    //                    if((d[77] >> 0) == 7){printf(",7");}/*0111*/
    //                    else{printf(",6");}/*0110*/
    //                }
    /*010x*/
    //                else{
    //                    if((d[77] >> 0) == 5){printf(",5");}/*0101*/
    //                    else{printf(",4");}/*0100*/
    //                }
    //            }
    /*00xx*/
    //            else{
    /*001x*/
    //                if((d[77] >> 1) == 1){
    //                    if((d[77] >> 0) == 3){printf(",3");}/*0011*/
    //                    else{printf(",2");}/*0010*/
    //                }
    /*000x*/
    //                else{
    //                    if((d[77] >> 4) == 1){printf(",1");}/*0001*/
    //                    else{printf(",0");}/*0000*/
    //                }
    //            }
    //        }
    //             }
    /* 緊急自動車用拡張情報 1byte */
    /*緊急自動車のための拡張情報。車両用途種別が「緊急自動車」の場合にセットする。*/
    /*printf( ", 緊急自動車用拡張情報:%X", d[78] );*/
    //             if((((d[48] >> 4) << 4) ^ d[48] )== 1){
    /* 運転運行情報 (上位4bit)*/
    //        if((d[78] >> 7) == 1){
    /*11xx*/
    //            if((d[78] >> 6) == 3){
    /*111x*/
    //                if((d[78] >> 5) == 7){
    //                    if((d[78] >> 4) == 15){printf(",15");}/*1111*/
    //                    else{printf(",14");}/*1110*/
    //                }
    /*110x*/
    //                else{
    //                    if((d[78] >> 4) == 6){printf(",13");}/*1101*/
    //                    else{printf(",12");}/*1100*/
    //                }
    //            }
    /*10xx*/
    //            else{
    /*101x*/
    //                if((d[78] >> 5) == 5){
    //                    if((d[78] >> 4) == 11){printf(",11");}/*1011*/
    //                    else{printf(",10");}/*1010*/
    //                }
    /*100x*/
    //                else{
    //                    if((d[78] >> 4) == 4){printf(",9");}/*1001*/
    //                    else{printf(",8");}/*1000*/
    //                }
    //            }
    //        }
    //        else{
    /*01xx*/
    //            if((d[78] >> 6) == 1){
    //                /*011x*/
    //                if((d[78] >> 5) == 3){
    //                    if((d[78] >> 4) == 7){printf(",7");}/*0111*/
    //                    else{printf(",6");}/*0110*/
    //                }
    /*010x*/
    //                else{
    //                    if((d[78] >> 4) == 5){printf(",5");}/*0101*/
    //                    else{printf(",4");}/*0100*/
    //                }
    //            }
    /*00xx*/
    //            else{
    /*001x*/
    //                if((d[78] >> 5) == 1){
    //                    if((d[78] >> 4) == 3){printf(",3");}/*0011*/
    //                    else{printf(",2");}/*0010*/
    //                }
    /*000x*/
    //                else{
    //                    if((d[78] >> 4) == 1){printf(",1");}/*0001*/
    //                    else{printf(",0");}/*0000*/
    //                }
    //            }
    //        }
    /* 状態情報 (下位4bit)*/
    //        if((d[78] >> 3) == 1){
    //            /*11xx*/
    //            if((d[78] >> 2) == 3){
    //                /*111x*/
    //                if((d[78] >> 1) == 7){
    //                    if((d[78] >> 0) == 15){printf(",15");}/*1111*/
    //                    else{printf(",14");}/*1110*/
    //                }
    //                /*110x*/
    //                else{
    //                    if((d[78] >> 0) == 6){printf(",13");}/*1101*/
    //                    else{printf(",12");}/*1100*/
    //                }
    //            }
    //            /*10xx*/
    //            else{
    //                /*101x*/
    //                if((d[78] >> 1) == 5){
    //                    if((d[78] >> 0) == 11){printf(",11");}/*1011*/
    //                    else{printf(",10");}/*1010*/
    //                }
    //                /*100x*/
    //                else{
    //                    if((d[78] >> 0) == 4){printf(",9");}/*1001*/
    //                    else{printf(",8");}/*1000*/
    //                }
    //            }
    //        }
    //        else{
    //            /*01xx*/
    //            if((d[78] >> 2) == 1){
    //                /*011x*/
    //                if((d[78] >> 1) == 3){
    //                    if((d[78] >> 0) == 7){printf(",7");}/*0111*/
    //                    else{printf(",6");}/*0110*/
    //                }
    //                /*010x*/
    //                else{
    //                    if((d[78] >> 0) == 5){printf(",5");}/*0101*/
    //                    else{printf(",4");}/*0100*/
    //                }
    //            }
    //            /*00xx*/
    //            else{
    //                /*001x*/
    //                if((d[78] >> 1) == 1){
    //                    if((d[78] >> 0) == 3){printf(",3");}/*0011*/
    //                    else{printf(",2");}/*0010*/
    //                }
    //                /*000x*/
    //                else{
    //                    if((d[78] >> 4) == 1){printf(",1");}/*0001*/
    //                    else{printf(",0");}/*0000*/
    //                }
    //            }
    //        }
    /* 道路維持作業用自動車用拡張情報 1byte */
    /*printf( ", 道路維持作業用自動車用拡張情報:%X", d[79] );*/
    
    return 0;
}

int I2V_Dataframe(unsigned long *d){/* 路車間通信のデータフレーム */

//printf("    I2V_Dataframe   ");
    
//if((d[27]>>4)==1){

    if((((d[20]>>7)<<7)^d[20])==6){
        /*車速*/
        if((d[57]>>7)==0){
            printf("車速：%lu",d[57]);
        }
        else{
            printf("車速：-%lu",254-d[57]);
        }
        //                printf("\n");
    }
//    if((((d[20]>>7)<<7)^d[20])==3){
//        if((d[16]>>5)==2){
            /*メッセージ種別コード*/
            //            printf("%lu",(d[16]>>5));
            /*メッセージバージョン*/
            //            printf(",%lu",(((d[16]>>5)<<5)^d[16])>>1);
            /*予備1*/
            //            printf(",%lu",(((d[16]>>1)<<1)^d[16]));
            /*都道府県コード*/
            //            printf(",%lu",d[17]);
            /*無線機ID*/
            //            printf(",%lu",((d[18]<<8)|d[19]));
            /*運用区分コード*/
            //            printf(",%lu",d[20]>>7);
            /*メッセージID*/
            //            printf(",%lu",((d[20]>>7)<<7)^d[20]);
            /*インクリメントカウンタ*/
            //            printf(",%lu",d[21]);
            /*送信日*/
            //            printf(",%lu%lu%lu%lu:",d[22]>>4,((d[22]>>4)<<4)^d[22],d[23]>>4,((d[23]>>4)<<4)^d[23]);
            //            printf("%lu%lu/",d[24]>>4,((d[24]>>4)<<4)^d[24]);
            //            printf("%lu%lu",d[25]>>4,((d[25]>>4)<<4)^d[25]);
            /*サマータイム指定*/
            //            printf(",%lu",d[26]>>7);
            /*休日指定*/
            //            printf(",%lu",(((d[26]>>7)<<7)^d[26])>>6);
            /*曜日*/
            //            printf(",%lu",(((d[26]>>6)<<6)^d[26])>>3);
            /*送信時刻*/
            //            printf("%lu%lu:",d[27]>>4,((d[27]>>4)<<4)^d[27]);
            //            printf("%lu%lu:",d[28]>>4,((d[28]>>4)<<4)^d[28]);
            //            printf("%lu%lu.",d[29]>>4,((d[29]>>4)<<4)^d[29]);
            //            printf("%lu%lu",d[30]>>4,((d[30]>>4)<<4)^d[30]);
            /*ここまでが路車間共通ヘッダ*/
            /*以下は、メッセージID毎に異なる*/
            /*都道府県コード*/
            //        printf(",%lu",d[36]);
            /*提供点種別コード*/
            //        printf(",%lu",d[37]>>7);
            /*交差点ID*/
            //            printf(",%lu",((((d[37]>>7)<<7)^d[37])|d[38]));
            /*予備8*/
            /*システム状態*/
            //            printf(",%lu",d[40]);
            /*イベントカウンタ*/
            //            printf(",%lu",d[41]);
            /*車灯器数*/
            //            printf(",%llu",d[42]);
            /*歩灯器数*/
            //            printf(",%lu",d[43]);
            /*接続方路数(I)*/
            //            printf(",I%lu",d[44]);
            /*サービス方路数(J)*/
            //            printf(",J%lu",d[45]);
            /*方路ID*/
            //            printf(",H%llu",d[46]);
            /*信号通行方向情報有無フラグ*/
            //            printf(",%lu",d[47]>>7);
            /*予備7*/
            /*信号通行方向情報*/
            printf(",%lu",d[48]);
            /*車灯器情報ポインタ:1*/
            //            printf(",%lu",d[49]<<8|d[50]);
            /*車灯器情報ポインタ:2*/
            //            printf(",%lu",d[51]<<8|d[52]);
            /*車灯器情報ポインタ:3*/
            //            printf(",%lu",d[53]<<8|d[54]);
            /*車灯器情報ポインタ:4*/
            //            printf(",%lu",d[55]<<8|d[56]);
            /*車灯器情報ポインタ:5*/
            //            printf(",%lu",d[57]<<8|d[58]);
            /*歩灯器情報ポインタ:1*/
            //            printf(",%lu",d[59]<<8|d[60]);
            /*歩灯器情報ポインタ:2*/
            //            printf(",%lu",d[61]<<8|d[62]);
            /*歩灯器情報ポインタ:3*/
            //            printf(",%lu",d[63]<<8|d[64]);
            /*歩灯器情報ポインタ:4*/
            //            printf(",%lu",d[65]<<8|d[66]);
            /*歩灯器情報ポインタ:5*/
            //            printf(",%lu",d[67]<<8|d[68]);
            /*車灯器ID*/
            //            printf(",%lu",d[69]>>4);
            /*灯色出力変化数(K)*/
            //            printf(",K%llu",((d[69]>>4)<<4)^d[69]);
            /*1丸信号灯色表示*/
            printf(",1丸信号灯色表示：%lu",d[70]);
            /*1青矢信号表示方向*/
            //            printf(",%lu",d[71]);
            /*1カウントダウン停止フラグ*/
            //            printf(",%lu",d[72]>>7);
            /*1最小残秒数(0.1秒)*/
            //            printf(",%lf",(((((d[72]>>7)<<7)^d[72])<<8)|d[73])*0.1);
            /*1最大残秒数(0.1秒)*/
            //            printf(",%lf",(d[74]|d[75])*0.1);
            /*2丸信号灯色表示*/
            //            printf(",%llu",d[76]);
            /*2青矢信号表示方向*/
            //            printf(",%lu",d[77]);
            /*2カウントダウン停止フラグ*/
            //            printf(",%lu",d[78]>>7);
            /*2最小残秒数(0.1秒)*/
            //            printf(",%lf",(((((d[78]>>7)<<7)^d[78])<<8)|d[79])*0.1);
            /*2最大残秒数(0.1秒)*/
            //            printf(",%lf",(d[80]|d[81])*0.1);
            /*3丸信号灯色表示*/
            //            printf(",%llu",d[82]);
            /*3青矢信号表示方向*/
            //            printf(",%lu",d[83]);
            /*3カウントダウン停止フラグ*/
            //            printf(",%lu",d[84]>>7);
            /*3最小残秒数(0.1秒)*/
            //            printf(",%lf",(((((d[84]>>7)<<7)^d[84])<<8)|d[85])*0.1);
            /*3最大残秒数(0.1秒)*/
            //            printf(",%lf",(d[86]|d[87])*0.1);
            /*4丸信号灯色表示*/
            //            printf(",%llu",d[88]);
            /*4青矢信号表示方向*/
            //            printf(",%lu",d[89]);
            /*4カウントダウン停止フラグ*/
            //            printf(",%lu",d[90]>>7);
            /*4最小残秒数(0.1秒)*/
            //            printf(",%lf",(((((d[90]>>7)<<7)^d[90])<<8)|d[91])*0.1);
            /*4最大残秒数(0.1秒)*/
            //            printf(",%lf",(d[92]|d[93])*0.1);
            /*歩灯器ID*/
            //            printf(",%lu",d[94]>>4);
            /*灯色出力変化数(L)*/
            //            printf(",L%lu",((d[94]>>4)<<4)^d[94]);
            //printf("DEBUG : k = %d", k);
            printf("\n");
//        } /* if((d[16]>>5)==2) */
//    } /* if((((d[20]>>7)<<7)^d[20])==3) */
//} /* if((d[27]>>4)==1) */

    return 0;
}
