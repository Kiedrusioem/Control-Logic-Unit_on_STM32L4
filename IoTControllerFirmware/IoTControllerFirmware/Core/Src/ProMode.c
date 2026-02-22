/*
 * ProMode.c
 *
 *  Created on: 6 Jan 2026
 *      Author: pawel
 */
#include "ProMode.h"
#include "rtc.h"
#include "usart.h"
#include "string.h"
//This type of variable will be in another file!
typedef enum{
	UART_TRANSMISSION_OK,
	UART_Tx_NOK,
	UART_Rx_NOK

}UART_TransmissionErrors_t;

typedef enum{
	VALID_OK,
	VALID_CHECKSUM_NOK,
	VALID_RANGE_NOK,


}Validation_Errors_t;

typedef enum{
	FLASH_OK,
	FLASH_ERASE_NOK,
	FLASH_WRITE_NOK,

}Flash_Errors_t;

struct Errors_Programming_Mode_s{
			UART_TransmissionErrors_t UError;
			Validation_Errors_t VError;
			Flash_Errors_t FError;
};

uint16_t lineEnable[NUMBERCONFIGPARAMETER];
uint16_t gpioOutPin[NUMBERCONFIGPARAMETER];
uint16_t gpioInPin[NUMBERCONFIGPARAMETER];
uint16_t workFunction[NUMBERCONFIGPARAMETER];
uint16_t subWorkFunction[NUMBERCONFIGPARAMETER];
uint16_t time1[NUMBERCONFIGPARAMETER];
uint16_t time2[NUMBERCONFIGPARAMETER];
uint16_t analogOutValue[NUMBERCONFIGPARAMETER];
uint16_t analogInValue[NUMBERCONFIGPARAMETER];
uint16_t nbCycle[NUMBERCONFIGPARAMETER];

void Write_data_in_BKUPR(uint32_t data, uint8_t backupRegister );
void Prepare_AttemptsBKUP(int maxAttempts, uint8_t backupRegister);
void Copy_recevied_data(uint16_t element[10][11], int r);
UART_TransmissionErrors_t Uart_recevie_config(uint16_t *element, int r, int c);
Flash_Errors_t Save_Config_In_Flash(int rSize,int cSize);
Flash_Errors_t Write_One_Array_In_Flash(int index, int aSize, uint16_t* arr);
Validation_Errors_t Validate_received_config(uint16_t* element, int r, int c);
Validation_Errors_t Validate_parameters_range(uint16_t element, int index);
//-------------------------------------------------------------------//
// Main function in this file!
void Programming_Mode(flashFlag_t* programmingFlag){

	/*------------------------------------------*/
	/* Programming_mode private type definition */
	/* CODE BEGIN                               */

	/* CODE END*/
	/*------------------------------------------*/
	/*------------------------------------------*/
	/* Programming_mode private variables       */
	/* CODE BEGIN                               */
	//pictureOfFlashMemory SortedConfigData[NUMBERKINDOFCONFIGPARAMETER];
	uint16_t rx_data_buffer[NUMBERKINDOFCONFIGPARAMETER][NUMBERRECEIVEDCONFIGPARAMETER];
	uint8_t rx_message = OK;

	struct Errors_Programming_Mode_s ErrorsProgrammingMode = {.FError = FLASH_OK,
															  .UError = UART_TRANSMISSION_OK,
															  .VError = VALID_OK
															 };

	/* CODE END                                 */
	/*------------------------------------------*/
	/*------------------------------------------*/
	/* CODE BEGIN                               */

	//Getting data from the computer!
	ErrorsProgrammingMode.UError = Uart_recevie_config((uint16_t*)rx_data_buffer, NUMBERKINDOFCONFIGPARAMETER, NUMBERRECEIVEDCONFIGPARAMETER);
	if(ErrorsProgrammingMode.UError != UART_TRANSMISSION_OK){
		goto exit;
	}

	//Validation! (Perhaps)
	ErrorsProgrammingMode.VError = Validate_received_config((uint16_t*)rx_data_buffer, NUMBERKINDOFCONFIGPARAMETER, NUMBERRECEIVEDCONFIGPARAMETER);
	if(ErrorsProgrammingMode.VError != VALID_OK ){
		goto exit;
	}

	//Putting Data into the struct object
	Copy_recevied_data(rx_data_buffer, NUMBERKINDOFCONFIGPARAMETER);

	//Assigning the addresses!
	//Assign_flash_addresses(NUMBERKINDOFCONFIGPARAMETER, &SortedConfigData);

	//Copying segregated data to flash!
	//ErrorsProgrammingMode.FError = Save_Config_In_Flash(NUMBERKINDOFCONFIGPARAMETER,NUMBERCONFIGPARAMETER);
	//if(ErrorsProgrammingMode.FError != FLASH_OK){
		//goto exit;
	//}

	//Removing the programming flag
	SetFlag(programmingFlag, false);

	//Reset the backup register!
	Write_data_in_BKUPR(0, RTC_BKP_DR0);

	uint8_t message[] = "All of it is okay!\r\n";
	HAL_UART_Transmit(&huart2, &rx_message, sizeof(rx_message), 25);
	HAL_UART_Transmit(&huart2, message, sizeof(message), 25);
	//System reset, using software reset
	HAL_Delay(20);
	NVIC_SystemReset();

	/*------------------------------------------*/
	/*Error handler block                       */
	/*CODE BEGIN*/
	exit:

	HAL_FLASH_Lock();

	// The Backup block!

	Prepare_AttemptsBKUP(MAXPROGATTEMPTS, RTC_BKP_DR0);
	// Reading data from BKUP
	uint32_t attemptsCounter = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);


	if((attemptsCounter >= 3)){
		//Return to the working_mode

		//Set the programmingFlag as false
		SetFlag(programmingFlag, false);

		//Clear backup register!
		Write_data_in_BKUPR(0, RTC_BKP_DR0);

		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		//Message
		rx_message = ERROR2;
		uint8_t message[] = "The IoT controller starts with the basic configuration\r\n";
		HAL_UART_Transmit(&huart2, &rx_message, sizeof(rx_message), 25);
		HAL_UART_Transmit(&huart2, message, sizeof(message), 100);

	}else{
		rx_message = ERROR;
		if(ErrorsProgrammingMode.VError != VALID_OK){
			uint8_t message[] = "The error with the validation occurred!\r\n";
			HAL_UART_Transmit(&huart2, &rx_message, sizeof(rx_message), 25);
			HAL_UART_Transmit(&huart2, message, sizeof(message), 100);

		}else if(ErrorsProgrammingMode.UError != UART_TRANSMISSION_OK){
			uint8_t message[] = "The error with the UART occurred!\r\n";
			HAL_UART_Transmit(&huart2, &rx_message, sizeof(rx_message), 25);
			HAL_UART_Transmit(&huart2, message, sizeof(message), 100);

		}else if(ErrorsProgrammingMode.FError != FLASH_OK){
			uint8_t message[] = "The error with the FALSH occurred!\r\n";
			HAL_UART_Transmit(&huart2, &rx_message, sizeof(rx_message), 25);
			HAL_UART_Transmit(&huart2, message, sizeof(message), 100);

		}
	}

	//Reset the system by using a software reset
	HAL_Delay(25);
	NVIC_SystemReset();


	/*CODE END                                  */
	/*------------------------------------------*/

}
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

