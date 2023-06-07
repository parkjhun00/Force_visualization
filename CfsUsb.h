/******************************************************************************/
/*                                                                            */
/*                  レプトリノ製 力覚センサ通信ライブラリ                     */
/*                                                                            */
/******************************************************************************/
// 共通ライブラリ
extern "C" __declspec(dllexport) int  __stdcall GetDllVersion();
extern "C" __declspec(dllexport) void __stdcall Initialize();									// ＤＬＬの初期化処理
extern "C" __declspec(dllexport) void __stdcall Finalize();									// ＤＬＬの終了処理
extern "C" __declspec(dllexport) bool __stdcall PortOpen(int pPortNo);								// ポートオープン
extern "C" __declspec(dllexport) void __stdcall PortClose(int pPortNo);								// ポートクローズ
extern "C" __declspec(dllexport) bool __stdcall SetDFilter(int pPort, INT16 pIn);						// フィルタ選択
extern "C" __declspec(dllexport) bool __stdcall GetDFilter(int pPort, INT16 *pOut);						// フィルタ確認

// 6軸力覚センサ用ライブラリ
extern "C" __declspec(dllexport) bool __stdcall SetSerialMode(int pPortNo, bool pSet);						// データの連続読込の開始/停止
extern "C" __declspec(dllexport) bool __stdcall GetSerialData(int pPortNo, double *pData, char *pStatus);			// 連続データ読込み
extern "C" __declspec(dllexport) bool __stdcall GetLatestData(int pPortNo, double *pData, char *pStatus);			// 最新データ読込
extern "C" __declspec(dllexport) bool __stdcall GetSensorLimit(int pPortNo, double *pData);					// センサ定格確認
extern "C" __declspec(dllexport) bool __stdcall GetSensorInfo(int pPortNo, char *psSerial);					// センサシリアルNo取得

// EXM003用ライブラリ
extern "C" __declspec(dllexport) bool __stdcall GetSensorInfo4ch(int pPortNo, int pChNo, char *psType, char *psSerial);		// センサシリアルNo取得
extern "C" __declspec(dllexport) bool __stdcall GetSensorLimit4ch(int pPortNo, int pChNo, double *pData);			// センサ定格確認
extern "C" __declspec(dllexport) bool __stdcall SetSerialMode4ch(int pPortNo, bool pSet);					// データの連続読込の開始/停止
extern "C" __declspec(dllexport) bool __stdcall GetSerialData4ch(int pPortNo, double pData[][6], char *pStatus);		// 連続データ読込み
extern "C" __declspec(dllexport) bool __stdcall GetLatestData4ch(int pPortNo, double pData[][6], char *pStatus);		// 最新データ読込
