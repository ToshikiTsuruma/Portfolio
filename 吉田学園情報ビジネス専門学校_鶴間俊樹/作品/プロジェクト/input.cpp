//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "input.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//******************************************************************************************************************
// インプットクラス
//******************************************************************************************************************
//=============================================================================
// インプットのデフォルトコンストラクタ
//=============================================================================
CInput::CInput()
{
	m_pDevice = nullptr;
}

//=============================================================================
// インプットのデストラクタ
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// インプットの初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd) {
	if (m_pInput == nullptr) {
		//DirectInputオブジェクトの作成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL))) {
			return E_FAIL;
		}
	}
	return S_OK;
}

//=============================================================================
// インプットの終了処理
//=============================================================================
void CInput::Uninit(void) {
	//入力デバイスの開放
	if (m_pDevice != nullptr) {
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	//DirectInputオブジェクトの開放
	if (m_pInput != nullptr) {
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//更新処理
void CInput::Update(void){}
//入力判定（プレス）
bool CInput::GetPress(CODE code) { return false; }
//入力判定（トリガー）
bool CInput::GetTrigger(CODE code) { return false; }


//******************************************************************************************************************
// キーボードクラス
//******************************************************************************************************************
//=============================================================================
// キーボードのデフォルトコンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//キー情報のクリア
	memset(&m_aKeyState[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));
}

//=============================================================================
// キーボードのデストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd) {
	//インプットの初期化
	CInput::Init(hInstance, hWnd);
	//キーボードの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL))) {
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void CInputKeyboard::Uninit(void) {
	CInput::Uninit();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void CInputKeyboard::Update(void) {
	BYTE aKeyState[256];
	int nCntKey;
	//入力デバイスからデータ取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0]))) {
		for (nCntKey = 0; nCntKey < 256; nCntKey++) {
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];//キーボードの入力情報保存
		}
	}
	else {
		m_pDevice->Acquire();
	}
}

//=============================================================================
// キーボードのプレス判定
//=============================================================================
bool CInputKeyboard::GetPress(int nKey) {
	return m_aKeyState[nKey] & 0x80;
}

//=============================================================================
// キーボードのトリガー判定
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey) {
	return m_aKeyStateTrigger[nKey] & 0x80;
}

//=============================================================================
// キーボードの入力判定（プレス）
//=============================================================================
bool CInputKeyboard::GetPress(CODE code) {
	switch (code)
	{
		//選択
	case CODE::SELECT:
		if (GetPress(DIK_SPACE) || GetPress(DIK_RETURN)) {
			return true;
		}
		break;

		//戻る
	case CODE::BACK:
		if (GetPress(DIK_ESCAPE)) {
			return true;
		}
		break;

		//上下左右
	case CODE::UP:
		if (GetPress(DIK_UP) || GetPress(DIK_W)) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetPress(DIK_DOWN) || GetPress(DIK_S)) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetPress(DIK_LEFT) || GetPress(DIK_A)) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetPress(DIK_RIGHT) || GetPress(DIK_D)) {
			return true;
		}
		break;

		//インタラクト
	case CODE::INTERACT:
		if (GetPress(DIK_E)) {
			return true;
		}
		break;
		//ダッシュ
	case CODE::DASH:
		if (GetPress(DIK_LSHIFT)) {
			return true;
		}
		break;
		//回避
	case CODE::DODGE:
		if (GetPress(DIK_SPACE)) {
			return true;
		}
		break;
		//アイテム使用
	case CODE::USE_ITME:
		if (GetPress(DIK_O)) {
			return true;
		}
		break;

		//ポーズ
	case CODE::PAUSE:
		if (GetPress(DIK_P)) {
			return true;
		}
		break;

		//カメラの上下左右
	case CODE::CAMERA_UP:
		if (GetPress(DIK_I)) {
			return true;
		}
		break;
	case CODE::CAMERA_DOWN:
		if (GetPress(DIK_K)) {
			return true;
		}
		break;
	case CODE::CAMERA_LEFT:
		if (GetPress(DIK_J)) {
			return true;
		}
		break;
	case CODE::CAMERA_RIGHT:
		if (GetPress(DIK_L)) {
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// キーボードの入力判定（トリガー）
//=============================================================================
bool CInputKeyboard::GetTrigger(CODE code) {
	switch (code)
	{
		//選択
	case CODE::SELECT:
		if (GetTrigger(DIK_SPACE) || GetTrigger(DIK_RETURN)) {
			return true;
		}
		break;

		//戻る
	case CODE::BACK:
		if (GetTrigger(DIK_ESCAPE)) {
			return true;
		}
		break;

		//上下左右
	case CODE::UP:
		if (GetTrigger(DIK_UP) || GetTrigger(DIK_W)) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetTrigger(DIK_DOWN) || GetTrigger(DIK_S)) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetTrigger(DIK_LEFT) || GetTrigger(DIK_A)) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetTrigger(DIK_RIGHT) || GetTrigger(DIK_D)) {
			return true;
		}
		break;

		//インタラクト
	case CODE::INTERACT:
		if (GetTrigger(DIK_E)) {
			return true;
		}
		break;
		//回避
	case CODE::DODGE:
		if (GetTrigger(DIK_SPACE)) {
			return true;
		}
		break;
		//攻撃
	case CODE::ATTACK:
		if (GetTrigger(DIK_U)) {
			return true;
		}
		break;
		//アイテムの使用
	case CODE::USE_ITME:
		if (GetTrigger(DIK_O)) {
			return true;
		}
		break;
		//武器の変更
	case CODE::CHANGE_WEAPON:
		if (GetTrigger(DIK_Q)) {
			return true;
		}
		break;

		//ポーズ
	case CODE::PAUSE:
		if (GetTrigger(DIK_P)) {
			return true;
		}
		break;
	}
	return false;
}

