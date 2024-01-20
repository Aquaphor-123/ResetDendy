Проект - сброс Dendy (Famicom/NES) с джойстика. 

Данный проект реализует сброс для приставок Dendy/Famicom/NES при нажатии определенной комбинации кнопок на джойстике.

Проект использует минимальное количество элементов:
1) Макетная плата sot23;
2) Микроконтроллер ATTiny10(4, 5, 9);
3) Керамический конденсатор 0,1 ... 1 мкФ.

Прошивку собираем в Microchip studio. Прошивка лежит в папке - firmware.
В main.asm можно выбрать:
1) Какой микроконтроллер вы используете;
2) Комбинацию кнопок, запускающую процедуру сброса.

Если устанавливайте в NES с CIC чипом (в этих приставках кнопка сброса подключает RESET к VCC, а не к GND) надо раскомментировать .equ NES = 1. Данную возможность я не проверял. 
(ВНИМАНИЕ! Убедитесь, что вы понимаете что делаете.)

После прошивки запрограммируйте Fuse bit RSTDISBL. 
(ВНИМАНИЕ! После программирования Fuse bit RSTDISBL перепрошить микроконтроллер можно будет только при подаче 12В
на вывод RESET микроконтроллера)

Пример сборки в папке - pcb.

В папке other вы можете найти:
1) Схемы Dendy/Famicom/NES;
2) Документацию на микроконтроллер и микросхему из джойстика;
3) Пример установки в мою Dendy.

По вопросам по проекту пишите.



The project is To reset of the Dendy (Famicom/NES) by joystick.

This project to do To reset for Dendy/Famicom/NES consoles when you pressed a certain combination of buttons on the joystick.

The project uses minimum number of elements:
1) SOT23 breadboard;
2) ATTiny10 microcontroller (4, 5, 9);
3) Ceramic capacitor 0,1 ... 1 uF;

We compile the firmware in Microchip studio. The firmware is in the folder "firmware".
In main.asm you can choose:
1) Microcontroller do you use;
2) Combination of buttons for start the reset.

If you install this project in concol NES with a CIC chip you need to uncomment .equ NES = 1 (in these consoles, the reset button connects RESET to VCC, not to GND). I did not check this function. 
ATTENTION! Make sure that you understand what you do :) 

After firmwared, you need set Fuse bit RSTDISBL.
ATTENTION! After seted Fuse bit RSTDISBL repeat of re-flash the microcontroller is possible only applying 12V to the RESET pin of the microcontroller.

Example is in the folder "pcb".

You can find in the folder "other":
1) Dendy/Famicom/NES circuits;
2) Documentation for the microcontroller and the joystick chip;
3) Example of installation in my Dendy.

If you have any questions, write me.