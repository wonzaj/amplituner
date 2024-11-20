/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "display_api.h"
#include "hal_buttons.h"
#include "hal_butt_pwr_LEDs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for displayTask */
osThreadId_t displayTaskHandle;
uint32_t displayTaskBuffer[ 128 ];
osStaticThreadDef_t displayTaskControlBlock;
const osThreadAttr_t displayTask_attributes = {
  .name = "displayTask",
  .cb_mem = &displayTaskControlBlock,
  .cb_size = sizeof(displayTaskControlBlock),
  .stack_mem = &displayTaskBuffer[0],
  .stack_size = sizeof(displayTaskBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for buttonHandlerTa */
osThreadId_t buttonHandlerTaHandle;
uint32_t buttonHandlerTaskBuffer[ 128 ];
osStaticThreadDef_t buttonHandlerTaskControlBlock;
const osThreadAttr_t buttonHandlerTa_attributes = {
  .name = "buttonHandlerTa",
  .cb_mem = &buttonHandlerTaskControlBlock,
  .cb_size = sizeof(buttonHandlerTaskControlBlock),
  .stack_mem = &buttonHandlerTaskBuffer[0],
  .stack_size = sizeof(buttonHandlerTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for powerLEDRefresh */
osThreadId_t powerLEDRefreshHandle;
uint32_t powerLEDRefreshBuffer[ 128 ];
osStaticThreadDef_t powerLEDRefreshControlBlock;
const osThreadAttr_t powerLEDRefresh_attributes = {
  .name = "powerLEDRefresh",
  .cb_mem = &powerLEDRefreshControlBlock,
  .cb_size = sizeof(powerLEDRefreshControlBlock),
  .stack_mem = &powerLEDRefreshBuffer[0],
  .stack_size = sizeof(powerLEDRefreshBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for buttonHandlerQueue */
osMessageQueueId_t buttonHandlerQueueHandle;
uint8_t buttonHandlerQueueBuffer[ 16 * sizeof( Queue_ButtonEvent_t ) ];
osStaticMessageQDef_t buttonHandlerQueueControlBlock;
const osMessageQueueAttr_t buttonHandlerQueue_attributes = {
  .name = "buttonHandlerQueue",
  .cb_mem = &buttonHandlerQueueControlBlock,
  .cb_size = sizeof(buttonHandlerQueueControlBlock),
  .mq_mem = &buttonHandlerQueueBuffer,
  .mq_size = sizeof(buttonHandlerQueueBuffer)
};
/* Definitions for RefreshDisplayTimer */
osTimerId_t RefreshDisplayTimerHandle;
osStaticTimerDef_t RefreshDisplayTimerControlBlock;
const osTimerAttr_t RefreshDisplayTimer_attributes = {
  .name = "RefreshDisplayTimer",
  .cb_mem = &RefreshDisplayTimerControlBlock,
  .cb_size = sizeof(RefreshDisplayTimerControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void displayTaskFunction(void *argument);
void buttonHandlerTaskFunction(void *argument);
void powerLEDRefreshFunction(void *argument);
void RefershDisplayTimer_Callback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of RefreshDisplayTimer */
  RefreshDisplayTimerHandle = osTimerNew(RefershDisplayTimer_Callback, osTimerPeriodic, NULL, &RefreshDisplayTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */

  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of buttonHandlerQueue */
  buttonHandlerQueueHandle = osMessageQueueNew (16, sizeof(Queue_ButtonEvent_t), &buttonHandlerQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of displayTask */
  displayTaskHandle = osThreadNew(displayTaskFunction, NULL, &displayTask_attributes);

  /* creation of buttonHandlerTa */
  buttonHandlerTaHandle = osThreadNew(buttonHandlerTaskFunction, NULL, &buttonHandlerTa_attributes);

  /* creation of powerLEDRefresh */
  powerLEDRefreshHandle = osThreadNew(powerLEDRefreshFunction, NULL, &powerLEDRefresh_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {

    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_displayTaskFunction */
/**
* @brief Function implementing the displayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_displayTaskFunction */
void displayTaskFunction(void *argument)
{
  /* USER CODE BEGIN displayTaskFunction */
  AppDisplay_OnInitTask();
  /* Infinite loop */
  for(;;)
  {
	AppDisplay_RefreshDisplayTask();
    osDelay(pdMS_TO_TICKS(Display_Controls.Refresh_Hz));
  }
  /* USER CODE END displayTaskFunction */
}

/* USER CODE BEGIN Header_buttonHandlerTaskFunction */
/**
* @brief Function implementing the buttonHandlerTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_buttonHandlerTaskFunction */
void buttonHandlerTaskFunction(void *argument)
{
  /* USER CODE BEGIN buttonHandlerTaskFunction */
  /* Infinite loop */
  for(;;)
  {
	AppButtons_EventHandler();
    osDelay(10);
  }
  /* USER CODE END buttonHandlerTaskFunction */
}

/* USER CODE BEGIN Header_powerLEDRefreshFunction */
/**
* @brief Function implementing the powerLEDRefresh thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_powerLEDRefreshFunction */
void powerLEDRefreshFunction(void *argument)
{
  /* USER CODE BEGIN powerLEDRefreshFunction */
	HAL_PowerButtonLEDs_Init();
  /* Infinite loop */
  for(;;)
  {
	HAL_PowerButtonLEDs_Task();
    osDelay(20);
  }
  /* USER CODE END powerLEDRefreshFunction */
}

/* RefershDisplayTimer_Callback function */
void RefershDisplayTimer_Callback(void *argument)
{
  /* USER CODE BEGIN RefershDisplayTimer_Callback */

  /* USER CODE END RefershDisplayTimer_Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

