#include "MPS.h"

#define STOCKER_POS1 0
#define STOCKER_POS2 50
#define STOCKER_POS3 100
#define STOCKER_POS4 150

using namespace std;

unsigned short output[80];
unsigned short input[80];

char command[COMMAND_MAX];
int idxCommand;

struct MODULE
{
	XUINT16 nStep;
	XUINT16 nOldStep;
	XBOOL bIsAuto;
	XBOOL bIsAlarm;
};

MODULE mSupply;
MODULE mConveyor;
MODULE mStocker;

//XBOOL bIsDryRun = TRUE; // 모의 테스트용 변수

clock_t curTime;

void StartTime()
{
	curTime = clock();
}
clock_t GetElapsedTime()
{
	clock_t tElapsedTime;
	tElapsedTime = clock();

	if (curTime <= tElapsedTime)
		return tElapsedTime - curTime;
	else
		return LONG_MAX + tElapsedTime - curTime;
}

void ServoRun(unsigned short speed, unsigned short target)
{
	SetSpeed(speed); // 속도 설정
	SetCommand(9003); // 미쯔비시 서브모터 컨트롤러 특징, 메모리에 입력 시 목적지까지 이송
	SetPosition(target); // 목적지 설정
	AxisRun(TRUE); // 기동 명령
}

void ServoRunStop()
{
	AxisRun(FALSE); // 기동 중단 명령
}

void ModuleSupply()
{
	switch (mSupply.nStep)
	{
	case 0:
		mSupply.nStep = 100;
		break;

	case 100:
		if (bIsSupplyDetect()) { // || bIsDryRun
			SupplyForward(TRUE);
		} // 공급 감지되면 공급 실린더 전진
		if (bIsSupplyForward()) {
			SupplyForward(FALSE);
			mSupply.nStep = 110;
		} // 공급 실린더 전진됐으면 전진 취소 (후 다음 스텝<이후 생략>)
		break;

	case 110:
		SupplyBackward(TRUE); // 공급 실린더 후진
		if (bIsSupplyBackward()) {
			SupplyBackward(FALSE);
			mSupply.nStep = 120;
		} // 공급 실린더 후진됐으면 후진 취소
		break;

	case 120:
		if (bIsEmitDetect()) { // || bIsDryRun
			WorkDown(TRUE);
			WorkDrillOn(TRUE);
		} // 송출 센서 감지되면 가공 실린더 아래로, 드릴 작동
		if (bIsWorkDown()) {
			WorkDown(FALSE);
			WorkDrillOn(FALSE);
			mSupply.nStep = 130;
		} // 가공 실린더 아래로 왔으면 아래로 작동 취소, 드릴 작동 취소
		break;

	case 130:
		if (bIsEmitDetect()) {
			if (bIsWorkUp()) {
				if (mConveyor.nStep == 100)
				mSupply.nStep = 140;
			}
		} // 송출 센서 감지되고 드릴이 위로 올라오고 컨베이어 스텝이 100(비어있는 초기 상태)이면
		else {
			//mSupply.bIsAlarm = TRUE;
			mSupply.nStep = 0;
		}
		break;

	case 140:
		EmitForward(TRUE);
		if (bIsEmitForward()) {
			EmitForward(FALSE);
			mSupply.nStep = 150;
		} // 송출 실린더 전진, 전진되면 전진 취소
		break;

	case 150:
		EmitBackward(TRUE);
		if (bIsEmitBackward()) {
			EmitBackward(FALSE);
			mSupply.nStep = 0;
		} // 송출 실린더 후진, 후진되면 후진 취소
		break;

	default:
		mSupply.nStep = 0;
		break;
	}

	if (mSupply.bIsAlarm) {
		mSupply.bIsAuto = FALSE;
	}

	if (mSupply.nOldStep != mSupply.nStep) {
		cout << "Supply Step Changed: " << mSupply.nOldStep << " -> " << mSupply.nStep << endl;
	}

	mSupply.nOldStep = mSupply.nStep;
}

void ModuleConveyor()
{
	switch (mConveyor.nStep)
	{
	case 0:
		mConveyor.nStep = 100;
		break;

	case 100:
		if(mSupply.nStep == 150)
			mConveyor.nStep = 110;
		break;

	case 110:
		ConveyorOn(TRUE);
		if (bIsNonmetalDetect()) {
			mConveyor.nStep = 120;
		} // 컨베이어 작동, 비금속이 감지되면
		break;

	case 120: // !!!분기 - 금속 / 비금속!!!
		StartTime();
		if (bIsMetalDetect()) {
			mConveyor.nStep = 200;
		} // 금속이 감지되면 200번 스텝으로
		else {
			mConveyor.nStep = 300;
		} // 아니라면(비금속 이라면) 300번 스텝으로
		break;

	case 200: // !!분기 - 금속!!
		if (GetElapsedTime() > 600) {
			ConveyorOn(FALSE);
			mConveyor.nStep = 210;
		} // 600ms(0.6sec)가 지나면 컨베이어 정지
		break;

	case 210:
		DischargeForward(TRUE);
		if (bIsDischargeForward()) {
			DischargeForward(FALSE);
			mConveyor.nStep = 220;
		} // 배출 실린더 전진, 전진되면 전진 취소
		break;

	case 220:
		DischargeBackward(TRUE);
		if (bIsDischargeBackward()) {
			DischargeBackward(FALSE);
			mConveyor.nStep = 0;
		} // 배출 실린더 후진, 후진되면 후진 취소
		break;

	case 300:
		//if (GetElapsedTime() > 7000) {
		//	ConveyorOn(FALSE);
		//	mConveyor.nStep = 0;
		//} // 7000ms(7sec)가 지나면 컨베이어 정지
		StopperDown(TRUE); // 스토퍼를 내림
		if (bIsStopperDetect()) {
			ConveyorOn(FALSE);
			StopperDown(FALSE);
			mConveyor.nStep = 310;
		} // 스토퍼에 제품이 감지되면 컨베이어를 멈추고 스토퍼 내림도 멈춤(올림)
		break;

	case 310:
		if (mStocker.nStep == 130) {
			mConveyor.nStep = 0;
		}
		break;
		
	default:
		mConveyor.nStep = 0;
		break;
	}

	if (mConveyor.nOldStep != mConveyor.nStep) {
		cout << "Conveyor Step Changed: " << mConveyor.nOldStep << " -> " << mConveyor.nStep << endl;
	}

	mConveyor.nOldStep = mConveyor.nStep;
}

