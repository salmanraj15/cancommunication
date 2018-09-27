#include "jdp.h"

void SIU_Init(void) {
  /***************************************************************************
   *                                  Task 1                                 *
   *    Please enter the PCR register numbers and initialitation code here   *
   ***************************************************************************/
   /* SWITCH */
   SIU.PCR[44].R = 0X0100; //SW1
   //SIU.PCR[0].R = 0X0100; //BT6
   
  /* LEDs */
  //SIU.PCR[61].R = 0x0200;    /* LED5 */
  SIU.PCR[11].R = 0x0200;    /* LED7 */

  //CONFIG TIMER
  PIT_ConfigureTimer(0, 1000 );
  
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