/*
������������ ����� ������ �� ���������.
���������� �������� temp � dataJoy.
Temp ��������, � � dataJoy ���������� ���������� ������.

1) ��������� ����������. 
2) ������������� ������� ��������� ��������� �����. 
3) ��� ���� ������ CLOCK ������� �� ������� �������. 
���������� ������ ������������� ����� 250 �� ����� ��������� ������ ������� CLOCK (����� ������� ���� - �� ����� � ���������� ����� 250 �� ����� ������� LATCH).
4) ��������� ������� ������� DATA:
	���� ������� - ���������� � �������� dataJoy ������� ������ � 1 � ��� ������
	���� ������ - ��� ������
5) ��� ��������� ������� ������ �� ������� CLOCK (������������ ������� ������ 360 ��).
6) ��������� ������� ��������� ��������� �����.
7) ���������, �������� �� ��� ����:
	���� ���, �� �������� ������ �������� dataJoy �����, ��������� � ��������� ���������� ���� (��� 3)
	���� ��, �� ������� �� ������������
*/
		readDataJoy:
			cli
			ldi	temp,	TOTAL_BITS

		readNewBit:

			sbis	PINB,	CLOCK
			rjmp	readNewBit

			sbic	PINB,	DATA
			rjmp	setBit
			rjmp	clrBitClrCLOCK
	
		setBit:

			ori	dataJoy,	0x01	

		clrBitClrCLOCK:
.ifndef	SIMULATOR
			sbic	PINB,	CLOCK
			rjmp	clrBitClrCLOCK
.endif
			dec	temp
			breq	allBitRecrived

			lsl	dataJoy

			rjmp	readNewBit
	
		allBitRecrived:
			sbi	EIFR,	INTF0
			sei

			ret
