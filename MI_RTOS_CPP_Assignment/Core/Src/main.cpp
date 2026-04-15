

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"
#include"STM32F407xx.h"

#define mainDELAY_LOOP_COUNT		( 0xffffff )

/* The task function. */
/* The two task functions. */
void vDriverAssistanceTask( void *pvParameters );
void vEmergencySystemTask( void *pvParameters );
void LED_Init(){
	RCC->AHB1ENR|=(1<<3);//PORTD

	GPIOD->MODER&=~(0xFF<<24);
	GPIOD->MODER|=(0x55<<24);
}
TaskHandle_t xDriverHandle;
TaskHandle_t xEmergencyHandle;

void vApplicationIdleHook(void);
typedef struct sensor
{
bool coll;//0,1
float temp;
bool door_status;
bool Airbag_status;
}Sensor_status;

Sensor_status t1;
static volatile uint32_t driver_counter=0;
static volatile uint32_t Emergency_counter=0;

int main ( void )
{
	LED_Init();
	      t1.coll=false;
	       t1.temp=50.00;
	       t1.door_status=false;
	       t1.Airbag_status=false;

	xTaskCreate(vDriverAssistanceTask,"DriverAssistanceTask",200,(void*)&t1,3,&xDriverHandle);
	xTaskCreate(vEmergencySystemTask, "EmergencySystemTask",200,(void*)&t1,2,&xEmergencyHandle);



         /* The task handle is the last parameter ^^^^^^^^^^^^^ */

	/* Start the scheduler to start the tasks executing. */
	vTaskStartScheduler();

	while(1)
	{
	}
	return 0;
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
 	taskENTER_CRITICAL();
	{
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}


void vDriverAssistanceTask(void *pvParameters ) //&t1
{

        Sensor_status* sptr= (Sensor_status*)pvParameters;
        UBaseType_t uxDPriority;
	uxDPriority = uxTaskPriorityGet(NULL);

	while(1)
	{
		GPIOD->ODR^=(1<<12);
		driver_counter++;
			if(driver_counter>=100000){
				sptr->coll=true;
			}
                if(sptr->coll)
                {
		vTaskPrioritySet(xEmergencyHandle ,(uxDPriority+1) );//xEmergencyHandle =3+1 =4
		GPIOD->ODR^=(1<<13);
		GPIOD->ODR&=~(1<<12);
                driver_counter=0;
			}


	}
}
void vEmergencySystemTask(void *pvParameters )
{
        Sensor_status* sptr= (Sensor_status*)pvParameters;
        UBaseType_t uxEPriority; //4

	uxEPriority = uxTaskPriorityGet(NULL);


	//vTaskPrioritySet(xEmergencyHandle,(uxEPriority-2) );
	while(1)
	{
		Emergency_counter++;
		if(Emergency_counter>=50000){
	     sptr->Airbag_status=true;
		}
	    if(sptr->Airbag_status){
	    	GPIOD->ODR^=(1<<14);
	vTaskPrioritySet(xEmergencyHandle,(uxEPriority-2));
	 Emergency_counter=0;
	}
}
}


void vApplicationIdleHook(void)
{
	/* This hook function does nothing but increment a counter. */

 printf("idle");
}
/*-----------------------------------------------------------*/


