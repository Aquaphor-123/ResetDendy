/*
Подпрограмма приёма данных от джойстика.
Использует регистры temp и dataJoy.
Temp обнуляет, а в dataJoy возвращает полученные данные.

1) Отключаем прерывания. 
2) Устанавливаем счётчик интераций получения битов. 
3) Ждём пока сигнал CLOCK перейдёт на высокий уровень. 
Считывание должно производиться через 250 нс после появления фронта сигнала CLOCK (кроме первого бита - он готов к считыванию через 250 нс после сигнала LATCH).
4) Проверяем уровень сигнала DATA:
	если высокий - выставляем в регистре dataJoy младший разряд в 1 и идём дальше
	если низкий - идём дальше
5) Ждём появления низкого уровня на сигнале CLOCK (длительность низкого уровня 360 нс).
6) Уменьшаем счётчик интераций получения битов.
7) Проверяем, получены ли все биты:
	если нет, то сдвигаем данные регистра dataJoy влево, переходим к получению следующего бита (шаг 3)
	если да, то выходим из подпрограммы
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
