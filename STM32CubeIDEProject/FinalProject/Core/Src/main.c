/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include "stm32l475e_iot01_accelero.h"
#include "string.h"
#include "bgm.h"
#include "stm32l475e_iot01_qspi.h"
#include "stdlib.h"
#include "finalmath.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ITM_Port32(n) (*((volatile unsigned long *) (0xE0000000+4*n)))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;

I2C_HandleTypeDef hi2c3;

QSPI_HandleTypeDef hqspi;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */
void write_playtime(float cur_time);
//void my_atan2(float x_cor, float y_cor, float *result);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int16_t dataXYZ_accel[3];
char buffer[50];
float roll;
float pitch;

char reciever_buffer[100];
int reciever_buffer_index = 0;
uint8_t uart_rx_buffer[1]; // Buffer to hold received character

uint8_t explosion[] = {
		210, 201, 198, 198, 40, 140, 128, 128, 215, 193, 214, 197, 230, 237, 244, 160, 144, 128, 128, 128, 129, 128, 129, 128, 8, 149, 128, 128, 8, 149, 128, 128, 129, 128, 136, 128, 228, 225, 244, 225, 4, 140, 128, 128, 246, 242, 252, 79, 97, 77, 244, 235, 215, 147, 145, 167, 211, 212, 5, 76, 76, 78, 81, 81, 88, 93, 93, 85, 78, 78, 84, 88, 87, 29, 246, 246, 45, 84, 84, 59, 39, 39, 10, 239, 239, 234, 229, 229, 9, 64, 64, 40, 242, 242, 235, 207, 207, 197, 150, 149, 154, 195, 200, 201, 209, 210, 210, 211, 211, 211, 242, 253, 253, 3, 7, 7, 226, 197, 197, 205, 214, 214, 242, 30, 30, 3, 204, 204, 227, 34, 34, 24, 249, 249, 243, 220, 220, 214, 190, 191, 213, 36, 36, 19, 205, 205, 205, 207, 207, 233, 94, 94, 60, 168, 168, 190, 28, 28, 41, 95, 95, 86, 59, 59, 50, 27, 27, 13, 239, 239, 208, 162, 162, 247, 75, 75, 51, 35, 34, 253, 239, 239, 5, 9, 5, 203, 201, 223, 64, 64, 51, 32, 32, 22, 16, 16, 189, 173, 178, 233, 234, 219, 191, 192, 197, 200, 201, 228, 230, 221, 198, 198, 202, 205, 207, 247, 248, 10, 33, 33, 216, 198, 211, 254, 254, 23, 35, 34, 28, 28, 219, 177, 177, 1, 19, 17, 232, 232, 228, 218, 218, 206, 196, 196, 208, 211, 211, 206, 206, 230, 4, 4, 16, 20, 26, 71, 70, 50, 26, 26, 9, 5, 7, 12, 12, 239, 221, 223, 61, 62, 58, 54, 54, 64, 65, 38, 221, 221, 207, 201, 203, 218, 218, 8, 42, 42, 53, 53, 38, 20, 20, 9, 7, 23, 52, 52, 19, 11, 8, 254, 254, 210, 187, 191, 245, 245, 233, 221, 221, 12, 17, 252, 202, 202, 203, 203, 204, 222, 222, 9, 65, 64, 25, 15, 4, 204, 204, 240, 17, 17, 59, 65, 43, 229, 229, 1, 21, 21, 207, 199, 209, 242, 242, 6, 25, 25, 13, 10, 15, 57, 57, 18, 209, 209, 4, 35, 35, 249, 242, 245, 38, 38, 30, 1, 1, 18, 43, 43, 10, 241, 241, 35, 58, 58, 217, 189, 189, 8, 20, 20, 58, 62, 61, 196, 187, 188, 30, 38, 38, 45, 46, 46, 213, 197, 197, 27, 53, 53, 27, 14, 14, 235, 201, 201, 218, 248, 248, 248, 249, 249, 13, 22, 22, 237, 226, 227, 34, 42, 42, 44, 44, 42, 22, 22, 18, 250, 250, 255, 26, 26, 11, 196, 196, 204, 247, 247, 253, 34, 34, 30, 4, 3, 8, 46, 48, 46, 22, 20, 20, 0, 253, 253, 198, 189, 189, 13, 39, 38, 9, 250, 250, 33, 63, 63, 26, 236, 236, 230, 218, 218, 222, 237, 237, 244, 27, 28, 24, 248, 245, 243, 219, 216, 217, 30, 44, 44, 243, 226, 226, 11, 27, 27, 234, 210, 210, 251, 20, 20, 239, 215, 215, 231, 242, 242, 214, 194, 194, 226, 251, 251, 226, 206, 206, 213, 219, 219, 226, 231, 231, 25, 66, 66, 33, 8, 8, 241, 226, 226, 27, 55, 55, 232, 202, 203, 248, 4, 4, 11, 13, 17, 46, 51, 45, 21, 19, 15, 4, 4, 25, 49, 49, 237, 200, 204, 9, 25, 20, 250, 247, 238, 220, 220, 4, 33, 28, 218, 200, 202, 207, 207, 250, 38, 39, 55, 60, 48, 13, 11, 245, 229, 236, 15, 23, 5, 233, 232, 205, 197, 212, 251, 251, 7, 11, 12, 17, 18, 10, 240, 238, 244, 253, 253, 38, 62, 56, 22, 12, 23, 54, 56, 50, 43, 40, 6, 250, 249, 244, 244, 224, 205, 207, 217, 220, 231, 0, 1, 3, 3, 1, 246, 244, 2, 18, 11, 217, 199, 222, 16, 18, 31, 37, 40, 51, 53, 28, 10, 3, 229, 222, 240, 6, 10, 31, 38, 32, 20, 16, 224, 202, 200, 195, 195, 6, 45, 36, 8, 2, 227, 194, 205, 6, 27, 20, 8, 6, 236, 222, 237, 24, 34, 251, 198, 199, 208, 213, 238, 40, 51, 8, 226, 234, 15, 31, 32, 35, 35, 245, 209, 221, 18, 38, 41, 46, 47, 52, 57, 55, 42, 37, 38, 41, 42, 36, 27, 27, 27, 27, 21, 252, 240, 1, 40, 51, 35, 7, 3, 247, 231, 231, 216, 206, 213, 13, 46, 33, 230, 200, 199, 197, 195, 213, 8, 28, 13, 231, 216, 214, 209, 207, 216, 239, 248, 0, 24, 35, 30, 12, 2, 7, 36, 54, 54, 56, 57, 57, 52, 46, 44, 19, 1, 3, 14, 22, 16, 1, 249, 243, 231, 225, 232, 246, 250, 249, 246, 245, 254, 11, 14, 15, 15, 16, 250, 220, 215, 233, 0, 4, 248, 232, 229, 238, 252, 255, 15, 38, 45, 28, 2, 249, 244, 234, 230, 222, 207, 200, 214, 250, 16, 17, 20, 22, 17, 245, 223, 224, 239, 253, 253, 24, 50, 51, 29, 5, 1, 6, 12, 14, 18, 23, 25, 13, 248, 237, 238, 239, 240, 240, 240, 241, 235, 222, 212, 224, 3, 33, 24, 252, 226, 231, 252, 14, 8, 241, 220, 222, 230, 238, 238, 236, 234, 235, 238, 241, 241, 240, 239, 242, 255, 13, 13, 13, 13, 15, 21, 27, 24, 14, 3, 8, 21, 35, 35, 37, 38, 35, 28, 22, 28, 37, 44, 39, 32, 28, 23, 18, 15, 15, 14, 16, 31, 46, 34, 253, 211, 211, 213, 214, 238, 8, 25, 243, 213, 204, 209, 216, 224, 237, 250, 246, 243, 240, 230, 219, 229, 248, 10, 28, 44, 45, 16, 237, 250, 13, 29, 41, 50, 49, 42, 34, 35, 37, 36, 32, 28, 9, 239, 213, 229, 244, 254, 3, 6, 4, 251, 239, 250, 9, 25, 5, 244, 236, 0, 18, 23, 23, 24, 9, 253, 243, 242, 241, 241, 241, 241, 255, 9, 13, 4, 249, 248, 248, 249, 252, 255, 246, 233, 216, 215, 215, 219, 227, 240, 227, 216, 212, 232, 254, 7, 12, 20, 34, 45, 28, 5, 229, 244, 253, 4, 5, 7, 18, 26, 27, 15, 0, 246, 241, 237, 251, 8, 0, 244, 226, 237, 243, 255, 10, 26, 25, 24, 17, 241, 215, 205, 205, 207, 214, 219, 222, 215, 208, 214, 221, 231, 241, 247, 253, 0, 3, 6, 7, 11, 8, 6, 6, 11, 16, 22, 26, 33, 28, 26, 24, 34, 41, 44, 43, 42, 36, 32, 24, 11, 4, 252, 10, 18, 22, 13, 5, 9, 21, 37, 28, 19, 253, 241, 233, 216, 224, 228, 237, 251, 3, 15, 18, 20, 23, 13, 9, 1, 251, 247, 242, 243, 243, 244, 236, 232, 222, 216, 213, 205, 228, 249, 33, 32, 22, 10, 254, 246, 232, 235, 238, 244, 14, 26, 51, 8, 247, 215, 215, 214, 216, 232, 240, 249, 245, 242, 242, 251, 1, 4, 252, 246, 243, 246, 249, 245, 228, 215, 215, 254, 23, 38, 2, 237, 217, 243, 255, 15, 248, 240, 224, 224, 224, 224, 253, 6, 34, 17, 11, 247, 245, 243, 240, 233, 228, 218, 215, 213, 210, 210, 212, 214, 216, 217, 218, 223, 242, 251, 7, 236, 228, 208, 224, 228, 242, 244, 245, 247, 253, 255, 7, 252, 249, 236, 230, 227, 220, 249, 6, 45, 44, 42, 36, 9, 251, 218, 231, 242, 11, 12, 10, 6, 8, 10, 14, 1, 248, 230, 246, 2, 28, 27, 23, 14, 18, 21, 28, 28, 28, 26, 30, 31, 36, 17, 14, 248, 2, 3, 13, 18, 19, 19, 251, 242, 232, 253, 11, 24, 19, 14, 254, 248, 233, 245, 247, 8, 255, 253, 245, 246, 248, 253, 0, 4, 244, 241, 223, 231, 233, 237, 232, 227, 236, 238, 250, 245, 243, 233, 230, 226, 0, 2, 34, 32, 30, 4, 254, 235, 244, 249, 12, 15, 21, 251, 248, 220, 219, 220, 226, 234, 239, 240, 236, 229, 225, 225, 229, 250, 253, 20, 22, 23, 0, 249, 230, 2, 6, 27, 8, 249, 234, 235, 245, 14, 19, 34, 25, 14, 18, 18, 18, 0, 245, 233, 235, 248, 10, 13, 25, 22, 19, 17, 17, 16, 15, 14, 0, 0, 245, 244, 244, 2, 4, 9, 253, 249, 226, 225, 221, 221, 221, 241, 250, 7, 6, 6, 241, 232, 226, 242, 244, 12, 21, 22, 32, 35, 38, 37, 37, 22, 9, 2, 244, 244, 248, 0, 0, 253, 243, 234, 235, 236, 246, 244, 244, 2, 13, 27, 21, 20, 253, 238, 237, 242, 253, 9, 22, 22, 16, 2, 2, 242, 242, 242, 234, 231, 225, 226, 226, 239, 8, 8, 29, 16, 11, 241, 237, 233, 246, 251, 8, 6, 4, 250, 9, 9, 23, 23, 23, 19, 26, 26, 35, 34, 34, 23, 1, 1, 235, 2, 2, 28, 22, 22, 13, 7, 7, 1, 243, 243, 232, 0, 2, 20, 12, 7, 252, 6, 7, 22, 25, 25, 18, 9, 9, 249, 240, 239, 236, 245, 248, 252, 243, 239, 228, 231, 234, 231, 227, 222, 226, 231, 237, 255, 2, 5, 2, 254, 250, 247, 249, 252, 253, 251, 250, 250, 253, 254, 255, 250, 249, 240, 240, 240, 249, 10, 10, 29, 11, 11, 247, 247, 247, 250, 240, 240, 232, 235, 236, 240, 238, 237, 234, 237, 240, 243, 250, 249, 249, 251, 253, 253, 254, 250, 250, 241, 233, 233, 228, 237, 237, 250, 5, 5, 12, 15, 15, 14, 2, 2, 249, 245, 245, 248, 14, 14, 28, 29, 29, 23, 8, 8, 2, 238, 238, 232, 1, 1, 16, 1, 1, 242, 0, 0, 15, 22, 22, 18, 19, 19, 22, 24, 24, 23, 22, 22, 23, 23, 23, 20, 18, 18, 11, 9, 9, 254, 252, 250, 9, 11, 17, 19, 19, 13, 10, 10, 6, 254, 254, 251, 254, 0, 24, 24, 22, 13, 13, 12, 14, 14, 29, 31, 28, 18, 18, 18, 18, 18, 253, 252, 247, 230, 230, 244, 251, 249, 241, 241, 241, 237, 237, 0, 1, 6, 18, 18, 17, 12, 12, 3, 0, 0, 247, 246, 244, 236, 236, 233, 230, 230, 251, 0, 0, 5, 5, 255, 242, 242, 242, 243, 242, 231, 231, 234, 237, 237, 232, 232, 239, 246, 246, 249, 1, 3, 8, 7, 15, 30, 30, 19, 19, 17, 9, 9, 3, 2, 0, 245, 245, 4, 8, 2, 254, 255, 247, 229, 230, 250, 252, 252, 3, 3, 242, 239, 247, 252, 252, 252, 4, 3, 241, 240, 237, 234, 234, 230, 229, 234, 239, 239, 234, 236, 242, 251, 250, 1, 17, 17, 248, 245, 252, 251, 251, 244, 250, 250, 249, 241, 241, 0, 0, 243, 234, 234, 224, 220, 225, 235, 235, 249, 8, 8, 0, 2, 4, 252, 251, 247, 246, 246, 253, 253, 253, 247, 251, 252, 253, 250, 252, 22, 22, 18, 9, 9, 254, 237, 237, 246, 252, 252, 10, 21, 21, 16, 13, 13, 12, 12, 12, 6, 3, 3, 1, 0, 0, 8, 12, 12, 12, 12, 12, 3, 244, 244, 240, 233, 234, 235, 242, 242, 254, 6, 6, 6, 12, 11, 249, 243, 243, 3, 2, 2, 0, 2, 5, 6, 5, 5, 25, 25, 23, 9, 9, 9, 7, 7, 4, 249, 249, 252, 3, 3, 2, 12, 12, 11, 242, 242, 242, 249, 249, 249, 2, 4, 4, 247, 245, 245, 242, 240, 240, 2, 12, 12, 21, 31, 31, 24, 13, 13, 14, 13, 13, 11, 4, 4, 5, 248, 248, 248, 12, 13, 13, 17, 18, 18, 8, 5, 5, 2, 255, 255, 252, 251, 251, 2, 5, 5, 13, 25, 25, 11, 0, 0, 252, 239, 239, 242, 249, 249, 250, 253, 253, 250, 249, 249, 1, 7, 7, 8, 20, 20, 16, 8, 8, 3, 254, 254, 251, 245, 245, 252, 0, 0, 247, 249, 249, 3, 253, 253, 251, 4, 4, 16, 10, 6, 253, 3, 6, 247, 242, 239, 243, 243, 246, 242, 242, 239, 242, 243, 243, 240, 242, 250, 251, 251, 2, 2, 246, 245, 246, 243, 238, 245, 254, 255, 239, 247, 252, 0, 246, 241, 0, 2, 244, 239, 239, 232, 228, 244, 247, 251, 10, 19, 9, 11, 11, 17, 5, 5, 6, 13, 10, 1, 0, 2, 252, 250, 250, 254, 253, 246, 245, 255, 5, 3, 247, 9, 8, 255, 245, 244, 240, 238, 238, 236, 240, 252, 252, 255, 12, 16, 249, 253, 7, 8, 253, 237, 1, 4, 2, 246, 237, 239, 243, 252, 243, 245, 255, 4, 243, 245, 250, 244, 236, 230, 238, 245, 252, 248, 235, 246, 252, 0, 245, 251, 7, 8, 250, 1, 9, 4, 253, 240, 253, 255, 251, 244, 242, 245, 245, 243, 242, 239, 236, 235, 241, 237, 239, 248, 252, 0, 9, 12, 1, 8, 7, 2, 252, 252, 252, 252, 240, 241, 246, 252, 245, 243, 253, 1, 251, 248, 251, 3, 255, 253, 4, 9, 4, 0, 255, 12, 5, 2, 247, 0, 6, 5, 252, 248, 255, 3, 7, 3, 2, 254, 3, 3, 254, 250, 250, 242, 239, 239, 243, 239, 239, 245, 249, 250, 0, 2, 2, 2, 4, 4, 15, 14, 14, 2, 9, 9, 10, 255, 255, 254, 2, 2, 252, 250, 249, 249, 243, 242, 239, 239, 240, 240, 238, 244, 255, 0, 251, 255, 4, 3, 250, 247, 1, 5, 4, 249, 255, 2, 14, 5, 3, 254, 12, 15, 16, 10, 9, 5, 4, 5, 5, 1, 255, 251, 247, 247, 247, 240, 237, 241, 249, 249, 250, 6, 14, 13, 11, 15, 15, 21, 17, 17, 17, 23, 23, 17, 14, 14, 9, 0, 255, 251, 241, 240, 244, 244, 242, 243, 252, 0, 254, 252, 254, 0, 7, 3, 1, 245, 253, 253, 253, 240, 240, 243, 246, 246, 254, 0, 0, 244, 251, 251, 249, 236, 236, 240, 238, 238, 238, 243, 243, 1, 2, 2, 0, 15, 16, 4, 2, 2, 1, 245, 244, 3, 2, 2, 3, 18, 18, 18, 19, 19, 20, 20, 18, 13, 14, 10, 1, 252, 254, 252, 246, 242, 243, 246, 247, 243, 242, 2, 3, 255, 241, 1, 10, 14, 0, 252, 3, 12, 12, 9, 4, 253, 253, 4, 6, 254, 247, 252, 4, 12, 5, 0, 1, 9, 3, 0, 251, 248, 242, 1, 2, 253, 248, 6, 14, 14, 9, 3, 6, 8, 15, 10, 7, 2, 10, 10, 8, 2, 1, 0, 255, 249, 248, 250, 253, 245, 253, 1, 3, 246, 252, 255, 254, 241, 251, 254, 253, 254, 7, 9, 7, 8, 10, 8, 2, 251, 252, 254, 253, 245, 243, 244, 244, 244, 242, 255, 254, 254, 255, 9, 8, 8, 7, 2, 255, 254, 253, 250, 2, 2, 254, 253, 6, 254, 252, 255, 2, 250, 240, 243, 252, 1, 249, 251, 253, 1, 252, 255, 255, 254, 248, 255, 254, 250, 246, 252, 252, 250, 245, 247, 246, 247, 248, 251, 244, 244, 250, 2, 253, 254, 1, 12, 1, 4, 6, 5, 253, 255, 1, 254, 251, 254, 1, 253, 252, 0, 4, 0, 1, 4, 9, 1, 2, 5, 9, 2, 251, 247, 254, 1, 1, 250, 244, 246, 248, 248, 245, 245, 0, 0, 253, 246, 2, 7, 12, 2, 254, 250, 2, 7, 12, 5, 5, 7, 16, 15, 15, 15, 10, 2, 1, 1, 1, 244, 246, 251, 251, 251, 246, 252, 1, 9, 5, 1, 254, 8, 10, 10, 5, 255, 254, 254, 255, 254, 249, 247, 248, 249, 5, 4, 2, 250, 0, 7, 14, 8, 4, 3, 8, 10, 5, 4, 3, 5, 1, 254, 249, 250, 250, 1, 253, 251, 1, 4, 8, 4, 6, 10, 2, 255, 252, 4, 5, 254, 255, 1, 11, 6, 1, 3, 6, 11, 1, 254, 246, 252, 254, 254, 247, 244, 244, 244, 245, 246, 246, 246, 255, 2, 3, 252, 253, 6, 9, 11, 3, 2, 0, 6, 4, 1, 255, 255, 255, 250, 250, 250, 250, 250, 251, 251, 254, 3, 3, 1, 253, 3, 4, 2, 254, 252, 253, 251, 251, 253, 254, 253, 250, 252, 252, 250, 247, 247, 249, 247, 249, 252, 255, 254, 252, 1, 6, 9, 6, 0, 253, 4, 4, 4, 252, 251, 251, 251, 252, 251, 251, 252, 252, 0, 4, 4, 4, 4, 9, 8, 7, 7, 6, 6, 4, 6, 6, 1, 254, 1, 3, 3, 254, 1, 1, 254, 251, 254, 255, 255, 249, 248, 247, 252, 254, 253, 248, 248, 254, 254, 252, 0, 0, 253, 250, 253, 5, 8, 6, 2, 0, 4, 2, 2, 254, 252, 252, 250, 251, 250, 247, 246, 248, 254, 254, 249, 246, 247, 2, 4, 1, 254, 254, 2, 255, 255, 3, 4, 0, 255, 255, 6, 7, 7, 7, 7, 5, 2, 1, 1, 1, 254, 251, 251, 250, 250, 251, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 251, 251, 250, 249, 249, 252, 253, 254, 0, 0, 3, 3, 3, 2, 2, 5, 7, 6, 2, 2, 5, 5, 5, 5, 5, 4, 3, 3, 1, 254, 254, 2, 3, 254, 248, 248, 0, 3, 1, 251, 251, 0, 4, 4, 1, 0, 1, 2, 2, 2, 2, 2, 255, 254, 254, 4, 5, 1, 252, 252, 2, 4, 3, 254, 254, 255, 253, 253, 253, 3, 3, 0, 252, 253, 5, 7, 6, 255, 252, 253, 3, 4, 1, 253, 253, 1, 0, 0, 255, 1, 2, 255, 254, 254, 0, 2, 2, 1, 0, 0, 1, 254, 253, 253, 253, 254, 251, 249, 249, 250, 252, 252, 252, 253, 253, 254, 254, 254, 255, 0, 0, 254, 252, 252, 254, 0, 0, 251, 249, 250, 254, 255, 254, 251, 251, 252, 254, 254, 254, 255, 1, 1, 255, 253, 254, 1, 2, 1, 253, 252, 252, 254, 254, 254, 253, 254, 254, 254, 254, 255, 0, 254, 254, 254, 1, 1, 254, 254, 254, 0, 255, 254, 254, 254, 255, 253, 252, 251, 252, 252, 253, 252, 252, 252, 252, 253, 255, 255, 255, 253, 253, 255, 255, 255, 253, 253, 253, 253, 252, 252, 252, 1, 1, 0, 254, 254, 1, 1, 1, 0, 1, 1, 0, 254, 253, 254, 0, 0, 255, 254, 254, 254, 254, 254, 253, 0, 1, 1, 253, 253, 254, 2, 2, 0, 0, 1, 1, 254, 254, 255, 0, 0, 254, 253, 253, 253, 253, 253, 254, 0, 0, 255, 253, 253, 0, 1, 0, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 1, 0, 255, 255, 1, 2, 2, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 1, 2, 1, 0, 0, 1, 2, 2, 0, 255, 255, 1, 1, 0, 253, 253, 255, 0, 0, 255, 255, 254, 255, 255, 0, 1, 1, 0, 0, 0, 0, 0, 0, 255, 255, 0, 255, 255, 254, 254, 254, 255, 255, 0, 255, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 0, 1, 0, 255, 255, 0, 0, 0, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255, 0, 255, 0, 255, 255, 0, 0, 255, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 255, 255, 255, 0, 0, 255, 255, 255, 0, 0, 255, 255, 255, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0, 0, 0
};

