#include "stdafx.h"
#include "GuzaiOkiba.h"
#include "Level.h"
#include "Player.h"
#include "PlayerGene.h"
#include "Guzai.h"
#include "SkinModelRender.h"
#include "Kitchen.h"
#include "Arrow.h"

GuzaiOkiba::GuzaiOkiba()
{

}

GuzaiOkiba::~GuzaiOkiba()
{
	for (int i = 0; i < 8; i++) {
		DeleteGO(stock[i]);
	}
	DeleteGO(m_guzaiRender01);
	DeleteGO(m_guzaiRender02);
}

float GuzaiOkiba::TargetDistance(Vector3 v1, Vector3 v2)
{
	//距離計算用の関数
	Vector3 v3 = v1 - v2;
	float ans = v3.Length();

	return ans;
}

bool GuzaiOkiba::Start()
{
	pl01 = FindGO<Player>("player01");
	pl02 = FindGO<Player>("player02");

	Level level;

	//レベルデータから具材置き場の座標を受け取り、モデルを出す。
	const wchar_t* StockRight = L"StockRight";
	const wchar_t* StockLeft = L"StockLeft";
	level.Init("Assets/level/level_new.tkl", [&](ObjectData& objectData) {
		if (wcsncmp(objectData.name, StockRight, 10) == 0) {
			m_kitchenNo = _wtoi(&objectData.name[10]);

			switch (m_kitchenNo)
			{
			case 1: {
				m_kitchenPos[m_kitchenNo - 1] = objectData.Pos;
				m_kitchenRender[m_kitchenNo - 1] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo - 1]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo - 1]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo - 1]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			case 2: {
				m_kitchenPos[m_kitchenNo - 1] = objectData.Pos;
				m_kitchenRender[m_kitchenNo - 1] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo - 1]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo - 1]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo - 1]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			case 3: {
				m_kitchenPos[m_kitchenNo - 1] = objectData.Pos;
				m_kitchenRender[m_kitchenNo - 1] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo - 1]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo - 1]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo - 1]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			case 4: {
				m_kitchenPos[m_kitchenNo - 1] = objectData.Pos;
				m_kitchenRender[m_kitchenNo - 1] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo - 1]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo - 1]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo - 1]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			default:
				break;
			}


		}
		if (wcsncmp(objectData.name, StockLeft, 9) == 0) {
			m_kitchenNo = _wtoi(&objectData.name[9]);

			switch (m_kitchenNo)
			{
			case 1: {
				m_kitchenPos[m_kitchenNo + 3] = objectData.Pos;
				m_kitchenRender[m_kitchenNo + 3] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo + 3]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo + 3]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo + 3]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			case 2: {
				m_kitchenPos[m_kitchenNo + 3] = objectData.Pos;
				m_kitchenRender[m_kitchenNo + 3] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo + 3]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo + 3]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo + 3]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			case 3: {
				m_kitchenPos[m_kitchenNo + 3] = objectData.Pos;
				m_kitchenRender[m_kitchenNo + 3] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo + 3]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo + 3]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo + 3]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			case 4: {
				m_kitchenPos[m_kitchenNo + 3] = objectData.Pos;
				m_kitchenRender[m_kitchenNo + 3] = NewGO<SkinModelRender>(0);
				m_kitchenRender[m_kitchenNo + 3]->Init("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_kitchenPos[m_kitchenNo - 1]);
				m_kitchenRender[m_kitchenNo + 3]->SetScale(m_kitchenScale);
				m_kitchenRender[m_kitchenNo + 3]->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				return true;
			}break;
			default:
				break;
			}
		}
	
		});

	//ターゲット中の具材置き場に出す仮モデルを初期化する。

	m_arrow01 = NewGO<Arrow>(0);
	m_arrow02 = NewGO<Arrow>(0);
	/*m_guzaiRender01 = NewGO<SkinModelRender>(0);
	m_guzaiRender01->Init("Assets/modelData/Arrow/Arrow_Yellow.tkm", nullptr, enModelUpAxisZ, m_targetPos01);
	m_guzaiRender02 = NewGO<SkinModelRender>(0);
	m_guzaiRender02->Init("Assets/modelData/Arrow/Arrow_Yellow.tkm", nullptr, enModelUpAxisZ, m_targetPos02);*/
	return true;
}

