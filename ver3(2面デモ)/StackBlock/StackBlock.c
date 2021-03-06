/***********************************************************************/
/*                                                                     */
/*  FILE        :StackBlock.c                                          */
/*  DATE        :Wed, Jan 30, 2013                                     */
/*  DESCRIPTION :main program file.                                    */
/*  CPU GROUP   :23                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.18).    */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/***********************************************************************/

#include "sfr_r823.h" 											// R8C/23 SFR定義ファイル
#include "StackBlock.h"
#define ONE_SEC 50
#define SIMULATE 01
#define ONE_TWENTY_SEC 60

// 関数のプロトタイプ宣言
void set_AD(void);
void set_AD_all(void);
void wait_2xusec(int t);
void recog_all(void);
void send_position(unsigned char);
void send_position_base(void);
char send_position_x(char);
char send_position_y(char);
void show_reception(void);
void send_surface(void);
void send_area(unsigned char);
void send_node_base(void);
void send_block(unsigned char);
void send_block_base(void);
char receive_data(unsigned char);
char recog(void);
void set_center(void);
void set_led_center(void);
void set_led_all(unsigned char);
void set_led_stripe(unsigned char);
void set_led_none(unsigned char);
void set_led(unsigned char);
void send_req(unsigned char);
void send_req2(unsigned char);
void send_req3(unsigned char);
int  receive_req(unsigned char);
int  receive_req2(unsigned char);
int  receive_req3(unsigned char);
void receive_id(unsigned char);
void send_id(unsigned char);
void show_reception_data(void);
void recog_all_data(void);
int  recog_data(void);

int  reception_up[5][10]={none};
char reception_down[5][10]={none};
char receive[100]={none};
int send_x_up = 0;
int send_x_down = 0;
int send_y_ = 0;
char active_x;
char active_y;
char flag_block =0;
char flag_receive1=0;
char flag_surface=1;
char flag_receive3=0;
char data[200];
char req[10];
char state =BASE;

void main(void)
{
 	int rcount;
	int i,j,flag;
	unsigned char command,ch,x;
	char c;
	asm("FCLR I");						/* Interrupt disabled */
	set_MainCLK();	
	init_PORT();
	set_UART1();											//     メインクロックの切替
	set_UART0();
	set_reset();										// 電圧検出１を有効にする
	fifo_init();
	fifo1_init();
	init_PWM();
	start_PWM();
	init_AD();
	wait_100usec();
	p4_5=1;
	UART1_tx(CR);
	send_y(UP,0xff);
	while(1){
		if(state == BASE){
			wait_250msec();
			send_req(UP);
			wait_2xusec(20);
			receive_id(UP);
			send_node_base();
			if(flag_block == 1){
				send_req3(UP);
				send_area(UP);
				send_req2(UP);
				recog_all();
//				send_node_base();
				send_block_base();
				send_position_base();
				for(i=0;i<200;i++){
					if(data[i] != 'N') UART1_tx(data[i]);
					else break;
				}
				UART1_tx(CR);
				UART1_tx(CR);
			}
		}
		else if(state == WAIT){
			if(flag_surface == 0){
				flag = 	receive_req(UP);
				if(flag == 1) send_id(UP);
				else if(flag == 2) send_area(UP);
				else if(flag == 3){
//					wait_2xusec(30);
					recog_all();
				}
//				flag_surface = 1;
			}
			else if(flag_surface == 1){	
				if(receive_req(DOWN) == 1) send_id(DOWN);
				flag_surface =0;
				p4_5 = 1;
			}
		}
		else if(state == CHECK){
			recog_all_data();
			show_reception_data();
			wait_250msec();
			wait_250msec();
			wait_250msec();
			wait_250msec();
			UART1_tx(CR);
		}
		else if(state == CHECK2){
			send_x(UP,0x1ff);
		}
	}
}

int receive_req(unsigned char surface)
{
	int i;
	char x = 0;
	switch(surface){
		case UP:
		recv_x(UP,4);
		recv_y(UP,2);		
		switch_out(CONTROL,1);
		send_x(UP,0x000);
		wait_2xusec(20);
		x = receive_data(UP);
		if(x == 'R') return 1;
		else if(x == 'A' ) return 2;
		else if(x == 'B' ) return 3;
		else return -1;
		break;
		case DOWN:
		recv_x(DOWN,4);
		recv_y(DOWN,2);		
		switch_out(CONTROL,2);
		send_x(DOWN,0x000);
		wait_2xusec(20);
		x = receive_data(DOWN);
		if(x == 'R') return 1;
		else if(x == 'A' ) return 2;
		else if(x == 'B' ) return 3;
		else return -1;
		break;
		default:
		return -1;
		break;
	}
}