uint8_t coin[] = {
		210, 201, 198, 198, 122, 133, 128, 128, 215, 193, 214, 197, 230, 237, 244, 160, 144, 128, 128, 128, 129, 128, 129, 128, 8, 149, 128, 128, 8, 149, 128, 128, 129, 128, 136, 128, 228, 225, 244, 225, 86, 133, 128, 128, 185, 61, 79, 204, 169, 40, 92, 225, 160, 19, 98, 246, 157, 255, 97, 9, 161, 236, 91, 28, 170, 218, 78, 46, 185, 201, 61, 63, 204, 185, 42, 76, 222, 175, 24, 83, 240, 171, 6, 84, 1, 172, 246, 80, 17, 178, 230, 72, 33, 189, 215, 58, 48, 205, 200, 42, 60, 221, 190, 26, 68, 237, 185, 11, 71, 251, 184, 253, 69, 9, 188, 239, 63, 22, 196, 227, 53, 34, 208, 215, 40, 46, 222, 205, 26, 54, 235, 199, 14, 57, 247, 198, 2, 57, 3, 200, 247, 53, 14, 204, 236, 47, 25, 212, 225, 39, 35, 222, 214, 28, 45, 233, 207, 17, 51, 244, 202, 6, 54, 254, 201, 251, 53, 9, 204, 241, 49, 20, 209, 230, 42, 30, 218, 219, 31, 41, 229, 210, 21, 48, 240, 205, 10, 52, 250, 202, 0, 53, 5, 203, 245, 50, 15, 208, 235, 44, 26, 215, 224, 35, 36, 225, 214, 25, 45, 236, 207, 14, 50, 246, 204, 4, 52, 1, 204, 249, 50, 11, 206, 239, 46, 21, 212, 229, 38, 31, 222, 219, 28, 41, 232, 211, 18, 47, 242, 206, 8, 50, 252, 204, 253, 50, 7, 206, 243, 47, 17, 211, 233, 41, 27, 218, 223, 32, 36, 228, 214, 22, 44, 239, 208, 11, 48, 249, 206, 2, 50, 2, 206, 248, 48, 12, 209, 238, 43, 22, 216, 228, 35, 32, 225, 218, 25, 40, 235, 211, 15, 46, 245, 207, 5, 48, 255, 206, 252, 48, 8, 209, 242, 44, 18, 214, 232, 38, 27, 222, 223, 28, 37, 232, 215, 19, 43, 241, 210, 9, 47, 251, 208, 0, 47, 4, 208, 246, 45, 14, 212, 237, 40, 23, 219, 227, 31, 32, 228, 218, 22, 40, 238, 212, 13, 45, 247, 209, 3, 46, 0, 209, 250, 45, 10, 211, 241, 41, 19, 217, 232, 34, 28, 225, 222, 25, 36, 234, 215, 16, 42, 244, 211, 7, 45, 253, 210, 254, 45, 6, 211, 245, 42, 15, 215, 236, 37, 24, 222, 227, 28, 33, 231, 219, 19, 39, 240, 213, 10, 43, 249, 211, 1, 44, 2, 211, 248, 43, 11, 214, 240, 38, 20, 220, 231, 31, 28, 228, 222, 22, 36, 237, 216, 13, 41, 246, 213, 5, 43, 255, 212, 252, 43, 7, 214, 243, 39, 16, 218, 235, 33, 24, 225, 226, 25, 33, 234, 219, 17, 38, 243, 215, 8, 42, 251, 213, 0, 42, 4, 214, 247, 40, 12, 217, 239, 35, 20, 223, 230, 28, 29, 231, 223, 20, 36, 240, 217, 11, 40, 248, 214, 3, 41, 0, 214, 251, 40, 9, 216, 242, 37, 17, 221, 234, 30, 25, 229, 226, 22, 32, 237, 220, 14, 37, 245, 216, 6, 40, 253, 215, 254, 40, 5, 216, 246, 37, 13, 220, 238, 32, 21, 226, 230, 25, 29, 234, 223, 17, 35, 242, 218, 9, 38, 250, 216, 1, 39, 2, 216, 249, 38, 10, 219, 241, 34, 243, 11, 19, 222, 20, 10, 223, 26, 1, 227, 31, 248, 233, 33, 239, 241, 32, 232, 249, 30, 227, 2, 25, 224, 11, 18, 223, 19, 10, 224, 26, 1, 227, 30, 248, 233, 32, 240, 241, 32, 232, 250, 29, 227, 2, 25, 224, 11, 18, 223, 19, 10, 225, 25, 1, 228, 29, 249, 234, 31, 240, 241, 31, 233, 250, 28, 228, 2, 24, 225, 10, 18, 224, 18, 9, 226, 24, 1, 229, 28, 249, 234, 30, 241, 242, 30, 234, 250, 28, 229, 2, 23, 226, 10, 17, 225, 18, 9, 226, 24, 1, 230, 28, 249, 235, 29, 241, 242, 29, 234, 250, 27, 229, 2, 23, 227, 10, 17, 226, 17, 9, 227, 23, 1, 230, 27, 249, 235, 29, 241, 242, 28, 235, 250, 26, 230, 2, 22, 227, 10, 16, 227, 17, 9, 228, 23, 1, 231, 26, 249, 236, 28, 242, 243, 28, 235, 250, 26, 231, 2, 22, 228, 9, 16, 227, 17, 8, 229, 22, 1, 232, 25, 249, 236, 27, 242, 243, 27, 236, 250, 25, 231, 2, 21, 229, 9, 15, 228, 16, 8, 229, 21, 1, 232, 25, 250, 237, 26, 242, 243, 26, 236, 251, 24, 232, 2, 20, 230, 9, 15, 229, 16, 8, 230, 21, 1, 233, 24, 250, 237, 26, 243, 244, 25, 237, 251, 23, 233, 2, 20, 230, 9, 14, 230, 15, 8, 231, 20, 1, 234, 23, 250, 238, 25, 243, 244, 25, 237, 251, 23, 233, 2, 19, 231, 8, 14, 230, 15, 7, 232, 19, 1, 234, 23, 250, 239, 24, 244, 244, 24, 238, 251, 22, 234, 2, 19, 232, 8, 13, 231, 14, 7, 232, 19, 1, 235, 22, 250, 239, 23, 244, 245, 23, 239, 251, 21, 235, 1, 18, 233, 8, 13, 232, 14, 7, 233, 18, 1, 236, 21, 250, 240, 22, 244, 245, 22, 239, 251, 21, 235, 1, 17, 233, 7, 13, 233, 13, 7, 234, 18, 1, 236, 20, 251, 240, 22, 245, 245, 21, 240, 251, 20, 236, 1, 17, 234, 7, 12, 234, 13, 6, 235, 17, 1, 237, 20, 251, 241, 21, 245, 246, 21, 240, 252, 19, 237, 1, 16, 235, 7, 12, 234, 12, 6, 235, 16, 1, 238, 19, 251, 241, 20, 245, 246, 20, 241, 252, 18, 238, 1, 15, 236, 7, 11, 235, 12, 6, 236, 16, 1, 238, 18, 251, 242, 19, 246, 247, 19, 241, 252, 18, 238, 1, 15, 236, 6, 11, 236, 11, 6, 237, 15, 0, 239, 17, 251, 242, 19, 246, 247, 18, 242, 252, 17, 239, 1, 14, 237, 6, 10, 237, 11, 5, 238, 14, 0, 240, 17, 251, 243, 18, 247, 247, 18, 242, 252, 16, 240, 1, 14, 238, 6, 10, 238, 10, 5, 238, 14, 0, 240, 16, 252, 243, 17, 247, 248, 17, 243, 252, 16, 240, 1, 13, 239, 6, 9, 238, 10, 5, 239, 13, 0, 241, 15, 252, 244, 16, 247, 248, 16, 244, 252, 15, 241, 1, 12, 239, 5, 9, 239, 9, 5, 240, 13, 0, 242, 15, 252, 244, 15, 248, 248, 15, 244, 253, 14, 242, 1, 12, 240, 5, 9, 240, 9, 4, 241, 12, 0, 242, 14, 252, 245, 15, 248, 249, 15, 245, 253, 13, 242, 1, 11, 241, 5, 8, 241, 8, 4, 241, 11, 0, 243, 13, 252, 245, 14, 248, 249, 14, 245, 253, 13, 243, 1, 11, 242, 4, 8, 241, 8, 4, 242, 11, 0, 244, 12, 253, 246, 13, 249, 249, 13, 246, 253, 12, 244, 1, 10, 242, 4, 7, 242, 8, 4, 243, 10, 0, 244, 12, 253, 247, 12, 249, 250, 12, 246, 253, 11, 244, 1, 9, 243, 4, 7, 243, 7, 3, 244, 9, 0, 245, 11, 253, 247, 12, 250, 250, 11, 247, 253, 11, 245, 0, 9, 244, 4, 6, 244, 7, 3, 244, 9, 0, 246, 10, 253, 248, 11, 250, 250, 11, 247, 253, 10, 246, 0, 8, 245, 3, 6, 245, 6, 3, 245, 8, 0, 246, 9, 253, 248, 10, 250, 251, 10, 248, 254, 9, 246, 0, 8, 246, 3, 5, 245, 6, 3, 246, 8, 0, 247, 9, 253, 249, 9, 251, 251, 9, 249, 254, 8, 247, 0, 7, 246, 3, 5, 246, 5, 2, 247, 7, 0, 248, 8, 254, 249, 8, 251, 251, 8, 249, 254, 8, 248, 0, 6, 247, 3, 4, 247, 5, 2, 247, 6, 0, 248, 7, 254, 250, 8, 252, 252, 8, 250, 254, 7, 248, 0, 6, 248, 2, 4, 248, 4, 2, 248, 6, 0, 249, 7, 254, 250, 7, 252, 252, 7, 250, 254, 6, 249, 0, 5, 249, 2, 4, 248, 4, 2, 249, 5, 0, 250, 6, 254, 251, 6, 252, 253, 6, 251, 254, 5, 250, 0, 5, 249, 2, 3, 249, 3, 1, 250, 4, 0, 250, 5, 254, 251, 5, 253, 253, 5, 251, 254, 5, 251, 0, 4, 250, 1, 3, 250, 3, 1, 250, 4, 0, 251, 4, 254, 252, 5, 253, 253, 4, 252, 255, 4, 251, 0, 3, 251, 1, 2, 251, 2, 1, 251, 3, 0, 252, 4, 255, 252, 4, 253, 254, 4, 252, 255, 3, 252, 0, 3, 252, 1, 2, 252, 2, 1, 252, 3, 0, 252, 3, 255, 253, 3, 254, 254, 3, 253, 255, 3, 253, 0, 2, 252, 1, 1, 252, 1, 0, 253, 2, 0, 253, 2, 255, 254, 2, 254, 254, 2, 254, 255, 2, 253, 0, 1, 253, 0, 1, 253, 1, 0, 253, 1, 0, 254, 1, 255, 254, 2, 255, 255, 1, 254, 255, 1, 254, 0, 1, 254, 0, 0, 254, 0, 0, 254, 1, 0, 254, 1, 255, 255, 1, 255, 255, 1, 255, 255, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 255, 255, 0, 255, 255, 0, 255, 255, 0, 255, 0, 0, 255, 0, 0, 255
};

