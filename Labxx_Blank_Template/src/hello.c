/* ============================================================================

About HAL and BSP Functions, and Why They're Really Nice

By Karl Nasrallah
EE/Physics dual, RPI Class of 2018

============================================================================= */

/* ============================================================================
Master LED Init Function:

void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpio_init_structure;

  LEDx_GPIO_CLK_ENABLE();
  *//* Configure the GPIO_LED pin *//*
  gpio_init_structure.Pin   = GPIO_PIN[Led];
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_HIGH;

  HAL_GPIO_Init(GPIO_PORT[Led], &gpio_init_structure);

}

Let's reverse-engineer this initialization function piece-by-piece:

Starting with Led_TypeDef:

typedef enum
{
 LED1 = 0,
 LED_RED = LED1,
 LED2 = 1,
 LED_GREEN = LED2
} Led_TypeDef;


From there, here's the GPIO Clock Enable:

#define LEDx_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOJ_CLK_ENABLE()

#define __HAL_RCC_GPIOJ_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOJEN);\
                                      *//* Delay after an RCC peripheral clock enabling *//* \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOJEN);\
                                        UNUSED(tmpreg); \
                                      } while(0)


#define RCC_AHB1ENR_GPIOJEN   0x00000200U
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define UNUSED(x)             ((void)(x))

Memory Address Stuff

#define PERIPH_BASE           0x40000000U < Base address of : AHB/ABP Peripherals
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000U)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800U)
#define RCC                   ((RCC_TypeDef *) RCC_BASE)

Therefore RCC is at address 0x40023800U

__IO uint32_t AHB1ENR;       *//*!< RCC AHB1 peripheral clock register, Address offset: 0x30 *//*

RCC's AHB1ENR is at address 0x40023830U


Putting it all together, we get this:

volatile uint32_t * RCC_AHB1_CLOCK_ENABLE = (uint32_t*) 0x40023830U; // Enable clock for peripheral bus on GPIO Port J
*RCC_AHB1_CLOCK_ENABLE |= 512U; // Bitmask for RCC AHB1 initialization: 0x00000200U or 512U in decimal

// All that just to enable a clock!

============================================================================= */

/* ============================================================================

Next let's look at the gpio_init_structure, from above:

gpio_init_structure.Pin   = GPIO_PIN[Led];
gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
gpio_init_structure.Pull  = GPIO_PULLUP;
gpio_init_structure.Speed = GPIO_SPEED_HIGH;

uint32_t GPIO_PIN[LEDn] = {LED1_PIN,
                           LED2_PIN};

#define  LED2_PIN                    ((uint32_t)GPIO_PIN_5)
#define  GPIO_PIN_5                  ((uint16_t)0x0020U)  *//* Pin 5 selected    *//*

#define  GPIO_MODE_OUTPUT_PP         ((uint32_t)0x00000001U)  *//*!< Output Push Pull Mode   *//*
#define  GPIO_PULLUP                 ((uint32_t)0x00000001U)  *//*!< Pull-up activation    *//*

#define  GPIO_SPEED_HIGH             GPIO_SPEED_FREQ_VERY_HIGH
#define  GPIO_SPEED_FREQ_VERY_HIGH   ((uint32_t)0x00000003U)  *//*!< High speed    *//*

For more defines like these, see stm32f7xx_hal_gpio.h, stm32f7xx_hal_gpio.c, and stm32f769i_discovery.h.
That is where these are defined explicitly.

============================================================================= */

/* ============================================================================

Finally, the monster: HAL GPIO init:

HAL_GPIO_Init(GPIO_PORT[Led], &gpio_init_structure);

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT,
                                 LED2_GPIO_PORT};

#define LED2_GPIO_PORT          ((GPIO_TypeDef*)GPIOJ)

Not putting the whole HAL_GPIO_Init function here because it's massive.
Have a look in stm32f7xx_hal_gpio.c to see it for yourself.

All we really care about are these parts:

void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{

************************************SNIP***************************************

  *//* Check the parameters *//*
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));    <--   #define  LED2_GPIO_PORT       ((GPIO_TypeDef*)GPIOJ)
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));    <--   #define  GPIO_PIN_5           ((uint16_t)0x0020U)  *//* Pin 5 selected    *//*
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));  <--   #define  GPIO_MODE_OUTPUT_PP  ((uint32_t)0x00000001U)  *//*!< Output Push Pull Mode   *//*
  assert_param(IS_GPIO_PULL(GPIO_Init->Pull));  <--   #define  GPIO_PULLUP          ((uint32_t)0x00000001U)  *//*!< Pull-up activation    *//*

************************************SNIP***************************************


*//* Configure IO Direction mode (Input, Output, Alternate or Analog) *//*
temp = GPIOx->MODER;
temp &= ~(GPIO_MODER_MODER0 << (position * 2));
temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2));
GPIOx->MODER = temp;

************************************SNIP***************************************

}

#define GPIO_MODER_MODER0      0x00000003U
#define GPIO_MODE             ((uint32_t)0x00000003U)

Memory Address stuff

#define PERIPH_BASE           0x40000000U < Base address of : AHB/ABP Peripherals
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000U)
#define GPIOJ_BASE            (AHB1PERIPH_BASE + 0x2400U)
#define GPIOJ                 ((GPIO_TypeDef *) GPIOJ_BASE)

Therefore GPIOJ is at address 0x40022400U

__IO uint32_t MODER;    *//*!< GPIO port mode register, Address offset: 0x00 *//*

So GPIO J's MODER is at address 0x40022400U

Putting it all together, we get this:

volatile uint32_t * GREENLEDMODER = (uint32_t*) 0x40022400U; // Init GPIO J Pin 5 (LED2 with no Alt. func.) to Output
*GREENLEDMODER |= 1024U; // Bitmask for GPIO J Pin 5 initialization: 0x00000400U or 1024U in decimal

And that's just to initialize the LED GPIO PIN!

============================================================================= */

