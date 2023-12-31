/*
�������������� ��������� ����������������.
���������� ������� temp.
Temp ��������.

1. ��������� �����.
2. ���������� Dendy.
3. ��������� ����������.
5. ��������� ��� � ������.
6. ����������� ������������ �� 8 MHz.
7. �������� ����� ��� (IDLE).
8. ����������� � �������� ���������� INT0 �� ������ (������� � 0 � 1).
(������������ ��� ������������ ������� LATCH)
9. ��������� ����������.
10. ������� ����� � Dendy.
11. ��������� � �������� ����.

*/
init: 

	ldi	temp,	low(RAMEND)
	out	SPL,	temp

	sbi	DDRB,	RESET

	ldi	temp,	(1<<ACD) | (1<<ACI)
	out	ACSR,	temp

.if defined(ATtiny10) || defined(ATtiny5)

	ldi	temp,	(1<<PRADC) | (1<<PRTIM0)   
	out	PRR,	temp

.elif defined(ATtiny9) || defined(ATtiny4)

	ldi	temp,	(1<<PRTIM0)   
	out	PRR,	temp

.endif 

	ldi	temp,	0xD8
	out	CCP,	temp
	clr	temp
	out	CLKPSR,	temp

	ldi	RepCount,	stepOverClock
cycleOverClock:
	in	temp,	OSCCAL
	ldi	dataJoy,	stepOSCCAL
	add	temp,	dataJoy
	out	OSCCAL,	temp

	ldi	temp,	delayStabil
cycleSabil:
	dec	temp
	brne	cycleSabil

	dec	RepCount
	brne	cycleOverClock

	ldi	temp,	(1<<SE)
	out	SMCR,	temp

	ldi	temp,	(1<<ISC00) | (1<<ISC01)
	out	EICRA,	temp
	ldi	temp,	(1<<INT0) 
	out	EIMSK,	temp
	ldi	temp,	(1<<INTF0)
	out	EIFR,	temp

	sei

	cbi	DDRB,	RESET

	rjmp	main
