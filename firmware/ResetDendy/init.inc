/*
Первоначальная настройка микроконтроллера.
Использует регистр temp.
Temp обнуляет.

1. Настройка стека.
2. Сбрасываем Dendy.
3. Отключаем компаратор.
5. Отключаем АЦП и таймер.
6. Настраиваем тактирование на 8 MHz.
7. Увеличиваем частоту тактирования в несколько этапов с задержкой между этапами.
8. Включаем режим сна (IDLE).
9. Настраиваем и включаем прерывание INT0 по фронту (переход с 0 в 1).
(Используется для отлавливания сигнала LATCH)
10. Разрешаем прерывания.
11. Снимаем сброс с Dendy.
12. Переходим в основной цикл.

*/
init: 

	ldi	temp,	low(RAMEND)
	out	SPL,	temp

.ifdef NES
	sbi	PORTB,	RESET
.endif 

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

.ifdef NES
	cbi	PORTB,	RESET
.endif 

	rjmp	main
