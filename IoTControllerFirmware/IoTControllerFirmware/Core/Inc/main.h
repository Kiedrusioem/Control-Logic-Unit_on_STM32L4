/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

//-------------------------------------------------------------------------------//
// The type of variables which give some information about the IoT controller's work
typedef enum{
	WORKING = 0,
	PROGRAMMING = !WORKING

}system_state_t;

//--------------------------------------------------------------------------------//
// Making a bool type.
/*
typedef enum{
	FALSE = 0,
	TRUE = !FALSE

}bool;
*/
//--------------------------------------------------------------------------------//

typedef struct{
		//Place
		uint32_t magicNumberAddress;
		uint32_t flagAddress;
		uint32_t bank;
		uint32_t page;
		uint32_t Nbpages;
		//
}flashFlag_t;


//---------------------------------------------------//
// Temporary place for the struct object!
typedef struct {
	uint32_t LineEnableAddr;
	uint16_t LineEnable;

	uint32_t OutIdAddr;
	uint16_t OutId;

	uint32_t InIdAddr;
	uint16_t InId;

	uint32_t FunctionAddr;
	uint16_t Function;

	uint32_t SubfunctionAddr;
	uint16_t Subfunction;

	uint32_t Time1Addr;
	uint16_t Time1;

	uint32_t Time2Addr;
	uint16_t Time2;

	uint32_t AnalogInValueAddr;
	uint16_t AnalogInValue;

	uint32_t AnalogOutValueAddr;
	uint16_t AnalogOutValue;

	uint32_t NbCyclesAddr;
	uint16_t NbCycles;
}pictureOfFlashMemory;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//--------------------------------------------------//
uint32_t Get_Line_First_Addr(int NbLine);

//--------------------------------------------------//
uint32_t Get_Next_Addr_In_Line(int NbElement, const uint32_t* LineAddr);

//-------------------------------------------------//

uint32_t SetFlag(flashFlag_t *programming_flag, bool state);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define GPIO_ADC_Input_9_Pin GPIO_PIN_0
#define GPIO_ADC_Input_9_GPIO_Port GPIOC
#define GPIO_ADC_Input_10_Pin GPIO_PIN_1
#define GPIO_ADC_Input_10_GPIO_Port GPIOC
#define GPIO_ADC_Input_11_Pin GPIO_PIN_2
#define GPIO_ADC_Input_11_GPIO_Port GPIOC
#define GPIO_Input_3_Pin GPIO_PIN_3
#define GPIO_Input_3_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define GPIO_PWM_Output_9_Pin GPIO_PIN_6
#define GPIO_PWM_Output_9_GPIO_Port GPIOA
#define GPIO_PWM_Output_10_Pin GPIO_PIN_7
#define GPIO_PWM_Output_10_GPIO_Port GPIOA
#define GPIO_PWM_Output_11_Pin GPIO_PIN_1
#define GPIO_PWM_Output_11_GPIO_Port GPIOB
#define GPIO_Output_0_Pin GPIO_PIN_2
#define GPIO_Output_0_GPIO_Port GPIOB
#define GPIO_Output_5_Pin GPIO_PIN_10
#define GPIO_Output_5_GPIO_Port GPIOB
#define GPIO_Output_6_Pin GPIO_PIN_11
#define GPIO_Output_6_GPIO_Port GPIOB
#define GPIO_Output_7_Pin GPIO_PIN_12
#define GPIO_Output_7_GPIO_Port GPIOB
#define GPIO_Output_8_Pin GPIO_PIN_13
#define GPIO_Output_8_GPIO_Port GPIOB
#define GPIO_Input_5_Pin GPIO_PIN_6
#define GPIO_Input_5_GPIO_Port GPIOC
#define GPIO_Input_6_Pin GPIO_PIN_7
#define GPIO_Input_6_GPIO_Port GPIOC
#define GPIO_Input_7_Pin GPIO_PIN_8
#define GPIO_Input_7_GPIO_Port GPIOC
#define GPIO_Input_8_Pin GPIO_PIN_9
#define GPIO_Input_8_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define GPIO_Input_0_Pin GPIO_PIN_10
#define GPIO_Input_0_GPIO_Port GPIOC
#define GPIO_Input_1_Pin GPIO_PIN_11
#define GPIO_Input_1_GPIO_Port GPIOC
#define GPIO_Input_2_Pin GPIO_PIN_12
#define GPIO_Input_2_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO_Output_1_Pin GPIO_PIN_5
#define GPIO_Output_1_GPIO_Port GPIOB
#define GPIO_Output_2_Pin GPIO_PIN_6
#define GPIO_Output_2_GPIO_Port GPIOB
#define GPIO_Output_3_Pin GPIO_PIN_8
#define GPIO_Output_3_GPIO_Port GPIOB
#define GPIO_Output_4_Pin GPIO_PIN_9
#define GPIO_Output_4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

//Flash memory addresses


//I could calculate the page number before program, because The compiler can perform a macro!

#define FLASH_FLAG_PAGE_ADDR 				0x080E1000UL //450-th page
#define FLASH_FLAG_PAGE_SIZE 				0x00000800UL //The page size!
#define FLASH_CONFIG_STARTPAGE_ADDR 		0x080DC000//440-th page; 184th page
#define FLASH_CONFIG_ENDPAGE_ADDR 			0x080DB800 //439-th page 183th page
#define FLASH_CONFIG_DATA_FIRST_ADDR 		0x080DC010
#define FLASH_CONFIG_MAGIC_NUMBER_ADDR		FLASH_CONFIG_STARTPAGE_ADDR
#define FLASH_CONFIG_PROGRAM_STATE_ADDR		FLASH_CONFIG_MAGIC_NUMBER_ADDR+sizeof(uint64_t)
#define FLASH_CONFIG_PAGE_SIZE 				0x00000800UL

#define FLASH_CONFIG_BANK 					FLASH_BANK_2
#define FLASH_CONFIG_PAGE 					184



/*
 * HAL_Erase function sets each bit on 1
 * HAL_Program can only sets each bit on 0
 *
 * W takim razie, teraz możemy ustawić FLAGE ON->OFF, bez wycinania.
 * Jeśli OFF->ON, We must use a HAL_ERASE
 *
 * Better, in order to set the flag on the ON-mode, without erasing!
 *
 * */
#define PROGRAMMING_FLAG_IS_ON 				0x0000000000000000UL
#define PROGRAMMING_FLAG_IS_OFF 			0xFFFFFFFFFFFFFFFFUL
#define PROGRAMMING_FLAG_MAGIC_NUMBER 		0x0AEBFFFFFFFFFFFFUL

#define CONFIG_MAGIC_NUMBER 				0x1B42FFFFFFFFFFFFUL
#define CONFIG_PROGRAM_EMPTY				0xFFFFFFFFFFFFFFFFUL
#define CONFIG_PROGRAM_BROKEN 				0xFFFFFFFFFFFFFF00UL
#define CONFIG_PROGRAM_GOOD 				0x0000000000000000UL


/*
 *
 *
 * COMMANDS
 *
 *
 * */
#define SYSRESET							'S'
#define PROG								'N'
#define PING								'P'
#define PONG								'R'
#define OK 									'K'
#define NEXT								'E'
#define STOP								'T'
#define START								'A'
#define ERROR								'F'
#define ERROR2								'X'
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
