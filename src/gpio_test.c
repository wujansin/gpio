//********************************
// wiringPi Lib Function Test
//********************************
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

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
