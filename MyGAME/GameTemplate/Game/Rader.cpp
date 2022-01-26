#include "stdafx.h"
#include "Rader.h"
#include "SpriteRender.h"
#include "EnemyIconOnRader.h"

namespace
{
	const float ICON_SIZE = 12;													//レーダー上のアイコンのおおきさ
	const Vector3 DIRECTION_Z = { 0.0f,0.0f,1.0f };								//Z方向
	const Vector3 SPRITE_DIRECTION_UP = { 0.0f,-1.0f,0.0f };					//画像の上方向
	const Vector3 BASE_SPRITE_POSITION = { 520.0f, -240.0f, 0.0f };				//画像の中心を置く基準位置
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);
extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

Rader::~Rader()
{
	//レーダー上の敵のアイコンをすべて削除
	for (int i = 0; i < 10; i++)
	{
		DeleteGO(m_enemyIcon[i]);
	}
	//レーダー上のプレイヤーのアイコンを削除
	DeleteGO(m_playerIcon);
	DeleteGO(m_rader);
}

bool Rader::Start()
{
	//レーダー作成
	m_rader = NewGO<SpriteRender>(0);
	m_rader->Init("Assets/Image/rader/rader.dds", 200, 200);
	m_rader->SetPosition(BASE_SPRITE_POSITION);

	//レーダー上の敵のアイコンを最大数作成
	for (int i = 0; i < 10; i++)
	{
		m_enemyIcon[i] = NewGO<EnemyIconOnRader>(0);
	}

	//レーダー上のプレイヤーのアイコンを生成
	m_playerIcon = NewGO<SpriteRender>(0);
	m_playerIcon->Init("Assets/Image/rader/playerIcon.dds", ICON_SIZE, ICON_SIZE);
	//画像が画面の左下くらいに来るように設定
	m_playerIcon->SetScale(Vector3::One);
	m_playerIcon->SetPosition(BASE_SPRITE_POSITION);

	return true;
}

void Rader::SaveEnemyPosAndIsAlive(int no, Vector4 enemyPos)
{
	//no番目の配列に敵の座標を代入
	m_enemyPosAndIsAlive[no] = enemyPos;
}

void Rader::Update()
{


	for (int i = 0; i < m_enemyNum; i++)
	{
		if (m_enemyPosAndIsAlive[i].w == false)
		{
			m_enemyIcon[i]->SetScale({ 0.0f,0.0f,0.0f });
			continue;
		}

		//敵の位置をXYZ座標系に入れ直す
		Vector3 enemyPos;
		enemyPos.x = m_enemyPosAndIsAlive[i].x; enemyPos.y = m_enemyPosAndIsAlive[i].y; enemyPos.z = m_enemyPosAndIsAlive[i].z;

		Quaternion qRot;
		//現在のプレイヤーの向きからワールドのZ方向への回転を作成
		Vector3 ForwardWithoutY = g_camera3D->GetForward();
		ForwardWithoutY.y = 0.0f;
		qRot.SetRotation(ForwardWithoutY, DIRECTION_Z);

		//プレイヤーから敵に向かうベクトル
		Vector3 playerToEnemyVec = enemyPos - m_playerPos;
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
		float distance = CalcMethods::CalcDistance(enemyPos, m_playerPos);
		//レーダーにちょうどいい具合に調整
		distance /= 100.0f;
		//値を制限
		if (distance >= 70.0f)distance = 70.0f;
		//位置関係と距離からアイコンを置きたい場所を計算
		m_screenPos = playerToEnemyVec * distance;

		//1フレーム前の位置と現在の位置から移動速度を求める
		Vector3 speed = enemyPos - m_prevEnemyPos[i];
		////画像座標に変換したいため、値を入れかえ
		std::swap(speed.y, speed.z);
		speed.Normalize();
		//z座標は不要
		speed.z = 0.0f;
		//xについて画像座標に調整
		speed.x *= -1.0f;
		//移動方向への回転
		Quaternion moveDirectionRot;
		moveDirectionRot.SetRotation(SPRITE_DIRECTION_UP, speed);

		//カメラの上方向を取得
		Vector3 cameraFront = g_camera3D->GetForward();
		std::swap(cameraFront.y, cameraFront.z);
		cameraFront.Normalize();
		cameraFront.z = 0.0f;
		cameraFront.x *= -1.0f;
		//カメラの回転を計算
		qRot.SetRotation(cameraFront, SPRITE_DIRECTION_UP);
		//カメラの回転に画像自体の回転を掛け合わせ、最終的な回転を計算
		moveDirectionRot.Multiply(qRot);
		m_enemyIcon[i]->SetRotation(moveDirectionRot);

		//1フレーム前の座標として設定
		m_prevEnemyPos[i] = enemyPos;

		//画像の位置を設定
		m_screenPos += BASE_SPRITE_POSITION;
		m_enemyIcon[i]->SetPosition(m_screenPos);
		//拡大率を1にして画面上で見れるようにする
		m_scale = Vector3::One;
		//画像の拡大率を更新
		m_enemyIcon[i]->SetScale(m_scale);
	}
}