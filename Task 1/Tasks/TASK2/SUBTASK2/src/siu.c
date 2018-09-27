#include "jdp.h"

void SIU_Init(void) {
  /***************************************************************************
   *                                  Task 1                                 *
   *    Please enter the PCR register numbers and initialitation code here   *
   ***************************************************************************/
  /* LEDs */
  SIU.PCR[9].R = 0x0200;    /* LED0 */
  SIU.PCR[42].R = 0x0200;    /* LED1 */
  SIU.PCR[13].R =0x0200 ;    /* LED2 */
  SIU.PCR[12].R = 0x0200;    /* LED3 */
  SIU.PCR[62].R = 0x0200;	/* LED4 */
	//ADC0 ANA1 AS ANALOG INPUT.
  SIU.PCR[33].R = 0x2500;
  
  /* ... */

  /***************************************************************************
   *                                  Task 2                                 *
   *                       Analog inputs configuration                       *
   ***************************************************************************/  
  /* SIU.PCR[].R = ; */
  /* ... */

  /***************************************************************************
   *                                  Task 3                                 *
   *                            Buttons & switches                           *
   ***************************************************************************/    
  /* SIU.PCR[].R = ; */   /* BT5 */
  /* ... */
}