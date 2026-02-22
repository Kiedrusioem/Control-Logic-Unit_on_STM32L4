/*
 * WorkMode.h
 *
 *  Created on: 20 Jan 2026
 *      Author: pawel
 */

#ifndef WORKMODE_H_
#define WORKMODE_H_

#include "main.h"
#include "adc.h"
#include "tim.h"



typedef struct {
	uint8_t lineEnable;

	uint16_t gpioOutPin;
	uint16_t gpioInPin;
	uint8_t workFunction;
	uint8_t subWorkFunction;
	uint16_t time1;
	uint16_t time2;
	uint16_t analogOutValue;
	uint16_t analogInValue;
	uint16_t nbCycles;

	GPIO_TypeDef* gpioOutPort;
	GPIO_TypeDef* gpioInPort;

}GPIO_line_t;

typedef struct {

	ADC_HandleTypeDef* Channel_ID;
	uint16_t currentValue;

}Analog_in_t;

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;

	GPIO_PinState current;
	GPIO_PinState previous;

	uint32_t lastChangeTick;
	bool level;
	bool risingEdge;
	bool fallingEdge;
}Digital_in_t;

typedef union {
	Analog_in_t analogInput;
	Digital_in_t digitalInput;

}Input_t;

typedef struct {
	TIM_HandleTypeDef* Timer_ID;
	uint16_t Channel_ID;

}Analog_out_t;

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
	GPIO_PinState current;
}Digital_out_t;


typedef union {
	Analog_out_t analogOutput;
	Digital_out_t digitalOutput;

}Output_t;

typedef struct {
	uint8_t enable;
	uint8_t inputId;
	uint8_t outputId;
	uint8_t function;
	uint8_t subFunction;
	uint16_t time1;
	uint16_t time2;
	uint16_t analogOutValue;
	uint16_t analogInValue;
	uint16_t nbCycles;

}Rule_t;
typedef struct{


	uint32_t currentTime1; //The time is compared with the set time
	uint32_t currentTime2; //The time is compared with the set time
	uint16_t currentCycleNb; //

}RuleStatus_t;

typedef struct{
	bool value;
	bool action;
	uint16_t analogValue;
}Virtual_input_buffer_t;


typedef struct{
	bool running;
	uint32_t TickStart;
	uint32_t currentTime1;

}Rule_timer_t;

typedef struct{

	Rule_timer_t timer1;

	Rule_timer_t timer2;

	bool toggle;

}Rule_current_state_t;




void Working_Mode_Init(void);
void Working_Mode_Start(flashFlag_t* programmingFlag);

#define FUN_NOP					0x00 //ALRIGHT, it works
#define FUN_ON					0X01 //ALRIGHT, it works
#define FUN_OFF					0X02 //ALRIGHT, it works
#define FUN_HOLD				0x03 //Alright, it works
#define FUN_DELAYON				0x04 //Alright, it works
#define FUN_DELAYOFF			0x05 //Alright, it works
#define FUN_TOGGLEONEDGE		0x06 //Alright, it's working correctly
#define FUN_PULS				0x07
#define FUN_PWMLEVEL			0x08
#define FUN_ANALOGTHRESHOLD		0x09
#define FUN_ANALOGTOPWM			0x0A



#define ON_OFF_DELAY_MAX_MS 	2550000UL
#define ON_OFF_DELAY_MIN_MS		10000UL
#define MAX_ANALOG_VALUE		4095
#define MAX_USER_VALUE			255

/*
 * 	FUN_OFF
 * 	DELAYOFF/ON
 *  FUN ANALOGTRESHOLD +-
 *  FUN_PULSING
 *  FUN_ANALOG_PWM
 *
 *
 *
 *
 *
 * */






#define QUANTITY_RULES				10 //12
#define NUMBER_PARAMETERS			10

#define NUMBER_OUTPUTS				12
#define NUMBER_INPUTS				12
#define NUMBER_DIGITAL_INPUTS 		9
#define NUMBER_ANALOG_INPUTS		3
#define NUMBER_DIGITAL_OUTPUTS		9
#define NUMBER_PWM_OUTPUTS			3




#define BLOCKSETTIME_MS		0x64 // 100ms
#define DEBOUNCE_MS			20

#define PROGCOMMAND			0x0F
#define RESETCOMMAND		0xFF
#define STHCOMMAND			0xAA

#endif /* WORKMODE_H_ */
