#include "stdafx.h"
#include "ModelRender.h"
#include "Guzai.h"
#include "FontRender.h"


#include "PathFactory.h"
#include "DeBuff.h"

#include "GameDirector.h"
#include "PopUp2D.h"

namespace
{
	float DEBUFFDISTANCE = 100.0f * 100.0f;
}

ModelRender::ModelRender()
{
	
}

ModelRender::~ModelRender()
{
	DeleteGO(m_popUp);
}

bool ModelRender::Start()
{
	//モデルの初期化処理
	//初期化用データの構築
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisZ;

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);
	modeldata.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);
	//初期化終わり//

	//キャラコンの初期化
	m_charaCon.Init(40.0f, 100.0f, m_position);

	//具材ナンバー配列のすべての要素を9で初期化
	for (int i = 0; i < 10; i++) {
		GuzaiNo[i] = 9;
	}
	
	//ポップアップ用表示
	m_popUp = NewGO<PopUp2D>(20, "popup");
	if (playerNo != 0) {
		if (playerNo == 1) {
			m_popUp->SetEnSelf(enPlayer01);
		}
		else if (playerNo == 2) {
			m_popUp->SetEnSelf(enPlayer02);
		}
	}

	return true;
}

/// <summary>
/// モデルを変更するときに使う
/// 変更したくない箇所はnullptr
/// </summary>
/// <param name="ModelPath">モデルのパス</param>
/// <param name="ShaderPath">シェーダーのパス</param>
void ModelRender::GiveData(const char* ModelPath, const char* ShaderPath)
{
	ModelInitData modeldata;

	modeldata.m_tkmFilePath = ModelPath;
	
	modeldata.m_fxFilePath = "Assets/shader/model.fx";
	if (ShaderPath != nullptr) {
		modeldata.m_fxFilePath = ShaderPath;
	}

	model.Init(modeldata);
}

void ModelRender::SetGuzaiNo9()
{
	for (int i = 0;i < 10;i++)
	{
		GuzaiNo[i] = 9;
	}
}

//Vector3 ModelRender::GetPosition()
//{
//	Vector3 Pos = m_charaCon.GetPosition();
//	return Pos;
//}
Vector3 ModelRender::GetPosition()
{
	return m_position;
}

