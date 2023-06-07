/******************************************************************************/
/*                                                                            */
/*                  ���v�g���m�� �͊o�Z���T�ʐM���C�u����                     */
/*                                                                            */
/******************************************************************************/
// ���ʃ��C�u����
extern "C" __declspec(dllexport) int  __stdcall GetDllVersion();
extern "C" __declspec(dllexport) void __stdcall Initialize();									// �c�k�k�̏���������
extern "C" __declspec(dllexport) void __stdcall Finalize();									// �c�k�k�̏I������
extern "C" __declspec(dllexport) bool __stdcall PortOpen(int pPortNo);								// �|�[�g�I�[�v��
extern "C" __declspec(dllexport) void __stdcall PortClose(int pPortNo);								// �|�[�g�N���[�Y
extern "C" __declspec(dllexport) bool __stdcall SetDFilter(int pPort, INT16 pIn);						// �t�B���^�I��
extern "C" __declspec(dllexport) bool __stdcall GetDFilter(int pPort, INT16 *pOut);						// �t�B���^�m�F

// 6���͊o�Z���T�p���C�u����
extern "C" __declspec(dllexport) bool __stdcall SetSerialMode(int pPortNo, bool pSet);						// �f�[�^�̘A���Ǎ��̊J�n/��~
extern "C" __declspec(dllexport) bool __stdcall GetSerialData(int pPortNo, double *pData, char *pStatus);			// �A���f�[�^�Ǎ���
extern "C" __declspec(dllexport) bool __stdcall GetLatestData(int pPortNo, double *pData, char *pStatus);			// �ŐV�f�[�^�Ǎ�
extern "C" __declspec(dllexport) bool __stdcall GetSensorLimit(int pPortNo, double *pData);					// �Z���T��i�m�F
extern "C" __declspec(dllexport) bool __stdcall GetSensorInfo(int pPortNo, char *psSerial);					// �Z���T�V���A��No�擾

// EXM003�p���C�u����
extern "C" __declspec(dllexport) bool __stdcall GetSensorInfo4ch(int pPortNo, int pChNo, char *psType, char *psSerial);		// �Z���T�V���A��No�擾
extern "C" __declspec(dllexport) bool __stdcall GetSensorLimit4ch(int pPortNo, int pChNo, double *pData);			// �Z���T��i�m�F
extern "C" __declspec(dllexport) bool __stdcall SetSerialMode4ch(int pPortNo, bool pSet);					// �f�[�^�̘A���Ǎ��̊J�n/��~
extern "C" __declspec(dllexport) bool __stdcall GetSerialData4ch(int pPortNo, double pData[][6], char *pStatus);		// �A���f�[�^�Ǎ���
extern "C" __declspec(dllexport) bool __stdcall GetLatestData4ch(int pPortNo, double pData[][6], char *pStatus);		// �ŐV�f�[�^�Ǎ�
