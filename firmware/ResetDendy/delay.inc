/*
Подпрограмма задержки.
Использует регистр temp.
temp обнуляет.

1) Сбрасываем Watchdog.
2) Запускаем и настраиваем Watchdog на прерывание.
3) Переходим в режим сна.
4) Выходим из режима сна по прерыванию от Watchdog (2 с).
5) Останавливаем Watchdog.
6) Выход.

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
