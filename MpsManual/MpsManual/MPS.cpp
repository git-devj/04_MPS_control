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

//XBOOL bIsDryRun = TRUE; // ���� �׽�Ʈ�� ����

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
	SetSpeed(speed); // �ӵ� ����
	SetCommand(9003); // ������ ������� ��Ʈ�ѷ� Ư¡, �޸𸮿� �Է� �� ���������� �̼�
	SetPosition(target); // ������ ����
	AxisRun(TRUE); // �⵿ ���
}

void ServoRunStop()
{
	AxisRun(FALSE); // �⵿ �ߴ� ���
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
		} // ���� �����Ǹ� ���� �Ǹ��� ����
		if (bIsSupplyForward()) {
			SupplyForward(FALSE);
			mSupply.nStep = 110;
		} // ���� �Ǹ��� ���������� ���� ��� (�� ���� ����<���� ����>)
		break;

	case 110:
		SupplyBackward(TRUE); // ���� �Ǹ��� ����
		if (bIsSupplyBackward()) {
			SupplyBackward(FALSE);
			mSupply.nStep = 120;
		} // ���� �Ǹ��� ���������� ���� ���
		break;

	case 120:
		if (bIsEmitDetect()) { // || bIsDryRun
			WorkDown(TRUE);
			WorkDrillOn(TRUE);
		} // ���� ���� �����Ǹ� ���� �Ǹ��� �Ʒ���, �帱 �۵�
		if (bIsWorkDown()) {
			WorkDown(FALSE);
			WorkDrillOn(FALSE);
			mSupply.nStep = 130;
		} // ���� �Ǹ��� �Ʒ��� ������ �Ʒ��� �۵� ���, �帱 �۵� ���
		break;

	case 130:
		if (bIsEmitDetect()) {
			if (bIsWorkUp()) {
				if (mConveyor.nStep == 100)
				mSupply.nStep = 140;
			}
		} // ���� ���� �����ǰ� �帱�� ���� �ö���� �����̾� ������ 100(����ִ� �ʱ� ����)�̸�
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
		} // ���� �Ǹ��� ����, �����Ǹ� ���� ���
		break;

	case 150:
		EmitBackward(TRUE);
		if (bIsEmitBackward()) {
			EmitBackward(FALSE);
			mSupply.nStep = 0;
		} // ���� �Ǹ��� ����, �����Ǹ� ���� ���
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
		} // �����̾� �۵�, ��ݼ��� �����Ǹ�
		break;

	case 120: // !!!�б� - �ݼ� / ��ݼ�!!!
		StartTime();
		if (bIsMetalDetect()) {
			mConveyor.nStep = 200;
		} // �ݼ��� �����Ǹ� 200�� ��������
		else {
			mConveyor.nStep = 300;
		} // �ƴ϶��(��ݼ� �̶��) 300�� ��������
		break;

	case 200: // !!�б� - �ݼ�!!
		if (GetElapsedTime() > 600) {
			ConveyorOn(FALSE);
			mConveyor.nStep = 210;
		} // 600ms(0.6sec)�� ������ �����̾� ����
		break;

	case 210:
		DischargeForward(TRUE);
		if (bIsDischargeForward()) {
			DischargeForward(FALSE);
			mConveyor.nStep = 220;
		} // ���� �Ǹ��� ����, �����Ǹ� ���� ���
		break;

	case 220:
		DischargeBackward(TRUE);
		if (bIsDischargeBackward()) {
			DischargeBackward(FALSE);
			mConveyor.nStep = 0;
		} // ���� �Ǹ��� ����, �����Ǹ� ���� ���
		break;

	case 300:
		//if (GetElapsedTime() > 7000) {
		//	ConveyorOn(FALSE);
		//	mConveyor.nStep = 0;
		//} // 7000ms(7sec)�� ������ �����̾� ����
		StopperDown(TRUE); // �����۸� ����
		if (bIsStopperDetect()) {
			ConveyorOn(FALSE);
			StopperDown(FALSE);
			mConveyor.nStep = 310;
		} // �����ۿ� ��ǰ�� �����Ǹ� �����̾ ���߰� ������ ������ ����(�ø�)
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
		ServoRun(10, STOCKER_POS1); // ������͸� �ӵ� 10���� 1�� ��ġ�� ����
		if (getPosition() == STOCKER_POS1) {
			ServoRunStop();
			mStocker.nStep = 120;
		} // 1�� ��ġ�� ��ġ�ϸ� ������͸� ���߰� 120 ��������
		break;

	case 120:
		VaccumOn(TRUE); // ���� ��
		if (bIsVaccumOn()) {
			mStocker.nStep = 130;
		} // ������ ���� ��ǰ�� �پ��� ��
		break;

	case 130:
		ServoRun(10, STOCKER_POS4);
		if (getPosition() == STOCKER_POS4) {
			ServoRunStop();
			mStocker.nStep = 140;
		}
		break;

	case 140:
		VaccumForward(TRUE); // ���� �Ǹ��� ����
		if (bIsVaccumForward()) {
			VaccumForward(FALSE);
			VaccumOn(FALSE);
			mStocker.nStep = 150;
		} // ���� �Ǹ����� �����Ǿ� ������
		break;

	case 150:
		if (bIsStockerForward()) {
			mStocker.nStep = 160;
		}
		else if (bIsStockerBackward()) {
			mStocker.nStep = 170;
		} // ����Ⱑ ���� �Ǿ�������
		break;

	case 160:
		StockerBackward(TRUE);
		if (bIsStockerBackward()) {
			StockerBackward(FALSE);
			mStocker.nStep = 180;
		} // ����⸦ ����, �����Ǿ����� ���� ����
		break;

	case 170:
		StockerForward(TRUE);
		if (bIsStockerForward()) {
			StockerForward(FALSE);
			mStocker.nStep = 180;
		} // ����⸦ ����, �����Ǿ����� ���� ����
		break;

	case 180:
		VaccumBackward(TRUE); // ���� �Ǹ��� ����
		if (bIsVaccumBackward()) {
			VaccumBackward(FALSE);
			mStocker.nStep = 0;
		} // �����Ǿ����� ���� ����
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
	
	ControllerReady(TRUE); // ������� �غ� ON
	ServoOn(TRUE); // ������� ON

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
			} // �ڵ�

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
			} // ���� �ڵ�
			
			if (!strcmp(command, "stop")) {
				mSupply.bIsAuto = FALSE;
				mConveyor.bIsAuto = FALSE;
				mStocker.bIsAuto = FALSE;
			} // ����

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