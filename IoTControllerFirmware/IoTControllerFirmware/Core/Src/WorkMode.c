/*
 * WorkMode.c
 *
 *  Created on: 20 Jan 2026
 *      Author: pawel
 */
#include "WorkMode.h"
#include "gpio.h"
#include "usart.h"
#include "string.h"

/*------------------configuration----------------*/
uint8_t lineEnableArray[QUANTITY_RULES];
uint8_t gpioOutPinArray[QUANTITY_RULES];
uint8_t gpioInPinArray[QUANTITY_RULES];
uint8_t workFunctionArray[QUANTITY_RULES];
uint8_t subWorkFunctionArray[QUANTITY_RULES];
uint16_t time1Array[QUANTITY_RULES];
uint16_t time2Array[QUANTITY_RULES];
uint16_t analogOutValueArray[QUANTITY_RULES];
uint16_t analogInValueArray[QUANTITY_RULES];
uint16_t nbCycleArray[QUANTITY_RULES];
uint32_t parameterMainAddrs[QUANTITY_RULES];
/*----------------------------------------------------*/




GPIO_line_t GPIO_lines[QUANTITY_RULES];
Input_t Inputs[NUMBER_INPUTS] = {
		{.digitalInput.pin = GPIO_PIN_10, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_11, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_12, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_3, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		.digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_5, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_6, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_7, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_8, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{.digitalInput.pin = GPIO_PIN_9, .digitalInput.port = GPIOC, .digitalInput.previous = 0, .digitalInput.current = 0,
		 .digitalInput.lastChangeTick = 0, .digitalInput.level = false, .digitalInput.risingEdge = false, .digitalInput.fallingEdge = false
		},
		{
				.analogInput.Channel_ID = &hadc1, .analogInput.currentValue = 0
		},
		{
				.analogInput.Channel_ID = &hadc2, .analogInput.currentValue = 0
		},
		{
				.analogInput.Channel_ID = &hadc3, .analogInput.currentValue = 0
		},
};
Output_t Outputs[NUMBER_OUTPUTS] = {
		{.digitalOutput.pin = GPIO_PIN_2, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_5, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_6, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_8, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_9, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_10, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_11, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_12, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.digitalOutput.pin = GPIO_PIN_13, .digitalOutput.port = GPIOB, .digitalOutput.current = GPIO_PIN_RESET},
		{.analogOutput.Timer_ID = &htim3, .analogOutput.Channel_ID = TIM_CHANNEL_1},
		{.analogOutput.Timer_ID = &htim3, .analogOutput.Channel_ID = TIM_CHANNEL_2},
		{.analogOutput.Timer_ID = &htim3, .analogOutput.Channel_ID = TIM_CHANNEL_4}
};

Virtual_input_buffer_t VirtualInputsBuffer[QUANTITY_RULES] ={
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0},
		{.value = false, .action = true, .analogValue = 0}
//		{.value = false, .action = true, .analogValue = 0},
//		{.value = false, .action = true, .analogValue = 0}

};


Rule_current_state_t RuleCurrentState[QUANTITY_RULES];


Rule_t Rules[QUANTITY_RULES];

void Copy_Data_From_Flash(uint32_t* arrayAddrs, int size);
void Copy_Data_From_BasicConfig(void);
void Set_Config_Data_In_RAM(void);
void Input_Update(Input_t* in);
void Virtual_Input_buffer_update(int nbInput, Input_t* input);

void Execute_Function();
void Execute_NOP();
void Execute_ON(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule);
void Execute_OFF(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule);
void Execute_DelayOn(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState);
void Execute_DelayOff(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState);
void Execute_HOLD(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule);
void Execute_ToggleOnEdge(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule);

//I must get ideas about these functions!
void Execute_Puls(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState);
void Execute_PWMLevel(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState);
void Execute_AnalogTreshold(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule);
void Execute_AnalogToPWM(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule);

void Execute_Command(uint8_t* buffer, flashFlag_t* programmingFlag);
uint32_t TimeElapsed(uint32_t* timeStart);
uint32_t Map_Time_to_MS(uint8_t time);


bool Read_Command(uint8_t* command);



