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
	PIT_ConfigureTimer(0, 200 );

	/* start the timer now */
	PIT_StartTimer(0);

	LED0 = 0;


    /* forever */
	for (;;)
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
		case 61:
			break;
		case 62:
			break;
		case 65:
			break;
		case 66:
			break;
		case 67:
			break;
		case 68:
			//FLEXCAN_BUF_00_03

			CAN_0.IFRL.B.BUF00I = 1;
			CAN_0.IFRL.B.BUF01I = 1;
			CAN_0.IFRL.B.BUF02I = 1;
			CAN_0.IFRL.B.BUF03I = 1;

			break;
		case 69:
			CAN_0.IFRL.B.BUF04I = 1;
			CAN_0.IFRL.B.BUF05I = 1;
			CAN_0.IFRL.B.BUF06I = 1;
			CAN_0.IFRL.B.BUF07I = 1;
			break;
		case 70:
			CAN_0.IFRL.B.BUF08I = 1;
			CAN_0.IFRL.B.BUF09I = 1;
			CAN_0.IFRL.B.BUF10I = 1;
			CAN_0.IFRL.B.BUF11I = 1;
			break;
		case 71:
			CAN_0.IFRL.B.BUF12I = 1;
			CAN_0.IFRL.B.BUF13I = 1;
			CAN_0.IFRL.B.BUF14I = 1;
			CAN_0.IFRL.B.BUF15I = 1;
			break;
		case 72:
			break;
		case 127:
			break;
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}