/*
������������ ��������.
���������� ������� temp.
temp ��������.

1) ���������� Watchdog.
2) ��������� � ����������� Watchdog �� ����������.
3) ��������� � ����� ���.
4) ������� �� ������ ��� �� ���������� �� Watchdog (0.5 �).
5) ������������� Watchdog.
6) �����.

*/
		delay:
			wdr
			ldi	temp,	(1<<WDIE) | (1<<WDP2) | (1<<WDP0)
			out	WDTCSR,	temp
.ifndef	SIMULATOR	 
			sleep
.endif
			nop

			clr	temp
			out	WDTCSR,	temp

			ret