void Working_Mode_Init(void){

	uint64_t configState;
	uint64_t configMagicNumber;
	bool conditionConfigState = false;
	bool conditionConfigMagicNumber = false;

	configState = *(volatile uint64_t*) FLASH_CONFIG_PROGRAM_STATE_ADDR;
	configMagicNumber = *(volatile uint64_t*) FLASH_CONFIG_MAGIC_NUMBER_ADDR;

	conditionConfigState = (configState == CONFIG_PROGRAM_GOOD ? true:false);
	conditionConfigMagicNumber = (configMagicNumber == CONFIG_MAGIC_NUMBER ? true:false);
	//Można było by uzupełnić ten fragment, aby obsługiwał wybór użytkownika czy wgrać configurację bazowa
	if(conditionConfigState && conditionConfigMagicNumber){
		//uploaded configuration!
		Copy_Data_From_Flash(parameterMainAddrs, NUMBER_PARAMETERS); //Mozna by ulepszyć kopiowanie danych From FLASH!

	}else{
		//basic configuration!
		Copy_Data_From_BasicConfig();
	}

	Set_Config_Data_In_RAM();



}

void Working_Mode_Start(flashFlag_t* programmingFlag){
	uint8_t buffer;

	flashFlag_t* progFlag = programmingFlag;
	uint32_t inTickStart = HAL_GetTick();
	uint32_t tickStart = HAL_GetTick();
	HAL_TIM_PWM_Start(Outputs[9].analogOutput.Timer_ID, Outputs[9].analogOutput.Channel_ID);
	HAL_TIM_PWM_Start(Outputs[10].analogOutput.Timer_ID, Outputs[10].analogOutput.Channel_ID);
	HAL_TIM_PWM_Start(Outputs[11].analogOutput.Timer_ID, Outputs[11].analogOutput.Channel_ID);

	while(1){

		if(Read_Command(&buffer)){
			Execute_Command(&buffer, progFlag);

		}




		if(TimeElapsed(&inTickStart) >= 5){
			Input_t* in;
			HAL_ADC_Start(&hadc1);
			for(int i = 0; i<NUMBER_INPUTS; i++){
				in = &Inputs[i];
				if(i < NUMBER_DIGITAL_INPUTS){
					Input_Update(in);

				}else{
					HAL_ADC_Stop(in->analogInput.Channel_ID);
					HAL_ADCEx_Calibration_Start(in->analogInput.Channel_ID, ADC_SINGLE_ENDED);
					HAL_ADC_Start(in->analogInput.Channel_ID);
					HAL_ADC_PollForConversion(in->analogInput.Channel_ID, 1); //Here, I should change it!
					in->analogInput.currentValue = HAL_ADC_GetValue(in->analogInput.Channel_ID);
					HAL_ADC_Stop(in->analogInput.Channel_ID);

				}
				Virtual_Input_buffer_update(i, in);
				in->digitalInput.fallingEdge = false;
				in->digitalInput.risingEdge = false;
			}

			//Reset time
			inTickStart = HAL_GetTick();
		}




		if(TimeElapsed(&tickStart) >= BLOCKSETTIME_MS){

			//FUNCTION!
			Execute_Function();

			//COMMUNICATION WITH PC!
			//Send_Status_to_PC


			//Reset time!
			tickStart = HAL_GetTick();
		}

		//Receive COMMAND


		// Check RFP (REQUEST FOR PROGRAMMING)
		//SET FLAG
		//RESET!


	}






}

