/**
  ******************************************************************************
  * @file    task.h
  * @brief   电控第一次作业 —— 任务层头文件
  *          业务代码全部放在 Tasks 目录中, CubeMX 重新生成不会覆盖。
  ******************************************************************************
  */

#ifndef __TASK_H
#define __TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* 第 2、3 题: 全局毫秒计数, 在 TIM2 更新中断回调里每 1ms 自增 1。
 * 必须是全局(非 static)且 volatile, 这样 Ozone 里才能 watch 到它。 */
extern volatile uint32_t tick;

/**
  * @brief  任务初始化。
  *         在第 1 题里设置 LED(GPIO) 的初始电平;
  *         并启动 TIM2 更新中断(第 2 题起)。
  *         在 main.c 的 USER CODE BEGIN 2 中调用。
  * @retval None
  */
void Tasks_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TASK_H */