void GuzaiOkiba::PlayerDistance()
{
	//1P側の処理 1P側の具材置き場は4～7
	for (int i = 4; i < 8; i++) {
		//プレイヤーと具材置き場の距離を入手
		m_distance[i] = TargetDistance(m_kitchenPos[i], pl01->GetPosition());
	}

	//2P側の処理 2P側の具材置き場は0～3
	for (int i = 0; i < 4; i++) {
		m_distance[i] = TargetDistance(m_kitchenPos[i], pl02->GetPosition());
	}
}

void GuzaiOkiba::Targeted()
{
	//具材置き場とプレイヤーの距離を測り、範囲内の一つの具材置き場にのみアクセス可能にする。

	//1P側の処理

	for (int i = 4; i < 8; i++) {
		//プレイヤーと具材置き場の距離が一定以下で、ターゲット中で無く、具材が置かれていない場合…
		if (m_distance[i] < 150.0f && m_targeted01 == false && m_guzaiSet[i] == false && pl01->have == 1) {
			//ターゲット中にアクセス可能な具材置き場に印を表示をするために、ターゲット中の具材置き場の座標を記憶する。
			m_targetPos01 = m_kitchenPos[i];
			m_targetPos01.y += 100.0f;
			//ターゲット状態に移行し、ターゲットした番号の具材置き場にアクセス可能にする。
			m_targeted01 = true;
			m_targetNo01 = i;
			m_kitchenSet[m_targetNo01] = true;
			m_Arrow01Pos = m_kitchenPos[m_targetNo01];
			m_Arrow01Pos.y += 80.0f;
			m_arrow01->SetPosition(m_Arrow01Pos);
			//ターゲット中の場合は距離が一定以下でも実行されないので、複数同時にアクセスすることはない。
		}
	}
	//ターゲット中の場合…
	if (m_targeted01 == true) {
		//ターゲット中の具材置き場とプレイヤーの距離を測り、一定以上になったとき…
		m_targetDistance01 = TargetDistance(m_kitchenPos[m_targetNo01], pl01->GetPosition());
		if (m_targetDistance01 >= 150.0f || m_guzaiSet[m_targetNo01] == true) {
			//ターゲット状態から戻り、ターゲット中だった具材置き場に対してアクセス不可能にする。
			m_targeted01 = false;
			m_kitchenSet[m_targetNo01] = false;
		}
	}
	//ターゲットしていない場合…
	else {
		//ターゲット座標を0,0,0にする。
		m_arrow01->SetPosition({ 0.0f,-1000.0f,0.0f });
		m_targetPos01 = { 0.0f,-200.0f,0.0f }/*Vector3::Zero*/;
	}

	//2P側の処理、1Pとほぼ同じ。

	for (int i = 0; i < 4; i++) {
		if (m_distance[i] < 150.0f && m_targeted02 == false && m_guzaiSet[i] == false && pl02->have == 1) {
			m_targetPos02 = m_kitchenPos[i];
			m_targetPos02.y += 100.0f;
			m_targeted02 = true;
			m_targetNo02 = i;
			m_kitchenSet[m_targetNo02] = true;
			m_Arrow02Pos = m_kitchenPos[m_targetNo02];
			m_Arrow02Pos.y += 80.0f;
			m_arrow02->SetPosition(m_Arrow02Pos);
		}
	}
	if (m_targeted02 == true) {
		m_targetDistance02 = TargetDistance(m_kitchenPos[m_targetNo02], pl02->GetPosition());
		if (m_targetDistance02 >= 150.0f || m_guzaiSet[m_targetNo02]) {
			m_targeted02 = false;
			m_kitchenSet[m_targetNo02] = false;
		}
	}
	else {
		m_arrow02->SetPosition({ 0.0f,-1000.0f,0.0f });
		m_targetPos02 = { 0.0f,-200.0f,0.0f };/*Vector3::Zero;*/
	}
}


void GuzaiOkiba::Update()
{
	//プレイヤーの情報を入手
	pl01 = FindGO<Player>("player01");
	pl02 = FindGO<Player>("player02");

	PlayerDistance();

	Targeted();

	
	
	//具材置き場の表示
	for (int i = 0; i < 8; i++) {
		m_kitchenRender[i]->SetPosition(m_kitchenPos[i]);
	}
}