int explosion_index = -1;
int explosion_size = 3248;

int coin_index = -1;
int coin_size = 1538;

int bgm_index = 0;
int bgm_size = 516339;

int played = 0;

int state = 0;
int stopped = 0;
int started = 0;

// Leaderboard SPI logic
int rank[3];
int temp[3];

float rank_time[3];
float temp_time[3];
uint32_t addr = 0x0D0000;
uint32_t addr_1 = 0x0E0000;

char rankbuffer[80];

// Struct for accel data.
//typedef struct {
//    int16_t x;
//    int16_t y;
//    int16_t z;
//} xyz_t;
//
//xyz_t accelDataBuffer[4];
//size_t accelBufferIndex = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_DAC1_Init();
  MX_TIM3_Init();
  MX_QUADSPI_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  BSP_ACCELERO_Init();
  BSP_QSPI_Init();
  MX_USART1_UART_Init();
  HAL_UART_Init(&huart1);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start_IT(&htim5);
  HAL_UART_Receive_IT(&huart1, uart_rx_buffer, 1);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim3);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (state == 0 && stopped == 0) {
		  started = 0;
		  HAL_DAC_Stop(&hdac1, DAC_CHANNEL_2);
		  bgm_index = 0;
		  explosion_index = -1;
		  coin_index = -1;
		  stopped = 1;
	  }
	  if (state == 1 && started == 0) {
		  stopped = 0;
		  HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
		  started = 1;
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x10909CEC;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2000000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 5000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 13333;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 7999;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 5000;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Function to calculate roll and pitch
void calculate_tilt_angles(int16_t X, int16_t Y, int16_t Z, float *roll, float *pitch) {
    // Convert int16_t data to float
    float fX = (float)X;
    float fY = (float)Y;
    float fZ = (float)Z;
    float a = 0.0;

    // Roll (Tilt Left/Right) in degrees
    ITM_Port32(31) = 1;
    *roll = atan2(fY, fZ) * (180 / M_PI);
//---------------ASM Enhancement-------------
//    my_atan2(fY, fZ, roll);
//    *roll = *roll * (180 / M_PI);
//---------------ASM Enhancement-------------

    // Pitch (Tilt Forward/Backward) in degrees
    *pitch = atan2(-fX, sqrt(fY*fY + fZ*fZ)) * (180 / M_PI);
//---------------ASM Enhancement-------------
//    my_atan2(-fX, sqrt(fY*fY + fZ*fZ), pitch);
//    *pitch = *pitch * (180 / M_PI);
//---------------ASM Enhancement-------------
    ITM_Port32(31) = 2;
}
//void my_atan2(float x_cor, float y_cor, float *result){
//    // Check for special cases to avoid division by zero
//	if (y_cor == 0) {
//			// y is zero, return the appropriate value
//			if (x_cor > 0) {
//				*result =  M_PI / 2.0;
//			} else if (x_cor < 0) {
//				*result = -M_PI / 2.0;
//			} else {
//				*result = 0.0;
//			}
//			return;
//	}
//
//	float ratio = x_cor * 100 / y_cor;
//	if(ratio > 100){
//		ratio = 100;
//	}else if(ratio < -100){
//		ratio = -100;
//	}
//    if (y_cor > 0) {
//        arctan_lookup((int)ratio, result);
//
//    } else {  // y_cor < 0
//        if (x_cor >= 0) {
//            arctan_lookup((int)ratio, result);
//			*result += M_PI;
//
//        } else {
//            arctan_lookup((int)ratio, result);
//            *result -= M_PI;
//
//        }
//    }
//}

int getLastTwoDigits(float f){
	int last = (int)(fabs(f-(int)f)*100)%100;
	if (last > 99) {
		last = 0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (state == 1){
		  if (htim->Instance == TIM2) {
		    BSP_ACCELERO_AccGetXYZ(dataXYZ_accel);
		    calculate_tilt_angles(dataXYZ_accel[0], dataXYZ_accel[1], dataXYZ_accel[2], &roll, &pitch);
		    sprintf(buffer, "\r\n roll and pitch, %d, %d", (int) roll, (int) pitch);
		    HAL_UART_Transmit(&huart1, buffer, (uint16_t) strlen(buffer), 50);
		  }
		  else if (htim->Instance == TIM4) {
			  	  if (explosion_index != -1) {
						HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, explosion[explosion_index]);
						explosion_index++;
						if (explosion_index >= explosion_size) {
							explosion_index = -1;
							HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
						}
			  	  } else if (coin_index != -1) {
						HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, coin[coin_index]);
						coin_index++;
						if (coin_index >= coin_size) {
							coin_index = -1;
							HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
						}
				  }
		  }
		  else if (htim->Instance == TIM3) {
				HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_8B_R, bgm[bgm_index]);
				bgm_index++;
				if (bgm_index >= bgm_size) {
					bgm_index = 0;
				}
		    }
	} else {
		  if (htim->Instance == TIM5) {
			  read_coin();
			  read_playtime();
			  memset(rankbuffer, 0, 80);
			  sprintf(rankbuffer, "\r\n rank, %d, %d, %d, %d, %d, %d",
					  rank[0], rank[1], rank[2],
					  (int)rank_time[0],
					  (int)rank_time[1],
					  (int)rank_time[2]);
			  HAL_UART_Transmit(&huart1, rankbuffer, (uint16_t) strlen(rankbuffer), 80);
		  }
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {  // Check if it is the correct UART
        char received_char = uart_rx_buffer[0];

        UART_Receive_Callback(received_char); // Process the received character

        HAL_UART_Receive_IT(&huart1, uart_rx_buffer, 1); // Set up next reception
    }
}


