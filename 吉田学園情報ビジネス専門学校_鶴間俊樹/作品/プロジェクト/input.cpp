//=============================================================================
//
// ���͏��� [input.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "input.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//******************************************************************************************************************
// �C���v�b�g�N���X
//******************************************************************************************************************
//=============================================================================
// �C���v�b�g�̃f�t�H���g�R���X�g���N�^
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
// �C���v�b�g�̃f�X�g���N�^
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// �C���v�b�g�̏���������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd) {
	if (m_pInput == NULL) {
		//DirectInput�I�u�W�F�N�g�̍쐬
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL))) {
			return E_FAIL;
		}
	}
	return S_OK;
}

//=============================================================================
// �C���v�b�g�̏I������
//=============================================================================
void CInput::Uninit(void) {
	//���̓f�o�C�X�̊J��
	if (m_pDevice != NULL) {
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL) {
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//�X�V����
void CInput::Update(void){}
//���͔���i�v���X�j
bool CInput::GetPress(CODE code) { return false; }
//���͔���i�g���K�[�j
bool CInput::GetTrigger(CODE code) { return false; }


//******************************************************************************************************************
// �L�[�{�[�h�N���X
//******************************************************************************************************************
//=============================================================================
// �L�[�{�[�h�̃f�t�H���g�R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//�L�[���̃N���A
	memset(&m_aKeyState[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));
}

//=============================================================================
// �L�[�{�[�h�̃f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// �L�[�{�[�h�̏���������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd) {
	//�C���v�b�g�̏�����
	CInput::Init(hInstance, hWnd);
	//�L�[�{�[�h�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL))) {
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void CInputKeyboard::Uninit(void) {
	CInput::Uninit();
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void CInputKeyboard::Update(void) {
	BYTE aKeyState[256];
	int nCntKey;
	//���̓f�o�C�X����f�[�^�擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0]))) {
		for (nCntKey = 0; nCntKey < 256; nCntKey++) {
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];//�L�[�{�[�h�̓��͏��ۑ�
		}
	}
	else {
		m_pDevice->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X����
//=============================================================================
bool CInputKeyboard::GetPress(int nKey) {
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[����
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey) {
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̓��͔���i�v���X�j
//=============================================================================
bool CInputKeyboard::GetPress(CODE code) {
	switch (code)
	{
		//�I��
	case CODE::SELECT:
		if (GetPress(DIK_SPACE) == true || GetPress(DIK_RETURN) == true) {
			return true;
		}
		break;

		//�߂�
	case CODE::BACK:
		if (GetPress(DIK_ESCAPE) == true) {
			return true;
		}
		break;

		//�㉺���E
	case CODE::UP:
		if (GetPress(DIK_UP) == true || GetPress(DIK_W) == true) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetPress(DIK_DOWN) == true || GetPress(DIK_S) == true) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetPress(DIK_LEFT) == true || GetPress(DIK_A) == true) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetPress(DIK_RIGHT) == true || GetPress(DIK_D) == true) {
			return true;
		}
		break;

		//�C���^���N�g
	case CODE::INTERACT:
		if (GetPress(DIK_E) == true) {
			return true;
		}
		break;
		//�_�b�V��
	case CODE::DASH:
		if (GetPress(DIK_LSHIFT) == true) {
			return true;
		}
		break;
		//���
	case CODE::DODGE:
		if (GetPress(DIK_SPACE) == true) {
			return true;
		}
		break;
		//�A�C�e���g�p
	case CODE::USE_ITME:
		if (GetPress(DIK_O) == true) {
			return true;
		}
		break;

		//�|�[�Y
	case CODE::PAUSE:
		if (GetPress(DIK_P) == true) {
			return true;
		}
		break;

		//�J�����̏㉺���E
	case CODE::CAMERA_UP:
		if (GetPress(DIK_I) == true) {
			return true;
		}
		break;
	case CODE::CAMERA_DOWN:
		if (GetPress(DIK_K) == true) {
			return true;
		}
		break;
	case CODE::CAMERA_LEFT:
		if (GetPress(DIK_J) == true) {
			return true;
		}
		break;
	case CODE::CAMERA_RIGHT:
		if (GetPress(DIK_L) == true) {
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̓��͔���i�g���K�[�j
//=============================================================================
bool CInputKeyboard::GetTrigger(CODE code) {
	switch (code)
	{
		//�I��
	case CODE::SELECT:
		if (GetTrigger(DIK_SPACE) == true || GetTrigger(DIK_RETURN) == true) {
			return true;
		}
		break;

		//�߂�
	case CODE::BACK:
		if (GetTrigger(DIK_ESCAPE) == true) {
			return true;
		}
		break;

		//�㉺���E
	case CODE::UP:
		if (GetTrigger(DIK_UP) == true || GetTrigger(DIK_W) == true) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetTrigger(DIK_DOWN) == true || GetTrigger(DIK_S) == true) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetTrigger(DIK_LEFT) == true || GetTrigger(DIK_A) == true) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetTrigger(DIK_RIGHT) == true || GetTrigger(DIK_D) == true) {
			return true;
		}
		break;

		//�C���^���N�g
	case CODE::INTERACT:
		if (GetTrigger(DIK_E) == true) {
			return true;
		}
		break;
		//���
	case CODE::DODGE:
		if (GetTrigger(DIK_SPACE) == true) {
			return true;
		}
		break;
		//�U��
	case CODE::ATTACK:
		if (GetTrigger(DIK_U) == true) {
			return true;
		}
		break;
		//�A�C�e���̎g�p
	case CODE::USE_ITME:
		if (GetTrigger(DIK_O) == true) {
			return true;
		}
		break;
		//����̕ύX
	case CODE::CHANGE_WEAPON:
		if (GetTrigger(DIK_Q) == true) {
			return true;
		}
		break;

		//�|�[�Y
	case CODE::PAUSE:
		if (GetTrigger(DIK_P) == true) {
			return true;
		}
		break;
	}
	return false;
}

//******************************************************************************************************************
// �Q�[���p�b�h�N���X(Xinput)
//******************************************************************************************************************
//=============================================================================
// �}�N����`(Xinput)
//=============================================================================
//�W���̃}�N����`�ɂȂ��������̂��`
#define XINPUT_GAMEPAD_L (0x100)	// L�{�^��
#define XINPUT_GAMEPAD_R (0x200)	// R�{�^��

//=============================================================================
// �Q�[���p�b�h�̃f�t�H���g�R���X�g���N�^(Xinput)
//=============================================================================
CInputGamepadX::CInputGamepadX()
{
	ZeroMemory(&m_state, sizeof(XINPUT_STATE));
	ZeroMemory(&m_stateLast, sizeof(XINPUT_STATE));
	ZeroMemory(&m_vibration, sizeof(XINPUT_VIBRATION));
	m_bConnect = false;
	m_nCntVibration = 0;
	m_bVibration = false;
}

//=============================================================================
// �Q�[���p�b�h�̃f�X�g���N�^(Xinput)
//=============================================================================
CInputGamepadX::~CInputGamepadX()
{
}

//=============================================================================
// �Q�[���p�b�h�̏���������(Xinput)
//=============================================================================
void CInputGamepadX::Init(void) {
	ZeroMemory(&m_state, sizeof(XINPUT_STATE));
	ZeroMemory(&m_stateLast, sizeof(XINPUT_STATE));
	ZeroMemory(&m_vibration, sizeof(XINPUT_VIBRATION));
	m_bConnect = false;
	m_nCntVibration = 0;
	m_bVibration = false;
}

//=============================================================================
// �Q�[���p�b�h�̏I������(Xinput)
//=============================================================================
void CInputGamepadX::Uninit(void) {
	CInput::Uninit();
}

//=============================================================================
// �Q�[���p�b�h�̍X�V����(Xinput)
//=============================================================================
void CInputGamepadX::Update(void) {
	XINPUT_STATE state;

	DWORD dwResult = XInputGetState(0, &state);

	m_stateLast = m_state;
	m_state = state;

	if (dwResult == ERROR_SUCCESS)
	{
		// Controller is connected
		m_bConnect = true;
	}
	else
	{
		// Controller is not connected
		m_bConnect = false;
	}

	// Zero value if thumbsticks are within the dead zone
	if ((m_state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		m_state.Gamepad.sThumbLX = 0;
		m_state.Gamepad.sThumbLY = 0;
	}

	//�o�C�u���[�V�����̃J�E���g
	if (m_nCntVibration > 0) {
		m_nCntVibration--;
	}
	//�o�C�u���[�V�����̏I��
	if (m_nCntVibration <= 0 && m_bVibration == true) {
		SetVibration(0, 0, 0);
		m_bVibration = false;
	}
}

//=============================================================================
// �Q�[���p�b�h�ڑ��̔���(Xinput)
//=============================================================================
bool CInputGamepadX::GetConnectGamepad(void) {
	return m_bConnect;
}

//=============================================================================
// �Q�[���p�b�h�̃{�^���̃v���X����(Xinput)
//=============================================================================
bool CInputGamepadX::GetButtonPress(int nButton) {
	if (m_bConnect == false) return false;	//�ڑ�����Ă��Ȃ��ꍇfalse��Ԃ�
	return m_state.Gamepad.wButtons & nButton ? true : false;
}

//=============================================================================
// �Q�[���p�b�h�̃{�^���̃g���K�[����(Xinput)
//=============================================================================
bool CInputGamepadX::GetButtonTrigger(int nButton) {
	if (m_bConnect == false) return false;	//�ڑ�����Ă��Ȃ��ꍇfalse��Ԃ�
	return (((m_stateLast.Gamepad.wButtons & nButton) == false) && (m_state.Gamepad.wButtons & nButton)) ? true : false;
}

//=============================================================================
// �Q�[���p�b�h�̃g���K�[�̃v���X����(Xinput)
//=============================================================================
bool CInputGamepadX::GetTriggerPress(TRIGGER_TYPE type) {
	if (m_bConnect == false) return false;	//�ڑ�����Ă��Ȃ��ꍇfalse��Ԃ�
	switch (type)
	{
	case TRIGGER_TYPE::RIGHT:
		return m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? true : false;
		break;
	case TRIGGER_TYPE::LEFT:
		return m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? true : false;
		break;
	default:
		return false;
		break;
	}
}

//=============================================================================
// �Q�[���p�b�h�̃g���K�[�̃g���K�[����(Xinput)
//=============================================================================
bool CInputGamepadX::GetTriggerTrigger(TRIGGER_TYPE type) {
	if (m_bConnect == false) return false;	//�ڑ�����Ă��Ȃ��ꍇfalse��Ԃ�
	switch (type)
	{
	case TRIGGER_TYPE::RIGHT:
		return (((m_stateLast.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) == false) && (m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) ? true : false;
		break;
	case TRIGGER_TYPE::LEFT:
		return (((m_stateLast.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) == false) && (m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) ? true : false;
		break;
	default:
		return false;
		break;
	}
}

//=============================================================================
// �Q�[���p�b�h�̍��X�e�B�b�N�̔���(Xinput)
//=============================================================================
bool CInputGamepadX::GetLeftStick(STICK_TYPE type) {
	if (m_bConnect == false) return false;	//�ڑ�����Ă��Ȃ��ꍇfalse��Ԃ�

	switch (type)
	{
	case STICK_TYPE::UP:
		return m_state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case STICK_TYPE::DOWN:
		return m_state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case STICK_TYPE::LEFT:
		return m_state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break; 
	case STICK_TYPE::RIGHT:
		return m_state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;

	default:
		return false;
		break;
	}
}

//=============================================================================
// �Q�[���p�b�h�̍��X�e�B�b�N�̔���(Xinput)
//=============================================================================
bool CInputGamepadX::GetRightStick(STICK_TYPE type) {
	if (m_bConnect == false) return false;	//�ڑ�����Ă��Ȃ��ꍇfalse��Ԃ�
	switch ((STICK_TYPE)type)
	{
	case STICK_TYPE::UP:
		return m_state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case STICK_TYPE::DOWN:
		return m_state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case STICK_TYPE::LEFT:
		return m_state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case STICK_TYPE::RIGHT:
		return m_state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;

	default:
		return false;
		break;
	}
}

//=============================================================================
// �Q�[���p�b�h�̓��͔���i�v���X�j
//=============================================================================
bool CInputGamepadX::GetPress(CODE code) {
	switch (code)
	{
		//�I��
	case CODE::SELECT:
		if (GetButtonPress(XINPUT_GAMEPAD_A) == true || GetButtonPress(XINPUT_GAMEPAD_START) == true) {
			return true;
		}
		break;

		//�߂�
	case CODE::BACK:
		if (GetButtonPress(XINPUT_GAMEPAD_B) == true || GetButtonPress(XINPUT_GAMEPAD_BACK) == true) {
			return true;
		}
		break;

		//�㉺���E �v���X��DPAD���g�p���邱�Ƃ͍��̂Ƃ���Ȃ��̂ŃX�e�B�b�N�ɂ��Ă���
	case CODE::UP:
		if (GetLeftStick(STICK_TYPE::UP) == true) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetLeftStick(STICK_TYPE::DOWN) == true) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetLeftStick(STICK_TYPE::LEFT) == true) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetLeftStick(STICK_TYPE::RIGHT) == true) {
			return true;
		}
		break;

		//�C���^���N�g
	case CODE::INTERACT:
		if (GetButtonPress(XINPUT_GAMEPAD_A) == true) {
			return true;
		}
		break;
		//�_�b�V��
	case CODE::DASH:
		if (GetTriggerPress(TRIGGER_TYPE::LEFT) == true) {
			return true;
		}
		break;
		//���
	case CODE::DODGE:
		if (GetButtonPress(XINPUT_GAMEPAD_L) == true) {
			return true;
		}
		break;
		//�A�C�e���̎g�p
	case CODE::USE_ITME:
		if (GetButtonPress(XINPUT_GAMEPAD_B) == true) {
			return true;
		}
		break;

		//�|�[�Y
	case CODE::PAUSE:
		if (GetButtonPress(XINPUT_GAMEPAD_START) == true) {
			return true;
		}
		break;

		//�J�����̏㉺���E
	case CODE::CAMERA_UP:
		if (GetRightStick(STICK_TYPE::UP) == true) {
			return true;
		}
		break;
	case CODE::CAMERA_DOWN:
		if (GetRightStick(STICK_TYPE::DOWN) == true) {
			return true;
		}
		break;
	case CODE::CAMERA_LEFT:
		if (GetRightStick(STICK_TYPE::LEFT) == true) {
			return true;
		}
		break;
	case CODE::CAMERA_RIGHT:
		if (GetRightStick(STICK_TYPE::RIGHT) == true) {
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// �Q�[���p�b�h�̓��͔���i�g���K�[�j
//=============================================================================
bool CInputGamepadX::GetTrigger(CODE code) {
	switch (code)
	{
		//�I��
	case CODE::SELECT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_A) == true || GetButtonTrigger(XINPUT_GAMEPAD_START) == true) {
			return true;
		}
		break;

		//�߂�
	case CODE::BACK:
		if (GetButtonTrigger(XINPUT_GAMEPAD_B) == true || GetButtonTrigger(XINPUT_GAMEPAD_BACK) == true) {
			return true;
		}
		break;

		//�㉺���E
	case CODE::UP:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP) == true) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT) == true) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) == true) {
			return true;
		}
		break;

		//�C���^���N�g
	case CODE::INTERACT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_A) == true) {
			return true;
		}
		break;
		//���
	case CODE::DODGE:
		if (GetButtonTrigger(XINPUT_GAMEPAD_L) == true) {
			return true;
		}
		break;
		//�U��
	case CODE::ATTACK:
		if (GetButtonTrigger(XINPUT_GAMEPAD_R) == true) {
			return true;
		}
		break;
		//�A�C�e���̎g�p
	case CODE::USE_ITME:
		if (GetButtonTrigger(XINPUT_GAMEPAD_B) == true) {
			return true;
		}
		break;
		//����̕ύX
	case CODE::CHANGE_WEAPON:
		if (GetButtonTrigger(XINPUT_GAMEPAD_X) == true) {
			return true;
		}
		break;

		//�|�[�Y
	case CODE::PAUSE:
		if (GetButtonTrigger(XINPUT_GAMEPAD_START) == true) {
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// �Q�[���p�b�h�̃o�C�u���[�V�����̐ݒ�(Xinput)
//=============================================================================
void CInputGamepadX::SetVibration(int nLeftValue, int nRightValue, int nCntVibration) {
	m_vibration.wLeftMotorSpeed = nLeftValue; // use any value between 0-65535 here
	m_vibration.wRightMotorSpeed = nRightValue; // use any value between 0-65535 here
	XInputSetState(0, &m_vibration);
	m_nCntVibration = nCntVibration;
	m_bVibration = true;
}