void Write_data_in_BKUPR(uint32_t data, uint8_t backupRegister ){
	HAL_PWR_EnableBkUpAccess();
	HAL_RTCEx_BKUPWrite(&hrtc, backupRegister, data);
	HAL_PWR_DisableBkUpAccess();

}

void Prepare_AttemptsBKUP(int maxAttempts, uint8_t backupRegister){
	uint32_t attemptsCounter = 0;
	//Read a current value that is in BKUP
	attemptsCounter = HAL_RTCEx_BKUPRead(&hrtc, backupRegister);

	if( attemptsCounter == 0 || attemptsCounter > maxAttempts ) attemptsCounter = 1;
	else attemptsCounter++;

	//Put new data into the 0th backup register
	Write_data_in_BKUPR(attemptsCounter, backupRegister);

}





//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
UART_TransmissionErrors_t Uart_recevie_config(uint16_t *element, int r, int c){
	uint8_t StartSending = START;
	uint8_t stop = STOP;
	uint8_t next = NEXT;
	uint8_t buffer;
	bool exitloop = false;
	UART_TransmissionErrors_t status = UART_TRANSMISSION_OK;


	HAL_StatusTypeDef stateSentMessage = HAL_UART_Transmit(&huart2, &StartSending , sizeof(StartSending), 30);



	if(stateSentMessage == HAL_OK)
	{

		//I could put the IF-command into this place! This
		for(int NbCycles = 0; NbCycles < r; NbCycles++){
			for(int j = 0; j < c; j++){
				if(HAL_UART_Receive(&huart2, &buffer, sizeof(buffer), 1000) != HAL_OK){
					HAL_UART_Transmit(&huart2, &stop, sizeof(stop), 25);
					status = UART_Rx_NOK;
					exitloop = true;
					break;

				}
				*(element + NbCycles * c + j) = (uint16_t)buffer;
				if((NbCycles == r-1) && (j==c-1)){
					if(HAL_UART_Transmit(&huart2, &stop, sizeof(stop), 25) != HAL_OK){
						status = UART_Tx_NOK;
						exitloop = true;
					}

				}else{
					if(HAL_UART_Transmit(&huart2, &next, sizeof(next), 25) != HAL_OK){
						HAL_UART_Transmit(&huart2, &stop, sizeof(stop), 25);
						status = UART_Tx_NOK;
						exitloop = true;
						break;
					}
				}
			}
			if(exitloop != false){
				break;
			}

		}
	}else{
		status = UART_Tx_NOK;
		}

	return status;

}