/*------------------------------------------------------*/
void Input_Update(Input_t* in){
	GPIO_PinState readValue = HAL_GPIO_ReadPin(in->digitalInput.port, in->digitalInput.pin);

	if (readValue != in->digitalInput.current) {
		if (HAL_GetTick() - in->digitalInput.lastChangeTick > DEBOUNCE_MS) {
			in->digitalInput.previous = in->digitalInput.current;
			in->digitalInput.current = readValue;
	        in->digitalInput.lastChangeTick = HAL_GetTick();

	        in->digitalInput.risingEdge  = (in->digitalInput.previous == 0 && in->digitalInput.current == 1);
	        in->digitalInput.fallingEdge = (in->digitalInput.previous == 1 && in->digitalInput.current == 0);

	    }


	}
	in->digitalInput.level = (in->digitalInput.current == 1 ? true:false);
}
/*
void Virtual_Input_buffer_update1(int nbInput, Input_t* input){

	uint8_t subfunction;
	bool value;
	Virtual_input_buffer_t* buffer;
	for(int i = 0; i<QUANTITY_RULES; i++){
		buffer = &VirtualInputsBuffer[i];
		if(Rules[i].inputId == nbInput){
			if(buffer->action == true){
				subfunction = Rules[i].subFunction;
				switch (subfunction){
				case 0:
					value = input->digitalInput.level;
					break;
				case 1:
					value = !input->digitalInput.level;
					break;
				case 2:
					value = input->digitalInput.risingEdge;
					break;
				case 3:
					value = input->digitalInput.fallingEdge;
					break;
				}

				if(value == true){
					buffer->value = value;
					buffer->action = false;
				}

			}else{
				continue;
			}

		}else{
			continue;

		}

	}



}
*/



void Virtual_Input_buffer_update(int inputNb, Input_t* input){

	bool value;
	uint8_t subfunction;
	Virtual_input_buffer_t* buffer;

	/* ------ 1 -------*/
	for (int i = 0; i < QUANTITY_RULES; i++){
		buffer = &VirtualInputsBuffer[i];
		subfunction = Rules[i].subFunction;

		/* ------ 2 -------*/
		if(Rules[i].inputId == inputNb){

			/* ------ 3 -------*/
			if(inputNb < NUMBER_DIGITAL_INPUTS){

				/* ------ 4 -------*/
				if(buffer->action == true){
					switch (subfunction){
					case 0:
						value = input->digitalInput.level;
						break;
					case 1:
						value = !input->digitalInput.level;
						break;
					case 2:
						value = input->digitalInput.risingEdge;
						break;
					case 3:
						value = input->digitalInput.fallingEdge;
						break;
					}

					if(value == true){
						buffer->value = value;
						buffer->action = false;
					}

				}else continue;
				/*------ 4 -------*/

			}else{
				buffer->analogValue = input->analogInput.currentValue;
			}
			/* ------ 3 -------*/

		}else continue;
		/* ------ 2 -------*/

	}
	/* ------ 1 -------*/


}


/*-------------------------------------------------------*/
bool Read_Command(uint8_t* command){

	if(HAL_UART_Receive(&huart2, command, sizeof(*command), 0) != HAL_OK){
		return false;
	}else{
		uint8_t messageBuffer = OK;
		HAL_UART_Transmit(&huart2, &messageBuffer, sizeof(messageBuffer), 0);

		return true;

	}

}
void Execute_Command(uint8_t* buffer, flashFlag_t* programmingFlag){
	uint8_t readCommand = *buffer;
	*buffer = 0x00;

	switch (readCommand){
	case PROG:
		SetFlag(programmingFlag, true); //Set the flag to the next iteration!
		NVIC_SystemReset(); //Reset the system

		// STH ELSE?
		break;
	case PING:
		uint8_t message = PONG;
		uint8_t* pMessage = &message;
		HAL_UART_Transmit(&huart2, pMessage, sizeof(*pMessage), 50);

		__NOP();
		__NOP();
		//Don't do anything!
		break;
	case SYSRESET:
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		HAL_Delay(2000);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		HAL_Delay(2000);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		HAL_Delay(2000);

		NVIC_SystemReset();
		break;
	default:
		__NOP();
		__NOP();
		//Don't do anything!
		break;
	}




}


/*-------------------------------------------------------*/
uint32_t TimeElapsed(uint32_t* timeStart){

	return HAL_GetTick() - (*timeStart);

}
uint32_t Map_Time_to_MS(uint8_t time){

	if(time == 0){
		return ON_OFF_DELAY_MIN_MS;
	}

	return ON_OFF_DELAY_MIN_MS + ( (uint32_t)( time-1 ) * ( ( ON_OFF_DELAY_MAX_MS - ON_OFF_DELAY_MIN_MS ) / 254 ) );

}

