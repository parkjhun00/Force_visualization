// CfsView.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <string>
#include <vector>
#include <chrono>
#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream
#include "CfsUsb.h"
#include <ctime>
#include <string>

#pragma comment(lib, "ws2_32.lib")


typedef void (CALLBACK* FUNC_Initialize)();
typedef void (CALLBACK* FUNC_Finalize)();
typedef bool (CALLBACK* FUNC_PortOpen)(int);
typedef void (CALLBACK* FUNC_PortClose)(int);
typedef bool (CALLBACK* FUNC_SetSerialMode)(int, bool);
typedef bool (CALLBACK* FUNC_GetSerialData)(int, double*, char*);
typedef bool (CALLBACK* FUNC_GetLatestData)(int, double*, char*);
typedef bool (CALLBACK* FUNC_GetSensorLimit)(int, double*);
typedef bool (CALLBACK* FUNC_GetSensorInfo)(int, char*);

struct tm t;
time_t now;

int _tmain(int argc, _TCHAR* argv[])
{
	// Initialize Winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed to initialize winsock.\n");
		return 1;
	}

	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		printf("Failed to create socket.\n");
		WSACleanup();
		return 1;
	}

	// Set up server details
	sockaddr_in serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(12345);  // Use a port that the Python server will be listening on
	serverInfo.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  // Localhost - Python server must be on the same machine

	// Connect to server
	if (connect(ClientSocket, (SOCKADDR*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR) {
		printf("Failed to connect.\n");
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	//

	
	HMODULE hDll;
	long cnt;
	int portNo = 6;
	char SerialNo[9];
	char Status;
	double Limit[6];
	double Data[6];
	double savedData[6];
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

	std::ofstream outputFile;

	std::string FileName;
	time(&now);
	localtime_s(&t, &now);

	FileName = std::to_string(t.tm_year + 1900) + "_"
		+ std::to_string(t.tm_mon + 1)
		+ std::to_string(t.tm_mday)
		+ std::to_string(t.tm_hour) + "."
		+ std::to_string(t.tm_min) + "."
		+ std::to_string(t.tm_sec) + ".csv";

	outputFile.open(FileName);
	outputFile << "time," << "Fz," << '\n';

	// ＤＬＬのロード
	hDll = LoadLibrary("CfsUsb.dll");

	// ＤＬＬが正常にロードできた
	if (hDll != NULL)
	{
		// 関数アドレスの取得
		Initialize = (FUNC_Initialize)GetProcAddress(hDll, "Initialize");		// ＤＬＬの初期化処理
		Finalize = (FUNC_Finalize)GetProcAddress(hDll, "Finalize");			// ＤＬＬの終了処理
		PortOpen = (FUNC_PortOpen)GetProcAddress(hDll, "PortOpen");			// ポートオープン
		PortClose = (FUNC_PortClose)GetProcAddress(hDll, "PortClose");		// ポートクローズ
		SetSerialMode = (FUNC_SetSerialMode)GetProcAddress(hDll, "SetSerialMode");	// データの連続読込の開始/停止
		GetSerialData = (FUNC_GetSerialData)GetProcAddress(hDll, "GetSerialData");	// 連続データ読込み
		GetLatestData = (FUNC_GetLatestData)GetProcAddress(hDll, "GetLatestData");	// 最新データ読込
		GetSensorLimit = (FUNC_GetSensorLimit)GetProcAddress(hDll, "GetSensorLimit");	// センサ定格確認
		GetSensorInfo = (FUNC_GetSensorInfo)GetProcAddress(hDll, "GetSensorInfo");	// シリアルNo取得

		// ＤＬＬの初期化処理
		Initialize();


		//定格が取得できないことが多いので，今のセンサの値を予め入れておく
		Limit[0] = 100.0;
		Limit[1] = 100.0;
		Limit[2] = 200.0;
		Limit[3] = 1.0;
		Limit[4] = 1.0;
		Limit[5] = 1.0;

		// ポートオープン
		if (PortOpen(portNo) == true)
		{
			/************/
			/* 連続読込 */
			/************/
			// 連続データ読込モードに移行
			if (SetSerialMode(portNo, true) == true)
			{
				// 10000回連続読込
				cnt = 0;
				printf("GetSerialData\n");
				while (cnt < 20000000)
				{
					// データ取得
					if (GetSerialData(portNo, Data, &Status) == true)
					{
						// 初期値セーブ
						if (cnt == 0) {
							for (int i = 0; i < 6; ++i) {
								savedData[i] = Data[i];
							}
							printf("Initial data has been saved!\n");
						}
						
						// Get current time
						time_t rawtime;
						struct tm* timeinfo;
						char buffer[10];

						time(&rawtime);
						timeinfo = localtime(&rawtime);

						// Format time as hh:mm:ss
						strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
						std::string strTime(buffer);


						//零点更新
						if (_kbhit()) {
							char c = _getch();
							if (c == 's' || c == 'S') { // 's' or 'S' key is pressed
								for (int i = 0; i < 6; ++i) {
									savedData[i] = Data[i];
								}
								printf("Zero!\n");
							}
						}

						Fx = Limit[0] / 10000 * (Data[0] - savedData[0]);						// Fxの値
						Fy = Limit[1] / 10000 * (Data[1] - savedData[1]);						// Fyの値
						Fz = Limit[2] / 10000 * (Data[2] - savedData[2]);						// Fzの値

						// Send the value over the socket
						char sendbuffer[32];
						sprintf(sendbuffer, "%.2f\n", Fz);  // Convert Fz to a string
						send(ClientSocket, sendbuffer, strlen(sendbuffer), 0);

						Mx = Limit[3] / 10000 * (Data[3] - savedData[3]);						// Mxの値
						My = Limit[4] / 10000 * (Data[4] - savedData[4]);						// Myの値
						Mz = Limit[5] / 10000 * (Data[5] - savedData[5]);						// Mzの値

						cnt++;

						printf("Fx:%.1f Fy:%.1f Fz:%.1f Mx:%.2f My:%.2f Mz:%.2f             \r", Fx, Fy, Fz, Mx, My, Mz);
						outputFile << strTime << "," << Fz << '\n';
						//outputFile << time << "," << Fz << '\n';
					}

				}

				// 連続データ読込モードを停止
				if (SetSerialMode(portNo, false) == false)
				{
					printf("連続読込モードを停止できません。");
				}
			}
			else
			{
				printf("連続読込モードに移行できません。");
			}

			// ポートクローズ
			PortClose(portNo);
		}
		else
		{
			printf("回線がオープンできません。");
		}

		// ＤＬＬの終了処理
		Finalize();

		// ＤＬＬの解放
		FreeLibrary(hDll);

		printf("\n完了");
	}
	// ＤＬＬのロードに失敗
	else
	{
		printf("DLLのロードに失敗しました。");
	}

	printf("\n何かキーを押してください。");
	while (!_kbhit()) {
	}

	//close socket
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}