void UART_Receive_Callback(char received_char) {
    if (received_char == '\n') {
    	reciever_buffer[reciever_buffer_index] = '\0'; // Null-terminate the string
        process_command(reciever_buffer);
    } else {
    	reciever_buffer[reciever_buffer_index++] = received_char;
    }
}

void process_command(char* command) {
    if (strncmp(command, "explosion", 9) == 0) {
    	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    	explosion_index = 0;
    } else if (strncmp(command, "coin", 4) == 0) {
    	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    	coin_index = 0;
    } else if (strncmp(command, "start", 5) == 0) {
    	state = 1;
    } else if (strncmp(command, "stop", 4) == 0) {
    	state = 0;
    } else if (strncmp(command, "stat,", 5) == 0) {
        // Parse the survival time and coin count
        char* token = strtok(command, ", ");
        float survival_time = 0.0;
        int coin_count = 0;

        if (token != NULL) {  // Skip the "stat" part
            token = strtok(NULL, ", ");
            if (token != NULL) {
                survival_time = atof(token);  // Convert to float
                token = strtok(NULL, ", ");
                if (token != NULL) {
                    coin_count = atoi(token);  // Convert to int
                }
            }
        }
        write_coin(coin_count);
        write_playtime(survival_time);
    }
    memset(reciever_buffer, 0, sizeof(reciever_buffer)); // Clear the buffer
    reciever_buffer_index = 0;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // Identify and log the error
        uint32_t error_code = huart->ErrorCode;
        int a = 0;
    }
}


