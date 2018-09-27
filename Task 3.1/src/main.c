#include "init.h"
/********************************************************************
*                          global variables                         *
********************************************************************/

/********************************************************************
*                    _____  ___  ___   ___                          *
*                   |_   _|/ _ \|   \ / _ \                         *
*                     | | | (_) | |) | (_) |                        *
*                     |_|  \___/|___/ \___/                         *
*                                                                   *
* If you need global variables define them here!                    *
********************************************************************/  

int speed =0;
int stateOfDoors =0;
int isECU4Alive =0;

void sendFuelLevel()
{
	int fuelLevel =0;
	fuelLevel = ADC_0.CDR[4].B.CDATA; //reading value from potentiometer
	// using buffer 0 to send the fuel level
	CAN_0.BUF[0].ID.B.STD_ID = 0x401;
	CAN_0.BUF[0].DATA.B[0] = fuelLevel & 0xFF;
	CAN_0.BUF[0].DATA.B[1] = (fuelLevel >> 8) & 0xFF;
	CAN_0.BUF[0].CS.B.LENGTH = 2;
	CAN_0.BUF[0].CS.B.IDE =0 ;
	CAN_0.BUF[0].CS.B.SRR = 0;
	CAN_0.BUF[0].CS.B.RTR = 0;
	CAN_0.BUF[0].CS.B.CODE = 0b1100;
}

void sendIndicatorSwitchStatus()
{
	int indicatorStatus = 0;
	int leftIndicator =0;
	int rightIndicator =0;
	int hazardLight = 0;

	//reading switch status for indicators
	/*
	 * Configuration:
	 * SW1 = left indicator
	 * SW3 = right indicator
	 * SW4 = hazard light
	 */
	leftIndicator = SIU.GPDI[44].R;   /* Left Indicator --SW1*/
	rightIndicator = SIU.GPDI[3].R;	// right indicator -- SW3
	hazardLight = SIU.GPDI[2].R;  // hazard light -- SW4
	indicatorStatus = leftIndicator|(rightIndicator<<1)|(hazardLight<<2);

	// using buffer 1 to send indicator switch status

	CAN_0.BUF[1].ID.B.STD_ID = 0x402;
	CAN_0.BUF[1].DATA.B[0] = indicatorStatus;
	CAN_0.BUF[1].CS.B.LENGTH = 1;
	CAN_0.BUF[1].CS.B.IDE =0 ;
	CAN_0.BUF[1].CS.B.SRR = 0;
	CAN_0.BUF[1].CS.B.RTR = 0;
	CAN_0.BUF[1].CS.B.CODE = 0b1100;
}

void updateECU4AliveStatus()
{
	if(1 == isECU4Alive)
	{
		LED7 =1;
		isECU4Alive = 0;
	}
	else if(0 == isECU4Alive)
	{
		LED7 = ~LED7;
		//sending error signal
		CAN_0.BUF[2].ID.B.STD_ID = 0x405;
		CAN_0.BUF[2].CS.B.LENGTH = 0;
		CAN_0.BUF[2].CS.B.IDE =0 ;
		CAN_0.BUF[2].CS.B.SRR = 0;
		CAN_0.BUF[2].CS.B.RTR = 0;
		CAN_0.BUF[2].CS.B.CODE = 0b1100;
	}
}

void sendOwnAliveStatus()
{
	CAN_0.BUF[3].ID.B.STD_ID = 0x404;
	CAN_0.BUF[3].CS.B.LENGTH = 0;
	CAN_0.BUF[3].CS.B.IDE =0 ;
	CAN_0.BUF[3].CS.B.SRR = 0;
	CAN_0.BUF[3].CS.B.RTR = 0;
	CAN_0.BUF[3].CS.B.CODE = 0b1100;
}

