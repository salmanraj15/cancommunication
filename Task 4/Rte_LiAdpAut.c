#include <Rte_LiAdpAut.h>
void LiAdpAut_FuncOfLiAdpAut()
{
	// These functions can be used to access the data elements
	boolean autoOn = Rte_IRead_FuncOfLiAdpAut_LiRPort1_AutoOn();
	boolean beamOn = Rte_IRead_FuncOfLiAdpAut_LiRPort2_BeamOn();
	uint8 valueOfLight = Rte_IRead_FuncOfLiAdpAut_LiRPort3_ValueOfLight();
	
	boolean lowBeam = 0;
	boolean dayRunningLight = 0;
	
	if(autoOn == 1){
		if(valueOfLight < 100 | beamOn == 1){
			lowBeam = 1;
			dayRunningLight = 0;
		}
		else{
			lowBeam = 0;
			dayRunningLight = 1;
		}
	}else{
		lowBeam = beamOn;
		dayRunningLight = 0;
	}
	
	
	Rte_IWrite_FuncOfLiAdpAut_LiPPort1_ActLowBeamSR(lowBeam);
	Rte_IWrite_FuncOfLiAdpAut_LiPPort2_ActDayRunningLightSR(dayRunningLight);
}