/* ============================================================================

Now, to actually turn on and off the LED, it's time to look at BSP_LED_On and BSP_LED_Off:

void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

GPIO_PIN_SET and GPIO_PIN_RESET are defined by this enum:

typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;

Note: GPIO_PIN_SET == 1.


Digging into HAL_GPIO_WritePin:

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  *//* Check the parameters *//*
  assert_param(IS_GPIO_PIN(GPIO_Pin));          <--   #define  GPIO_PIN_5    ((uint16_t)0x0020U)  *//* Pin 5 selected *//*
  assert_param(IS_GPIO_PIN_ACTION(PinState));   <--   GPIO_PIN_SET (1) or GPIO_PIN_RESET (0)

  if(PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;   <--   Remember, GPIOx is GPIOJ since we're only working with Port J, Pin 5
  }
  else
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16;   <--  Remember, GPIOx is GPIOJ since we're only working with Port J, Pin 5
  }
}

__IO uint32_t BSRR;     *//*!< GPIO port bit set/reset register, Address offset: 0x18 *//*

So GPIO J's BSRR is at address 0x40022418U

From this, we get the ability to turn on LED2:

volatile uint32_t * GREENLEDBSRR = (uint32_t*) 0x40022418U; // Address of GPIO J Bit Set/Reset Register
*GREENLEDBSRR = (uint16_t)0x0020U; // Turn on Green LED (LED2)

Or turn it off:

*GREENLEDBSRR = (uint32_t)0x0020U << 16; // Turn off Green LED (LED2)

============================================================================= */

/* ============================================================================

There's also BSP_LED_Toggle:

void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  *//* Check the parameters *//*
  assert_param(IS_GPIO_PIN(GPIO_Pin));   <--   #define  GPIO_PIN_5  ((uint16_t)0x0020U)  *//* Pin 5 selected *//*

  GPIOx->ODR ^= GPIO_Pin;   <--   Remember, GPIOx is GPIOJ since we're only working with Port J, Pin 5
}

__IO uint32_t ODR;      *//*!< GPIO port output data register, Address offset: 0x14 *//*

So GPIO J's ODR is at address 0x40022414U

And thus we get this to toggle LED2:

volatile uint32_t * GREENLEDODR = (uint32_t*) 0x40022414U; // Address of GPIO J Output Data Register
*GREENLEDODR ^= (uint16_t)0x0020U; // Toggle Green LED (LED2)

Woot woot!
============================================================================= */

//------------------------------------------------------------------------------------
// Hello.c
//------------------------------------------------------------------------------------
//
// Test program to demonstrate serial port I/O.  This program writes a message on
// the console using the printf() function, and reads characters using the getchar()
// function.  An ANSI escape sequence is used to clear the screen if a '2' is typed.
// A '1' repeats the message and the program responds to other input characters with
// an appropriate message.
//
// Any valid keystroke turns on the green LED on the board; invalid entries turn it off
//


//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include "hello.h"