void ModelRender::Update()
{
	
	//ゲームプレイ中じゃなかったら。
	if (!GetGameDirector().GetIsGamePlay())
	{
		//処理しない。
		return;
	}

	//スケルトンを更新。
	m_skeleton.Update(model.GetWorldMatrix());

	//P1の処理
	if (playerNo == 1) {
		
		//回転処理
		
		//自身の上と右を定義(見下ろしなので)
		Vector3 frontDirP1 = { 0.0f,0.0f,1.0f };
		frontDirP1.Normalize();
		Vector3 rightDirP1 = { 1.0f,0.0f,0.0f };
		rightDirP1.Normalize();
		
		//回転軸の決定
		Vector3 AxisYP1;
		AxisYP1.Cross(frontDirP1, rightDirP1);

		//水平方向と奥行方向への入力を受け取り
		float LStickXP1 = g_pad[0]->GetLStickXF() * 1.0f;
		float LStickZP1 = g_pad[0]->GetLStickYF() * -1.0f; //奥行方向の逆転を-1.0fを掛けて補正

		//回転させるかどうかのチェック(スティックの入力があるかどうかをチェック)
		if (fabsf(LStickXP1) < 0.001f && fabsf(LStickZP1) < 0.001f) {
			//return; //returnすると以下の処理がすっ飛ばされてUpdateの最後にいってしまう。
		}
		else {
			//二つの入力値が成す角を求める(ラジアン角)
			angle = atan2(LStickXP1, LStickZP1);
			//縦軸まわりの回転を求める(ラジアン角を引数に渡すためSetRotation)
			m_rotation.SetRotation(AxisYP1, -angle);
		}
		
		//バフの効果がついているか確認後移動速度を決め、移動させる。
		if (Buff == true) {
			moveSpeed.x = g_pad[0]->GetLStickXF() * -20.0f;
			moveSpeed.z = g_pad[0]->GetLStickYF() * -20.0f;

			BuffTime--;
			if (BuffTime == 0) {
				Buff = false;
				BuffTime = 120;
			}
		}
		if(Buff == false) {
			moveSpeed.x = g_pad[0]->GetLStickXF() * -10.0f;
			moveSpeed.z = g_pad[0]->GetLStickYF() * -10.0f;
		}

		/*if (setPos == 0) {
			Vector3 Pos1 = { 900.0f, 0.0f, 0.0f };
			m_charaCon.SetPosition(Pos1);
			setPos = 1;
		}*/
	}
	//P2の処理
	if (playerNo == 2) {
		
		//回転処理
		
		//自身の上と右を定義(見下ろしなので)
		Vector3 frontDirP2 = { 0.0f,0.0f,1.0f };
		frontDirP2.Normalize();
		Vector3 rightDirP2 = { 1.0f,0.0f,0.0f };
		rightDirP2.Normalize();
		
		//回転軸の決定
		Vector3 AxisYP2;
		AxisYP2.Cross(frontDirP2, rightDirP2);
		
		//水平方向と奥行方向への入力を受け取り
		float LStickXP2 = g_pad[1]->GetLStickXF() * 1.0f;
		float LStickZP2 = g_pad[1]->GetLStickYF() * -1.0f; //奥行方向の逆転を-1.0fを掛けて補正

		//回転させるかどうかのチェック(スティックの入力があるかどうかをチェック)
		if (fabsf(LStickXP2) < 0.001f && fabsf(LStickZP2) < 0.001f) {
			//return; //returnすると以下の処理がすっ飛ばされてUpdateの最後にいってしまう。
		}
		else {
			//二つの入力値が成す角を求める(ラジアン角)
			angle = atan2(LStickXP2, LStickZP2);
			//縦軸まわりの回転を求める(ラジアン角を引数に渡すためSetRotation)
			m_rotation.SetRotation(AxisYP2, -angle);
		}
		
		//バフの効果がついているか確認後移動速度を決め、移動させる。
		if (Buff == true) {
			moveSpeed.x = g_pad[1]->GetLStickXF() * -20.0f;
			moveSpeed.z = g_pad[1]->GetLStickYF() * -20.0f;

			BuffTime--;
			if (BuffTime == 0) {
				Buff = false;
				BuffTime = 120;
			}
		}
		if (Buff == false) {
			moveSpeed.x = g_pad[1]->GetLStickXF() * -10.0f;
			moveSpeed.z = g_pad[1]->GetLStickYF() * -10.0f;
		}

		/*if (setPos == 0) {
			Vector3 Pos2 = { -900.0f, 0.0f, 0.0f };
			m_charaCon.SetPosition(Pos2);
			setPos = 1;
		}*/
	}
	//アイテム使用処理。
	UseItem();
	//Vector3 plPos = m_charaCon.GetPosition();

	
	//m_charaCon.Execute(moveSpeed, 1.0f);

	m_position = m_charaCon.Execute(moveSpeed, 1.0f);
	
	
	////テスト:ポップアップモデルの変更
	//if (m_popUp->GetEnState() == enNone) {
	//	m_popUp->SetEnState(enNearFood);
	//	m_popUp->ChangeModel();
	//}
	
	//ポップアップ用座標設定
	Vector3 playerToPopUp = { 200.0f,100.0f,0.0f };
	m_popUpPosition = m_position;
	m_popUpPosition = m_popUpPosition + playerToPopUp;
	m_popUp->SetPosition(m_popUpPosition);
	
	
	model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	
}

void ModelRender::UseItem()
{
	//Aボタン押してなかったら。
	if (!g_pad[0]->IsTrigger(enButtonA))
	{
		return;
	}

	ObjectGene* gene;

	if (playerNo == 1)
	{
		gene = FindGO<ObjectGene>("gene01");
	}
	else if (playerNo == 2)
	{
		gene = FindGO<ObjectGene>("gene02");
	}
	else {
		return;
	}

	//バフアイテム持ってる時。
	if (m_enItem == enBuffItem)
	{
		m_enItem = enNonItem;
		SetBuffAffect(true);
		gene->Buffnum = 0;

	}
	//デバフアイテム持ってる時。
	else if (m_enItem == enDebuffItem)
	{
		auto path = PathFactory::GetInstance().GetPath(enDeBuffLane, playerNo);
		Vector3 pos = path->GetFirstPoint()->s_vector;
		if ((m_charaCon.GetPosition() - pos).LengthSq() < DEBUFFDISTANCE)
		{
			m_enItem = enNonItem;

			DeBuff* deBuff = NewGO<DeBuff>(0, "debuff");
			deBuff->SetBuffNo(playerNo);
			deBuff->SetPosition(m_charaCon.GetPosition());
			deBuff->m_isDeBuffLane = true;

			gene->DeBuffnum = 0;
		}
	}
}
