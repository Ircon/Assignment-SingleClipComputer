CC = iccavr
CFLAGS =  -IC:\icc\include\ -IC:\ygksb0108\programme2 -e -DATMEGA -DATMega32  -l -g -Mavr_enhanced 
ASFLAGS = $(CFLAGS)  -Wa-g
LFLAGS =  -LC:\icc\lib\ -g -ucrtatmega.o -bfunc_lit:0x54.0x8000 -dram_end:0x85f -bdata:0x60.0x85f -dhwstk_size:16 -beeprom:1.1024 -fihx_coff -S2
FILES = main.o m32macro.o bitmap.o font.o graphic.o LCD_DRIVER.o 

version1:	$(FILES)
	$(CC) -o version1 $(LFLAGS) @version1.lk   -lcatmega
main.o: F:\FÅÌ\KS0108B\YANGGA~1/LCD_DRIVER.h C:/icc/include/iom32v.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/connection.h F:\FÅÌ\KS0108B\YANGGA~1/m32_init.c C:/icc/include/iom32v.h C:/icc/include/macros.h\
 F:\FÅÌ\KS0108B\YANGGA~1/connection.h F:\FÅÌ\KS0108B\YANGGA~1/font.h F:\FÅÌ\KS0108B\YANGGA~1/graphic.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/LCD_DRIVER.H F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h C:/icc/include/stdio.h\
 C:/icc/include/stdarg.h C:/icc/include/_const.h F:\FÅÌ\KS0108B\YANGGA~1/bitmap.h F:\FÅÌ\KS0108B\YANGGA~1/graphic.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/graphic.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h
main.o:	F:\FÅÌ\KS0108B\YANGGA~1\main.c
	$(CC) -c $(CFLAGS) F:\FÅÌ\KS0108B\YANGGA~1\main.c
m32macro.o: F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h
m32macro.o:	F:\FÅÌ\KS0108B\YANGGA~1\m32macro.c
	$(CC) -c $(CFLAGS) F:\FÅÌ\KS0108B\YANGGA~1\m32macro.c
bitmap.o: F:\FÅÌ\KS0108B\YANGGA~1/bitmap.h F:\FÅÌ\KS0108B\YANGGA~1/graphic.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/LCD_DRIVER.H C:/icc/include/iom32v.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/connection.h\
 F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h
bitmap.o:	F:\FÅÌ\KS0108B\YANGGA~1\bitmap.c
	$(CC) -c $(CFLAGS) F:\FÅÌ\KS0108B\YANGGA~1\bitmap.c
font.o: F:\FÅÌ\KS0108B\YANGGA~1/font.h F:\FÅÌ\KS0108B\YANGGA~1/graphic.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/LCD_DRIVER.H C:/icc/include/iom32v.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/connection.h\
 F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h C:/icc/include/stdio.h C:/icc/include/stdarg.h C:/icc/include/_const.h
font.o:	F:\FÅÌ\KS0108B\YANGGA~1\font.c
	$(CC) -c $(CFLAGS) F:\FÅÌ\KS0108B\YANGGA~1\font.c
graphic.o: F:\FÅÌ\KS0108B\YANGGA~1/graphic.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/LCD_DRIVER.H C:/icc/include/iom32v.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/connection.h
graphic.o:	F:\FÅÌ\KS0108B\YANGGA~1\graphic.c
	$(CC) -c $(CFLAGS) F:\FÅÌ\KS0108B\YANGGA~1\graphic.c
LCD_DRIVER.o: F:\FÅÌ\KS0108B\YANGGA~1/LCD_DRIVER.H C:/icc/include/iom32v.h F:\FÅÌ\KS0108B\YANGGA~1/m32macro.h F:\FÅÌ\KS0108B\YANGGA~1/connection.h
LCD_DRIVER.o:	F:\FÅÌ\KS0108B\YANGGA~1\LCD_DRIVER.c
	$(CC) -c $(CFLAGS) F:\FÅÌ\KS0108B\YANGGA~1\LCD_DRIVER.c