//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
Validation_Errors_t Validate_received_config(uint16_t *element, int r, int c){
	int row = r;
	int column = c;
	int gotChecksum;
	int calculatedChecksum;

	//We are iterating through rows
	//Check the sum! -> Every row!
	for(int i = 0; i<row; i++){
		calculatedChecksum = 0;
		gotChecksum = *(element + i * column + 10); //10-th element is a check-sum!

		for(int j = 0; j < (column-1); j++){
			uint16_t value = *(element + i * column + j);
			calculatedChecksum += value;

			if(Validate_parameters_range(value, i) != VALID_OK){
				return VALID_RANGE_NOK;

			}
		}

		if(gotChecksum != calculatedChecksum){
			return VALID_CHECKSUM_NOK;
		}
	}

	return VALID_OK;
}
//-----------------------------------------------------------------------------------------------//
Validation_Errors_t Validate_parameters_range(uint16_t element, int index){
	Validation_Errors_t result;
	/* The range for each element should be corrected!
	 *
	 *
	 *
	 *
	 * */
	switch (index) {
	case 0:
		result = (element >= 0 && element <=1)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 1:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 2:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 3:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 4:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 5:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 6:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 7:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 8:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	case 9:
		result = (element >= 0 && element <=5)?VALID_OK:VALID_RANGE_NOK;
		break;
	default:
		result =  VALID_RANGE_NOK;
		break;

	}

	return result;

}

//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

void Copy_recevied_data(uint16_t element[10][11], int r){
	memcpy(lineEnable, element[0], sizeof(lineEnable));
	memcpy(gpioOutPin, element[1], sizeof(gpioOutPin));
	memcpy(gpioInPin, element[2], sizeof(gpioInPin));
	memcpy(workFunction, element[3], sizeof(workFunction));
	memcpy(subWorkFunction, element[4], sizeof(subWorkFunction));
	memcpy(time1, element[5], sizeof(time1));
	memcpy(time2, element[6], sizeof(time2));
	memcpy(analogOutValue, element[7], sizeof(analogOutValue));
	memcpy(analogInValue, element[8], sizeof(analogInValue));
	memcpy(nbCycle, element[9], sizeof(nbCycle));

}
//--------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//
Flash_Errors_t Save_Config_In_Flash(int rSize, int cSize){

	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;

	HAL_StatusTypeDef status;
	uint8_t iterator = 0;
	Flash_Errors_t result = FLASH_OK;




	/* Unlock the flash to enable the flash control register access*/
	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = FLASH_CONFIG_BANK;
	EraseInitStruct.Page = FLASH_CONFIG_PAGE;
	EraseInitStruct.NbPages = 1;

	//THE ERASING BLOCK
	do{
		iterator++;
		status = HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
		if(status == HAL_OK){
			break;
		}
	}while(iterator < 4); //4 - NUMBER OF ATTEMPTS!

	if(status != HAL_OK){
		result = FLASH_ERASE_NOK;
	}

	//THE SAVING BLOCK

	//Change it!

	if(result == FLASH_OK){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_CONFIG_PROGRAM_STATE_ADDR, CONFIG_PROGRAM_BROKEN);
		uint16_t* arrayPointer;

		for(int i = 0; i<rSize; i++){
			int choice = i;
			switch (choice){
			case 0:
				arrayPointer = lineEnable;
				break;
			case 1:
				arrayPointer = gpioOutPin;
				break;
			case 2:
				arrayPointer = gpioInPin;
				break;
			case 3:
				arrayPointer = workFunction;
				break;
			case 4:
				arrayPointer = subWorkFunction;
				break;
			case 5:
				arrayPointer = time1;
				break;
			case 6:
				arrayPointer = time2;
				break;
			case 7:
				arrayPointer = analogOutValue;
				break;
			case 8:
				arrayPointer = analogInValue;
				break;
			case 9:
				arrayPointer = nbCycle;
				break;
			}
			result = Write_One_Array_In_Flash(choice, cSize, arrayPointer);
		}

		if(result == FLASH_OK){
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_CONFIG_MAGIC_NUMBER_ADDR, CONFIG_MAGIC_NUMBER)==HAL_OK){
				if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_CONFIG_PROGRAM_STATE_ADDR, CONFIG_PROGRAM_GOOD) != HAL_OK){
					result = FLASH_WRITE_NOK;
				}
			}else{
				result = FLASH_WRITE_NOK;
				}

		}
	}

	//HERE, THE BAD RESULTS FLOW!

	HAL_FLASH_Lock();
	return result;

}

Flash_Errors_t Write_One_Array_In_Flash(int index, int aSize, uint16_t* arr){
	uint32_t firstAddress = Get_Line_First_Addr(index);
	uint32_t address = 0;
	for(int i = 0; i < aSize; i++){
		address = Get_Next_Addr_In_Line(i, &firstAddress);
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, arr[i]) != HAL_OK){
			return FLASH_WRITE_NOK;
		}
	}
	return FLASH_OK;
}






