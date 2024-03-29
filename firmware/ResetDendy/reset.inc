/*
������������ ������.
���������� ������� temp, tik, repCount.

1) ��������� ��������� INT0, PCINT0, WDT.
2) ����������� ����� RESET ���������������� �� ����� (�� ������ �������� ������ ������� - Dendy ��������).
3) ��������� ������������ �������� �� 2 �. (���������� Watchdog)
4) ����������� ����� RESET ���������������� �� ����, ��� ������ �� ������ Dendy 
(��� �������, ����� �� ��������� ����� ����������������, ���� ����� ����� ���������� ������� �� ���������).
5) ���������� ��������. ������� repCount ������������� � ���������� WDT, ��� �������� ������. 
������� tik ������������� � ���������� PCINT0 ���� ����� LATCH � ������� ������.
6) ���������� Watchdog.
7) ��������� � ����������� Watchdog �� ���������� (���������� ����� 1 �).
8) ����������� ���������� PCINT0, ����� ��� ���� ����� �������� 8 ������ ������� LATCH, ��� ����� ��������� ���������.
9) ��������� � ���. 
10) ����������� �� ��������� Watchdog ��� PCINT0.
	9.1) ��������� repCount �� ���-�� ������, ���� ������� ��������� �����. ��� �� 1 ����.
	9.2) ��������� tik �� 8, ���� ������� �������, ��� ����������. ��� �� 10 ����.
	9.3) ��������� � ���. 
10) ��������� ��������� PCINT0, WDT.
12) ����������� � �������� ���������� INT0.
13) �����.      
*/
resetDendy:

	cli

	clr	temp
	out	EIMSK,	temp
	out	PCICR,	temp
	out	WDTCSR,	temp

.ifdef NES
	sbi	PORTB,	RESET
.endif 

	sbi	DDRB,	RESET

	rcall	delay

	cbi	DDRB,	RESET

.ifdef NES
	cbi	PORTB,	RESET
.endif 

	clr	tik
	clr	repCount

	wdr
	ldi	temp,	(1<<WDIE) | (1<<WDP2) | (1<<WDP1) 
	out	WDTCSR,	temp

	ldi	temp,	(1<<PCIE0) 
	out	PCICR,	temp
	ldi	temp,	(1<<PCIF0) 
	out	PCIFR,	temp

	sei

waitReset:
	sleep
	nop

	cpi	repCount,	REPTIME
	breq	resetDendy

	cpi	tik,	TOTAL_BITS
	breq	okReset

	rjmp	waitReset
okReset:
	cli

	clr	temp
	out	WDTCSR,	temp
	out	PCICR,	temp

	ldi	temp,	(1<<INT0) 
	out	EIMSK,	temp
	ldi	temp,	(1<<INTF0)
	out	EIFR,	temp

	sei	

	ret
