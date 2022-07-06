// write LCD for Apple Pi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int lcd_cmdwrite(int fd, unsigned char dat)
{
	unsigned char buff[2];
	buff[0] = 0;
	buff[1] = dat;
	return write(fd,buff,2);
}

int lcd_datawrite(int fd, char dat[])
{
	int len;
	char buff[100];

	len = strlen(dat);  // don't count EOS (Null char)
	if (len>99) {printf("too long string\n"); exit(1); }
	memcpy(buff+1, dat, len);	// shift 1 byte, ignore EOS
	buff[0] = 0x40;	// DATA Write command
	return write(fd, buff, len+1);
}
void initLCD(int fd)
{
	int i;
	unsigned char init1[]={ 0x38, 0x39, 0x14, 0x70, 0x56, 0x6c };
	unsigned char init2[]={ 0x38, 0x0c, 0x01 };

	usleep(100000);	// wait 100ms
	for (i=0;i<sizeof(init1)/sizeof(unsigned char);i++) {
		if(lcd_cmdwrite(fd, init1[i])!=2){
			printf("internal error1\n");
			exit(1);
		}
		usleep(50); // wait 50us
	}

	usleep(300000);	// wait 300ms

	for (i=0;i<sizeof(init2)/sizeof(unsigned char);i++) {
		if(lcd_cmdwrite(fd, init2[i])!=2){
			printf("internal error2\n");
			exit(1);
		}
		usleep(50);
	}
	usleep(2000);	// wait 2ms
}

int location(int fd, int y)
{
	int x = 0;
	int cmd=0x80 + y * 0x40 + x;
	return lcd_cmdwrite(fd, cmd);
}

int clear(int fd)
{
	int val = lcd_cmdwrite(fd, 1);
	usleep(1000);	// wait 1ms
	return val;
}

int open_i2c(){
	int i2c;
	i2c=open("/dev/i2c-1",O_RDWR);
	if(i2c<0){
		perror("i2c_open error");
		exit(1);	
	}
	int val;
	val=ioctl(i2c,I2C_SLAVE,0x3e);
	if(val<0){
		perror("ioctl error");
		exit(1);	
	}
	initLCD(i2c);
	return i2c;
}

void close_i2c(int i2c){
	int val;
	val=close(i2c);
	if(val<0){
		perror("i2c_close error");
		exit(1);	
	}
}

void lcd(int i2c)
{
	int val;
	val=lcd_datawrite(i2c,"hello");
	usleep(30);	//
	if(val<0){
		perror("datawrite error");
		exit(1);
	}
}