void Execute_Function(){
	Rule_t* oneRule;
	Virtual_input_buffer_t* input;
	Output_t* output;
	Rule_current_state_t* oneRuleState;


	for(int i = 0; i < QUANTITY_RULES; i++){
		oneRule = &Rules[i];
		input = &VirtualInputsBuffer[i];
		output = &Outputs[oneRule->outputId]; // To musze przesa prawdopodobnie do kadej funckji! (Odpowiada za wyjscia)
		oneRuleState = &RuleCurrentState[i];

		if( oneRule->enable == 0xFF ){
			uint8_t function;
			function = oneRule->function;

			switch (function){
			case FUN_NOP:
				Execute_NOP();
				break;
			case FUN_ON:
				Execute_ON(input, output, oneRule);
				break;
			case FUN_OFF:
				Execute_OFF(input, output, oneRule);
				break;
			case FUN_HOLD:
				Execute_HOLD(input, output, oneRule);
				break;
			case FUN_DELAYOFF:
				Execute_DelayOff(input, output, oneRule, oneRuleState);
				break;
			case FUN_DELAYON:
				Execute_DelayOn(input, output, oneRule, oneRuleState);
				break;
			case FUN_TOGGLEONEDGE:
				Execute_ToggleOnEdge(input, output, oneRule);
				break;
			case FUN_PULS:
				//Execute_Puls(input, output, oneRule);
				break;
			case FUN_PWMLEVEL:
				Execute_PWMLevel(input, output, oneRule, oneRuleState);
				break;
			case FUN_ANALOGTHRESHOLD:
				Execute_AnalogTreshold(input, output, oneRule);
				break;
			case FUN_ANALOGTOPWM:
				Execute_AnalogToPWM(input, output, oneRule);
				break;
			default:
				break;
			}
		}
		continue;
	}



}
/*--------------------------------------------------------*/
void Execute_NOP(){

	__asm__("nop");
	__asm__("nop");
	__asm__("nop");

}



void Execute_ON(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule){

	if(in->value == true && in->action == false){
		HAL_GPIO_WritePin(out->digitalOutput.port, out->digitalOutput.pin, GPIO_PIN_SET);

		in->value = false;
		in->action = true;

	}




}
void Execute_OFF(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule){

	if(in->value == true && in->action == false){
			HAL_GPIO_WritePin(out->digitalOutput.port, out->digitalOutput.pin, GPIO_PIN_RESET);

			in->value = false;
			in->action = true;

		}

}
void Execute_DelayOn(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState){

	// I should map the time1 from a particular rule onto the required delay!
	// 1 in time1 is equal 10s (here, of course)!!
	uint32_t delayMs = Map_Time_to_MS(rule->time1);
	uint32_t* now =  &ruleState->timer1.currentTime1;
	bool* runningTimer = &ruleState->timer1.running;



	if(in->value == true && in->action == false){

		if(*runningTimer != true){
			ruleState->timer1.TickStart = HAL_GetTick();
			*runningTimer = true;

		}

		*now = TimeElapsed(&ruleState->timer1.TickStart);

		if(*now >= delayMs){
			HAL_GPIO_WritePin(out->digitalOutput.port, out->digitalOutput.pin, GPIO_PIN_SET);

			in->value = false;
			in->action = true;
			*runningTimer = false;
		}

	}



}


void Execute_DelayOff(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState){

	// I should map the time1 from a particular rule onto the required delay!
	// 1 in time1 is equal 10s (here, of course)!!
	uint32_t delayMs = Map_Time_to_MS(rule->time1);
	uint32_t* now =  &ruleState->timer1.currentTime1;
	bool* runningTimer = &ruleState->timer1.running;



	if(in->value == true && in->action == false){

		if(*runningTimer != true){
			ruleState->timer1.TickStart = HAL_GetTick();
			*runningTimer = true;

		}

		*now = TimeElapsed(&ruleState->timer1.TickStart);

		if(*now >= delayMs){
			HAL_GPIO_WritePin(out->digitalOutput.port, out->digitalOutput.pin, GPIO_PIN_RESET);

			in->value = false;
			in->action = true;
			*runningTimer = false;
		}

	}

}





