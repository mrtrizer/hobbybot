//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "diag/Trace.h"
#include "gpio.h"
#include "pins.h"
#include "stm32f4xx_hal_rcc.h"

// ----------------------------------------------------------------------------
//
// STM32F4 empty sample (trace via ITM).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the ITM output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define N 10

double tarX;
double tarY;
double curX;
double curY;
double oldX;
double oldY;
double xStep;
double yStep;
int n;

extern "C"
{
	void SysTick_Handler(void)
	{

	}

	void TIM2_IRQHandler(void)
	{
		TIM2->SR = 0;
		if (n >= N)
		{
			oldX = curX;
			oldY = curY;
			do
			{
				tarX = rand() % 60 + 28;
				tarY = rand() % 60 + 28;
				xStep = (tarX - oldX) / N;
				yStep = (tarY - oldY) / N;
			}
			while ((fabs(xStep) < 1.0) || (fabs(yStep) < 1.0));
			n = 0;
		}
		else
		{
			curX += xStep;
			curY += yStep;
			TIM3->CCR1 = (int) curX;
			TIM3->CCR2 = (int) curY;
			TIM3->CCR3 = (int) curX;
			TIM3->CCR4 = (int) curY;
			n++;
		}
	}


}

void timPWMConfigure(TIM_TypeDef * TIM, uint16_t prescaler, uint16_t autoReset,
                     char ch1, char ch2, char ch3, char ch4)
{
  if(ch1)
  {
    TIM->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1FE;
    TIM->CCER  |= TIM_CCER_CC1E;
  }
  if(ch2)
  {
    TIM->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2FE;
    TIM->CCER  |= TIM_CCER_CC2E;
  }
  if(ch3)
  {
    TIM->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3FE;
    TIM->CCER  |= TIM_CCER_CC3E;
  }
  if(ch4)
  {
    TIM->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4FE;
    TIM->CCER  |= TIM_CCER_CC4E;
  }

  TIM->PSC  = prescaler;
  TIM->ARR  = autoReset;
  TIM->CR1  = TIM_CR1_ARPE  | TIM_CR1_CEN | TIM_CR1_URS;
  TIM->DIER = TIM_DIER_UIE;
  TIM->BDTR = TIM_BDTR_MOE  | TIM_BDTR_AOE;

  TIM->BDTR = TIM_BDTR_MOE  | TIM_BDTR_AOE;
  TIM->CCR1 = 0x00;
  TIM->CCR2 = 0x00;
  TIM->EGR  = TIM_EGR_UG;
}

void timConfigure(TIM_TypeDef * TIM, uint16_t prescaler, uint16_t autoReset)
{
  TIM->CNT   = 0;
  TIM->PSC   = prescaler;
  TIM->ARR   = autoReset;
  TIM->DIER |= TIM_DIER_UIE;
  TIM->CR1   = TIM_CR1_ARPE | TIM_CR1_CEN;
}

int
main(int argc, char* argv[])
{
	  // At this stage the system clock should have already been configured
	  // at high speed.

	srand(100);

	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__TIM3_CLK_ENABLE();
	__TIM2_CLK_ENABLE();

	conf_pin(ENCODER1_A, INPUT, PUSH_PULL, FAST_S, PULL_DOWN);
	conf_pin(ENCODER1_B, INPUT, PUSH_PULL, FAST_S, PULL_DOWN);
	conf_pin(LED3, GENERAL, PUSH_PULL, FAST_S, NO_PULL_UP);
	conf_pin(LED4, GENERAL, PUSH_PULL, FAST_S, NO_PULL_UP);
	conf_pin(SERVO1, ALTERNATE, PUSH_PULL, FAST_S, NO_PULL_UP);
	conf_pin(SERVO2, ALTERNATE, PUSH_PULL, FAST_S, NO_PULL_UP);
	conf_pin(SERVO3, ALTERNATE, PUSH_PULL, FAST_S, NO_PULL_UP);
	conf_pin(SERVO4, ALTERNATE, PUSH_PULL, FAST_S, NO_PULL_UP);
	conf_af(SERVO1, AF2);
	conf_af(SERVO2, AF2);
	conf_af(SERVO3, AF2);
	conf_af(SERVO4, AF2);

	timPWMConfigure(TIM3, HAL_RCC_GetSysClockFreq() / 100000, 2000, 1, 1, 1, 1);
	timConfigure(TIM2, HAL_RCC_GetSysClockFreq() / 1000, 20);

	NVIC_EnableIRQ(TIM2_IRQn);

	TIM3->CCR1 = 28;
	TIM3->CCR2 = 28;

  // Infinite loop
  while (1)
    {
	  if (!pin_val(ENCODER1_A))
		  set_pin(LED3);
	  else
		  reset_pin(LED3);
	  if (!pin_val(ENCODER1_B))
		  set_pin(LED4);
	  else
		  reset_pin(LED4);
       // Add your code here.
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