void ModuleStocker()
{
	switch (mStocker.nStep)
	{
	case 0:
		mStocker.nStep = 100;
		break;

	case 100:
		if (mConveyor.nStep == 310) {
			mStocker.nStep = 110;
		}
		break;

	case 110:
		ServoRun(10, STOCKER_POS1); // 서브모터를 속도 10으로 1번 위치로 내림
		if (getPosition() == STOCKER_POS1) {
			ServoRunStop();
			mStocker.nStep = 120;
		} // 1번 위치에 위치하면 서브모터를 멈추고 120 스텝으로
		break;

	case 120:
		VaccumOn(TRUE); // 진공 켬
		if (bIsVaccumOn()) {
			mStocker.nStep = 130;
		} // 진공에 의해 제품이 붙었을 때
		break;

	case 130:
		ServoRun(10, STOCKER_POS4);
		if (getPosition() == STOCKER_POS4) {
			ServoRunStop();
			mStocker.nStep = 140;
		}
		break;

	case 140:
		VaccumForward(TRUE); // 흡착 실린더 전진
		if (bIsVaccumForward()) {
			VaccumForward(FALSE);
			VaccumOn(FALSE);
			mStocker.nStep = 150;
		} // 흡착 실린더가 전진되어 있으면
		break;

	case 150:
		if (bIsStockerForward()) {
			mStocker.nStep = 160;
		}
		else if (bIsStockerBackward()) {
			mStocker.nStep = 170;
		} // 적재기가 후진 되어있으면
		break;

	case 160:
		StockerBackward(TRUE);
		if (bIsStockerBackward()) {
			StockerBackward(FALSE);
			mStocker.nStep = 180;
		} // 적재기를 후진, 후진되었으면 후진 종료
		break;

	case 170:
		StockerForward(TRUE);
		if (bIsStockerForward()) {
			StockerForward(FALSE);
			mStocker.nStep = 180;
		} // 적재기를 전진, 전진되었으면 전진 종료
		break;

	case 180:
		VaccumBackward(TRUE); // 흡착 실린더 후진
		if (bIsVaccumBackward()) {
			VaccumBackward(FALSE);
			mStocker.nStep = 0;
		} // 후진되었으면 후진 종료
		break;

	default:
		mStocker.nStep = 0;
		break;
	}

	if (mStocker.nOldStep != mStocker.nStep) {
		cout << "Stocker Step Changed: " << mStocker.nOldStep << " -> " << mStocker.nStep << endl;
	}

	mStocker.nOldStep = mStocker.nStep;
}


void MpsUpdate()
{
	char key;
	
	ControllerReady(TRUE); // 서브모터 준비 ON
	ServoOn(TRUE); // 서브모터 ON

	if(mSupply.bIsAuto)
		ModuleSupply();
	if(mConveyor.bIsAuto)
		ModuleConveyor();
	if (mStocker.bIsAuto)
		ModuleStocker();

	if (_kbhit()) {
		key = _getch();

		if (key == KEY_ENTER) {
			idxCommand = 0;
			cout << command << endl;

			if (!strcmp(command, "auto")) {
				mSupply.bIsAuto = TRUE;
				mConveyor.bIsAuto = TRUE;
				mStocker.bIsAuto = TRUE;
			} // 자동

			if (!strcmp(command, "supply auto")) {
				mSupply.bIsAlarm = FALSE;
				mSupply.nStep = 0;
				mSupply.bIsAuto = TRUE;
			}
			if (!strcmp(command, "conveyor auto")) {
				mConveyor.bIsAuto = TRUE;
			}
			if (!strcmp(command, "stocker auto")) {
				mStocker.bIsAuto = TRUE;
			} // 각각 자동
			
			if (!strcmp(command, "stop")) {
				mSupply.bIsAuto = FALSE;
				mConveyor.bIsAuto = FALSE;
				mStocker.bIsAuto = FALSE;
			} // 정지

			for (int i = 0; i < COMMAND_MAX; i++)
				command[i] = 0;
		}

		else {
			if (idxCommand < COMMAND_MAX) {
				command[idxCommand] = key;
				idxCommand++;
			}
		}
	}
}