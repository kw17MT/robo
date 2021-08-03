#include "stdafx.h"
#include "GuzaiOkiba.h"
#include "Level.h"
#include "Player.h"
#include "PlayerGene.h"
#include "Guzai.h"
#include "SkinModelRender.h"
#include "Kitchen.h"
#include "Arrow.h"

namespace 
{
	const Vector3 POS_ANYONE_CANT_REACH = Vector3::Zero;

	const int NAME_SIZE_STOCKRIGHT = 10;
	const int NAME_SIZE_STOCKLEFT = 9;
	const int AJUST_ARRAY_NUM_RIGHT = 1;
	const int AJUST_ARRAY_NUM_LEFT = 3;
	const int NUMBER_OF_OKIBA_RIGHT = 10;
	const int NUMBER_OF_OKIBA_LEFT = 9;
	const int GUZAIOKIBA_NUM_MIN = 0;
	const int GUZAIOKIBA_NUM_MIDDLE = 4;
	const int GUZAIOKIBA_NUM_MAX = 8;
	const int GUZAIOKIBA_NUMBER_ONE = 1;
	const int GUZAIOKIBA_NUMBER_TWO = 2;
	const int GUZAIOKIBA_NUMBER_THREE = 3;
	const int GUZAIOKIBA_NUMBER_FOUR = 4;
	const int HAVE_GUZAI = 1;

	const float CAN_TARGET_DISTANCE = 100.0f;
	const float CAN_TARGET_POS_Y = 100.0f;
	const float AJUST_ARROW_HEIGHT = 80.0f;
}

