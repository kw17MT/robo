#include "stdafx.h"
#include "system/system.h"
#include "RenderingEngine.h"
#include "CaptureStateManager.h"

#include "effect/Effect.h"

#include "Game.h"
#include "Title.h"
#include "GameDirector.h"

// �E�B���h�E�v���O�����̃��C���֐��B
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("SkySentinel"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	//�����_�����O�𓝊�����C���X�^���X�B
	RenderingEngine::CreateInstance();
	//�����_�����O�ɂ��������̂̏������B
	RenderingEngine::GetInstance()->PrepareRendering();
	//���[���h��̃��C�e�B���O�֘A�̃C���X�^���X�B
	LightManager::CreateInstance();
	//�T�E���h��炷�p�̃C���X�^���X
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();
	//�G�t�F�N�g���o���C���X�^���X
	EffectEngine::CreateInstance();
	//�Q�[���^�C���𑪂����
	CStopwatch stopWatch;

	Title* title = NewGO<Title>(0);
	//Game* game = NewGO<Game>(0, "game");

	g_camera3D->SetPosition({ 0.0f, 0.0f, 3000.0f });
	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		if (GameDirector::GetInstance().GetGameScene() == enExit)
		{
			break;
		}
		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		//�I�u�W�F�N�g�̃A�b�v�f�[�g�֐�����C�ɍs��
		GameObjectManager::GetInstance()->ExecuteUpdate();

		//�I�u�W�F�N�g�̃h���[���s��
		RenderingEngine::GetInstance()->Render(renderContext);
		LightManager::GetInstance().UpdateEyePos();

		//�X�s�����b�N���s���B
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = 16 - (int)stopWatch.GetElapsedMillisecond();
		} while (restTime > 0);
		//�X�g�b�v�E�H�b�`�̌v���I��
		stopWatch.Stop();
		//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
		GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());

		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	//�T�E���h�G���W���̏���
	CSoundEngine::DeleteInstance();
	PhysicsWorld::DeleteInstance();
	//RenderingEngine::DeleteInstance();
	LightManager::DeleteInstance();
	return 0;
}