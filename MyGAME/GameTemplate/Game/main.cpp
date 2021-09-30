#include "stdafx.h"
#include "system/system.h"
#include "effect/Effect.h"

#include "SkinModelRender.h"
#include "RenderingEngine.h"

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

	//���{�P�@�e�o��
	SkinModelRender* test[3] = { nullptr };
	test[0] = NewGO<SkinModelRender>(0);
	test[0]->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	test[0]->SetPosition({ 0.0f,400.0f,0.0f });

	//�����ȁ@�e�o��
	test[1] = NewGO<SkinModelRender>(0);
	test[1]->Init("Assets/modelData/testFish.tkm", "Assets/modelData/testFish.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);

	//���{�Q�@�����Ȃ�
	test[2] = NewGO<SkinModelRender>(0);
	test[2]->SetPosition({ 2000.0f, 800.0f, -2000.0f });
	test[2]->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	const float moveZspeed = 30.0f;
	Vector3 pos1 = { 3000.0f,800.0f, -2000.0f };
	bool comeFlag = true;

	//���j�e�B�@�e�o��
	SkinModelRender* unity;
	unity = NewGO<SkinModelRender>(0);
	unity->Init("Assets/modelData/unity/unityChan.tkm", "Assets/modelData/unity/unityChan.tks", enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);
	AnimationClip unityAnim[1];
	unityAnim[0].Load("Assets/modelData/unity/jump.tka");
	unityAnim[0].SetLoopFlag(true);
	//unity->InitAnimation(&unityAnim[0], 1);
	//unity->PlayAnimation(1, 1);
	unity->SetPosition({ 0.0f, 1000.0f,1000.0f });

	AnimationClip anim[2];
	anim[0].Load("Assets/modelData/robo.tka");
	anim[0].SetLoopFlag(true);
	anim[1].Load("Assets/modelData/testFish.tka");
	anim[1].SetLoopFlag(true);
	/*test[0]->InitAnimation(&anim[0], 1);
	test[0]->PlayAnimation(1, 1);*/
	//test[1]->InitAnimation(&anim[1], 1);
	//test[1]->PlayAnimation(1, 1);

	//����
	SkinModelRender* building[3] = { nullptr };
	building[0] = NewGO<SkinModelRender>(0);
	building[0]->Init("Assets/modelData/testBox/bil1.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	building[0]->SetPosition({ 800.0f, 0.0f, -200.0f });
	building[0]->SetScale({ 1.0f,1.0f,1.0f });
	building[1] = NewGO<SkinModelRender>(0);
	building[1]->Init("Assets/modelData/testBox/bil2.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	building[1]->SetPosition({ -1000.0f, 0.0f, -800.0f });
	building[1]->SetScale({ 3.0f,3.0f,3.0f });
	building[2] = NewGO<SkinModelRender>(0);
	building[2]->Init("Assets/modelData/testBox/bil3.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	building[2]->SetPosition({ 100.0f, 0.0f, -2000.0f });
	building[2]->SetScale({ 4.0f,4.0f,4.0f });

	SkinModelRender* sky[2] = { nullptr };
	sky[0] = NewGO<SkinModelRender>(0);
	sky[0]->Init("Assets/modelData/testBox/sky.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, false);
	sky[0]->SetPosition({ 0.0f,0.0f,-2000.0f });
	sky[0]->SetScale({ 10.0f,10.0f,1.0f });
	//��
	sky[1] = NewGO<SkinModelRender>(0);
	sky[1]->Init("Assets/modelData/testBox/sky.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, false);
	sky[1]->SetPosition({ 0.0f,-400.0f,-1000.0f });
	sky[1]->SetScale({ 10.0f,10.0f,10.0f });

	SkinModelRender* sun;
	sun = NewGO<SkinModelRender>(0);
	sun->Init("Assets/modelData/sun/sun.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, false);
	sun->SetPosition({ 0.0f,10000.0f,0.0f });
	sun->SetScale({ 6.0f,6.0f,6.0f });
	sun->SetIsSun();

	int time = 0;
	Vector3 pos = { 0.0f,400.0f,0.0f };

	g_camera3D->SetPosition({ 0.0f,1500.0f,5000.0f });
		
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

		//�J�����̈ړ�
		//if (g_pad[0]->IsPress(enButtonLeft)) {
		//	Vector3 a = g_camera3D->GetPosition();
		//	Vector3 b = g_camera3D->GetTarget();
		//	a.x += 20.0f;
		//	b.x += 20.0f;
		//	g_camera3D->SetPosition(a);	
		//	g_camera3D->SetTarget(b);
		//}
		//if (g_pad[0]->IsPress(enButtonRight)) {
		//	Vector3 a = g_camera3D->GetPosition();
		//	Vector3 b = g_camera3D->GetTarget();
		//	a.x -= 20.0f;
		//	b.x -= 20.0f;
		//	g_camera3D->SetPosition(a);
		//	g_camera3D->SetTarget(b);
		//}
		//if (g_pad[0]->IsPress(enButtonUp)) {
		//	Vector3 a = g_camera3D->GetPosition();
		//	Vector3 b = g_camera3D->GetTarget();
		//	a.z -= 20.0f;
		//	b.z -= 20.0f;
		//	g_camera3D->SetPosition(a);
		//	g_camera3D->SetTarget(b);
		//}
		//if (g_pad[0]->IsPress(enButtonDown)) {
		//	Vector3 a = g_camera3D->GetPosition();
		//	a.z += 20.0f;
		//	Vector3 b = g_camera3D->GetTarget();
		//	b.z += 20.0f;
		//	g_camera3D->SetPosition(a);
		//	g_camera3D->SetTarget(b);
		//}
		//if (g_pad[0]->IsPress(enButtonX)) {
		//	Vector3 a = g_camera3D->GetPosition();
		//	Vector3 b = g_camera3D->GetTarget();
		//	a.y -= 10.0f;
		//	b.y -= 10.0f;
		//	//g_camera3D->SetPosition(a);
		//	g_camera3D->SetTarget(b);
		//}
		//if (g_pad[0]->IsPress(enButtonY)) {
		//	Vector3 a = g_camera3D->GetPosition();
		//	Vector3 b = g_camera3D->GetTarget();
		//	a.y += 10.0f;
		//	b.y += 10.0f;
		//	//g_camera3D->SetPosition(a);
		//	g_camera3D->SetTarget(b);
		//}

		//�J�����i��U��̂݁j
		if (g_pad[0]->IsPress(enButtonLeft)) {
			Vector3 a = g_camera3D->GetPosition();
			Vector3 b = g_camera3D->GetTarget();
			a.x += 80.0f;
			b.x += 80.0f;
			//g_camera3D->SetPosition(a);	
			g_camera3D->SetTarget(b);
		}
		if (g_pad[0]->IsPress(enButtonRight)) {
			Vector3 a = g_camera3D->GetPosition();
			Vector3 b = g_camera3D->GetTarget();
			a.x -= 80.0f;
			b.x -= 80.0f;
			//g_camera3D->SetPosition(a);
			g_camera3D->SetTarget(b);
		}
		if (g_pad[0]->IsPress(enButtonUp)) {
			Vector3 a = g_camera3D->GetPosition();
			Vector3 b = g_camera3D->GetTarget();
			a.y += 80.0f;
			b.y += 80.0f;
			//g_camera3D->SetPosition(a);
			g_camera3D->SetTarget(b);
		}
		if (g_pad[0]->IsPress(enButtonDown)) {
			Vector3 a = g_camera3D->GetPosition();
			Vector3 b = g_camera3D->GetTarget();
			a.y -= 80.0f;
			b.y -= 80.0f;
			//g_camera3D->SetPosition(a);
			g_camera3D->SetTarget(b);
		}

		/*time++;
		if (time < 200) {
			pos.x += 10;
			test[0]->SetPosition(pos);
		}
		else if (time >= 200 && time < 400) {
			pos.x -= 10;
			test[0]->SetPosition(pos);
		}
		else {
			time = 0;
		}

		if (comeFlag) {
			pos1.z += moveZspeed;
			if (pos1.z >= 0.0f) {
				comeFlag = false;
			}
		}
		if (!comeFlag)
		{
			pos1.z -= moveZspeed;
			if (pos1.z < -2000.0f)
			{
				comeFlag = true;
			}
		}
		test[2]->SetPosition(pos1);*/


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