void read_coin(){
	 if (BSP_QSPI_Read((uint8_t *)rank, addr, sizeof(rank)) != QSPI_OK) {
		Error_Handler();
	 }
	for (int i = 0; i < 3; i++) {
		if (temp[i] < 0) {
			temp[i] = 0; // Replace NaN with a default value, e.g., 0.0
		}
		if (rank[i] < 0) {
			rank[i] = 0; // Replace NaN with a default value, e.g., 0.0
		}
	}
}

void write_coin(int coin_num) {
	read_coin();
    // Find the position where coin_num fits into the temp array.
    int16_t insertPos = -1;
    for (int16_t i = 0; i < 3; i++) {
        if (coin_num > temp[i]) {
            insertPos = i;
            break;
        }
    }

    // If we found a position, shift the temp array and insert coin_num.
    if (insertPos != -1) {
        for (int16_t i = 2; i > insertPos; i--) {
            temp[i] = temp[i - 1];
        }
        temp[insertPos] = coin_num;
    }

    // Write the updated array to flash.
    if (BSP_QSPI_Erase_Block(addr) != QSPI_OK) {
        Error_Handler();
    }
    if (BSP_QSPI_Write((uint8_t *)temp, addr, sizeof(temp)) != QSPI_OK) {
        Error_Handler();
    }
}


