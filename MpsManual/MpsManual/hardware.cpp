#include "hardware.h"

XBOOL bIsSupplyForward()
{
	if ((input[0] & 0x0001) == 0x0001)
		return TRUE;
	return FALSE;
}
XBOOL bIsSupplyBackward()
{
	if ((input[0] & 0x0002) == 0x0002)
		return TRUE;
	return FALSE;
}
XBOOL bIsEmitForward()
{
	if ((input[0] & 0x0004) == 0x0004)
		return TRUE;
	return FALSE;
}
XBOOL bIsEmitBackward()
{
	if ((input[0] & 0x0008) == 0x0008)
		return TRUE;
	return FALSE;
}
XBOOL bIsWorkDown()
{
	if ((input[0] & 0x0010) == 0x0010)
		return TRUE;
	return FALSE;
}
XBOOL bIsWorkUp()
{
	if ((input[0] & 0x0020) == 0x0020)
		return true;
	return false;
}
XBOOL bIsDischargeForward()
{
	if ((input[0] & 0x0040) == 0x0040)
		return true;
	return false;
}
XBOOL bIsDischargeBackward()
{
	if ((input[0] & 0x0080) == 0x0080)
		return true;
	return false;
}
XBOOL bIsStopperDown()
{
	if ((input[0] & 0x0100) == 0x0100)
		return true;
	return false;
}
XBOOL bIsStopperUp()
{
	if ((input[0] & 0x0200) == 0x0200)
		return true;
	return false;
}
XBOOL bIsVaccumForward()
{
	if ((input[0] & 0x0400) == 0x0400)
		return true;
	return false;
}
XBOOL bIsVaccumBackward()
{
	if ((input[0] & 0x0800) == 0x0800)
		return true;
	return false;
}
XBOOL bIsVaccumOn()
{
	if ((input[0] & 0x1000) == 0x1000)
		return true;
	return false;
}
XBOOL bIsStockerForward()
{
	if ((input[0] & 0x2000) == 0x2000)
		return true;
	return false;
}
XBOOL bIsStockerBackward()
{
	if ((input[0] & 0x4000) == 0x4000)
		return true;
	return false;
}
XBOOL bIsSupplyDetect()
{
	if ((input[0] & 0x8000) == 0x8000)
		return true;
	return false;
}
XBOOL bIsEmitDetect()
{
	if ((input[1] & 0x0001) == 0x0001)
		return true;
	return false;
}
XBOOL bIsNonmetalDetect()
{
	if ((input[1] & 0x0002) == 0x0002)
		return true;
	return false;
}
XBOOL bIsMetalDetect()
{
	if ((input[1] & 0x0004) == 0x0004)
		return true;
	return false;
}
XBOOL bIsStopperDetect()
{
	if ((input[1] & 0x0008) == 0x0008)
		return true;
	return false;
}
XBOOL bIsDriverReady()
{
	if ((input[2] & 0x0001) == 0x0001)
		return true;
	return false;
}
XBOOL bIsAxisError()
{
	if ((input[2] & 0x0100) == 0x0100)
		return true;
	return false;
}
XBOOL bIsAxisBusy()
{
	if ((input[2] & 0x1000) == 0x1000)
		return true;
	return false;
}
XBOOL bIsAxisRunCompt()
{
	if ((input[3] & 0x0001) == 0x0001)
		return true;
	return false;
}
XBOOL bIsAxisPosCompt()
{
	if ((input[3] & 0x0010) == 0x0010)
		return true;
	return false;
}
XUINT16 getPosition()
{
	return input[40];
}
XUINT16 getErrorNo()
{
	return input[42];
}
XUINT16 getInput()
{
	return input[43];
}
XUINT16 getCurStatus()
{
	return input[44];
}
void SupplyForward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0001;
	else output[0] &= ~0x0001;
}
void SupplyBackward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0002;
	else output[0] &= ~0x0002;
}
void EmitForward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0004;
	else output[0] &= ~0x0004;
}
void EmitBackward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0008;
	else output[0] &= ~0x0008;
}
void WorkDown(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0010;
	else output[0] &= ~0x0010;
}
void DischargeForward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0020;
	else output[0] &= ~0x0020;
}
void DischargeBackward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0040;
	else output[0] &= ~0x0040;
}
void StopperDown(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0080;
	else output[0] &= ~0x0080;
}
void VaccumForward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0100;
	else output[0] &= ~0x0100;
}
void VaccumBackward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0200;
	else output[0] &= ~0x0200;
}
void VaccumOn(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0400;
	else output[0] &= ~0x0400;
}
void StockerForward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x0800;
	else output[0] &= ~0x0800;
}
void StockerBackward(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x1000;
	else output[0] &= ~0x1000;
}
void WorkDrillOn(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x2000;
	else output[0] &= ~0x02000;
}
void ConveyorOn(XBOOL bIsStatus)
{
	if (bIsStatus) output[0] |= 0x4000;
	else output[0] &= ~0x4000;
}
void ControllerReady(XBOOL bIsStatus)
{
	if (bIsStatus) output[2] |= 0x0001;
	else output[2] &= ~0x0001;
}
void ServoOn(XBOOL bIsStatus)
{
	if (bIsStatus) output[2] |= 0x0002;
	else output[2] &= ~0x0002;
}
void AxisStop(XBOOL bIsStatus)
{
	if (bIsStatus) output[2] |= 0x0010;
	else output[2] &= ~0x0010;
}
void AxisJogP(XBOOL bIsStatus)
{
	if (bIsStatus) output[2] |= 0x0100;
	else output[2] &= ~0x0100;
}
void AxisJogM(XBOOL bIsStatus)
{
	if (bIsStatus) output[2] |= 0x1000;
	else output[2] &= ~0x1000;
}
void AxisRun(XBOOL bIsStatus)
{
	if (bIsStatus) output[3] |= 0x0001;
	else output[3] &= ~0x0001;
}
void SetSpeed(XUINT16 param)
{
	output[40] = param;
}
void SetCommand(XUINT16 param)
{
	output[42] = param;
}
void SetErrorClear(XUINT16 param)
{
	output[43] = param;
}
void SetPosition(XUINT16 param)
{
	output[44] = param;
}
void SetJogSpeed(XUINT16 param)
{
	output[46] = param;
}