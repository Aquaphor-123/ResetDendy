/*
Подпрограмма сброса.
Использует регистр temp, tik, repCount.

1) Отключаем прервание INT0, PCINT0, WDT.
2) Настраиваем вывод RESET микроконтроллера на выход (на выводе появится низкий уровень - Dendy сброшена).
3) Запускаем подпрограмму задержки на 2 с. (Использует Watchdog)
4) Настраиваем вывод RESET микроконтроллера на вход, для вывода из сброса Dendy 
(так сделано, чтобы не повредить вывод микроконтроллера, если сброс будет произведен кнопкой на приставке).
5) Сбрасываем счётчики. Счётчик repCount увеличивается в прерывание WDT, для подсчёта секунд. 
Счётчик tik увеличивается в прерывание PCINT0 если вывод LATCH в высоком уровне.
6) Сбрасываем Watchdog.
7) Запускаем и настраиваем Watchdog на прерывание (прерывание через 1 с).
8) Настраиваем прерывание PCINT0, нужно для того чтобы отловить 8 тактов сигнала LATCH, что будет признаком включения.
9) Переходим в сон. 
10) Просыпаемся от перывания Watchdog или PCINT0.
	9.1) Проверяем repCount на кол-во секунд, если совпало повторяем сброс. Идём на 1 этап.
	9.2) Проверяем tik на 8, если совпало считаем, что включилась. Идём на 10 этап.
	9.3) Переходим в сон. 
10) Отключаем прервание PCINT0, WDT.
12) Настраиваем и включаем прерывание INT0.
13) Выход.      
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