void read_playtime(){
	 if (BSP_QSPI_Read((uint8_t *)rank_time, addr_1, sizeof(rank_time)) != QSPI_OK) {
		Error_Handler();
	 }
	for (int i = 0; i < 3; i++) {
		if (isnan(temp_time[i])) {
			temp_time[i] = 0.0f; // Replace NaN with a default value, e.g., 0.0
		}
		if (isnan(rank_time[i])) {
			rank_time[i] = 0.0f; // Replace NaN with a default value, e.g., 0.0
		}
	}
}
void write_playtime(float cur_time){
	read_playtime();
    // Find the position where coin_num fits into the temp array.
    int16_t insertPos = -1;
    for (int16_t i = 0; i < 3; i++) {
        if (cur_time > temp_time[i]) {
            insertPos = i;
            break;
        }
    }

    // If we found a position, shift the temp array and insert coin_num.
    if (insertPos != -1) {
        for (int16_t i = 2; i > insertPos; i--) {
        	temp_time[i] = temp_time[i - 1];
        }
        temp_time[insertPos] = cur_time;
    }

	if (BSP_QSPI_Erase_Block(addr_1) != QSPI_OK) {
						   Error_Handler();
				  }
	if (BSP_QSPI_Write((uint8_t *)temp_time, addr_1, sizeof(temp_time)) != QSPI_OK) {
	   Error_Handler();
   }
}

//float generateSineWave(int note, float time) {
//    float frequency;
//    switch (note) {
//        case 0: frequency = C_FREQ; break;
//        case 1: frequency = D_FREQ; break;
//        // ... (handle other cases)
//        default: frequency = C_FREQ;
//    }
//    return sin(2 * M_PI * frequency * time);
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