void Execute_HOLD(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule){
/*
	if(rule -> subFunction == 0x01){ //Positive Logic
		out->current = (in->level != 0?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(out->port, out->pin, out->current);

	}else{ //Negative Logic
		out->current = (in->level == 0?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(out->port, out->pin, out->current);

	}
*/
	if(in->value == true && in->action == false){

		HAL_GPIO_WritePin(out->digitalOutput.port, out->digitalOutput.pin, GPIO_PIN_SET);
		in->value = false;
		in->action = true;

	}else{
		HAL_GPIO_WritePin(out->digitalOutput.port, out->digitalOutput.pin, GPIO_PIN_RESET);
	}


}

void Execute_ToggleOnEdge(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule){
	/*
	if(rule->subFunction == 0x01){
		if(in->risingEdge){
		HAL_GPIO_TogglePin(out->port, out->pin);
		in->risingEdge = false;

		}
	}else{D
		if(in->fallingEdge){
		HAL_GPIO_TogglePin(out->port, out->pin);
		in->fallingEdge = false;

		}
	}
	*/

	if(in->value == true && in->action == false){

			HAL_GPIO_TogglePin(out->digitalOutput.port, out->digitalOutput.pin);
			in->value = false;
			in->action = true;

	}else{
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
	}

}
void Execute_Puls(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState){

	if(in->value == true && in->action == false){


			HAL_GPIO_TogglePin(out->digitalOutput.port, out->digitalOutput.pin);
			in->value = false;
			in->action = true;

	}else{

	}




}
//void Execute_Puls(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState);

/*void Execute_PWMLevel(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule){
	bool condition = rule->subFunction != 0x00 && rule->subFunction != 0x01;
	int duty = ((rule->analogOutValue*1000)/255);
	__HAL_TIM_SET_COMPARE(out->analogOutput.Timer_ID, out->analogOutput.Channel_ID, duty);
	if(condition){
		if(in->value == true && in->action == false){
			HAL_TIM_PWM_Start(out->analogOutput.Timer_ID, out->analogOutput.Channel_ID);
			in->value = false;
			in->action = true;

		}
		else{
			HAL_TIM_PWM_Stop(out->analogOutput.Timer_ID, out->analogOutput.Channel_ID);

		}

	}else{

	}




}
*/
void Execute_PWMLevel(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule, Rule_current_state_t* ruleState){
	int userDuty = ((rule->analogOutValue*1000)/255);
	int duty;
	if(rule->subFunction == 0x00 || rule->subFunction == 0x01){
		duty = ((in->value == true && in->action == false) ? userDuty: 0);
		__HAL_TIM_SET_COMPARE(out->analogOutput.Timer_ID, out->analogOutput.Channel_ID, duty);
		in->value = false;
		in->action = true;

	}else{
		if(in->value == true && in->action == false){
			ruleState->toggle = !ruleState->toggle;
			duty = ((ruleState->toggle) ? userDuty : 0);
			__HAL_TIM_SET_COMPARE(out->analogOutput.Timer_ID, out->analogOutput.Channel_ID, duty);
			in->value = false;
			in->action = true;
		}

	}
}



void Execute_AnalogTreshold(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule){
	uint8_t subfunction = rule->subFunction;
	GPIO_PinState outputState;
	float readVoltage = (in->analogValue*3.3f)/(4096.0f);
	float userVoltage = (rule->analogInValue*3.3f)/(256.0f);


	if(subfunction != 0){
		outputState = ( (readVoltage >= userVoltage) ? GPIO_PIN_SET:GPIO_PIN_RESET);
	}
	else{
		outputState = ( (readVoltage <= userVoltage) ? GPIO_PIN_SET:GPIO_PIN_RESET);
	}

	HAL_GPIO_WritePin(out->digitalOutput.port, out->digitalOutput.pin, outputState);

}


void Execute_AnalogToPWM(Virtual_input_buffer_t* in, Output_t* out, Rule_t* rule){


	float readValue = (in->analogValue)/(4096.0f);
	int duty = readValue * 1000;

	__HAL_TIM_SET_COMPARE(out->analogOutput.Timer_ID, out->analogOutput.Channel_ID, duty);


}




/*--------------------------------------------------------*/
void Flash_Address_Init(uint32_t* arrayAddrs, int size){
	for(int i = 0; i < size; i++){
		*(arrayAddrs+i) = Get_Line_First_Addr(i);
	}
}

