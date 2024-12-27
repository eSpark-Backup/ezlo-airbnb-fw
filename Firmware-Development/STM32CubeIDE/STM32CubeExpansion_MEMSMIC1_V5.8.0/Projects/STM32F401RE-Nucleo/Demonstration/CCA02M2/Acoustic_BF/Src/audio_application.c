/**
  ******************************************************************************
  * @file    audio_application.c
  * @author  SRA
  * @version v1.3.0
  * @date    17-Sep-2021
  * @brief   Beam forming example.
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

/* Includes ------------------------------------------------------------------*/
#include "audio_application.h"
#include "acoustic_bf.h"
#include "arm_math.h"
#include "usart.h"

#include "smr_glo.h"  // Ensure this header is included for SMR module definitions

// Define the parameters and buffers as static or global
static smr_static_param_t smr_static_params;
static smr_dynamic_param_t smr_dynamic_params;
static uint8_t* persistent_mem;
static uint8_t* scratch_mem;
static buffer_t input_buffer, output_buffer;

/** @addtogroup X_CUBE_MEMSMIC1_Applications
  * @{
  */

/** @addtogroup ACOUSTIC_BF_Example
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/** @defgroup AUDIO_APPLICATION_Exported_Variables
  * @{
  */
uint16_t PDM_Buffer[((((AUDIO_IN_CHANNELS * AUDIO_IN_SAMPLING_FREQUENCY) / 1000) * MAX_DECIMATION_FACTOR) / 16)* N_MS ];
uint16_t PCM_Buffer[((AUDIO_IN_CHANNELS * AUDIO_IN_SAMPLING_FREQUENCY) / 1000)  * N_MS ];
#define PCM_BUFFER_SIZE (sizeof(PCM_Buffer)/ sizeof(PCM_Buffer[0]))
/**
  * @}
  */

/** @defgroup AUDIO_APPLICATION_Private_Variables
  * @{
  */
/* Private variables ---------------------------------------------------------*/

/*Handler and Config structure for BeamForming*/
AcousticBF_Handler_t libBeamforming_Handler_Instance;
AcousticBF_Config_t lib_Beamforming_Config_Instance;
int16_t USBOUT[16 * 4];
/**
  * @}
  */

/** @defgroup AUDIO_APPLICATION_Exported_Function
  * @{
  */

/**
  * @brief  Half Transfer user callback, called by BSP functions.
  * @param  None
  * @retval None
  */
void CCA02M2_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  UNUSED(Instance);
  AudioProcess();
}

/**
  * @brief  Transfer Complete user callback, called by BSP functions.
  * @param  None
  * @retval None
  */
void CCA02M2_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  UNUSED(Instance);
  AudioProcess();
}

/**
  * @brief  User function that is called when 1 ms of PDM data is available.
  *       In this application only PDM to PCM conversion and USB streaming
  *                  is performed.
  *       User can add his own code here to perform some DSP or audio analysis.
  *                In this case audio libraries are used
  * @param  none
  * @retval None
  */
void AudioProcess(void)
{
	if (CCA02M2_AUDIO_IN_PDMToPCM(CCA02M2_AUDIO_INSTANCE, (uint16_t *)PDM_Buffer, PCM_Buffer) != BSP_ERROR_NONE)
	{
		Error_Handler();
	}

	input_buffer.data_ptr = PCM_Buffer;

	if (smr_process(&input_buffer, &input_buffer, persistent_mem) != SMR_ERROR_NONE) {
		Error_Handler();
	}

	if (smr_getConfig(&smr_dynamic_params, persistent_mem) != SMR_ERROR_NONE) {
		Error_Handler();
	}

	// Extract and convert the sound level from Q29.2 format
	float dbFS = smr_dynamic_params.mean_level_left;
	dbFS = dbFS / 4.0f; // Conversion from Q29.2
	float_b dBSPL;
	dBSPL.value = MP34DT06J_AOP + dbFS - MP34DT06J_EIN; /* offset subtracted workaround */

	 static uint32_t count;
	 if(!(++count % 100))
	 {
//		HAL_UART_Transmit(&huart2, &dBSPL.value, sizeof(dBSPL.value), 1000);
		uint8_t packet[] = {0xAA, 0x04, dBSPL.byte[0], dBSPL.byte[1], dBSPL.byte[2], dBSPL.byte[3], 0};
		uint8_t checksum = packet[0] ^ packet[1] ^ packet[2] ^ packet[3] ^ packet[4] ^ packet[5];
		packet[6] = checksum;
		HAL_UART_Transmit(&huart2, packet, sizeof(packet), 1000);
	 }

//	  Send_Audio_to_USB((int16_t*) PCM_Buffer, (AUDIO_IN_SAMPLING_FREQUENCY / 1000)*AUDIO_IN_CHANNELS * N_MS);
}

