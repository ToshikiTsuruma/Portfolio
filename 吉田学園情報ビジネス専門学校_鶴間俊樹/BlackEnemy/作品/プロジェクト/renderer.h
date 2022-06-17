//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FOG_DISTANCE (3000.0f)
#define MIN_FOG_DISTANCE (500.0f)

//*****************************************************************************
// �����_�����O�N���X
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();	//�f�t�H���g�R���X�g���N�^
	~CRenderer();	//�f�X�g���N�^
	HRESULT Init(HWND hWnd, bool bWindow);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	//�f�o�C�X���擾

private:

	LPDIRECT3D9	m_pD3D;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
	void DrawFPS(void);	//FPS�`�揈��
	LPD3DXFONT m_pFont = NULL;			// �t�H���g�ւ̃|�C���^
#endif

};

#endif // !_RENDERER_H_