void send_area(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x(UP,0x1ff);
		wait_2xusec(1000000);
		send_x(UP,0x000);
		break;
		case DOWN:
		send_x(DOWN,0x1ff);
		wait_2xusec(1000000);
		break;
		default:
		break;
	}
}

void send_req(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x(UP,0x1ff);
		wait_2xusec(20);
		UART0_tx_str("   ");
		UART0_3tx('R');
		UART0_3tx('R');
		UART0_3tx('R');
		wait_2xusec(100);
		send_x(UP,0x000);
		break;
		case DOWN:
		send_x(DOWN,0x1ff);
		wait_2xusec(20);
		UART1_tx_str("   ");
		UART1_3tx('R');
		UART1_3tx('R');
		UART1_3tx('R');
		wait_2xusec(100);
		send_x(DOWN,0x000);
		break;
		default:
		break;
	}
}

void send_req2(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x(UP,0x1ff);
		wait_2xusec(20);
		UART0_tx_str("   ");
		UART0_3tx('A');
		UART0_3tx('A');
		UART0_3tx('A');
		wait_2xusec(100);
		send_x(UP,0x000);
		break;
		case DOWN:
		send_x(DOWN,0x1ff);
		wait_2xusec(20);
		UART1_tx_str("   ");
		UART1_3tx('A');
		UART1_3tx('A');
		UART1_3tx('A');
		wait_2xusec(100);
		send_x(DOWN,0x000);
		break;
		default:
		break;
	}
}

void send_req3(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x(UP,0x1ff);
		wait_2xusec(20);
		UART0_tx_str("   ");
		UART0_3tx('B');
		UART0_3tx('B');
		UART0_3tx('B');
		wait_2xusec(100);
		send_x(UP,0x000);
		break;
		case DOWN:
		send_x(DOWN,0x1ff);
		wait_2xusec(20);
		UART1_tx_str("   ");
		UART1_3tx('B');
		UART1_3tx('B');
		UART1_3tx('B');
		wait_2xusec(100);
		send_x(DOWN,0x000);
		break;
		default:
		break;
	}
}

void send_id(unsigned char surface)
{
	int i=0;
	switch(surface){
		case UP:
		send_x(UP,0x1ff);
		wait_2xusec(20);
		UART0_tx_str("   ");
		for(i=0;i<4;i++) UART0_3tx('S');
		send_block(UP);
		send_position(UP);
		UART0_tx_str("   ");
		UART0_3tx('G');
		p4_5=1;
		break;
		case DOWN:
		send_x(DOWN,0x1ff);
		wait_2xusec(20);
		UART1_tx_str("   ");
		UART1_tx_str("   ");
		UART1_tx_str("   ");
		for(i=0;i<4;i++) UART1_3tx('S');
		send_block(DOWN);
		send_position(DOWN);
		UART1_tx_str("   ");
		UART1_3tx('G');
		break;
	}
}

void receive_id(unsigned char surface)
{
	int t=0;
	int i=0;
	int j;
	char x;
	char flag_id_start = 0;
	char flag_id_goal = 0;
	flag_block = 0;
	for(j=0;j<200;j++){
		data[j]='N';
	}
	switch(surface){
		case UP:
		switch_out(CONTROL,1);
		recv_x(UP,4);
		recv_y(UP,2);
//		set_center();
		send_x(UP,0x000);
		while(i < 200){
			if(flag_id_goal == 1){
				for(j=i;j<200;j++){
					data[j]='N';
				}
				return;
			}
			else{
				x = receive_data(UP);
				if(x == 'E'){
//					UART1_tx('D');
					if(t>1){
						return;
					}
					t++;
				}
//				UART1_tx(x);
//				else if(x == CR);
				else if(x == 'S') flag_id_start = 1;
				else if(x == 'G') flag_id_goal = 1;

				else if(flag_id_start == 1 && flag_id_goal == 0 && x != 'E'){
					flag_block = 1;
					data[i] = x;
					i++;
//					UART1_tx(x);
				}
			}
		}
		break;
		case DOWN:
		send_x(DOWN,0x000);
		switch_out(CONTROL,2);
		recv_x(DOWN,4);
		recv_y(DOWN,2);
		while(i < 200){
			if(flag_id_goal == 1){
				for(j=i;j<200;j++){
					data[j]='N';
				}
				return;
			}
			else{
				x = receive_data(DOWN);
				if(x == 'E'){
					UART1_tx('D');
					return;
				}
				UART1_tx(x);
				if(x == 'S') flag_id_start = 1;
				else if(x == 'G') flag_id_goal = 1;
				else if(flag_id_start == 1 && flag_id_goal == 0){
					data[i] = x;
					i++;
	//				UART1_tx(x);
				}
			}
		}
		break;
		default:
		break;
	}
}

