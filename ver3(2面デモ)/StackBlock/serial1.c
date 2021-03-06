/***********************************************************************/
/*                                                                     */
/*  FILE        :serial1.c                                             */
/*  DATE        :Mon, May 26, 2008                                     */
/*  DESCRIPTION :                                                      */
/*  CPU TYPE    :R8C/1B                                                */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.0).     */
/*                                                                     */
/***********************************************************************/

#include "sfr_r823.h" 					// R8C/23 SFR定義ファイル
#include "StackBlock.h"

void set_UART1(void);                // UART初期化関数
void UART1_3tx(char);
void UART1_tx(char);                 // 1文字UART送信関数
void UART1_tx_str(char *);            // 文字列UART送信関数
void UART1_tx_data(unsigned int);             // 数値データUART送信関数
void UART1_rx_int(void);
void chrstr1(char*);
int receive_work1(void);

unsigned int rque1_r,rque1_w,rque1_pw;
unsigned char rque1[RQUEMAX];
unsigned char data1[80];

int	count1;						// ｶﾚﾝﾄﾒｯｾｲｼﾞ長



void fifo1_init()
{
    rque1_r = 0;
    rque1_w = 0;
    rque1_pw = 1;
}

// UART1初期設定関数
void set_UART1(void)
{
 	u1sr		= 3;				//  UART1を使用するときに必要
   clk0_u1c0   = 0;                
    clk1_u1c0   = 0;                // クロックソースに f0 を選ぶ
    smd0_u1mr   = 1;                // シリアルI/Oモード選択ビット
    smd1_u1mr   = 0;                // （調歩同期式 8ビットモードに設定）
    smd2_u1mr   = 1;                // （調歩同期式 8ビットモードに設定）
    te_u1c1     = 1;                // 送信制御ビット（送信許可に設定）
    re_u1c1     = 1;                // 受信制御ビット（受信許可に設定）
    u1brg       = rate;  // ビットレートレジスタ(38400bpsに設定）
    ilvl0_s1ric = 0;                // UART1受信割り込み優先レベル（レベル1）<---受信割り込み使用時に設定
    ir_s1ric    = 0;                // UART1受信割り込み要求ビットクリア     <---受信割り込み使用時に設定

	u1pinsel = 1;					// UART1の入出力を有効にする
 //   u1sel0 = 1;
 //   u1sel1 = 1;
}

// 1文字送信関数
void  UART1_tx(char data)
{
 	while(ti_u1c1 != 1);			// データ転送待ち（送信バッファ空）
	u1tb = data;					// 送信バッファにデータセット
}

void UART1_3tx(char data)
{
	UART1_tx(data);
	UART1_tx(data);
	UART1_tx(data);	
}

/****



   long cnt;

    for (cnt = 0; cnt < 1000000; cnt++) // 約50ns * 1000000 = 50ms (2400bps : 416us)
        if (1 == ti_u1c1)
        {
            u1tb = (unsigned int)data;            // 送信バッファにデータセット
            break;
        }
}                                   //     （1文字送信の終了）
****/

// 文字列送信関数
void UART1_tx_str(char* str)
{
     while ('\0' != *str)
     {                              // 文字が\0になるまで繰り返す
          UART1_tx(*str);           // 1文字送信
          ++str;                    // 次の文字に移る
     }
}

// 数値データをUART1送信する関数
void UART1_tx_data(unsigned int data)
{
    int tmp,tmp0,tmp1,tmp2,tmp3;
	tmp0 = data;
	tmp1 = tmp0/10;
	tmp2 = tmp1/10;
	tmp3 = tmp2/10;
    UART1_tx((tmp3 % 10) + '0');
	UART1_tx((tmp2 % 10) + '0');
	UART1_tx((tmp1 % 10) + '0');
	UART1_tx((tmp0 % 10) + '0');	
}

// 割り込み受信関数
#pragma interrupt UART1_rx_int (vect=20)
void UART1_rx_int(void)
{
    char err,rc;
	unsigned int u1rb_buf;

	u1rb_buf=u1rb;
	rc = u1rb_buf & 0xff;
	UART1_tx(rc);
//	rc = u1rbl;

//	if(rque1_pw != rque1_r){		// ring buffer is no full   
//		rque1[rque1_w]=rc;
//		rque1_w = rque1_pw;
//		rque1_pw = (rque1_pw+1)&(RQUEMAX-1);   // modify 2008.6.5
//	}
//	err = (u1rb_buf>>8) & 0xf0;

//    err = u1rbh & 0xf0;                        // エラーデータの取り出し

/*    if (0 < err)
    {
        re_u1c1     = 0;                       // 受信制御ビット（受信禁止に設定）
        smd0_u1mr   = 0;                       // シリアルI/Oモード選択ビット
        smd1_u1mr   = 0;                       // （調歩同期式 8ビットモードに設定）
        smd2_u1mr   = 0;                       // （調歩同期式 8ビットモードに設定）
        smd0_u1mr   = 1;                       // シリアルI/Oモード選択ビット
        smd1_u1mr   = 0;                       // （調歩同期式 8ビットモードに設定）
        smd2_u1mr   = 1;                       // （調歩同期式 8ビットモードに設定）
        re_u1c1     = 1;                       // 受信制御ビット（受信許可に設定）
        te_u1c1     = 1;                       // 送信制御ビット（送信許可に設定）
    }
*/
}

// 1文字受信関数
char UART1_rx (void)
{
    char data;						// 受信データ格納変数
    char err;						// エラーデータ格納変数
	long li=0;
	while (ri_u1c1 != 1 && li<100000L){// 受信待ち
		li++;	
	}		// 受信待ち
 	if(li > 99999L){
		return 'E';
	}
	data = u1rbl;					// 受信データの取り出し
	err = u1rbh & 0xf0;				// エラーデータの取り出し
    return data;					// 受信データの引渡し
}


int receive_work1()
{
    int c;

	if(rque1_r != rque1_w){		// データがあったら空になるまで処理を続行
		c = rque1[rque1_r];
		rque1_r = (rque1_r+1)&(RQUEMAX-1);
	}
	else c = -1;
	return c;
}


void chrstr1(char* cp)
{
    char* ccp;

    ccp = cp;

    while (*ccp != 0)
    {
        UART1_tx(*ccp);
        ++ccp;
    }

    UART1_tx('LF');
    UART1_tx('CR');
}

