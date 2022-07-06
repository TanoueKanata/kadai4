#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

extern unsigned int gpio(int fd);	//gpio hyouzi
extern int timer();	//1byou go ni hello
extern void lcd(int i2c);
int sig;

int main(){
	int val;
	//init
	int fd;
	fd=open_fd();
	int i2c;
	i2c=open_i2c();
	unsigned int before;
	val=lcd_cmdwrite(i2c,0x0F);	//Disply on
	usleep(30);
	if(val<0){
		perror("lcd_cmdwrite error");
		exit(1);	
	}
	val=clear(i2c);
	if(val<0){
		perror("clear error");
		exit(1);
	}
	//main
	unsigned int gp;
	int count=0;	//lcd count
	while(1){
		gp=gpio(fd);
		gp=gp&0x400000;	//mask
		if(gp==0){	//switch on
			if(sig==0 && count==0){
				lcd(i2c);
				count=1;
			}
		}
		else if(sig==0 && count==1){
			timer();
			while(sig==1){}
			clear(i2c);
			count=0;
		}
	}
	close_fd(fd);
	close_i2c(i2c);
	return 0;
}