void Copy_Data_From_Flash(uint32_t* arrayAddrs, int size){
	Flash_Address_Init(arrayAddrs, size);

	for(int i = 0; i<QUANTITY_RULES; i++){

		gpioOutPinArray[i] = *(volatile uint8_t*)(Get_Next_Addr_In_Line(i, &arrayAddrs[0]));
		gpioInPinArray[i] = *(volatile uint8_t*)(Get_Next_Addr_In_Line(i, &arrayAddrs[1]));
		lineEnableArray[i] = *(volatile uint8_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[2]));
		workFunctionArray[i] = *(volatile uint8_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[3]));
		subWorkFunctionArray[i] = *(volatile uint8_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[4]));
		time1Array[i] = *(volatile uint16_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[5]));
		time2Array[i] = *(volatile uint16_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[6]));
		analogInValueArray[i] = *(volatile uint16_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[7]));
		analogOutValueArray[i] = *(volatile uint16_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[8]));
		nbCycleArray[i] = *(volatile uint16_t*) (Get_Next_Addr_In_Line(i, &arrayAddrs[9]));
	}
}


void Copy_Data_From_BasicConfig(void){
	uint8_t lineEnableBasic[QUANTITY_RULES] =		{0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t gpioOutPinBasic[QUANTITY_RULES] = 		{0x00,0x09,0x0A,0x00,0x02,0x05,0x06,0x07,0x08,0x09};
	uint8_t gpioInPinBasic[QUANTITY_RULES] = 		{0x0A,0x00,0x0A,0x00,0x00,0x05,0x06,0x07,0x08,0x09};
	uint8_t workFunctionBasic[QUANTITY_RULES] = 	{0x09,0x08,0x0A,0x05,0x05,0x00,0x00,0x00,0x00,0x00};
	uint8_t subWorkFunctionBasic[QUANTITY_RULES] =  {0x01,0x00,0x00,0x00,0x00,0,0,0,0,0};
	uint16_t time1Basic[QUANTITY_RULES] = 			{0	 ,0   ,0   ,3   ,3   ,0,0,0,0,0};
	uint16_t time2Basic[QUANTITY_RULES] = 			{0   ,0   ,0   ,0   ,0   ,0,0,0,0,0};
	uint16_t analogOutValueBasic[QUANTITY_RULES] = 	{0   ,10 ,0   ,0   ,0   ,0,0,0,0,0};
	uint16_t analogInValueBasic[QUANTITY_RULES] = 	{127 ,0   ,0   ,0   ,0   ,0,0,0,0,0};
	uint16_t nbCycleBasic[QUANTITY_RULES] = 		{0   ,0   ,0   ,0   ,0   ,0,0,0,0,0};


	memcpy(lineEnableArray, lineEnableBasic, sizeof(lineEnableBasic));
	memcpy(gpioOutPinArray, gpioOutPinBasic, sizeof(gpioOutPinBasic));
	memcpy(gpioInPinArray, gpioInPinBasic, sizeof(gpioInPinBasic));
	memcpy(workFunctionArray, workFunctionBasic, sizeof(workFunctionBasic));
	memcpy(subWorkFunctionArray, subWorkFunctionBasic, sizeof(subWorkFunctionBasic));
	memcpy(time1Array, time1Basic, sizeof(time1Basic));
	memcpy(time2Array, time2Basic, sizeof(time2Basic));
	memcpy(analogOutValueArray, analogOutValueBasic, sizeof(analogOutValueBasic));
	memcpy(analogInValueArray, analogInValueBasic ,sizeof(analogInValueBasic));
	memcpy(nbCycleArray, nbCycleBasic, sizeof(nbCycleBasic));


}


void Set_Config_Data_In_RAM(void){
	Rule_t* rule;
	for(int i = 0; i< QUANTITY_RULES; i++){
		rule = &Rules[i];
		rule->enable = lineEnableArray[i];
		rule->outputId = gpioOutPinArray[i];
		rule->inputId = gpioInPinArray[i];
		rule->function = workFunctionArray[i];
		rule->subFunction = subWorkFunctionArray[i];
		rule->time1 = time1Array[i];
		rule->time2 = time2Array[i];
		rule->analogOutValue = analogOutValueArray[i];
		rule->analogInValue = analogInValueArray[i];
		rule->nbCycles = nbCycleArray[i];

	}

}





