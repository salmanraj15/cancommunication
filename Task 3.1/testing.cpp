/*
 * testing.c
 *
 *  Created on: May 22, 2016
 *      Author: AJITESH
 */




#include<iostream>

using namespace std;

int main()
{
#if 0
	int indicatorStatus = 0;
	int leftIndicator =0;
	int rightIndicator =0;
	int hazardLight = 0;

	cin>>leftIndicator>>rightIndicator>>hazardLight;
	indicatorStatus = leftIndicator|(rightIndicator<<1)|(hazardLight<<2);
	cout<<indicatorStatus<<endl;

#endif
#if 0
	int doorStatus =0;
	int stateOfDoors;
	int speed;
	cin>>stateOfDoors>>speed;
		if(0b11 != stateOfDoors)
		{
			if(speed > 5)
			{
				doorStatus = 1;
			}
		}
		doorStatus = doorStatus|(stateOfDoors<<1);

		cout<<doorStatus<<endl;
#endif

		int speed =0;
		int gear,RPM;
		int gearNum =0;
		long gearRatioTable[8] = {7842,0,7842,13112,19861,27038,33149,40035};
		cin>>gear>>RPM;
		while(gear>1)
		{
			gearNum++;
			gear>>=1;
		}
		cout<<"Gear num = "<<gearNum<<endl;
		if(gearNum>=0 && gearNum<=7)
		{
			speed = gearRatioTable[gearNum]*RPM*10/100000;
		}
		cout <<"Speed ="<<speed<<endl;
		return 0;
}
