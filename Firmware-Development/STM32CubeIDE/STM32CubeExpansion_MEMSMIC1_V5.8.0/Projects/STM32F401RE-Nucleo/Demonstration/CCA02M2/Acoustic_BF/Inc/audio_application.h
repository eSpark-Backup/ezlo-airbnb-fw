/**
  ******************************************************************************
  * @file    audio_application.h
  * @author  SRA
  * @version v1.3.0
  * @date    17-Sep-2021
  * @brief   Header for audio_application.c module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_APPLICATION_H
#define __AUDIO_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"


/* Exported constants --------------------------------------------------------*/
/* Exported types --------------------------------------------------------*/
typedef union  {
	float value;
	uint8_t byte[4];
} float_b;
/* Exported macro ------------------------------------------------------------*/
#define ACOUSTIC_BF_MIC_DISTANCE 150

/* Microphone Characteristics*/
#define MP34DT06J_AOP 122.5
#define MP34DT06J_SNR 64
#define MP34DT06J_SENSITIVITY -26

/* Derived Values*/  // AN4426 Tutorial for MEMS microphones
#define MP34DT06J_RESIDUAL_NOISE (MP34DT06J_SENSITIVITY - MP34DT06J_SNR)  // -26 - 64 = -90 dbFS
#define MP34DT06J_EIN (90 - MP34DT06J_SNR)  // 94 - 64 = 30 dbSPL

/*Uncomment this define if you want to configure and start acquisition
independentrly from USB functionalities*/
#define DISABLE_USB_DRIVEN_ACQUISITION


/* Exported functions ------------------------------------------------------- */
void Audio_Libraries_Init(void);
void Error_Handler(void);
void AudioProcess(void);
void SW_IRQ_Tasks_Init(void);
void SW_Task1_Callback(void);
void SW_Task2_Callback(void);
void SW_Task1_Start(void);
void SW_Task2_Start(void);



#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_APPLICATION_H */


