#include "init.h"

#define LED0  SIU.GPDO[9].R /* LED0 */
#define LED1  SIU.GPDO[42].R /* LED1 */
#define LED2  SIU.GPDO[13].R /* LED2 */
#define LED3  SIU.GPDO[12].R /* LED3 */
#define LED4  SIU.GPDO[62].R /* LED4 */
#define LED5  SIU.GPDO[61].R /* LED5 */
#define LED6  SIU.GPDO[59].R /* LED6 */
#define LED7  SIU.GPDO[11].R /* LED7 */

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
	//configure the timer 0 and set payload to 1000ms.
	PIT_ConfigureTimer(0, 1000 );

	/* start the timer now */
	PIT_StartTimer(0);

	LED0 = 0;

    /* forever */
    for(;;)
    {
/********************************************************************
 *                           Main Loop                              *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                  Write down your logic here.                     *
 ********************************************************************/  
    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                Interrupts can be handled below.                  *
 ********************************************************************/  
 
 void PITCHANNEL0() {
	LED0 =~ LED0; //Toggeling the LED0.
	/* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
}

void PITCHANNEL1() {
    /* clear flag */
    PIT.CH[1].TFLG.B.TIF = 1;
}

/* ... ISRs ... */

/********************************************************************
 *                   Interrupt Vector Table                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            /* Timer Interrupt */
            PITCHANNEL0();
            break;
        case 60:
            /* Timer Interrupt */
            PITCHANNEL1();
            break;            
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}