GuzaiOkiba::~GuzaiOkiba()
{
	//具材置き場のオブジェクトを消去
	for (int i = GUZAIOKIBA_NUM_MIN; i < GUZAIOKIBA_NUM_MAX; i++) {
		DeleteGO(m_guzaiOkibaRender[i]);
	}
	//両プレイヤー用の矢印を消去
	DeleteGO(m_arrow00);
	DeleteGO(m_arrow01);
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

	m_player00 = FindGO<Player>("player00");
	m_player01 = FindGO<Player>("player01");

	Level level;

	//レベルデータから具材置き場の座標を受け取り、モデルを出す。
	//不変箇所を定義
	const wchar_t* StockRight = L"StockRight";
	const wchar_t* StockLeft = L"StockLeft";
	level.Init("Assets/level/level.tkl", [&](ObjectData& objectData) {
		//オブジェクトの名前を比較して、StockRightが一致した時
		if (wcsncmp(objectData.name, StockRight, NAME_SIZE_STOCKRIGHT) == 0) {
			//レベル上のオブジェクトの番号を取得
			int m_guzaiOkibaNo = _wtoi(&objectData.name[NUMBER_OF_OKIBA_RIGHT]);

			switch (m_guzaiOkibaNo)
			{
			case GUZAIOKIBA_NUMBER_ONE:
			case GUZAIOKIBA_NUMBER_TWO:
			case GUZAIOKIBA_NUMBER_THREE:
			case GUZAIOKIBA_NUMBER_FOUR: 
				//具材置き場のナンバリングが1~4の時、具材置き場のインスタンスを作成
				m_guzaiOkibaRender[m_guzaiOkibaNo - AJUST_ARRAY_NUM_RIGHT] = NewGO<SkinModelRender>(0);
				//通常描画用のモデル初期化
				m_guzaiOkibaRender[m_guzaiOkibaNo - AJUST_ARRAY_NUM_RIGHT]->InitForRecieveShadow("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_guzaiOkibaPos[m_guzaiOkibaNo - 1]);
				//シャドウキャスト用の初期化
				m_guzaiOkibaRender[m_guzaiOkibaNo - AJUST_ARRAY_NUM_RIGHT]->InitForCastShadow("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_guzaiOkibaPos[m_guzaiOkibaNo - 1]);
				//位置をレベル上と同じにする
				m_guzaiOkibaPos[m_guzaiOkibaNo - AJUST_ARRAY_NUM_RIGHT] = objectData.Pos;
				//拡大率を調節。
				m_guzaiOkibaRender[m_guzaiOkibaNo - AJUST_ARRAY_NUM_RIGHT]->SetScale(m_guzaiOkibaScale);
				return true;
				break;
			//ナンバリングが正しくできていないとき
			default:
				//何も出さない
				break;
			}
		}
		//オブジェクトの名前を比較して、StockLeftが一致した時
		if (wcsncmp(objectData.name, StockLeft, NAME_SIZE_STOCKLEFT) == 0) {
			//レベル上のオブジェクトの番号を取得
			int m_guzaiOkibaNo = _wtoi(&objectData.name[NUMBER_OF_OKIBA_LEFT]);
			switch (m_guzaiOkibaNo)
			{
			case GUZAIOKIBA_NUMBER_ONE: 
			case GUZAIOKIBA_NUMBER_TWO:
			case GUZAIOKIBA_NUMBER_THREE:
			case GUZAIOKIBA_NUMBER_FOUR:
				m_guzaiOkibaRender[m_guzaiOkibaNo + AJUST_ARRAY_NUM_LEFT] = NewGO<SkinModelRender>(0);
				m_guzaiOkibaRender[m_guzaiOkibaNo + AJUST_ARRAY_NUM_LEFT]->InitForRecieveShadow("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_guzaiOkibaPos[m_guzaiOkibaNo - 1]);
				m_guzaiOkibaRender[m_guzaiOkibaNo + AJUST_ARRAY_NUM_LEFT]->InitForCastShadow("Assets/modelData/desk/desk_new.tkm", nullptr, enModelUpAxisZ, m_guzaiOkibaPos[m_guzaiOkibaNo - 1]);
				m_guzaiOkibaPos[m_guzaiOkibaNo + AJUST_ARRAY_NUM_LEFT] = objectData.Pos;
				m_guzaiOkibaRender[m_guzaiOkibaNo + AJUST_ARRAY_NUM_LEFT]->SetScale(m_guzaiOkibaScale);
				return true;
				break;
			default:
				break;
			}
		}
		return true;
	});

	//具材置き場に近づいたときに出現する矢印
	m_arrow00 = NewGO<Arrow>(0);
	//１P側にセット
	m_arrow00->SetPosition(m_Arrow01Pos);
	m_arrow01 = NewGO<Arrow>(0);
	//２P側にセット
	m_arrow01->SetPosition(m_Arrow02Pos);

	return true;
}

void GuzaiOkiba::PlayerDistance()
{
	//1P側の処理 1P側の具材置き場は4～7
	for (int i = GUZAIOKIBA_NUM_MIDDLE; i < GUZAIOKIBA_NUM_MAX; i++) {
		//プレイヤーと具材置き場の距離を入手
		m_distance[i] = TargetDistance(m_guzaiOkibaPos[i], m_player00->GetPosition());
	}

	//2P側の処理 2P側の具材置き場は0～3
	for (int i = GUZAIOKIBA_NUM_MIN; i < GUZAIOKIBA_NUM_MIDDLE; i++) {
		m_distance[i] = TargetDistance(m_guzaiOkibaPos[i], m_player01->GetPosition());
	}
}

void GuzaiOkiba::Targeted()
{
	//1P側の処理
	for (int i = GUZAIOKIBA_NUM_MIDDLE; i < GUZAIOKIBA_NUM_MAX; i++) {
		//プレイヤーと具材置き場の距離が一定以下で、ターゲット中で無く、具材が置かれていない場合…
		if (m_distance[i] < CAN_TARGET_DISTANCE && m_targeted01 == false && m_guzaiSet[i] == false && m_player00->GetPlayerState() == HAVE_GUZAI) {
			//ターゲット中にアクセス可能な具材置き場に印を表示をするために、ターゲット中の具材置き場の座標を記憶する。
			m_targetPos01 = m_guzaiOkibaPos[i];
			m_targetPos01.y += CAN_TARGET_POS_Y;
			//ターゲット状態に移行し、ターゲットした番号の具材置き場にアクセス可能にする。
			m_targeted01 = true;
			m_targetNo01 = i;
			m_guzaiOkibaSet[m_targetNo01] = true;
			m_Arrow01Pos = m_guzaiOkibaPos[m_targetNo01];
			m_Arrow01Pos.y += AJUST_ARROW_HEIGHT;
			m_arrow00->SetPosition(m_Arrow01Pos);
			//ターゲット中の場合は距離が一定以下でも実行されないので、複数同時にアクセスすることはない。
		}
	}
	//ターゲット中の場合…
	if (m_targeted01 == true) {
		m_arrow00->Expansion();
		//ターゲット中の具材置き場とプレイヤーの距離を測り、一定以上になったとき…
		m_targetDistance01 = TargetDistance(m_guzaiOkibaPos[m_targetNo01], m_player00->GetPosition());
		if (m_targetDistance01 >= CAN_TARGET_DISTANCE || m_guzaiSet[m_targetNo01] == true) {
			//ターゲット状態から戻り、ターゲット中だった具材置き場に対してアクセス不可能にする。
			m_targeted01 = false;
			m_guzaiOkibaSet[m_targetNo01] = false;
		}
	}
	//ターゲットしていない場合…
	else {
		//矢印の拡大率を小さくしていき、見えないようにする。
		m_arrow00->Shrink();
		m_targetPos01 = POS_ANYONE_CANT_REACH;
	}

	//2P側の処理、1Pとほぼ同じ。

	for (int i = GUZAIOKIBA_NUM_MIN; i < GUZAIOKIBA_NUM_MIDDLE; i++) {
		if (m_distance[i] < CAN_TARGET_DISTANCE && m_targeted02 == false && m_guzaiSet[i] == false && m_player01->GetPlayerState() == HAVE_GUZAI) {
			m_targetPos02 = m_guzaiOkibaPos[i];
			m_targetPos02.y += CAN_TARGET_POS_Y;
			m_targeted02 = true;
			m_targetNo02 = i;
			m_guzaiOkibaSet[m_targetNo02] = true;
			m_Arrow02Pos = m_guzaiOkibaPos[m_targetNo02];
			m_Arrow02Pos.y += AJUST_ARROW_HEIGHT;
			m_arrow01->SetPosition(m_Arrow02Pos);
		}
	}
	if (m_targeted02 == true) {
		m_arrow01->Expansion();
		m_targetDistance02 = TargetDistance(m_guzaiOkibaPos[m_targetNo02], m_player01->GetPosition());
		if (m_targetDistance02 >= CAN_TARGET_DISTANCE || m_guzaiSet[m_targetNo02]) {
			m_targeted02 = false;
			m_guzaiOkibaSet[m_targetNo02] = false;
		}
	}
	else {
		m_arrow01->Shrink();
		m_targetPos02 = POS_ANYONE_CANT_REACH;
	}
}


void GuzaiOkiba::Update()
{
	//プレイヤーの情報を入手
	if (m_player00 == nullptr) {
		m_player00 = FindGO<Player>("player00");
	}
	if (m_player01 == nullptr) {
		m_player01 = FindGO<Player>("player01");
	}

	PlayerDistance();

	Targeted();

	//具材置き場の表示
	for (int i = GUZAIOKIBA_NUM_MIN; i < GUZAIOKIBA_NUM_MAX; i++) {
		m_guzaiOkibaRender[i]->SetPosition(m_guzaiOkibaPos[i]);
	}
}