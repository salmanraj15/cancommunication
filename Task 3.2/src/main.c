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

/********************************************************************
 *                              main                                *
 ********************************************************************/
int RPM =0;
int gearRatio =0;
int isECU0Alive=0;
static const int MAX_SPEED = 1023;
void sendCoolantTemp()
{
	int coolantTemp =0;
	coolantTemp = ADC_0.CDR[4].B.CDATA; //reading value from potentiometer
	// using buffer 0 to send the coolant temp
	CAN_0.BUF[0].ID.B.STD_ID = 0x501;
	CAN_0.BUF[0].DATA.B[0] = coolantTemp & 0xFF;
	CAN_0.BUF[0].DATA.B[1] = (coolantTemp >> 8) & 0xFF;
	CAN_0.BUF[0].CS.B.LENGTH = 2;
	CAN_0.BUF[0].CS.B.IDE =0 ;
	CAN_0.BUF[0].CS.B.SRR = 0;
	CAN_0.BUF[0].CS.B.RTR = 0;
	CAN_0.BUF[0].CS.B.CODE = 0b1100;
}

void sendHeadlightSwitchStatus()
{
	int headlightStatus = 0;
	int highBeam =0;
	int lowBeam =0;
	int parkingLight = 0;

	//reading switch status for indicators
	/*
	 * Configuration:
	 * SW1 = high beam
	 * SW3 = low beam
	 * SW4 = parking light
	 */
	highBeam = SIU.GPDI[44].R;   /* high beam --SW1*/
	lowBeam = SIU.GPDI[3].R;	// low beam -- SW3
	parkingLight = SIU.GPDI[2].R;  // parking light -- SW4
	headlightStatus = highBeam|(lowBeam<<1)|(parkingLight<<2);

	// using buffer 1 to send indicator switch status

	CAN_0.BUF[1].ID.B.STD_ID = 0x503;
	CAN_0.BUF[1].DATA.B[0] = headlightStatus;
	CAN_0.BUF[1].CS.B.LENGTH = 1;
	CAN_0.BUF[1].CS.B.IDE =0 ;
	CAN_0.BUF[1].CS.B.SRR = 0;
	CAN_0.BUF[1].CS.B.RTR = 0;
	CAN_0.BUF[1].CS.B.CODE = 0b1100;
}

void calculateAndSendSpeed()
{
	int speed =0;
	int gear = gearRatio;
	int gearNum =0;
	long gearRatioTable[8] = {7842,0,7842,13112,19861,27038,33149,40035};
	while(gear>1)
	{
		gearNum++;
		gear>>=1;
	}
	if(gearNum>=0 && gearNum<=7)
	{
		speed = gearRatioTable[gearNum]*RPM*10/100000;
	}

	if(speed >= MAX_SPEED)
	{
		speed = MAX_SPEED;
	}

	//sending speed on buffer 2
	CAN_0.BUF[2].ID.B.STD_ID = 0x502;
	CAN_0.BUF[2].CS.B.LENGTH = 2;
	CAN_0.BUF[2].DATA.B[0] = speed && 0xFF;
	CAN_0.BUF[2].DATA.B[1] = (speed>>8)&& 0xFF;
	CAN_0.BUF[2].CS.B.IDE =0 ;
	CAN_0.BUF[2].CS.B.SRR = 0;
	CAN_0.BUF[2].CS.B.RTR = 0;
	CAN_0.BUF[2].CS.B.CODE = 0b1100;
}

void updateECU0AliveStatus()
{
	if(1 == isECU0Alive)
	{
		LED7 =1;
		isECU0Alive = 0;
	}
	else if(0 == isECU0Alive)
	{
		LED7 = ~LED7;
		//sending error signal
		CAN_0.BUF[3].ID.B.STD_ID = 0x505;
		CAN_0.BUF[3].CS.B.LENGTH = 0;
		CAN_0.BUF[3].CS.B.IDE =0 ;
		CAN_0.BUF[3].CS.B.SRR = 0;
		CAN_0.BUF[3].CS.B.RTR = 0;
		CAN_0.BUF[3].CS.B.CODE = 0b1100;
	}
}

void sendOwnAliveStatus()
{
	CAN_0.BUF[4].ID.B.STD_ID = 0x504;
	CAN_0.BUF[4].CS.B.LENGTH = 0;
	CAN_0.BUF[4].CS.B.IDE =0 ;
	CAN_0.BUF[4].CS.B.SRR = 0;
	CAN_0.BUF[4].CS.B.RTR = 0;
	CAN_0.BUF[4].CS.B.CODE = 0b1100;
}
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

    PIT_ConfigureTimer(0,200); // configuring timer for 200ms!
    PIT_ConfigureTimer(1,100);// configuring timer for 100ms!
    PIT_StartTimer(0);
    PIT_StartTimer(1);
    /* forever */
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
    /* your own code above! */
	sendCoolantTemp(); //sending coolant temp @ 200ms
	sendHeadlightSwitchStatus(); // sending headlight status @ 200 ms!
	updateECU0AliveStatus(); // updating alive status of ECU0
	sendOwnAliveStatus(); // sending own alive status

    PIT.CH[0].TFLG.B.TIF = 1;
}

void PITCHANNEL1(void)
{
    /* your own code above! */
	calculateAndSendSpeed(); // calculating and sending speed @100 ms!!
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
		if(0x201 == recdId) //rpm is received from ECU1
		{
			RPM = ((CAN_0.RXFIFO.DATA.B[1])<<8)| CAN_0.RXFIFO.DATA.B[0];
		}
		else if(0x202 == recdId) // gear ratio recd from ecu1
		{
			gearRatio = CAN_0.RXFIFO.DATA.B[0];
		}
		else if(0x105 == recdId) //
		{
			isECU0Alive = 1;

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
