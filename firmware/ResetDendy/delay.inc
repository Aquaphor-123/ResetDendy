/*
������������ ��������.
���������� ������� temp.
temp ��������.

1) ���������� Watchdog.
2) ��������� � ����������� Watchdog �� ����������.
3) ��������� � ����� ���.
4) ������� �� ������ ��� �� ���������� �� Watchdog (2 �).
5) ������������� Watchdog.
6) �����.

*/
delay:
	wdr
	ldi	temp,	(1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0)
	out	WDTCSR,	temp
	sei

.ifndef	SIMULATOR	 
	sleep
.endif
	nop
	cli				
	clr	temp
	out	WDTCSR,	temp

	ret
