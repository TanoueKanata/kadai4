#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

struct itimerval timval;
int sig=0;

void sig_handler(int signum){
	sig=0;
}

int timer(){
	sig=1;
	timval.it_interval.tv_sec=0;
	timval.it_interval.tv_usec=0;
	timval.it_value.tv_sec=1;
	timval.it_value.tv_usec=0;
	setitimer(ITIMER_REAL,&timval,NULL);
	signal(SIGALRM, sig_handler);
	return sig;
}
