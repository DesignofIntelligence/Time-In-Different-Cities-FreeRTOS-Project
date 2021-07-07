/* Library includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Stellaris library includes. */
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"

/* Include LCD */
#include "lcd_drivers.h"
#include "UART.h"

/* Include UART */
# define GPIO_PA0_U0RX 0x00000001
# define GPIO_PA1_U0TX 0x00000401

/* Function Prototypes */
//void UART_init(void);
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

/*-----------------------------------------------------------*/
char cities[][10] = {"London","Paris", "Madrid", "Rome", "Athens", "Ankara", "Istanbul", 
"Cairo", "Moscow", "Tehran"};
char timediff[] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 4};
int currentcity = 0;
portTickType xLastWakeTime;
xQueueHandle xQueue1;
xQueueHandle xQueue2;
int main()
{
	xQueue1 = xQueueCreate(1, 8); //Queue of one item, such that this item is 8 bytes size.
	xQueue2 = xQueueCreate(1, 8);
	if (xQueue1 != NULL && xQueue2 != NULL)
	{
		xTaskCreate(Task1, "Timer", 240, NULL, 1, NULL);
		xTaskCreate(Task2, "LCD Display", 240, NULL, 1, NULL);
		xTaskCreate(Task3, "PC Display", 240, NULL, 1, NULL);
		vTaskStartScheduler();

		//should never reach here
		for (;;);
	}
	return 0;
}
typedef struct Message //create a message struct that will be sent in queues, containing the time.
{
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
} AMessage;


void Task1(void *pvParameters){
	AMessage Tim;
	portBASE_TYPE xStatus; 
	xStatus = xQueueReceive(xQueue2, &Tim, portMAX_DELAY); //Receive time from Task 3 first time only
	xLastWakeTime = xTaskGetTickCount();
	for (;;)
	{
		vTaskDelayUntil(&xLastWakeTime, (1000 / portTICK_RATE_MS));
		Tim.seconds++;
		if (Tim.seconds == 60)
		{
			Tim.seconds = 0;
			Tim.minutes++;
			if (Tim.minutes == 60)
			{
				Tim.minutes = 0;
				Tim.hours++;
				if (Tim.hours == 24)
				{
					Tim.hours = 0;
				}
			}
		}
		xQueueSendToBack(xQueue1, &Tim, 0); //Send the Time to task 2
	}
}
void Task2(void *pvParameters)
{
	// Declare some variables
	AMessage Tim;
	portBASE_TYPE xStatus; 
	// Define the LCD interface
	LCD_init();
	LCD_Clear();	
	// DO FOREVER
	for(;;)
	{
		// Get the current time from Task 1 via xQueue1
		// Wait until new time is received
		xStatus = xQueueReceive(xQueue1, &Tim, portMAX_DELAY); 
		if (xStatus == pdPASS)
		{
			LCD_Clear();
			LCD_PrintLn(0, cities[currentcity]);  //printing the city name
			//LCD_writeStringWithPos(cities[currentcity],0,0);
			
			// Extract the required city name (global)416 21. Time in different cities

			Tim.hours += timediff[currentcity];
			if(Tim.hours > 24)
			{
				Tim.hours -= 24;
			}
			//printing time
			if(Tim.hours <10) //print hours first
			{
				LCD_writeCharWithPos('0',1,0);
				LCD_writeCharWithPos(Tim.hours + '0',1,1);
				
			}
			else
			{
				int temp0 = Tim.hours/10;
				int temp1= Tim.hours %10;
				LCD_writeCharWithPos(temp0 + '0', 1, 0);
				LCD_writeCharWithPos(temp1 + '0', 1, 1);
			}
			LCD_writeCharWithPos(':',1,2); //print :
			
			if(Tim.minutes <10) //print minutes 
			{
				LCD_writeCharWithPos('0',1,3);
				LCD_writeCharWithPos(Tim.minutes + '0',1,4);
			}
			else
			{
				int temp0 = Tim.minutes/10;
				int temp1= Tim.minutes %10;
				LCD_writeCharWithPos(temp0 + '0', 1, 3);
				LCD_writeCharWithPos(temp1 + '0', 1, 4);
			}
			
			LCD_writeCharWithPos(':',1,5); //print :
			
			
			if(Tim.seconds <10) //print seconds 
			{
				LCD_writeCharWithPos('0',1,6);
				LCD_writeCharWithPos(Tim.seconds + '0',1,7);	
			}
			else
			{
				int temp0 = Tim.seconds/10;
				int temp1= Tim.seconds %10;
				LCD_writeCharWithPos(temp0 + '0', 1, 6);
				LCD_writeCharWithPos(temp1 + '0', 1, 7);
				
			}
			
		}
		
	}
	
	// ENDDO
}

void Task3(void *pvParameters)
{
	int i;
	//declare some variables
	AMessage Tim;
	char time[10];
	// Configure UART at 9600 baud
	UARTInit();
	
	PrintString("\n\rTime in Different Countries");	
  PrintString("\n\r========================== ");
  PrintString("\n\rEnter the time in London (hh:mm:ss): ");
	// Read current time in London
	ReadTime(time);
	// Send current time to Task 1 via xQueue2
	Tim.hours		 = 10* (time[0] - '0') + (time[1] - '0');
	Tim.minutes	 = 10* (time[3] - '0') + (time[4] - '0');
	Tim.seconds	 = 10* (time[6] - '0') + (time[7] - '0');	
	xQueueSendToBack(xQueue2, &Tim, 0);
	// DO FOREVER
	for(;;)
	{
		// Display the city names
		PrintString("\n\r\n\rSelect a City:");
		for(i =0; i<10; i++)
		{
			char temp[1];
			PrintString("\n\r");
			temp[0] = i + '0';
			PrintString(temp);
      PrintString(". "); 
			PrintString(cities[i]); 
		}
		// Select the required city
		currentcity = ReadSelection();
	}


	// ENDDO
}
void vApplicationIdleHook(void)
{
	/* This example does not use the idle hook to perform any processing.  The
	idle hook will only be called if configUSE_IDLE_HOOK is set to 1 in 
	FreeRTOSConfig.h. */
}
/*-----------------------------------------------------------*/