void set_AD()
{
	adst=1;
	wait_2usec();
	UART1_tx_data(ad);
	adst=0;
}

void set_AD_all(void)
{
	int i, j;
	for(i=0;i<5;i++){
		recv_y(UP,i);
		for(j=0;j<10;j++){
			recv_x(UP,j);
			wait_2xusec(20);
			adst=1;
			wait_2usec();
			UART1_tx_data(ad);
			adst=0;
			UART1_tx_str(" ");
		}
		UART1_tx(CR);
	}
	UART1_tx(CR);
	UART1_tx(CR);
}

void recog_all(void)
{
	int i, j;
	for(i=0;i<10;i++){
		recv_x(UP,i);
		for(j=0;j<5;j++){
			recv_y(UP,j);
			wait_2xusec(100);
			reception_up[j][i] = recog();
		}
	}
}

char recog(void)
{
	char x;
	adst=1;
	wait_2usec();
	if(ad <= 500){
		return(none);
	}else if(ad >500){
		flag_receive1 = 1;
		return(block);
	}

/*	else{
		wait_250msec();
		adst=1;
		wait_2usec();
		if(ad <= 913 && ad >= 600)return(finger);
		else return(none1);	
	}
*/	return(none);
}

void recog_all_data(void)
{
	int i, j;
	for(i=0;i<10;i++){
		recv_x(UP,i);
		for(j=0;j<5;j++){
			recv_y(UP,j);
			wait_2xusec(100);
			reception_up[j][i] = recog_data();
		}
	}
}

int recog_data(void)
{
	char x;
	adst=1;
	wait_2usec();
	return(ad);
}

void show_reception_data(void)
{
	char i,j;
	for(i=0;i<5;i++){
		for(j=0;j<10;j++){
			UART1_tx_data(reception_up[i][j]);
			UART1_tx_str(" ");
		}
		UART1_tx(CR);
	}
}

void show_reception(void)
{
	char i,j;
	for(i=0;i<5;i++){
		for(j=0;j<10;j++){
			if(reception_up[i][j] == none) UART1_tx_str("- ");
			else if(reception_up[i][j] == block) UART1_tx_str("B ");
		}
		UART1_tx(CR);
	}
}

void set_led(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x(UP,send_x_up);
		send_y(UP,send_y_);
		break;
		case DOWN:
		send_x(UP,send_x_down);
		send_y(UP,send_y_);
		break;
		default:
		break;
	}
}

void set_center(void)
{
	int i,j,flag;
	int x,x0,x1,y,y0,y1;
	flag=0;
	x=0,
	y=0;
	x0=0;
	x1=0;
	y0=0;
	y1=0;
	for(i=0;i<=4;i++){
		for(j=0;j<=9;j++){
			if(reception_up[i][j] == block){
				y0 = i;
				flag = 1;
				break;
			}
		}
		if(flag == 1) break;		
	}
	flag = 0;
	for(i=4;i>=0;i--){
		for(j=0;j<=9;j++){
			if(reception_up[i][j] == block){
				y1 = i;
				flag = 1;
				break;
			}
		}
		if(flag == 1) break;
	}
	flag = 0;
	for(j=0;j<=9;j++){
		for(i=0;i<=4;i++){
			if(reception_up[i][j] == block){
				x0 = j;
				break;
			}
		}
		if(flag == 1) break;
	}
	flag = 0;
	for(j=9;j>=0;j--){
		for(i=0;i<=4;i++){
			if(reception_up[i][j] == block){
				x1 = j;
				break;
			}
		}
		if(flag == 1) break;
	}
	flag = 0;
	x = (x0 + x1) / 2;
	y = (y0 + y1) / 2;
	recv_x(UP,x);
	recv_y(UP,y);
	active_x=x;
	active_y=y;
}
/*
void set_led_center(void)
{
	switch(active_x){
		case 0:
		send_x1 = 0x001;
		break;
		case 1:
		send_x1 = 0x003;
		break;
		case 2:
		send_x1 = 0x006;
		break;
		case 3:
		send_x1 = 0x00c;
		break;
		case 4:
		send_x1 = 0x018;
		break;
		case 5:
		send_x1 = 0x030;
		break;
		case 6:
		send_x1 = 0x060;
		break;
		case 7:
		send_x1 = 0x0c0;
		break;
		case 8:
		send_x1 = 0x180;
		break;
		default:
		break;
	}
	send_y1 &= 0xf0;
	switch(active_y){
		case 0:
		send_y1 |= 0x01;
		break;
		case 1:
		send_y1 |= 0x03;
		break;
		case 2:
		send_y1 |= 0x06;
		break;
		case 3:
		send_y1 |= 0x08;
		break;
	}
	set_led();
}
*/
void set_led_all(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x_up = 0x1ff;
		send_y_ &= 0xf0;
		send_y_ |= 0x0f;
		set_led(UP);	
		break;
		case DOWN:
		send_x_down = 0x1ff;
		send_y_ &= 0x0f;
		send_y_ |= 0xf0;
		set_led(DOWN);
		break;
		default:
		break;
	}
}

