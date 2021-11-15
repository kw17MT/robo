#include "stdafx.h"
#include "Rader.h"
#include "SpriteRender.h"

namespace
{
	const float ICON_SIZE = 4;
	const Vector2 PIVOT = { -125.0f, 50.0f };
	const Vector3 DIRECTION_Z = { 0.0f,0.0f,1.0f };
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);
extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

Rader::~Rader()
{
	//レーダー上の敵のアイコンをすべて削除
	for (int i = 0; i < 10; i++)
	{
		DeleteGO(m_spriteRender[i]);
	}
	//レーダー上のプレイヤーのアイコンを削除
	DeleteGO(m_playerIcon);
}

bool Rader::Start()
{
	//レーダー上の敵のアイコンを最大数作成
	for (int i = 0; i < 10; i++)
	{
		m_spriteRender[i] = NewGO<SpriteRender>(0);
		m_spriteRender[i]->Init("Assets/Image/HPBar/HPBar.dds", ICON_SIZE, ICON_SIZE);
		//画像が画面の左下くらいに来るように設定
		m_spriteRender[i]->SetPivot(PIVOT);
		m_spriteRender[i]->SetScale(m_scale);
	}

	//レーダー上のプレイヤーのアイコンを生成
	m_playerIcon = NewGO<SpriteRender>(0);
	m_playerIcon->Init("Assets/Image/HPBar/HPBar.dds", ICON_SIZE, ICON_SIZE);
	//画像が画面の左下くらいに来るように設定
	m_playerIcon->SetPivot(PIVOT);
	m_playerIcon->SetScale(Vector3::One);
	//座標は以降固定
	m_playerIcon->SetPosition(Vector3::Zero);

	return true;
}

void Rader::SaveEnemyPos(int no, Vector3 enemyPos)
{
	m_enemyPos[no] = enemyPos;
}

void Rader::Update()
{
	Quaternion qRot;
	//現在のプレイヤーの向きからワールドのZ方向への回転を作成
	qRot.SetRotation(g_camera3D->GetForward(), DIRECTION_Z);

	for (int i = 0; i < m_enemyNum - 1; i++)
	{
		//プレイヤーから敵に向かうベクトル
		Vector3 playerToEnemyVec = m_enemyPos[i] - m_playerPos;
		//正規化
		playerToEnemyVec.Normalize();
		//y座標は不要
		playerToEnemyVec.y = 0.0f;
		//回転を適用
		qRot.Apply(playerToEnemyVec);
		//反転を調整
		playerToEnemyVec.x *= -1.0f;
		//ワールドを俯瞰した絵を画面に表現したいため、yとzを入れかえ
		playerToEnemyVec.y = playerToEnemyVec.z;
		//zは不要
		playerToEnemyVec.z = 0.0f;
		
		//プレイヤーと敵の距離を測定
		float distance = CalcMethods::CalcDistance(m_enemyPos[i], m_playerPos);
		//レーダーにちょうどいい具合に調整
		distance /= 100.0f;
		//値を制限
		if (distance >= 100.0f)distance = 100.0f;
		//位置関係と距離からアイコンを置きたい場所を計算
		m_screenPos = playerToEnemyVec * distance;

		//画像の位置を設定
		m_spriteRender[i]->SetPosition(m_screenPos);
		//拡大率を1にして画面上で見れるようにする
		m_scale = Vector3::One;
		//画像の拡大率を更新
		m_spriteRender[i]->SetScale(m_scale);
	}
}