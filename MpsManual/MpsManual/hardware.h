#pragma once
#include "ctype.h"
#include "MPS.h"

/*
* ����� Hardware Input�� ���� ������ ��ƺ� �̴ϴ�.
* input[0]
*  -> 0 : ���޽Ǹ��� ����
*  -> 1 : ���޽Ǹ��� ����
*  -> 2 : ����Ǹ��� ����
*  -> 3 : ����Ǹ��� ����
*  -> 4 : �����Ǹ��� �ϰ�
*  -> 5 : �����Ǹ��� ���
*  -> 6 : ����Ǹ��� ����
*  -> 7 : ����Ǹ��� ����
*  -> 8 : ����Ǹ��� �ϰ�
*  -> 9 : ����Ǹ��� ���
*  -> 10: �����Ǹ��� ����
*  -> 11: �����Ǹ��� ����
*  -> 12: �����е� ON
*  -> 13: â��Ǹ��� ����
*  -> 14: â��Ǹ��� ����
*  -> 15: ���޼�������
* intput[1]
*  -> 0 : ���⼾�� ����
*  -> 1 : ��ǰ���� ��ݼ�
*  -> 2 : ��ǰ���� �ݼ�
*  -> 3 : ��ǰ���� ������
* input[2]
*  -> 0 : DRIVER READY
*  -> 8 : 1�� Error
*  -> 12: 1�� Busy
* input[3]
*  -> 0 : 1�� �⵿�Ϸ�
*  -> 4 : 1�� ��ġ�Ϸ�
*/

XBOOL bIsSupplyForward();
XBOOL bIsSupplyBackward();
XBOOL bIsEmitForward();
XBOOL bIsEmitBackward();
XBOOL bIsWorkDown();
XBOOL bIsWorkUp();
XBOOL bIsDischargeForward();
XBOOL bIsDischargeBackward();
XBOOL bIsStopperDown();
XBOOL bIsStopperUp();
XBOOL bIsVaccumForward();
XBOOL bIsVaccumBackward();
XBOOL bIsVaccumOn();
XBOOL bIsStockerForward();
XBOOL bIsStockerBackward();
XBOOL bIsSupplyDetect();
XBOOL bIsEmitDetect();
XBOOL bIsNonmetalDetect();
XBOOL bIsMetalDetect();
XBOOL bIsStopperDetect();
XBOOL bIsDriverReady();
XBOOL bIsAxisError();
XBOOL bIsAxisBusy();
XBOOL bIsAxisRunCompt();
XBOOL bIsAxisPosCompt();
XUINT16 getPosition();
XUINT16 getErrorNo();
XUINT16 getInput();
XUINT16 getCurStatus();

/*
* ����� Hardware Output �� ���� ������ ��ƺ� �̴ϴ�.
* Output[0]
*  -> 0 : ���޽Ǹ��� ����
*  -> 1 : ���޽Ǹ��� ����
*  -> 2 : ����Ǹ��� ����
*  -> 3 : ����Ǹ��� ����
*  -> 4 : �����Ǹ��� �ϰ�
*  -> 5 : ����Ǹ��� ����
*  -> 6 : ����Ǹ��� ����
*  -> 7 : ����Ǹ��� �ϰ�
*  -> 8 : �����Ǹ��� ����
*  -> 9 : �����Ǹ��� ����
*  -> 10: �����е� ON
*  -> 11: â��Ǹ��� ����
*  -> 12: â��Ǹ��� ����
*  -> 13: �����帱 ON
*  -> 14: �����̾� ON
* Output[2]
*  -> 0 : CONTROLLER READY
*  -> 1 : SERVO ON
*  -> 4 : 1�� STOP
*  -> 8 : JOG+
*  -> 12: JOG-
* Output[3]
*  -> 0 : 1�� ��ġ�⵿
*/

void SupplyForward(XBOOL bIsStatus);
void SupplyBackward(XBOOL bIsStatus);
void EmitForward(XBOOL bIsStatus);
void EmitBackward(XBOOL bIsStatus);
void WorkDown(XBOOL bIsStatus);
void DischargeForward(XBOOL bIsStatus);
void DischargeBackward(XBOOL bIsStatus);
void StopperDown(XBOOL bIsStatus);
void VaccumForward(XBOOL bIsStatus);
void VaccumBackward(XBOOL bIsStatus);
void VaccumOn(XBOOL bIsStatus);
void StockerForward(XBOOL bIsStatus);
void StockerBackward(XBOOL bIsStatus);
void WorkDrillOn(XBOOL bIsStatus);
void ConveyorOn(XBOOL bIsStatus);
void ControllerReady(XBOOL bIsStatus);
void ServoOn(XBOOL bIsStatus);
void AxisStop(XBOOL bIsStatus);
void AxisJogP(XBOOL bIsStatus);
void AxisJogM(XBOOL bIsStatus);
void AxisRun(XBOOL bIsStatus);
void SetSpeed(XUINT16 param);
void SetCommand(XUINT16 param);
void SetErrorClear(XUINT16 param);
void SetPosition(XUINT16 param);
void SetJogSpeed(XUINT16 param);