void SMR_Init(void)
{
    persistent_mem = malloc(smr_persistent_mem_size); /* smr_persistent_mem_size  0x188 */
    scratch_mem = malloc(smr_scratch_mem_size);       /* smr_scratch_mem_size  0xF04 */

    if (smr_reset(persistent_mem, scratch_mem) != SMR_ERROR_NONE) {
        Error_Handler();
    }

    smr_static_params.sampling_rate = AUDIO_IN_SAMPLING_FREQUENCY;
    if (smr_setParam(&smr_static_params, persistent_mem) != SMR_ERROR_NONE) {
        Error_Handler();
    }

    smr_dynamic_params.enable = 1;
    smr_dynamic_params.averaging_time = 100;
    smr_dynamic_params.filter_type = SMR_PREFILTER_NONE;

    if (smr_setConfig(&smr_dynamic_params, persistent_mem) != SMR_ERROR_NONE) {
        Error_Handler();
    }

    input_buffer.nb_channels = AUDIO_IN_CHANNELS;
    input_buffer.nb_bytes_per_Sample = 2;
    input_buffer.buffer_size = AUDIO_IN_BUFFER_SIZE / AUDIO_IN_CHANNELS;
    input_buffer.mode = INTERLEAVED;

    output_buffer = input_buffer;
}

/**
  * @brief  Initialize the audio libraries adopted
  * @param  None
  * @retval None
  */
void Audio_Libraries_Init(void)
{
  __IO uint32_t error_value = 0;
  /* Enable CRC peripheral to unlock the library */
  __CRC_CLK_ENABLE();
  /*Setup Beamforming static parameters*/
  error_value = 0;
  libBeamforming_Handler_Instance.algorithm_type_init = ACOUSTIC_BF_TYPE_STRONG;
  libBeamforming_Handler_Instance.ref_mic_enable = ACOUSTIC_BF_REF_ENABLE;
  libBeamforming_Handler_Instance.ptr_out_channels = 2;
  libBeamforming_Handler_Instance.data_format = ACOUSTIC_BF_DATA_FORMAT_PDM;
  libBeamforming_Handler_Instance.sampling_frequency = PDM_FREQ_16K;
  libBeamforming_Handler_Instance.ptr_M1_channels = 2;
  libBeamforming_Handler_Instance.ptr_M2_channels = 2;
  libBeamforming_Handler_Instance.delay_enable = 1;
  (void)AcousticBF_getMemorySize(&libBeamforming_Handler_Instance);
  libBeamforming_Handler_Instance.pInternalMemory =
    (uint32_t *)malloc(libBeamforming_Handler_Instance.internal_memory_size);
  error_value = AcousticBF_Init(&libBeamforming_Handler_Instance);

  /*Error Management*/
  if (error_value != 0U)
  {
    Error_Handler();
  }

  /*Malloc Failure*/
  if (libBeamforming_Handler_Instance.pInternalMemory == NULL)
  {
    Error_Handler();
  }

  /*Setup Beamforming dynamic parameters*/
  lib_Beamforming_Config_Instance.algorithm_type = ACOUSTIC_BF_TYPE_STRONG;
  lib_Beamforming_Config_Instance.M2_gain = 0.0f;
  lib_Beamforming_Config_Instance.mic_distance = ACOUSTIC_BF_MIC_DISTANCE;
  lib_Beamforming_Config_Instance.volume = 24;
  error_value = AcousticBF_setConfig(&libBeamforming_Handler_Instance, &lib_Beamforming_Config_Instance);

  /*Error Management*/
  if (error_value != 0U)
  {
    Error_Handler();
  }

  SMR_Init();
//  meterSetup();
}

/**
  * @brief  Initializes two SW interrupt with different priorities
  * @param  None
  * @retval None
  */
void SW_IRQ_Tasks_Init(void)
{
  HAL_NVIC_SetPriority((IRQn_Type)EXTI1_IRQn, 0x0D, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI1_IRQn);

  HAL_NVIC_SetPriority((IRQn_Type)EXTI2_IRQn, 0x0E, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI2_IRQn);
}

/**
  * @brief  Highest priority interrupt handler routine
  * @param  None
  * @retval None
  */
void SW_Task1_Callback(void)
{
  (void)AcousticBF_SecondStep(&libBeamforming_Handler_Instance);
}

/**
  * @brief Lower priority interrupt handler routine
  * @param  None
  * @retval None
  */
void SW_Task2_Callback(void)
{

}

/**
  * @brief Throws Highest priority interrupt
  * @param  None
  * @retval None
  */
void SW_Task1_Start(void)
{
  HAL_NVIC_SetPendingIRQ(EXTI1_IRQn);
}

/**
  * @brief Throws Lower priority interrupt
  * @param  None
  * @retval None
  */
void SW_Task2_Start(void)
{
  HAL_NVIC_SetPendingIRQ(EXTI2_IRQn);
}

void Error_Handler(void)
{
  while (1);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

