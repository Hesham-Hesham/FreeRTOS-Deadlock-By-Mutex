#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

SemaphoreHandle_t xBinarySemaphore ;
portBASE_TYPE xMutex;
portBASE_TYPE xMutex2;

void init(void);
static void FirstTask     (void * pvParameters);
static void SecondTask     (void * pvParameters);

void outChar(char x){
	
	while ((UART0_FR_R & (1<<5)) != 0);
	UART0_DR_R = x ;
}

void prntStr(char* cArr){
	
	while (*cArr){
		outChar(*(cArr++));
	}
	outChar('\r');
	outChar('\n');
}

int main (void){
	
init();
	
xTaskCreate(FirstTask, (const portCHAR*)"Continous Task", configMINIMAL_STACK_SIZE, NULL ,3,NULL);	
xTaskCreate(SecondTask, (const portCHAR*)"Periodic Task", configMINIMAL_STACK_SIZE, NULL ,1,NULL);	
	
vSemaphoreCreateBinary(xBinarySemaphore);
xMutex = xSemaphoreCreateMutex();
xMutex2 = xSemaphoreCreateMutex();

vTaskStartScheduler();
}

static void FirstTask(void * pvParameters){
	prntStr("This is The FirstTask_secondResource");
	for( ; ; )
	{
		prntStr("First task is now attempting to take the second resource...");
		xSemaphoreTake(xMutex2 , portMAX_DELAY);
		prntStr("Success!");
		prntStr("Second resource is now taken by the first task!");

		
		vTaskDelay(500);
		
		prntStr("First task is now attempting to take the first resource...");
		xSemaphoreTake(xMutex, portMAX_DELAY);
		prntStr("Success!");
		prntStr("First resource is now taken by the first task!");

		xSemaphoreGive(xMutex);
		xSemaphoreGive(xMutex2);
	}
}

static void SecondTask(void * pvParameters){
	
	xSemaphoreTake(xBinarySemaphore,0);
	for (;;)
	{
		prntStr("Second task is now attempting to take the first resource...");
		xSemaphoreTake(xMutex, portMAX_DELAY);
		prntStr("Success!");
 		prntStr("First resource is now taken by the second task!");
		
		prntStr("Second task is now attempting to take the second resource...");
		xSemaphoreTake(xMutex2 , portMAX_DELAY);
		prntStr("Success!");
		prntStr("Second resource is now taken by the second task!");

		xSemaphoreGive(xMutex2);
		xSemaphoreGive(xMutex);
	}
}

void init()
{	
	SYSCTL_RCGCGPIO_R |= 0x00000020;                                     // initialize clock
	while((SYSCTL_PRGPIO_R&0x00000020) == 0){}                           // looping until clock is initiallized
	GPIO_PORTF_LOCK_R = 0x4C4F434B;                                      // Unlocking PortF Pins
	GPIO_PORTF_CR_R |= (1<<0 | 1<<4);                                    // unlocking commit register for switch 1 & switch 2 
  GPIO_PORTF_DIR_R |= (1<<1 | 1<<2 | 1<<3);                            // detrmining the output pins  
	GPIO_PORTF_DIR_R &=~ (1<<0 | 1<<4);                                  // detrmining the input pins 
	GPIO_PORTF_PUR_R |= (1<<0 | 1<<4);                                   // configuring the switches with PullUp resistors
  GPIO_PORTF_DEN_R |= (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4);              // detrmining the pins direction 
	GPIO_PORTF_DATA_R &=~ (1<<1 | 1<<2 | 1<<3);                          // initially all leds are of 
	
	//UART init
	SYSCTL_RCGCUART_R  |= 1<<0 ;                      //initiallizing uart0
	SYSCTL_RCGCGPIO_R  |= 1<<0 ;                      //initiallizing portA pins 
	GPIO_PORTA_AFSEL_R |= (1<<0 | 1<<1);              //choosing the alternative function for pins 0 & 1 (Tx & Rx)
	GPIO_PORTA_PCTL_R  |= (1<<(0*4) | 1<<(1*4));      //choosing the alternative function to the pin
	GPIO_PORTA_DEN_R   |= (1<<0 | 1<<1);              //openning digital enable for pins 0 & 1 in PortA
	UART0_CTL_R &=~ 1<<0;     
	
	UART0_IBRD_R = 104;
	UART0_FBRD_R = 11;
	UART0_LCRH_R = 0x60;
	UART0_CC_R =0x0;
	
	UART0_CTL_R = 0x301;    
}

