/**
  ******************************************************************************
  * @file    task.c
  * @brief   电控第一次作业 —— 任务层实现
  *          包含三题内容:
  *            1. GPIO : PC13 低电平(点亮 F103 最小系统板板载 LED)
  *            2. 定时器: TIM2 更新中断 1ms, 回调里 tick 自增并喂狗
  *            3. 看门狗: 去掉回调里的喂狗, 观察 IWDG 约 2s 复位
  ******************************************************************************
  */

#include "task.h"
#include "main.h"

/* CubeMX 在 main.c 中定义的全局句柄 */
extern TIM_HandleTypeDef htim2;
extern IWDG_HandleTypeDef hiwdg;

/* ======================= 第 2、3 题: 全局 tick ======================= */
/* 全局变量, 每 1ms 在 TIM2 更新中断里自增 1 (第 2 题要求)
 * 定义成 volatile uint32_t, 供 Ozone 实时观察 */
volatile uint32_t tick = 0;

/* ---------------------- 第 2 / 3 题 切换开关 ------------------------
 * 1 = 第 2 题: 在回调里调用 HAL_IWDG_Refresh() 喂狗,
 *             tick 会一直增长(~1000/s), 不会复位。
 * 0 = 第 3 题: 去掉回调里的喂狗, IWDG 约 2s 溢出复位,
 *             tick 涨到约 1000~2000 后归零, 然后不断重复。
 * 交作业时按题目最后一步(第 3 题)默认填 0。
 */
#define IWDG_REFRESH_IN_TIM_CALLBACK   0

/**
  * @brief  TIM 更新中断回调 (HAL 的弱函数在此被重写)。
  *         TIM2 每 1ms 产生一次更新中断, 因此 tick 每秒约 +1000。
  * @param  htim: 触发中断的定时器句柄
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    tick++;   /* 每 1ms 自增 1 */

#if IWDG_REFRESH_IN_TIM_CALLBACK
    HAL_IWDG_Refresh(&hiwdg);   /* 第 2 题: 喂狗, 防止复位 */
#endif
  }
}

/**
  * @brief  任务初始化。在 main.c 的 USER CODE BEGIN 2 中调用。
  * @retval None
  */
void Tasks_Init(void)
{
  /* ------------------ 第 1 题: GPIO ------------------
   * F103 最小系统板: PC13 低电平点亮板载 LED。
   * 引脚方向/推挽输出已经在 CubeMX 的 MX_GPIO_Init() 里配好,
   * 这里只按要求在 Tasks 的初始化里写入电平(不要在 while(1) 里翻转)。 */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /* ------------------ 第 2、3 题: 定时器 ------------------
   * 启动 TIM2 更新中断。NVIC(TIM2_IRQn) 已由 CubeMX 在
   * HAL_TIM_Base_MspInit() 里使能, 这里只需启动计数即可。 */
  HAL_TIM_Base_Start_IT(&htim2);
}
