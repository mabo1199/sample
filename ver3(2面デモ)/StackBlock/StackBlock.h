/************************************************************************************
*   File Name   : o_chan.h                                                            *
*   Contents    : o_chan application program header       							*
*   Copyright, 2012 System Watt Co.,Ltd.        									*
*   Version     : 1.01 (H Kikuchi )                                                   *
*                                                                                   *
************************************************************************************/


#define	CR	0xd
#define	LF	0xa
#define SPACE 0x20
#define MAXCOUNT	32
#define RQUEMAX 64


#define WD_COUNT	5   // �E�I�b�`�h�b�O�N�����ԁi�b�j

#define UP		1	//�u���b�N�̖�
#define DOWN 	0
#define CONTROL	3
#define m_PWM	0x4e20	//
#define n_PWM	0x2300	//�������l�d���ݒ�(n/m*5V���炢)
#define rate	0x81
#define none	0
#define finger	1
#define block	2
#define none1	3
#define BLOCK_ID	1
#define SEND 	1
#define RECV	2
#define BASE	1
#define ACTIVE	2
#define WAIT	3
#define CHECK	4
#define CHECK2	5


//main.c

//init.c
void init_PORT(void);
void init_AD(void);

//control_power.c
extern	void send_x(unsigned char num, unsigned short data);
extern	void send_y(unsigned char num, unsigned char data);
extern	void recv_x(unsigned char surface, unsigned char x);
extern	void recv_y(unsigned char surface, unsigned char y);
extern 	void switch_out(unsigned char surface, unsigned char x);

// serial.c

extern	void	fifo_init(void);
extern	void	set_UART0(void);				// UART�������֐�
extern	void	UART0_tx(char);					// 1����UART���M�֐�
extern	void  	UART0_3tx(char);
extern	void	UART0_tx_str(char *);			// ������UART���M�֐�
extern	void	UART0_tx_data(unsigned int);	// ���l�f�[�^UART���M�֐�
extern	void	UART0_rx_int(void);
extern	void	receive_work0(void);
extern	void 	UART0_bin_data( unsigned int);
extern	void	data_send(int,int,char *);		// PC�ɑ����ް��@S/N ch ү����
extern	char	bin2asc(char);
extern	char 	UART0_rx (void);

// serial1.c
extern	int		receive_work1(void);
extern	void 	set_UART1(void);                // UART�������֐�
extern	void 	UART1_tx(char);                 // 1����UART���M�֐�
extern	void	UART1_3tx(char);
extern	void	UART1_tx_str(char *);			// ������UART���M�֐�
extern	void	UART1_tx_data(unsigned int);	// ���l�f�[�^UART���M�֐�
extern	void	UART1_rx_int(void);
extern	int		receive_work1(void);
extern	char 	UART1_rx (void);

//timer.c
void set_MainCLK(void);											// ���C���N���b�N�ؑ֊֐�
void set_reset(void);
void init_PWM(void);
void start_PWM(void);
void wait_250msec(void);
void wait_100usec(void);
void wait_2usec(void);