//******************************************************************************************************************
// ゲームパッドクラス(Xinput)
//******************************************************************************************************************
//=============================================================================
// マクロ定義(Xinput)
//=============================================================================
//標準のマクロ定義になかったものを定義
#define XINPUT_GAMEPAD_L (0x100)	// Lボタン
#define XINPUT_GAMEPAD_R (0x200)	// Rボタン

//=============================================================================
// ゲームパッドのデフォルトコンストラクタ(Xinput)
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
// ゲームパッドのデストラクタ(Xinput)
//=============================================================================
CInputGamepadX::~CInputGamepadX()
{
}

//=============================================================================
// ゲームパッドの初期化処理(Xinput)
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
// ゲームパッドの終了処理(Xinput)
//=============================================================================
void CInputGamepadX::Uninit(void) {
	CInput::Uninit();
}

//=============================================================================
// ゲームパッドの更新処理(Xinput)
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

	//バイブレーションのカウント
	if (m_nCntVibration > 0) {
		m_nCntVibration--;
	}
	//バイブレーションの終了
	if (m_nCntVibration <= 0 && m_bVibration) {
		SetVibration(0, 0, 0);
		m_bVibration = false;
	}
}

//=============================================================================
// ゲームパッド接続の判定(Xinput)
//=============================================================================
bool CInputGamepadX::GetConnectGamepad(void) {
	return m_bConnect;
}

//=============================================================================
// ゲームパッドのボタンのプレス判定(Xinput)
//=============================================================================
bool CInputGamepadX::GetButtonPress(int nButton) {
	if (!m_bConnect) return false;	//接続されていない場合falseを返す
	return m_state.Gamepad.wButtons & nButton;
}

//=============================================================================
// ゲームパッドのボタンのトリガー判定(Xinput)
//=============================================================================
bool CInputGamepadX::GetButtonTrigger(int nButton) {
	if (!m_bConnect) return false;	//接続されていない場合falseを返す
	return !(m_stateLast.Gamepad.wButtons & nButton) && (m_state.Gamepad.wButtons & nButton);
}

//=============================================================================
// ゲームパッドのトリガーのプレス判定(Xinput)
//=============================================================================
bool CInputGamepadX::GetTriggerPress(TRIGGER_TYPE type) {
	if (!m_bConnect) return false;	//接続されていない場合falseを返す
	switch (type)
	{
	case TRIGGER_TYPE::RIGHT:
		return m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		break;
	case TRIGGER_TYPE::LEFT:
		return m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		break;
	default:
		return false;
		break;
	}
}

