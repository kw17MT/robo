#include "stdafx.h"
#include "PopUp2D.h"
//#include "ModelRender.h"
#include "Guzai.h"
#include "Kitchen.h"
#include "Counter.h"
#include<vector>

bool PopUp2D::Start()
{
	////自身の状態に応じて検索するものを決定(デフォルトはP1)
	//if (m_self == enPlayer01 && m_player == nullptr) {
	//	m_player = FindGO<ModelRender>("player01");
	//	m_kitchen = FindGO<Kitchen>("kitchen01");
	//	m_counter = FindGO<Counter>("counter01");
	//}
	//else if(m_self == enPlayer02 && m_player == nullptr){
	//	m_player = FindGO<ModelRender>("player02");
	//	m_kitchen = FindGO<Kitchen>("kitchen02");
	//	m_counter = FindGO<Counter>("counter02");
	//}
	//else {
	//	//何もしない
	//}

	//m_guzai = FindGO<Guzai>("guzai");

	//モデルの初期化処理
	//初期化用データの構築
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/popup/invisible.tkm"; //不可視のポリゴンにしておく
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);

	m_model.Init(modeldata);
	//初期化終わり//

	//拡大率を設定
	m_scale *= 7.0f;

	return true;
}

void PopUp2D::Move()
{
	////平行移動
	////表示テスト(P1の付近に表示、追従)
	//Vector3 posToPlayer = { 200.0f,100.0f,0.0f };
	////プレイヤー位置を取得
	//Vector3 position = m_player->GetPosition();
	//m_position = position + posToPlayer;
}

void PopUp2D::Rotation()
{
	//カメラ位置に合わせて角度が変わる処理
	//x軸回転
	Quaternion rotX;
	//カメラ位置を取得
	Vector3 CameraPos = g_camera3D->GetPosition();
	//自身からカメラ位置へのベクトル
	Vector3 toCameraPos = CameraPos - m_position;
	toCameraPos.Normalize();
	float angleX = atan2(toCameraPos.z, toCameraPos.y);
	if (angleX > 0.0f * PI) {
		angleX = 0.0f;
	}
	rotX.SetRotation(Vector3::AxisX, angleX);
	m_rotation = rotX;

	//※今のとこx軸回転だけでもよさそう
	////z軸回転
	//Quaternion rotZ;
	////軸を設定
	//Vector3 AxisZ = Cross(toCameraPos, Vector3::AxisY);
	//float angleZ = atan2(toCameraPos.x, toCameraPos.z);
	//if (angleZ < 0.0f * PI) {
	//	angleZ = 0.0f;
	//}
	//rotZ.SetRotation(AxisZ, angleZ);
	//m_rotation = rotZ;

	////合成用クォータニオン
	//Quaternion mulRot;
	//mulRot *= rotX;
	//mulRot *= rotZ;
	//
	//m_rotation = mulRot;

}

void PopUp2D::ChangeState()
{
	
}

void PopUp2D::ChangeModel()
{
	ModelInitData modeldata;

	switch (m_state) {
	case enNone:
		modeldata.m_tkmFilePath = "Assets/modelData/popup/invisible.tkm";
		break;
	case enNearFood:
		modeldata.m_tkmFilePath = "Assets/modelData/popup/press_a_get.tkm";
		break;
	case enDisposable:
		modeldata.m_tkmFilePath = "Assets/modelData/popup/hold_x_disposal.tkm";
		break;
	
	default:
		break;
	}

	modeldata.m_fxFilePath = "Assets/shader/model.fx";
	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";
	modeldata.m_modelUpAxis = enModelUpAxisY;
	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);
	
	m_model.Init(modeldata);
}

void PopUp2D::Update()
{
	/*vecGuzaiSize = vecGuzai.size();

	if (vecGuzaiSize > MAXSIZE) {
		vecGuzai.erase(vecGuzai.begin());
	}*/
	
	//m_guzai = FindGO<Guzai>("guzai");
	////vecGuzai.push_back(m_guzai);


	//if (m_guzai != nullptr && m_guzai->GetGuzaiToPlayer() != 0.0f) {
	//	if (m_guzai->GetGuzaiToPlayer() < 200.0f && m_state == enNone) {
	//		ChangeModel("Assets/modelData/popup/press_a_get.tkm", "Assets/shader/model.fx");
	//		m_state = enNearFood;
	//	}
	//	else if (m_guzai->GetGuzaiToPlayer() >= 200.0f && m_state != enNone) {
	//		ChangeModel("Assets/modelData/popup/invisible.tkm", "Assets/shader/model.fx");
	//		m_state = enNone;
	//	}
	//}

	//テスト:手動回転用
	/*if (g_pad[0]->IsPress(enButtonRight)) {
		rotX += 1.0f;
	}
	else if (g_pad[0]->IsPress(enButtonLeft)) {
		rotX -= 1.0f;
	}
	else if (g_pad[0]->IsPress(enButtonUp)) {
		rotY += 1.0f;
	}
	else if (g_pad[0]->IsPress(enButtonDown)) {
		rotY -= 1.0f;
	}*/

	//m_rotation.SetRotationDeg(Vector3::AxisZ, rotX);
	//m_rotation.SetRotationDeg(Vector3::AxisX, rotY);
	
	//平行移動
	Move();
	//カメラ位置に合わせて回転
	Rotation();


	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}