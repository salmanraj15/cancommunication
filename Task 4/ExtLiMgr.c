#include <Rte_ExtLiMgr.h>
void ExtLiMgr_FuncOfExtLiMgr()
{
	// These functions can be used to access the data elements
	boolean actLowBeam = Rte_IRead_FuncOfExtLiMgr_ExtRPort1_ActLowBeamSR();
	boolean actDayRunningLight = Rte_IRead_FuncOfExtLiMgr_ExtRPort2_ActDayRunningLightSR();
	boolean opMode = Rte_IRead_FuncOfExtLiMgr_ExtRPort3_OpMode();
	
	boolean activateLowBeam = 0;
	boolean activateLowBeamIndicator = 0;
	boolean activateDayRunningLight = 0;
	
	if(opMode == 1){
		activateLowBeam = actLowBeam;
		activateLowBeamIndicator = actLowBeam;
		activateDayRunningLight = actDayRunningLight;
	}else{
		activateLowBeam = 0;
		activateLowBeamIndicator = 0;
		activateDayRunningLight = 0;
	}
	
	Rte_IWrite_FuncOfExtLiMgr_ExtPPort1_ActivateLowBeam(activateLowBeam);
	Rte_IWrite_FuncOfExtLiMgr_ExtPPort2_ActivateDayRunningLight(activateDayRunningLight);
	Rte_IWrite_FuncOfExtLiMgr_ExtPPort3_ActivateLowBeamIndication(activateLowBeamIndicator);
}