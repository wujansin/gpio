//********************************
// wiringPi Lib Function Test
//********************************
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

/*
RED: Red LED
YEL: Yellow LED
GRN: Green LED
ON: LED ON
OFF: LED OFF
BTN: Button 
*/
#define LED_RED 5
#define LED_YEL 6
#define LED_GRN 1
#define BTN_1 	4

#define BTN_ON	HIGH
#define BTN_OFF	LOW

#define LED_ON(n)	digitalWrite(LED_##n, HIGH)
#define LED_OFF(n)	digitalWrite(LED_##n, LOW)
#define BTN_READ(n)	digitalRead(BTN_##n)

/* Global Variable*/
volatile int g_timer_counter = 0;


/***********************************************
* InitTimer (設定Timer的時距)  					*
* intv_m_sec: 定時時距		(msec)	 			*
* start_sec: 開始執行的時間 (sec)				*
***********************************************/
int InitTimer(int intv_m_sec, int start_sec)
{
	struct itimerval value;
	//10秒後開始執行，每xxx m sec 執行一次。
 	value.it_interval.tv_sec  = 0;
    value.it_interval.tv_usec = 1000*intv_m_sec; 
    value.it_value.tv_sec = start_sec;	
    value.it_value.tv_usec =0;

	//value.it_interval = value.it_value;
	setitimer(ITIMER_REAL, &value, NULL);
			
	return 0;
}
/************************************************
* DoTimerWork (定時(100ms)動作)     			*  
************************************************/
int DoTimerWork(int signalnum)
{
	g_timer_counter++;
	if(g_timer_counter>8)
		LED_ON(YEL);
	else
		LED_OFF(YEL);	
	if(g_timer_counter>9)
		g_timer_counter = 0;

	/* Exit */
	signal(SIGALRM, DoTimerWork);
	return 0;			

}	

int main(void)
{
	int btn_val;
	int i = 0;
	struct timeval tv;
	pid_t pid;

	printf("Build Time %s, %s \n", __TIME__, __DATE__);

	/* Set up GPIO */
	wiringPiSetup();
	pinMode(BTN_1, INPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_YEL, OUTPUT);
	pinMode(LED_GRN, OUTPUT);

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
	}
	else if (pid == 0)
	{
		// child process
		printf("Child process\n");
		for (;;)
		{
			
			LED_ON(GRN);
			delay(500);
			LED_OFF(GRN);
			delay(500);
			printf("Child %d\n", i++);
		}
	}
	else
	{
		// parent process
		printf("Parent process\n");
		/* 每 100 ms (0.1秒)檢查系統狀態 */			
		if(signal(SIGALRM, DoTimerWork) == SIG_ERR)
			perror("signal");
		else
			InitTimer(100,5);			

		for (;;)
		{
			gettimeofday(&tv, NULL);

			btn_val = BTN_READ(1);	//digitalRead(BTN);

			if (btn_val == BTN_ON)
			{
				// printf("Time :%d.%d LED ON\n", tv.tv_sec,tv.tv_usec);
				LED_ON(RED);
				printf("Parent %d\n", i++);	
			}
			else
			{
				LED_OFF(RED);
			}
		}
	}
	printf("END fork\n");

	return 0;
}