void set_led_stripe(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x_up = 0b101010101;
		send_y_ &= 0xf0;
		send_y_ |= 0x0f;
		set_led(UP);	
		break;
		case DOWN:
		send_x_down = 0b101010101;
		send_y_ &= 0x0f;
		send_y_ |= 0xf0;
		set_led(UP);
		break;
		default:
		break;
	}
	
}

void set_led_none(unsigned char surface)
{
	switch(surface){
		case UP:
		send_x_up = 0x000;
		send_y_ &= 0xf0;
		set_led(UP);
		break;
		case DOWN:
		send_x_down = 0x000;
		send_y_ &= 0x0f;
		set_led(DOWN);
		break;
	}
}

void send_position_base(void)
{
	char i,j;
	for(i=0;i<5;i++){
		for(j=0;j<10;j++){
			if(reception_up[i][j] == block){
				UART1_tx('#');
				UART1_tx(send_position_x(j));
				UART1_tx(send_position_y(i));
				UART1_tx_str(" ");
			}
		}
	}
	UART1_tx(CR);
}

void send_position(unsigned char surface)
{
	char i,j;
	switch(surface){
		case UP:
		for(i=0;i<5;i++){
			for(j=0;j<10;j++){
				if(reception_up[i][j] == block){
					UART0_3tx('#');
					UART0_3tx(send_position_x(j));
					UART0_3tx(send_position_y(i));
					UART0_tx_str("   ");
				}
			}
		}
		break;
		case DOWN:
		for(i=0;i<5;i++){
			for(j=0;j<10;j++){
				if(reception_up[i][j] == block){
					UART1_3tx('#');
					UART1_3tx(send_position_x(j));
					UART1_3tx(send_position_y(i));
					UART1_tx_str("   ");
				}
			}
		}
	}
}

char send_position_x(char x)
{
	switch(x){
		case 0:
		return('a');
		break;
		case 1:
		return('b');
		break;
		case 2:
		return('c');
		break;
		case 3:
		return('d');
		break;
		case 4:
		return('e');
		break;
		case 5:
		return('f');
		break;
		case 6:
		return('g');
		break;
		case 7:
		return('h');
		break;
		case 8:
		return('i');
		break;
		case 9:
		return('j');
		break;
		default:
		break;
	}
	return('-');
}

char send_position_y(char y)
{
	return(y + '0');
}

void send_surface1(void)
{
	char x = BLOCK_ID;
	UART1_tx('$');
	UART1_tx(x);
}


void send_node_base(void)
{
	UART1_tx('!');
	UART1_tx('a');
	UART1_tx_str(" ");
	UART1_tx(CR);
}

void send_block_base(void)
{
	char x = BLOCK_ID;
	UART1_tx('$');
	UART1_tx(x + '0');
	UART1_tx('1');
	UART1_tx_str(" ");
	UART1_tx(CR);
}

void send_block(unsigned char surface)
{
	char x = BLOCK_ID;
	switch(surface){
		case UP:
		UART0_3tx('$');
		UART0_3tx(x + '0');
		UART0_3tx('1');
		UART0_tx_str("   ");
		UART0_3tx(CR);
		break;
		case DOWN:
		UART1_3tx('$');
		UART1_3tx(x + '0');
		UART1_3tx('2');
		UART1_tx_str("   ");
		UART1_3tx(CR);
		break;
		default:
		break;
	}
}

char receive_data(unsigned char surface)
{
	char i,x,y,z;
	switch(surface){
		case UP:	
		x = UART0_rx();
		y= UART0_rx();
		z= UART0_rx();
		break;
		case DOWN:
		x = UART0_rx();
		y= UART0_rx();
		z= UART0_rx();
		break;
		default:
		break;
	}		
	if(x == y){
		return(x);
	}else if(y == z){
		return(y);
	}
	else return(' ');
}
	
void wait_2xusec(int t){
	int i;
	for(i=0;i<t;i++) wait_2usec();
}

