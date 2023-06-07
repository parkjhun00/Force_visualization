// CfsView.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <windows.h>
#include <conio.h>

#include "CfsUsb.h"

typedef void (CALLBACK* FUNC_Initialize)();
typedef void (CALLBACK* FUNC_Finalize)();
typedef bool (CALLBACK* FUNC_PortOpen)(int);
typedef void (CALLBACK* FUNC_PortClose)(int);
typedef bool (CALLBACK* FUNC_SetSerialMode)(int, bool);
typedef bool (CALLBACK* FUNC_GetSerialData)(int, double*, char*);
typedef bool (CALLBACK* FUNC_GetLatestData)(int, double*, char*);
typedef bool (CALLBACK* FUNC_GetSensorLimit)(int, double*);
typedef bool (CALLBACK* FUNC_GetSensorInfo)(int, char*);

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hDll;
	long cnt;
	int portNo = 6;
	char SerialNo[9];
	char Status;
	double Limit[6];
	double Data[6];
	double Fx, Fy, Fz, Mx, My, Mz;

	FUNC_Initialize Initialize;
	FUNC_Finalize Finalize;
	FUNC_PortOpen PortOpen;
	FUNC_PortClose PortClose;
	FUNC_SetSerialMode SetSerialMode;
	FUNC_GetSerialData GetSerialData;
	FUNC_GetLatestData GetLatestData;
	FUNC_GetSensorLimit GetSensorLimit;
	FUNC_GetSensorInfo GetSensorInfo;

	// �c�k�k�̃��[�h
	hDll = LoadLibrary("CfsUsb.dll");

	// c++ からpythonに送信
	FILE *python = _popen("collect_test.py", "w");
    if (!python) {
    	printf("Failed to run script.");
        return 1;
    }

	// �c�k�k������Ƀ��[�h�ł���
	if (hDll != NULL)
	{
		// �֐��A�h���X�̎擾
		Initialize = (FUNC_Initialize)GetProcAddress(hDll, "Initialize");		// �c�k�k�̏���������
		Finalize = (FUNC_Finalize)GetProcAddress(hDll, "Finalize");			// �c�k�k�̏I������
		PortOpen = (FUNC_PortOpen)GetProcAddress(hDll, "PortOpen");			// �|�[�g�I�[�v��
		PortClose = (FUNC_PortClose)GetProcAddress(hDll, "PortClose");		// �|�[�g�N���[�Y
		SetSerialMode = (FUNC_SetSerialMode)GetProcAddress(hDll, "SetSerialMode");	// �f�[�^�̘A���Ǎ��̊J�n/��~
		GetSerialData = (FUNC_GetSerialData)GetProcAddress(hDll, "GetSerialData");	// �A���f�[�^�Ǎ���
		GetLatestData = (FUNC_GetLatestData)GetProcAddress(hDll, "GetLatestData");	// �ŐV�f�[�^�Ǎ�
		GetSensorLimit = (FUNC_GetSensorLimit)GetProcAddress(hDll, "GetSensorLimit");	// �Z���T��i�m�F
		GetSensorInfo = (FUNC_GetSensorInfo)GetProcAddress(hDll, "GetSensorInfo");	// �V���A��No�擾

		// �c�k�k�̏���������
		Initialize();


		//��i���擾�ł��Ȃ����Ƃ������̂ŁC���̃Z���T�̒l��\�ߓ���Ă���
		Limit[0] = 100.0;
		Limit[1] = 100.0;
		Limit[2] = 200.0;
		Limit[3] = 1.0;
		Limit[4] = 1.0;
		Limit[5] = 1.0;

		// �|�[�g�I�[�v��
		if (PortOpen(portNo) == true)
		{
			// �Z���T��i�m�F
			if (GetSensorLimit(portNo, Limit) == false)
			{
				printf("�Z���T��i�m�F���ł��܂���B");
			}
			// �V���A��No�m�F
			if (GetSensorInfo(portNo, SerialNo) == false)
			{
				printf("�V���A��No���擾�ł��܂���B");
			}
			/****************************/
			/* �n���h�V�F�C�N�ɂ��Ǎ� */
			/****************************/
			// �ŐV�f�[�^�Ǎ�
			// ���Z���T����͒�i��10000�Ƃ��ăf�[�^���o�͂���Ă���

			if (GetLatestData(portNo, Data, &Status) == true)
			{
				Fx = Limit[0] / 10000 * Data[0];								// Fx�̒l
				Fy = Limit[1] / 10000 * Data[1];								// Fy�̒l
				Fz = Limit[2] / 10000 * Data[2];								// Fz�̒l
				Mx = Limit[3] / 10000 * Data[3];								// Mx�̒l
				My = Limit[4] / 10000 * Data[4];								// My�̒l
				Mz = Limit[5] / 10000 * Data[5];								// Mz�̒l

				printf("GetLastData\n");
				printf("Fx:%.1f Fy:%.1f Fz:%.1f Mx:%.2f My:%.2f Mz:%.2f\n", Fx, Fy, Fz, Mx, My, Mz);
			}
			else
			{
				printf("�ŐV�f�[�^�擾�Ɏ��s���܂����B");
			}

			/************/
			/* �A���Ǎ� */
			/************/
			// �A���f�[�^�Ǎ����[�h�Ɉڍs
			if (SetSerialMode(portNo, true) == true)
			{
				// 10000��A���Ǎ�
				cnt = 0;
				printf("GetSerialData\n");
				while (cnt < 10000)
				{
					// �f�[�^�擾
					if (GetSerialData(portNo, Data, &Status) == true)
					{
						Fx = Limit[0] / 10000 * Data[0];						// Fx�̒l
						Fy = Limit[1] / 10000 * Data[1];						// Fy�̒l
						Fz = Limit[2] / 10000 * Data[2];						// Fz�̒l
						Mx = Limit[3] / 10000 * Data[3];						// Mx�̒l
						My = Limit[4] / 10000 * Data[4];						// My�̒l
						Mz = Limit[5] / 10000 * Data[5];						// Mz�̒l

						cnt++;

						printf("Fx:%.1f Fy:%.1f Fz:%.1f Mx:%.2f My:%.2f Mz:%.2f             \r", Fx, Fy, Fz, Mx, My, Mz);

					}
				}

				// �A���f�[�^�Ǎ����[�h���~
				if (SetSerialMode(portNo, false) == false)
				{
					printf("�A���Ǎ����[�h���~�ł��܂���B");
				}
			}
			else
			{
				printf("�A���Ǎ����[�h�Ɉڍs�ł��܂���B");
			}

			// �|�[�g�N���[�Y
			PortClose(portNo);
		}
		else
		{
			printf("������I�[�v���ł��܂���B");
		}

		// �c�k�k�̏I������
		Finalize();

		// �c�k�k�̉��
		FreeLibrary(hDll);

		printf("\n����");

		_pclose(python);
	}
	// �c�k�k�̃��[�h�Ɏ��s
	else
	{
		printf("DLL�̃��[�h�Ɏ��s���܂����B");
	}

	printf("\n�����L�[�������Ă��������B");
	while (!_kbhit()) {
	}

	return 0;
}

