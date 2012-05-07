#include "inc/lm3s6965.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rit128x96x4.h"

//*****************************************************************************
//
// Function prototypes.
//
//*****************************************************************************
void SetupHardware(void);

//*****************************************************************************
//
// Global Variables, Arrays and Pointers.
//
//*****************************************************************************
/************ UART ************/
char 			Rx_String0;
char 			Rx_String1;
char			Rx_Buffer1[5];
char 			Tx_String0[1000];
char 			Tx_String1[1000];
unsigned int 	Tx_ptr0;
unsigned int 	Tx_ptr1;
char			*Pr_ptr0;
char			*Pr_ptr1;

/************ Flags ************/
unsigned char flag1 = 0;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// The UART0 interrupt handler - prototype defined in startup_ccs.
//
//*****************************************************************************
void
UART0IntHandler(void)
{
    // Clear the asserted interrupts.
    UARTIntClear(UART0_BASE, UART_INT_RX);

    // Gets value from UART0.
    Rx_String0 = UARTCharGet(UART0_BASE);
    UARTCharPut(UART1_BASE, Rx_String0);
}

//*****************************************************************************
//
// The UART1 interrupt handler - prototype defined in startup_ccs.
//
//*****************************************************************************
void
UART1IntHandler(void)
{
    // Clear the asserted interrupts.
    UARTIntClear(UART1_BASE, UART_INT_RX);

    // Gets value from UART1.
    Rx_String1 = UARTCharGet(UART1_BASE);
    UARTCharPut(UART0_BASE, Rx_String1);
}

//*****************************************************************************
//
// Hardware setup.
//
//*****************************************************************************
void
SetupHardware(void)
{
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_8MHZ);

    // Initialize OLED Display
    RIT128x96x4Init(1000000);

    // Enable the peripherals used by this example.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    // Enable processor interrupts.
    IntMasterEnable();

    // Sets C7 High.
    GPIODirModeSet( GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_DIR_MODE_OUT );
    GPIOPadConfigSet( GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );
    GPIOPinWrite( GPIO_PORTC_BASE, GPIO_PIN_7, 0 );


    /************ Initialize the UART0 ************/
    // Set GPIO A0 and A1 as UART0 pins.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure the UART0 for 115200, 8-N-1 operation.
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    // Enable the UART0 interrupt.
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    // Disables FIFO UART0.
    UARTFIFODisable(UART0_BASE);

    /************ Initialize the UART1 ************/
    // Set GPIO D1 and D2 as UART1 pins.
    GPIOPinConfigure(GPIO_PD2_U1RX);
    GPIOPinConfigure(GPIO_PD3_U1TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    // Configure the UART1 for 115200, 8-N-1 operation.
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    // Enable the UART1 interrupt.
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    // Disables FIFO UART1.
    UARTFIFODisable(UART1_BASE);
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{
	SetupHardware();

    // Writes on the OLED Display.
    RIT128x96x4StringDraw("Song title: ",			12,  8, 15);
    RIT128x96x4StringDraw("Length: ",       		12, 16, 15);
    RIT128x96x4StringDraw("Pause, Play, Stop ",  	12, 24, 15);

    while(1)
	{
    	// When UART1 is not busy and buffer is not empty, send data through UART1.
		if(UARTBusy(UART1_BASE) == false && Tx_String1[Tx_ptr1]!=0)
		{
			UARTCharPut(UART1_BASE, Tx_String1[Tx_ptr1]);
			Tx_ptr1++;
		}

		// 1sec passed and flag2 is 0, send command through UART1.
		if( flag1 == 0 )
		{
			Pr_ptr1 = &Tx_String1[0];

			// For further information about the commands, please see iWRAP4 User Guide.
			// Commands sent through string (Pr_ptr1).
			Pr_ptr1 += sprintf(Pr_ptr1, "AT\r\n"); // Names WT-12 module: ClassD.

			// Test purpose!
			//RIT128x96x4StringDraw("TEST ",			  	12, 36, 15);

			flag1 = 1;
			Tx_ptr1 = 0;
		}
	}
}
