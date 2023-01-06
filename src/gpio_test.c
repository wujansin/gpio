//********************************
// wiringPi Lib Function Test
//********************************
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define LED_RED	5
#define LED_YEL 6
#define BTN		4

int main (void)
{
	int btn_val;
	int i=0;
	struct timeval tv;
	pid_t pid;
	
	printf("Build Time %s, %s \n", __TIME__, __DATE__ );
	
	/* Set up GPIO */
	wiringPiSetup() ;
	pinMode(BTN, INPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_YEL, OUTPUT);

	pid =fork();
	if(pid <0)
	{
		perror("fork error");
	}
	else if(pid == 0){
		//child process
		printf("Child process\n");
		for(;;)
		{			
			digitalWrite (LED_YEL, HIGH);
			delay(500);
			digitalWrite (LED_YEL, LOW);
			delay(500);		
			printf("Child %d\n",i++);
		}
	}
	else
	{
		//parent process
		printf("Parent process\n");
		for (;;)
		{
			gettimeofday(&tv, NULL);
			
			btn_val = digitalRead(BTN);

			if(btn_val == HIGH)
			{
				digitalWrite (LED_RED, HIGH); 
				//digitalWrite (LED_YEL, HIGH);
				//delay (500);
				//printf("Time :%d.%d LED ON\n", tv.tv_sec,tv.tv_usec);
				printf("Parent %d\n",i++);
			}	
			else
			{
				digitalWrite (LED_RED,  LOW); 
				//digitalWrite (LED_YEL,  LOW);
				//delay (500);
			}	
		}
	}

	printf("END fork\n");
	
	return 0 ;
}