void checkAndSendDoorStatus()
{
	int doorStatus =0;
	if(0b11 != stateOfDoors)
	{
		if(speed > 5)
		{
			doorStatus = 1;
		}
	}
	doorStatus = doorStatus|stateOfDoors<<1;
	//using buffer 4 to send door status and warning if any
	CAN_0.BUF[4].ID.B.STD_ID = 0x403;
	CAN_0.BUF[4].CS.B.LENGTH = 0;
	CAN_0.BUF[4].CS.B.IDE =0 ;
	CAN_0.BUF[4].CS.B.SRR = 0;
	CAN_0.BUF[4].CS.B.RTR = 0;
	CAN_0.BUF[4].CS.B.CODE = 0b1100;

}
/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();

    /* turn off leds */
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1; 

    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * Some configuration/initialisation must                            *
    * be done outside of the for-loop....(!)                            *
    ********************************************************************/  

    /* forever */
    PIT_ConfigureTimer(0,200); // configuring timer for 200ms periodic communication
    PIT_ConfigureTimer(1,400); // configuring timer for 400ms periodic communication
    PIT_StartTimer(0);
    PIT_StartTimer(1);

    for(;;)
    {
        /********************************************************************
        *                    _____  ___  ___   ___                          *
        *                   |_   _|/ _ \|   \ / _ \                         *
        *                     | | | (_) | |) | (_) |                        *
        *                     |_|  \___/|___/ \___/                         *
        *                                                                   *
        * Write down your logic here.                                       *
        ********************************************************************/  

    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 ********************************************************************/  

void PITCHANNEL0(void)
{
	sendFuelLevel(); // sending fuel Level periodically @ 200ms!
	sendIndicatorSwitchStatus(); // sending light indicator status @ 200 ms!
	updateECU4AliveStatus(); //updating alive status of ECU4!
	sendOwnAliveStatus(); //sending own alive status
    /* your own code above! */
    PIT.CH[0].TFLG.B.TIF = 1;
}

void PITCHANNEL1(void)
{
    /* your own code above! */
	checkAndSendDoorStatus(); //sending door status @ 400 ms!!
    PIT.CH[1].TFLG.B.TIF = 1;
}

void CANMB0003(void)
{
/* No modifications needed here */
/* Receive interrupts are being cleared here */
    CAN_0.IFRL.B.BUF00I = 1;
    CAN_0.IFRL.B.BUF01I = 1;
    CAN_0.IFRL.B.BUF02I = 1;
    CAN_0.IFRL.B.BUF03I = 1;
}

void CANMB0407(void)
{

    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * CAN reception is handled here                                     *
    * The following buffers are important:                              *
    * CAN_0.RXFIFO.ID.B.STD_ID: ID of received message                  *
    * CAN_0.RXFIFO.DATA.B[i]: value of data byte 'i'                    *
    * IMPORTANT: check for the flag in CAN_0.IFRL.B.BUF05I first!       *
    ********************************************************************/  
	int recdId =0;

	if(!CAN_0.IFRL.B.BUF05I)
	{
		recdId = CAN_0.RXFIFO.ID.B.STD_ID;
		if(0x502 == recdId) //speed is received from ECU4
		{
			speed = ((CAN_0.RXFIFO.DATA.B[1])<<8)| CAN_0.RXFIFO.DATA.B[0];
		}
		else if(0x102 == recdId) // state of door received from ECU0
		{
			stateOfDoors = CAN_0.RXFIFO.DATA.B[0];
		}
		else if(0x504 == recdId) //
		{
			isECU4Alive = 1;
		}
	}
    /* end of own code! */
    /* clear flags as last step here! */
    /* don't change anything below! */
    CAN_0.IFRL.B.BUF04I = 1;
    CAN_0.IFRL.B.BUF05I = 1;
    CAN_0.IFRL.B.BUF06I = 1;
    CAN_0.IFRL.B.BUF07I = 1;
}

void CANMB0811(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */

    CAN_0.IFRL.B.BUF08I = 1;
    CAN_0.IFRL.B.BUF09I = 1;
    CAN_0.IFRL.B.BUF10I = 1;
    CAN_0.IFRL.B.BUF11I = 1;
}

void CANMB1215(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */
    CAN_0.IFRL.B.BUF12I = 1;
    CAN_0.IFRL.B.BUF13I = 1;
    CAN_0.IFRL.B.BUF14I = 1;
    CAN_0.IFRL.B.BUF15I = 1;
}



/********************************************************************
 *                   Interrupt Vector Table                         *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            PITCHANNEL0();
            break;
        case 60:
            PITCHANNEL1();
            break;
        case 68:
            CANMB0003();
            break;
        case 69:
            CANMB0407();
            break;
        case 70:
            CANMB0811();
            break;
        case 71:
            CANMB1215();
            break;        
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
