#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"

#define PIN_DIG1 LL_GPIO_PIN_6
#define PIN_DIG2 LL_GPIO_PIN_13
#define PIN_DIG3 LL_GPIO_PIN_12
#define PIN_DIG4 LL_GPIO_PIN_0

#define PIN_SEGA LL_GPIO_PIN_8
#define PIN_SEGB LL_GPIO_PIN_11
#define PIN_SEGC LL_GPIO_PIN_1
#define PIN_SEGD LL_GPIO_PIN_3
#define PIN_SEGE LL_GPIO_PIN_2
#define PIN_SEGF LL_GPIO_PIN_9
#define PIN_SEGG LL_GPIO_PIN_2
#define PIN_DOT  LL_GPIO_PIN_0

#define PORT_DIG1 GPIOF
#define PORT_DIG2 GPIOA
#define PORT_DIG3 GPIOA
#define PORT_DIG4 GPIOB

#define PORT_SEGA GPIOC
#define PORT_SEGB GPIOA
#define PORT_SEGC GPIOA
#define PORT_SEGD GPIOC
#define PORT_SEGE GPIOC
#define PORT_SEGF GPIOC
#define PORT_SEGG GPIOA
#define PORT_DOT  GPIOA

long long unsigned tick = 0, ttick = 0;

void digit(int val)
{
    uint8_t codes[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111,
                         0b01100110, 0b01101101, 0b01111101, 0b00000111,
                         0b01111111, 0b01101111};
#define SET_SEGMENT(BIT, PIN, PORT) \
    if (codes[val] & (1 << BIT)) \
    { \
        LL_GPIO_SetOutputPin(PORT, PIN); \
    } else \
    { \
        LL_GPIO_ResetOutputPin(PORT, PIN); \
    }
    SET_SEGMENT(5, PIN_SEGA, PORT_SEGA);
    SET_SEGMENT(1, PIN_DIG1, PORT_DIG1);
    SET_SEGMENT(2, PIN_SEGC, PORT_SEGC);
    SET_SEGMENT(3, PIN_SEGD, PORT_SEGD);
    SET_SEGMENT(4, PIN_SEGE, PORT_SEGE);
    SET_SEGMENT(0, PIN_SEGF, PORT_SEGF);
    SET_SEGMENT(6, PIN_SEGG, PORT_SEGG);
#undef SET_SEGMENT
}

void digit_num(int num)
{
    LL_GPIO_SetOutputPin(PORT_SEGB, PIN_SEGB);
    LL_GPIO_SetOutputPin(PORT_DIG2, PIN_DIG2);
    LL_GPIO_SetOutputPin(PORT_DIG3, PIN_DIG3);
    LL_GPIO_SetOutputPin(PORT_DIG4, PIN_DIG4);
    switch (num)
    {
        case 2:
            LL_GPIO_ResetOutputPin(PORT_SEGB, PIN_SEGB);
            break;
        case 0:
            LL_GPIO_ResetOutputPin(PORT_DIG2, PIN_DIG2);
            break;
        case 1:
            LL_GPIO_ResetOutputPin(PORT_DIG3, PIN_DIG3);
            break;
        case 3:
            LL_GPIO_ResetOutputPin(PORT_DIG4, PIN_DIG4);
            break;
    }
}

int digit_arr[4] = {0, 0, 0, 0};

void SystemClock_Config(void);

int
main(void) {

        SystemClock_Config();
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);

        LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);

        LL_GPIO_SetPinMode(PORT_DIG1, PIN_DIG1, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_DIG2, PIN_DIG2, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_DIG3, PIN_DIG3, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_DIG4, PIN_DIG4, LL_GPIO_MODE_OUTPUT);

        LL_GPIO_SetPinMode(PORT_SEGA, PIN_SEGA, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_SEGB, PIN_SEGB, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_SEGC, PIN_SEGC, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_SEGD, PIN_SEGD, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_SEGE, PIN_SEGE, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_SEGF, PIN_SEGF, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(PORT_SEGG, PIN_SEGG, LL_GPIO_MODE_OUTPUT);

        int dignum = 0;
        while (1)
        {
            digit(digit_arr[dignum]);
            digit_num(dignum);
            dignum++;
            dignum %= 4;
            while (ttick < 2);
            ttick = 0;
        }
}

/**
  * System Clock Configuration
  * The system Clock is configured as follow :
  *    System Clock source            = PLL (HSI/2)
  *    SYSCLK(Hz)                     = 48000000
  *    HCLK(Hz)                       = 48000000
  *    AHB Prescaler                  = 1
  *    APB1 Prescaler                 = 1
  *    HSI Frequency(Hz)              = 8000000
  *    PLLMUL                         = 12
  *    Flash Latency(WS)              = 1
  */

void
SystemClock_Config() {
        /* Set FLASH latency */
        LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

        /* Enable HSI and wait for activation*/
        LL_RCC_HSI_Enable();
        while (LL_RCC_HSI_IsReady() != 1);

        /* Main PLL configuration and activation */
        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,
                                    LL_RCC_PLL_MUL_12);

        LL_RCC_PLL_Enable();
        while (LL_RCC_PLL_IsReady() != 1);

        /* Sysclk activation on the main PLL */
        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
        while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

        /* Set APB1 prescaler */
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

        /* Set systick to 1ms */
        SysTick_Config(48000000/1000);

        /* Update CMSIS variable (which can be updated also
         * through SystemCoreClockUpdate function) */
        SystemCoreClock = 168000000;
}

void
NMI_Handler(void) {
}

void
HardFault_Handler(void) {
        while (1)
            LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);
}

void
SVC_Handler(void) {
}

void
PendSV_Handler(void) {
}

int sec = 0, min = 0;
void
SysTick_Handler(void) {
    tick++;
    ttick++;
    if (tick % 1000 == 0)
    {
        sec++;
        if (sec == 60)
        {
            sec = 0;
            min++;
            min %= 60;
        }
        digit_arr[0] = min / 10;
        digit_arr[1] = min % 10;
        digit_arr[2] = sec / 10;
        digit_arr[3] = sec % 10;
    }
}