//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------
int main(void)
{
    Sys_Init(); // This always goes at the top of main (defined in init.c)

    char choice;
		char in[3];
    int size = 3;

    printf("\033[2J\033[;H"); // Erase screen & move cursor to home position
    fflush(stdout); // Need to flush stdout after using printf that doesn't end in \n
    printf("Test of the printf() function.\n\n");

    //    __HAL_RCC_GPIOJ_CLK_ENABLE(); // Need to enable clock for peripheral bus on GPIO Port J

    volatile uint32_t * RCC_AHB1_CLOCK_ENABLE = (uint32_t*) 0x40023830U; // Enable clock for peripheral bus on GPIO Port J
    *RCC_AHB1_CLOCK_ENABLE |= 512U; // Bitmask for RCC AHB1 initialization: 0x00000200U or 512U in decimal

//    A cool trick to have printf give you the address of a pointer:
//
//    printf("GPIOJ_BASE_ADDR: %p\n", (void*)GPIOJ);
//    printf("BSRR: %p\r\n", (void*)&(GPIOJ->BSRR));
//    printf("ODR: %p\r\n", (void*)&(GPIOJ->ODR));

//    GPIOJ->MODER |= 1024U; //Bitmask for GPIO J Pin 5 initialization (set it to Output mode): 0x00000400U or 1024U in decimal
//    GPIOJ->BSRR = (uint16_t)GPIO_PIN_5; // Turn on Green LED (LED2)
//    GPIOJ->BSRR = (uint32_t)GPIO_PIN_5 << 16; // Turn off Green LED (LED2)
//    GPIOJ->ODR ^= (uint16_t)GPIO_PIN_5; // Toggle LED2

// It doesn't get lower level than this!
    volatile uint32_t * GREENLEDMODER = (uint32_t*) 0x40022400U; // Init GPIO J Pin 5 (LED2 with no Alt. func.) to Output
    *GREENLEDMODER |= 1024U; // Bitmask for GPIO J Pin 5 initialization: 0x00000400U or 1024U in decimal

    volatile uint32_t * GREENLEDBSRR = (uint32_t*) 0x40022418U; // Address of GPIO J Bit Set/Reset Register
    *GREENLEDBSRR = (uint16_t)0x0020U; // Turn on Green LED (LED2)

    HAL_Delay(1000); // Pause for a second

    volatile uint32_t * GREENLEDODR = (uint32_t*) 0x40022414U; // Address of GPIO J Output Data Register
    *GREENLEDODR ^= (uint16_t)0x0020U; // Toggle Green LED (LED2)

    while(1)
    {

        printf("Hello World!\r\n\n");
        printf("( greetings from Russell P. Kraft )\r\n\n\n");
        printf("1=repeat, 2=clear, 0=quit.\r\n\n"); // Menu of choices
				// Don't forget to end printf with newline or run fflush(stdout) after it!

//        choice = uart_getchar(&USB_UART, 1);
				choice = getchar();
				putchar(choice);

// Messing around with stuff:
//        putchar('9'); // Putchar is weird, man.
//				choice = uart_getchar(&USB_UART, 0);
//				uart_putchar(&USB_UART, &choice);
//				puts("stuff\r\n");

        // select which option to run
//        HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_SET);         // Turn green LED on (GPIO_PIN_SET == 1)
        *GREENLEDBSRR = (uint16_t)0x0020U; // Turn on Green LED (LED2)
        if (choice == '0')
            return 1;
        else if(choice == '1')
            printf("\r\n\nHere we go again.\r\n\n");
        else if(choice == '2')          // clear the screen with <ESC>[2J
        {
            printf("\033[2J\033[;H");
            fflush(stdout);
        }
        else
        {
            // inform the user how bright he is
            *GREENLEDBSRR = (uint32_t)0x0020U << 16; // Turn off Green LED (LED2)
            printf("\r\nA \"");
						putchar(choice);
//						uart_print(&USB_UART, choice);
            printf("\" is not a valid choice.\r\n\n");
        }

// Messing around with more stuff
				printf("Enter an alphanumeric sequence (press <enter> if you want to send less than %d characters): \r\n", size + 1);
				int a = uart_getline(&USB_UART, in, size);
				printf("\r\nuart_getline result: %d\r\n", a);
		}
}

//------------------------------------------------------------------------------------
//Extra thing to consider...
//------------------------------------------------------------------------------------
void serial_print_things(void) {
	//Input Buffer
	char input[2];
	input[0]=0;
	input[1]=0;

	//Initialize the system
	Sys_Init();
	initUart(&Second_UART, 9600, USART6); // Allow printing over USART6 (Arduino pins D0 - Rx and D1 - TX)
	uart_print(&USB_UART, "\033[2J\033[;H");
	uart_print(&Second_UART, "\033[2J\033[;H");
	uart_print(&USB_UART, "Hello World: This is the USB Serial Connection\r\n");
	uart_print(&Second_UART, "Hello World: This is the other UART Connection\r\n");
	uart_print(&USB_UART, "In order to send newlines to this terminal,\r\n");
	uart_print(&USB_UART, "Press <ESC> and type \"[20h\" (without quotes)\r\n");
	uart_print(&USB_UART, "To get out of newline mode and back to line feed mode,\r\n");
	uart_print(&USB_UART, "Press <ESC> and type \"[20l\" (without quotes)\r\n");
	uart_print(&USB_UART, "\"Thanks for Playing!\" --Super Monkey Ball Announcer Guy\r\n");

	printf("OMG THIS SENTENCE USES PRINTF!!!\r\n");
  // Don't forget to end printf with newline or run fflush(stdout) after it!

	while(1) {
		input[0]=uart_getchar(&USB_UART, 0);
		uart_print(&USB_UART, input);
		uart_print(&Second_UART, input);
	}

	while(1);// HALT AND CATCH FIRE
}
