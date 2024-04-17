#pragma once
#include "ctype.h"
#include "MPS.h"

/*
* 여기는 Hardware Input에 대한 정보를 모아볼 겁니다.
* input[0]
*  -> 0 : 공급실린더 전진
*  -> 1 : 공급실린더 후진
*  -> 2 : 송출실린더 전진
*  -> 3 : 송출실린더 후진
*  -> 4 : 가공실린더 하강
*  -> 5 : 가공실린더 상승
*  -> 6 : 배출실린더 전진
*  -> 7 : 배출실린더 후진
*  -> 8 : 스톱실린더 하강
*  -> 9 : 스톱실린더 상승
*  -> 10: 흡착실린더 전진
*  -> 11: 흡착실린더 후진
*  -> 12: 흡착패드 ON
*  -> 13: 창고실린더 전진
*  -> 14: 창고실린더 후진
*  -> 15: 공급센서감지
* intput[1]
*  -> 0 : 송출센서 감지
*  -> 1 : 물품감지 비금속
*  -> 2 : 물품감지 금속
*  -> 3 : 물품감지 스토퍼
* input[2]
*  -> 0 : DRIVER READY
*  -> 8 : 1축 Error
*  -> 12: 1축 Busy
* input[3]
*  -> 0 : 1축 기동완료
*  -> 4 : 1축 위치완료
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
* 여기는 Hardware Output 에 대한 정보를 모아볼 겁니다.
* Output[0]
*  -> 0 : 공급실린더 전진
*  -> 1 : 공급실린더 후진
*  -> 2 : 송출실린더 전진
*  -> 3 : 송출실린더 후진
*  -> 4 : 가공실린더 하강
*  -> 5 : 배출실린더 전진
*  -> 6 : 배출실린더 후진
*  -> 7 : 스톱실린더 하강
*  -> 8 : 흡착실린더 전진
*  -> 9 : 흡착실린더 후진
*  -> 10: 흡착패드 ON
*  -> 11: 창고실린더 전진
*  -> 12: 창고실린더 후진
*  -> 13: 가공드릴 ON
*  -> 14: 컨베이어 ON
* Output[2]
*  -> 0 : CONTROLLER READY
*  -> 1 : SERVO ON
*  -> 4 : 1축 STOP
*  -> 8 : JOG+
*  -> 12: JOG-
* Output[3]
*  -> 0 : 1축 위치기동
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