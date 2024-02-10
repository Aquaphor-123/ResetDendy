/*
Первоначальная настройка микроконтроллера.
Использует регистр temp.

1. Проверяем причину сброса микроконтроллера
2. Если из-за WDT отключаем его. (Рекомендации от производителя)
3. Если из-за появления питания, продолжаем работать. Если причины нет, то что-то пошло совсем не так)).
4. Обнуляем регистр причин сброса.
5. Настройка стека.
6. Сбрасываем Dendy.
7. Отключаем компаратор.
8. Отключаем АЦП и таймер.
9. Настраиваем тактирование на 8 MHz.
10. Увеличиваем частоту тактирования в несколько этапов с задержкой между этапами.
11. Включаем режим сна (IDLE).
12. Настраиваем маску прерывания PCINT0.
13. Настраиваем и включаем прерывание INT0 по фронту (переход с 0 в 1).
(Используется для отлавливания сигнала LATCH)
14. Разрешаем прерывания.
15. Снимаем сброс с Dendy.
16. Переходим в основной цикл.

*/
preinit: 
	in	temp,	RSTFLR	

	sbrs	temp,	WDRF
	rjmp	noWDTreset
	
	wdr
	andi	temp,	~(1<<WDRF)
	out	RSTFLR,	temp
	ldi	temp,	0xD8
	out	CCP,	temp
	clr	temp
	out	WDTCSR,	temp

	rjmp	init

noWDTreset:
	sbrs	temp,	PORF
	rjmp	fail

init:
	clr	temp
	out	RSTFLR,	temp

	ldi	temp,	high(RAMEND)
	out	SPH,	temp
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

	ldi	temp,	(1<<CLOCK)
	out	PCMSK,	temp

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

fail:

	rjmp	fail
