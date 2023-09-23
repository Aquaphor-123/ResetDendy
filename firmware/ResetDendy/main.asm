/*
���� ������.
������ ��������� ���� �������� ��� ��������������� ATTiny10(4, 5, 9).
��������� ��������� ����� Dendy ��� ������ ���������� ������ �� ���������.
///////////////////////////////////////////////////////////////////////////////
��������� ��������� �� ��������� � NTSC ������, � PAL ������ �� �������������.
��������� ��� ���������, ��� ����� �������������� �������, ������������� ����� res ���������� PPU � ����� �������. 
///////////////////////////////////////////////////////////////////////////////

��� ������ ��������� ���������� ��������� ����� PB3 � ����� I/O, ��� ����� ����� ����������������� Fuse bit RSTDISBL.

���� ������ ���������� � ����������, ���������������� ������ ����.
��� ������� ����������� ������ ������ � �������� �������� ������.
*/
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
.equ	REPEAT = 3	//255 ����

//��������� ������ ������� 
.equ	stepOverClock = 4
.equ	stepOSCCAL = 20
.equ	delayStabil = 50

.ifdef	SIMULATOR
.set	BUTTON_MASK = 0xff
.warning "Only for the simulator!" 
.endif

.def	temp	=	r16
.def	dataJoy	=	r17
.def	flag	=	r18
.def	RepCount	=	r19

.CSEG
.org	0x0000
rjmp	init

.org	INT0addr
reti

.org	WDTaddr
ser	flag
reti

.include	"init.inc" 
.include	"delay.inc"
.include	"read.inc"
.include	"reset.inc"	

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
rep:
	sleep
	
	rcall	readDataJoy

	cpi	dataJoy,	BUTTON_MASK
	brne	main

	inc	RepCount

	cpi	RepCount,	REPEAT
	brne	rep

	cli
	rcall	resetDendy

	rjmp	main
