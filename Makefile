keydisp: timer.o gpio.o lcd.o keydisp.o
	arm-linux-gnueabihf-gcc timer.o gpio.o lcd.o keydisp.o -o keydisp
timer.o: timer.c
	arm-linux-gnueabihf-gcc -c timer.c
gpio.o: gpio.c
	arm-linux-gnueabihf-gcc -c gpio.c
lcd.o: lcd.c
	arm-linux-gnueabihf-gcc -c lcd.c
keydisp.o: keydisp.c
	arm-linux-gnueabihf-gcc -c keydisp.c

clean:
	rm *.o keydisp
