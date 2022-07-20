/*
 * ultrasonic.c
 *
 *  Created on: Jul 16, 2022
 *      Author: khaled Atef
 */


#include <util/delay.h> /* For the delay functions */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "ultrasonic.h"
#include "ICU.h"
#include "gpio.h"
#include "std_types.h"

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;

void Ultrasonic_init(void){
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};
	Icu_init(&Icu_Config);
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(TRIGGER_PORT_ID, TRIGGER_PIN_ID, PIN_OUTPUT);
}

void Ultrasonic_Trigger(void){
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void){
	Ultrasonic_Trigger();

	uint16 distance;

	distance=g_timeHigh/58;

	return distance;
}

void Ultrasonic_edgeProcessing(void){

		g_edgeCount++;
		if(g_edgeCount == 1)
		{
			/*
			 * Clear the timer counter register to start measurements from the
			 * first detected rising edge
			 */
			Icu_clearTimerValue();
			/* Detect falling edge */
			Icu_setEdgeDetectionType(FALLING);
		}
		else if(g_edgeCount == 2)
		{
			/* Store the High time value */
			g_timeHigh = Icu_getInputCaptureValue();
			/* Detect rising edge */
			Icu_setEdgeDetectionType(RISING);
		}

}
