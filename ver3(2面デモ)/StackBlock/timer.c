//
/***********************************************************************/
/*                                                                     */
/*  FILE        :timer.c                                               */
/*  DATE        :Thu, Apr 19, 2012                                     */
/*  DESCRIPTION :serial comunication file.                             */
/*  CPU GROUP   :15                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.13).    */
/*                                                                     */
/***********************************************************************/
#include "sfr_r823.h" 											// R8C/23 SFR定義ファイル
#include "StackBlock.h"

#define ONE_SEC 50

void set_MainCLK(void);											// メインクロック切替関数
void set_reset(void);
void wait_1msec(void);
void wait_5msec(void);
void wait_50msec(void);
void wait_250msec(void);
void wait_100usec(void);
void wait_2usec(void);
void init_flag(void);
unsigned char get_ct(void);
void	timer_ra_init(void);
void init_PWM(void);
void start_PWM(void);

int counter2_z;							// add 2008.7.7
unsigned char sync_scan_flag,start_measure_flag;	// add 2008.7.7
char rx_data;													// キー入力データ格納変数
int	LED_count;
char rx_data;
unsigned char counter_z;
unsigned char message_flag=1;
unsigned int rled_counter;


void init_PWM(){
	csel0_trdstr=0;
	pwmd1_trdpmr=1;
	ed1_trdoer1=0;
	cclr0_trdcr0=1;
	cclr0_trdcr1=1;
	pold_trdpocr1=1;
	bfd1_trdmr=0;
	trdgra1=m_PWM;
	trdgrd1=n_PWM;
	cmd1_trdfcr=0;
	cmd0_trdfcr=0;
	pwm3_trdfcr=1;	
	
}

void start_PWM(){
	tstart1_trdstr=1;
}



void wait_250msec(void)
{
		wait_50msec();
		wait_50msec();
		wait_50msec();
		wait_50msec();
		wait_50msec();
}

void wait_50msec(void)
{
		wait_5msec();
		wait_5msec();
		wait_5msec();
		wait_5msec();
		wait_5msec();
		wait_5msec();
		wait_5msec();
		wait_5msec();
		wait_5msec();
		wait_5msec();
}
		
void	wait_5msec()
{
           wait_1msec();
           wait_1msec();
           wait_1msec();
           wait_1msec();
           wait_1msec();
}


void	wait_1msec()
{
	long li;
	
	for(li=0;li<500L;li++);
}

void   wait_100usec()
{
	long li;
	
	for(li=0;li<50L;li++);
}
	
void wait_2usec()
{
	long li;
	for(li=0;li<1L;li++);
}





// メインクロック切替関数
void set_MainCLK(void)
{
	prc0 = 1; 						// プロテクト制御（書き換え許可）
	cm13 = 1; 						// Xin-Xout端子切り替え
	cm05 = 0; 						// Xin-Xoutメインクロック発振
	cm06 = 0; 						// 8分周以外(分周なし)
	asm("nop");						// 発振の安定待ち
	asm("nop");						//　   （同上）
	asm("nop");						//　   （同上）
	asm("nop"); 					//　   （同上）
	ocd2 = 0; 						// メインクロック選択
	prc0 = 0; 						// プロテクト制御（書き換え禁止）
}

// 　電圧検出１によるリセットを有効にする
void	set_reset()
{
	char i;

	prc3 = 1;		// プロテクト解除
	vca2 = 0x80;	// 電圧検出１有効
	for(i=0;i<100;i++); //100uSec待つ
	vw1c1 = 1;		// ディジタルフィルタ無効 
	vw1c7 = 1;
	vw1c6 = 1;		// 電圧監視１リセットモード
	vw1c2 = 0;		// Vdet1通過未検出
	vw1c0 = 1;		// 電圧監視リセット許可
	prc3 = 0;		// プロテクトセット
}

void init_flag()
{
	counter2_z=0;
	sync_scan_flag=1;
	start_measure_flag=1;
}

void timer_ra_init(void)
{
	tstart_tracr = 0;					/* Stop Timer RA operation */
	while(tcstf_tracr != 0);			// wait untile timer to stop

	traic = 0x00;						/* Disable Timer RA Interrupt */
	tstop_tracr = 1;					/* The TRAPRE and TRA registers are initialized. */
										/* The TSTART and TCSTF bits in the TRACR register */
										/* are initialized too. */
	trapre = 100-1;						/* Set (20 - 1) in TRAPRE register */
	tra = 250-1;							/* Set (5 - 1) in TRA register */

	tedgf_tracr = 0;					/* Set to "0" in event counter mode */
	tundf_tracr = 0;					/* Set to "0" in event counter mode */

	traioc = 0;							// at timer mode set 0

	tmod0_tramr = 0;					/* Set to "000" timer mode */
	tmod1_tramr = 0;
	tmod2_tramr = 0;

	tck0_tramr = 1;						// f8
	tck1_tramr = 0;
	tck2_tramr = 0;
	
	tckcut_tramr = 0;					/* Supply countsource */

	traic =  0x07;						/* Set Timer RA interrupt Control Register */
										/* Interrupt priority level is set to level 7 */
	tstart_tracr = 1;					/* Start Timer RA operation */
	while(tcstf_tracr != 1);			// wait until timer to start

}

/************************************************************************************
Name:        TRA_int
Parameters:  None
Returns:     None
Description: Timer RA interruption process
************************************************************************************/
#pragma INTERRUPT TRA_int (vect=22)
void TRA_int(void)
{
	
	sync_scan_flag=0;
	if(counter_z){
		counter_z--;
	}
	else{
		counter_z = ONE_SEC;
//		if(RLED==0)RLED=1;
//		else RLED=0;
	}
}


void	RLED_on_delay()
{
	rled_counter=ONE_SEC;
}

unsigned char get_ct(void)
{
	unsigned char retval;
	
	retval = p2 & 0x1f;
//	UART0_tx('$');UART0_tx_data(retval); UART0_tx(CR);UART0_tx(LF);
	return(retval);
}