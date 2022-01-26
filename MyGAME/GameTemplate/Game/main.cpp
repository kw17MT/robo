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

	//Title* title;
	//title = NewGO<Title>(0);
	Game* a = NewGO<Game>(0, "game");

	g_camera3D->SetPosition({ 0.0f, 0.0f, 3000.0f });
	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });

	//////////////////////////////////////
	// �C���X�^���V���O�e�X�g
	//////////////////////////////////////


	//const int width = 50;
	//const int height = 10;
	//const int numHumanModel = width * height;
	//Vector3* humanPos = new Vector3[numHumanModel];

	//int humanNo = 0;
	//for (int x = 0; x < width; x++)
	//{
	//	for (int y = 0; y < height; y++)
	//	{
	//		humanPos[humanNo].x = -2400.0f + 1000.0f * x;
	//		humanPos[humanNo].y = -1250.0f + 2500.0f * y;
	//		humanPos[humanNo].z = 0.0f;

	//		humanNo++;
	//	}
	//}

	//// step-2 500�̕��̃��[���h�s��֌W�̊e��o�b�t�@���m�ہB
	//Matrix* worldMatrixArray = new Matrix[numHumanModel];

	//StructuredBuffer worldMatrixSB;
	//worldMatrixSB.Init(
	//	sizeof(Matrix),
	//	numHumanModel,
	//	nullptr
	//);

	//ModelInitData modelData;
	//modelData.m_tkmFilePath = "Assets/modelData/robo/robo.tkm";
	//modelData.m_fxFilePath = "Assets/shader/sample3DInstancing.fx";

	//modelData.m_expandShaderResoruceView[0] = &worldMatrixSB;
	//Model model;
	//model.Init(modelData);


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

		// �C���X�^���V���O�e�X�g
		//Quaternion qRot = Quaternion::Identity;

		//for (int i = 0; i < numHumanModel; i++)
		//{
		//	worldMatrixArray[i] = model.CalcWorldMatrix(humanPos[i], qRot, g_vec3One);
		//}
		//// step-5 ���[���h�s��̓��e���O���t�B�b�N�������ɃR�s�[�B
		//worldMatrixSB.Update(worldMatrixArray);

		//// step-6 �l���̂̃��f�����C���X�^���V�O�`��B
		//model.DrawInstancing(renderContext, numHumanModel);
		////////////////////////////////////////////////////////////////////////////////////////////

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