//=============================================================================
// ゲームパッドのトリガーのトリガー判定(Xinput)
//=============================================================================
bool CInputGamepadX::GetTriggerTrigger(TRIGGER_TYPE type) {
	if (!m_bConnect) return false;	//接続されていない場合falseを返す
	switch (type)
	{
	case TRIGGER_TYPE::RIGHT:
		return !(m_stateLast.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && (m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		break;
	case TRIGGER_TYPE::LEFT:
		return !(m_stateLast.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && (m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
		break;
	default:
		return false;
		break;
	}
}

//=============================================================================
// ゲームパッドの左スティックの判定(Xinput)
//=============================================================================
bool CInputGamepadX::GetLeftStick(STICK_TYPE type) {
	if (!m_bConnect) return false;	//接続されていない場合falseを返す

	switch (type)
	{
	case STICK_TYPE::UP:
		return m_state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case STICK_TYPE::DOWN:
		return m_state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case STICK_TYPE::LEFT:
		return m_state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break; 
	case STICK_TYPE::RIGHT:
		return m_state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;

	default:
		return false;
		break;
	}
}

//=============================================================================
// ゲームパッドの左スティックの判定(Xinput)
//=============================================================================
bool CInputGamepadX::GetRightStick(STICK_TYPE type) {
	if (!m_bConnect) return false;	//接続されていない場合falseを返す
	switch ((STICK_TYPE)type)
	{
	case STICK_TYPE::UP:
		return m_state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case STICK_TYPE::DOWN:
		return m_state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case STICK_TYPE::LEFT:
		return m_state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case STICK_TYPE::RIGHT:
		return m_state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;

	default:
		return false;
		break;
	}
}

//=============================================================================
// ゲームパッドの入力判定（プレス）
//=============================================================================
bool CInputGamepadX::GetPress(CODE code) {
	switch (code)
	{
		//選択
	case CODE::SELECT:
		if (GetButtonPress(XINPUT_GAMEPAD_A) || GetButtonPress(XINPUT_GAMEPAD_START)) {
			return true;
		}
		break;

		//戻る
	case CODE::BACK:
		if (GetButtonPress(XINPUT_GAMEPAD_B) || GetButtonPress(XINPUT_GAMEPAD_BACK)) {
			return true;
		}
		break;

		//上下左右 プレスでDPADを使用することは今のところないのでスティックにしている
	case CODE::UP:
		if (GetLeftStick(STICK_TYPE::UP)) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetLeftStick(STICK_TYPE::DOWN)) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetLeftStick(STICK_TYPE::LEFT)) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetLeftStick(STICK_TYPE::RIGHT)) {
			return true;
		}
		break;

		//インタラクト
	case CODE::INTERACT:
		if (GetButtonPress(XINPUT_GAMEPAD_A)) {
			return true;
		}
		break;
		//ダッシュ
	case CODE::DASH:
		if (GetTriggerPress(TRIGGER_TYPE::LEFT)) {
			return true;
		}
		break;
		//回避
	case CODE::DODGE:
		if (GetButtonPress(XINPUT_GAMEPAD_L)) {
			return true;
		}
		break;
		//アイテムの使用
	case CODE::USE_ITME:
		if (GetButtonPress(XINPUT_GAMEPAD_B)) {
			return true;
		}
		break;

		//ポーズ
	case CODE::PAUSE:
		if (GetButtonPress(XINPUT_GAMEPAD_START)) {
			return true;
		}
		break;

		//カメラの上下左右
	case CODE::CAMERA_UP:
		if (GetRightStick(STICK_TYPE::UP)) {
			return true;
		}
		break;
	case CODE::CAMERA_DOWN:
		if (GetRightStick(STICK_TYPE::DOWN)) {
			return true;
		}
		break;
	case CODE::CAMERA_LEFT:
		if (GetRightStick(STICK_TYPE::LEFT)) {
			return true;
		}
		break;
	case CODE::CAMERA_RIGHT:
		if (GetRightStick(STICK_TYPE::RIGHT)) {
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// ゲームパッドの入力判定（トリガー）
//=============================================================================
bool CInputGamepadX::GetTrigger(CODE code) {
	switch (code)
	{
		//選択
	case CODE::SELECT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_A) || GetButtonTrigger(XINPUT_GAMEPAD_START)) {
			return true;
		}
		break;

		//戻る
	case CODE::BACK:
		if (GetButtonTrigger(XINPUT_GAMEPAD_B) || GetButtonTrigger(XINPUT_GAMEPAD_BACK)) {
			return true;
		}
		break;

		//上下左右
	case CODE::UP:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP)) {
			return true;
		}
		break;
	case CODE::DOWN:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN)) {
			return true;
		}
		break;
	case CODE::LEFT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT)) {
			return true;
		}
		break;
	case CODE::RIGHT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			return true;
		}
		break;

		//インタラクト
	case CODE::INTERACT:
		if (GetButtonTrigger(XINPUT_GAMEPAD_A)) {
			return true;
		}
		break;
		//回避
	case CODE::DODGE:
		if (GetButtonTrigger(XINPUT_GAMEPAD_L)) {
			return true;
		}
		break;
		//攻撃
	case CODE::ATTACK:
		if (GetButtonTrigger(XINPUT_GAMEPAD_R)) {
			return true;
		}
		break;
		//アイテムの使用
	case CODE::USE_ITME:
		if (GetButtonTrigger(XINPUT_GAMEPAD_B)) {
			return true;
		}
		break;
		//武器の変更
	case CODE::CHANGE_WEAPON:
		if (GetButtonTrigger(XINPUT_GAMEPAD_X)) {
			return true;
		}
		break;

		//ポーズ
	case CODE::PAUSE:
		if (GetButtonTrigger(XINPUT_GAMEPAD_START)) {
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// ゲームパッドのバイブレーションの設定(Xinput)
//=============================================================================
void CInputGamepadX::SetVibration(int nLeftValue, int nRightValue, int nCntVibration) {
	m_vibration.wLeftMotorSpeed = nLeftValue; // use any value between 0-65535 here
	m_vibration.wRightMotorSpeed = nRightValue; // use any value between 0-65535 here
	XInputSetState(0, &m_vibration);
	m_nCntVibration = nCntVibration;
	m_bVibration = true;
}