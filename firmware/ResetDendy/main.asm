/*
Смотри файл настроек - setting.conf.

Назначение выводов микроконтроллера можно переназначить:
1) Сигнал LATCH должен быть подключен к выводу PINB2, так как он используется для выхода микроконтроллера из режима сна по прерыванию.
2) Сигнал RESET возможно переподключить с вывода PINB1 на PINB0. 
3) Сигнал DATA возможно переподключить с вывода PINB0 на PINB1 или PINB3.
4) Сигнал CLOCK возможно переподключить с вывода PINB3 на PINB0.

Для переназначения выводов измените значения ниже.
*/

.equ	DATA =	PINB0
.equ	RESET =	PINB1
.equ	LATCH =	PINB2
.equ	CLOCK =	PINB3  

// Маски кнопок джойстика.
.equ	RIGHT =	0b11111110
.equ	LEFT =	0b11111101
.equ	DOWN =	0b11111011
.equ	UP =	0b11110111
.equ	START =	0b11101111
.equ	SELECT =	0b11011111
.equ	B =	0b10111111
.equ	A =	0b01111111

.equ	TOTAL_BITS	=	8

.include "setting.conf"

.ifdef	J_8BitDo_N30
.set	BUTTON_MASK = DOWN & SELECT
.endif

.ifdef	SIMULATOR
.set	BUTTON_MASK = 0xff
.set	REPEAT = 1
.set	REPTIME = 2	
.warning "Only for the simulator!" 
.endif

.def	temp	=	r16
.def	dataJoy	=	r17
.def	tik	=	r18
.def	RepCount	=	r19

.CSEG
.org	0x0000
rjmp	preinit

.org	INT0addr
reti

.org	PCI0addr
rjmp	PCI0	

.org	WDTaddr
rjmp	WDT
// Обработчик прерывания PCINT0 (сохраняем SREG - вдруг прерывание пришло в момент сравнения переменных), увеличиваем tik, если CLOCK в 0.
PCI0:
in	temp,	SREG
sbic	PINB,	LATCH
inc	tik
out	SREG,	temp
reti
// Обработчик прерывания WDT (сохраняем SREG - вдруг прерывание пришло в момент сравнения переменных), увеличиваем repCount.
WDT:
in	temp,	SREG
inc	repCount
out	SREG,	temp
reti

.include	"init.inc" 
.include	"read.inc"
.include	"reset.inc"	
.include	"delay.inc"
/*
Основной цикл:
1) Очищаем счётчик повторов комбинации кнопок.
2) Переводим микроконтроллер в режим сна.
3) Выход из режима сна происходит по прерыванию INT0 (по фронту) от сигнала LATCH (после сигнала LATCH есть 6,3 мкс для того, чтобы считать первый бит посылки, но не ранее чем 250 нс).
4) Запускаем подпрограмму получения данных от джойстика.
5) Сверяем данные с маской. 
	5.1) Если не совпало переходим к шагу 1 (сбрасываем счётчик повторов, потому что не было необходимого количества повторов).
	5.2) Если совпало, увеличиваем количество повторов комбинации кнопок.
6) Проверяем, получены ли все повторы комбинации кнопок.
	6.1) Если нет, переходим к шагу 2 (счётчик повторов не сбрасываем, ждём, будет ли ещё повтор комбинации кнопок).
	6.2) Если все получены, выполняем сброс Dendy.
*/

main:
	clr	RepCount
loop:
	sleep
	
	rcall	readDataJoy

	cpi	dataJoy,	BUTTON_MASK
	brne	main

	inc	RepCount

	cpi	RepCount,	REPEAT
	brne	loop

	rcall	resetDendy

	rjmp	main	
