#include "stdafx.h"
#include "system/system.h"
#include "effect/Effect.h"

#include "SkinModelRender.h"
#include "RenderingEngine.h"
#include "Player.h"
#include "Ground.h"
#include "SkyCube.h"
#include "Enemy.h"
#include "UI.h"
#include "CaptureStateManager.h"

// �E�B���h�E�v���O�����̃��C���֐��B
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	//PhysicsWorld::CreateInstance();
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




	CaptureStateManager::CreateInstance();
	Player* player;
	player = NewGO<Player>(0,"player");

	Ground* ground = nullptr;
	ground = NewGO<Ground>(0);

	SkyCube* skyCube = nullptr;
	skyCube = NewGO<SkyCube>(0);

	Enemy* enemy[3] = { nullptr };
	enemy[0] = NewGO<Enemy>(0);
	enemy[1] = NewGO<Enemy>(0);
	enemy[1]->SetPosition({ 1000.0f,1000.0f,10000.0f });
	enemy[2] = NewGO<Enemy>(0);
	enemy[2]->SetPosition({ 10000.0f,3000.0f,30000.0f });

	UI* ui = nullptr;
	ui = NewGO<UI>(0);

	SkinModelRender* sun;
	sun = NewGO<SkinModelRender>(0);
	sun->Init("Assets/modelData/sun/sun.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, false);
	sun->SetPosition({ 0.0f,1000000.0f,0.0f });
	sun->SetScale({ 600.0f,600.0f,600.0f });
	sun->SetIsSun();

	g_camera3D->SetFar(1000000.0f);
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
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


		enemy[1]->SetPosition({ 1000.0f,1000.0f,10000.0f });
		enemy[2]->SetPosition({ 10000.0f,3000.0f,30000.0f });





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

		//�Q�[���^�C����p���ăG�t�F�N�g�̍Đ��i�s�x�̍X�V
		EffectEngine::GetInstance()->Update(GameTime().GetFrameDeltaTime());
		//�G�t�F�N�g�̕`��
		EffectEngine::GetInstance()->Draw();

		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	//�T�E���h�G���W���̏���
	CSoundEngine::DeleteInstance();
	//PhysicsWorld::DeleteInstance();
	//RenderingEngine::DeleteInstance();
	LightManager::DeleteInstance();
	return 0;
}