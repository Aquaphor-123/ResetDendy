/*
���� ������.
������ ��������� ���� �������� ��� ��������������� ATTiny10(4, 5, 9).
��������� ��������� ����� Dendy ��� ������ ���������� ������ �� ���������.
��� ������ ��������� ���������� ��������� ����� PB3 � ����� I/O, ��� ����� ����� ����������������� Fuse bit RSTDISBL.
��������� ��������� �� ��������� � NTSC ������, � PAL ������ �� �������������.
///////////////////////////////////////////////////////////////////////////////
*/
//��������� �� ��������� ��� ���������, ��� ����� �������������� �������, ������������� ����� res ���������� PPU � ����� �������. 
//���� � ��� NES � CIC (3193a) �����, �� ����� �������������� �������, ������������� ����� 7 CIC � ����� �������. � �� ���������� ������ �����, �� ���� �� �������� ��� ���������, 
//��� �������, �� ���������������� ������ ���� � �������� �� NES � CIC.   
//.equ	NES	=	1


//���� ������ ���������� � ����������, ���������������� ������ ����.
//��� ������� ����������� ������ ������ � �������� �������� ������.

//.equ	SIMULATOR	=	1

// ����� ����������������:
.equ	ATtiny10	=	1
//.equ	ATtiny9	=	1
//.equ	ATtiny5	=	1
//.equ	ATtiny4	=	1

/*
���������� ������� ���������������� ����� �������������:
1) ������ LATCH ������ ���� ��������� � ������ PINB2, ��� ��� �� ������������ ��� ������ ���������������� �� ������ ��� �� ����������.
2) ������ RESET �������� �������������� � ������ PINB1 �� PINB0. 
3) ������ DATA �������� �������������� � ������ PINB0 �� PINB1 ��� PINB3.
4) ������ CLOCK �������� �������������� � ������ PINB3 �� PINB0.

��� ���������������� ������� �������� �������� ����.
*/

.equ	DATA =	PINB0
.equ	RESET =	PINB1
.equ	LATCH =	PINB2
.equ	CLOCK =	PINB3  

//����� ������ ���������
.equ	RIGHT =	0b11111110
.equ	LEFT =	0b11111101
.equ	DOWN =	0b11111011
.equ	UP =	0b11110111
.equ	START =	0b11101111
.equ	SELECT =	0b11011111
.equ	B =	0b10111111
.equ	A =	0b01111111

.equ	TOTAL_BITS =	8

/*
���� ����� ���������� ������ ��������� ��� ������ Dendy (��������� ������ START & SELEC).
������:
1. START & SELECT
2. START & SELECT & B
3. START & SELECT & UP
*/

.set	BUTTON_MASK = START & SELECT

/*
���� ����� ������� �������� ���������� ���������� ���������� ������ �� �������� � ������.
����� �������� ����������� ��������� �������.
*/
.set	REPEAT = 10	//255 ����

/*
������� ������ ����� �������������� ������ ��������� �� ��������� ������������.
*/
.set	REPTIME = 5	//255 ����

//��������� ������� ������� 
.equ	stepOverClock = 4
.equ	stepOSCCAL = 20
.equ	delayStabil = 50

.ifdef	SIMULATOR
.set	BUTTON_MASK = 0xff
.set	REPEAT = 1
.set	REPTIME = 2	
.warning "Only for the simulator!" 
.endif

.def	temp	=	r16
.def	dataJoy	=	r17
.def	tik	=	r18
.def	RepCount	=	r19

.CSEG
.org	0x0000
rjmp	preinit

.org	INT0addr
reti

.org	PCI0addr
rjmp	PCI0	

.org	WDTaddr
rjmp	WDT
//���������� ���������� PCINT0 (��������� SREG, ����� ���������� ������ � ������ ��������� ����������), ����������� tik ���� CLOCK � 0.
PCI0:
in	temp,	SREG
sbic	PINB,	LATCH
inc	tik
out	SREG,	temp
reti
//���������� ���������� WDT (��������� SREG, ����� ���������� ������ � ������ ��������� ����������), ����������� repCount.
WDT:
in	temp,	SREG
inc	repCount
out	SREG,	temp
reti

.include	"init.inc" 
.include	"read.inc"
.include	"reset.inc"	
.include	"delay.inc"
/*
�������� ����:
1) ������� ������� �������� ���������� ������.
2) ��������� ��������������� � ����� ���.
3) ����� �� ������ ��� ���������� �� ���������� INT0 (�� ������) �� ������� LATCH (����� ������� LATCH ���� 6.3 ��� ��� ����, ����� ������� ������ ��� �������, �� �� �����, ��� 250 ��).
4) ��������� ������������ ��������� ������ �� ���������.
5) ������� ������ � ������. 
	5.1) ���� �� ������� ��������� � ���� 1 (���������� ������� ��������, ������ ��� �� ���� ������������ ���������� ��������).
	5.2) ���� �������, ����������� ���������� �������� ���������� ������.
6) ��������� �������� �� ��� ������� ���������� ������.
	6.1) ���� ���, ��������� � ���� 2 (������� �������� �� ����������, ��� ����� �� ��� ������ ���������� ������).
	6.2) ���� ��� �������� ��������� ����� Dendy.
*/

main:
	clr	RepCount
loop:
	sleep
	
	rcall	readDataJoy

	cpi	dataJoy,	BUTTON_MASK
	brne	main

	inc	RepCount

	cpi	RepCount,	REPEAT
	brne	loop

	rcall	resetDendy

	rjmp	main	
