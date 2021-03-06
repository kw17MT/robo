#include "stdafx.h"
#include "PlayerMove.h"
#include "SoundSource.h"
#include "GameDirector.h"

namespace 
{
const float MOVE_SPEED = 3.0f;
const float SPEED_UP_RATE = 100.0f;
const float SPEED_LIMIT = 1000.0f;
}

const void PlayerMove::Dash()
{
	//R1を押すとダッシュする
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		if (m_canDash) {
			//ダッシュしている状態にする
			m_isDash = true;
			if (!m_isSoundDash)
			{
				CSoundSource* dashSE = NewGO<CSoundSource>(0);
				dashSE->Init(L"Assets/sound/dash.wav", false);
				dashSE->SetVolume(1.0f);
				dashSE->Play(false);

				CSoundSource* dashKeepSE = NewGO<CSoundSource>(0, "dashKeep");
				dashKeepSE->Init(L"Assets/sound/dashKeep.wav", false);
				dashKeepSE->SetVolume(1.0f);
				dashKeepSE->Play(false);

				m_isSoundDash = true;
			}

			//以前のダッシュのスピードを消して計算しなおし
			m_dashSpeed = Vector3::Zero;
			//Lスティックを触っておらず、ダッシュすると前方に行くようにするためif文必要
			//倒してる方向にダッシュ
			if (abs(g_pad[0]->GetLStickXF()) || abs(g_pad[0]->GetLStickYF()))
			{
				//横に倒している分だけカメラから見てサイドに加速
				m_dashSpeed += m_sideDirection * g_pad[0]->GetLStickXF() * MOVE_SPEED * SPEED_UP_RATE;
				//縦に倒している分だけカメラから見て奥、手前に加速
				m_dashSpeed += m_forwardDirection * g_pad[0]->GetLStickYF() * MOVE_SPEED * SPEED_UP_RATE;;
			}
			//スティック入力なければ前方だけにダッシュ
			else
			{
				m_dashSpeed += m_forwardDirection * MOVE_SPEED * SPEED_UP_RATE;
			}
			//Lスティックに加えY押してれば急上昇、Aは急下降
			m_dashSpeed += m_upDirection * g_pad[0]->IsPress(enButtonY) * MOVE_SPEED * SPEED_UP_RATE;
			m_dashSpeed -= m_upDirection * g_pad[0]->IsPress(enButtonA) * MOVE_SPEED * SPEED_UP_RATE;

			//通常の移動スピードは加算しない
			m_nextPos -= m_currentSpeed;
		}
		else
		{
			m_isDash = false;			
			m_isDecayDash = false;
			CSoundSource* keepSE = FindGO<CSoundSource>("dashKeep");
			if (keepSE != nullptr)
			{
				DeleteGO(keepSE);
			}
		}
	}
	else
	{
		m_isSoundDash = false;
		CSoundSource* keepSE = FindGO<CSoundSource>("dashKeep");
		if (keepSE != nullptr)
		{
			DeleteGO(keepSE);
		}
	}
	
	//次フレームにダッシュボタンを押していなくても減速したダッシュ速度を加算したいため、除算
	m_dashSpeed /= 1.05f;

	//ダッシュスピードを加算
	m_nextPos += m_dashSpeed;
}

const void PlayerMove::Move(const Vector3 currentPos)
{
	//パラメータを更新
	m_nextPos = currentPos;
	//画面の前方
	m_forwardDirection = g_camera3D->GetForward();
	//画面の右方向
	m_sideDirection = g_camera3D->GetRight();
	//画面の上方向
	m_upDirection = Cross(m_forwardDirection, m_sideDirection);

	//スティック入力で操作
	m_currentSpeed += m_sideDirection * g_pad[0]->GetLStickXF() * MOVE_SPEED;
	m_currentSpeed += m_forwardDirection * g_pad[0]->GetLStickYF() * MOVE_SPEED;
	m_currentSpeed += m_upDirection * g_pad[0]->IsPress(enButtonY) * MOVE_SPEED;
	m_currentSpeed -= m_upDirection * g_pad[0]->IsPress(enButtonA) * MOVE_SPEED;

	//各速度に制限を設ける
	if (m_currentSpeed.x < -SPEED_LIMIT) { m_currentSpeed.x = -SPEED_LIMIT; }
	if (m_currentSpeed.y < -SPEED_LIMIT) { m_currentSpeed.y = -SPEED_LIMIT; }
	if (m_currentSpeed.z < -SPEED_LIMIT) { m_currentSpeed.z = -SPEED_LIMIT; }
	if (m_currentSpeed.x > SPEED_LIMIT) { m_currentSpeed.x = SPEED_LIMIT; }
	if (m_currentSpeed.y > SPEED_LIMIT) { m_currentSpeed.y = SPEED_LIMIT; }
	if (m_currentSpeed.z > SPEED_LIMIT) { m_currentSpeed.z = SPEED_LIMIT; }

	
	//移動に関する入力がなくても次のフレームも慣性を効かせたいため除算
	m_currentSpeed /= 1.05f;

	//通常速度を加算する
	m_nextPos += m_currentSpeed;
}

Vector3 PlayerMove::Execute(Vector3 currentPos)
{
	//通常移動
	Move(currentPos);
	//ダッシュ使用時の移動
	Dash();

	//ダッシュをやめたら徐々に減速
	if (!g_pad[0]->IsPress(enButtonRB1))
	{
		m_isDash = false;
		//ダッシュの影響度を減らしていく
		m_dashSpeedRate /= 1.04f;
		//ダッシュの影響がなくなれば、減速に関するパラメータを最初の状態に戻す。
		if (m_dashSpeedRate <= 1.0f)
		{
			m_isDecayDash = false;
			m_dashSpeedRate = 1.0f;
			count = 0.0f;
		}
	}

	//計算済みの位置座標を返す
	return m_nextPos;
}

Vector3 PlayerMove::CalcPlayerPos(Vector3 homePos)
{
	//何も入力していないときの座標＝ホームポジション
	Vector3 playerPos = homePos;
	
	//ダッシュ中ならもっとずらし、だんだん元の位置へ
	if (m_isDash)
	{
		//十分に最初の加速をし、カメラとプレイヤーの位置をある程度合わせに行く状態ならば
		if (!m_isDecayDash) {
			//ダッシュによってだんだん加速する。
			m_dashSpeedRate += 0.2f;
			//一定数以上に加速したら
			if (m_dashSpeedRate >= 2.5f)
			{
				//一定時間その速度をキープ
				count += GameTime().GetFrameDeltaTime();
				
				if (count >= 1.0f)
				{
					//徐々にカメラ位置を元に戻すフラグをたてる
					m_isDecayDash = true;
					count = 0.0f;
				}
				m_dashSpeedRate = 2.5f;
			}
		}
		//カメラ位置を元に戻すタイミングなら
		else if(m_isDecayDash == true)
		{
			//減速させていく
			m_dashSpeedRate -= 0.005f;
			if (m_dashSpeedRate <= 1.0f)
			{
				m_dashSpeedRate = 1.0f;
			}
		}

		//ホームポジションからずらしたい移動量
		Vector3 plusSpeed = m_currentSpeed * m_dashSpeedRate;

		//Y座標の移動量を調整
		plusSpeed.y /= 1.3f;
		//前フレームの加速速度を保存
		m_prevPlusSpeed = m_dashSpeed;
		//プレイヤーの位置座標に加算
		playerPos += plusSpeed;
	}
	else
	{
		//減速させていく
		m_dashSpeedRate -= 0.005f;
		if (m_dashSpeedRate <= 1.0f)
		{

			m_dashSpeedRate = 1.0f;
		}

		//ホームポジションからずらしたい移動量
		Vector3 plusSpeed = m_currentSpeed * m_dashSpeedRate;
		//Y座標の移動量を調整
		plusSpeed.y /= 1.3f;
		//前フレームの加速速度を保存
		m_prevPlusSpeed = m_currentSpeed + m_dashSpeed;
		//プレイヤーの位置座標に加算
		playerPos += plusSpeed;
	}

	return playerPos;
}

Vector3 PlayerMove::DeadMove(Vector3 currentPos)
{
	currentPos += m_prevPlusSpeed * 0.01f;

	m_afterDeathTime += GameTime().GetFrameDeltaTime();
	float dropAmount = 0.5f * m_afterDeathTime * m_afterDeathTime;

	//プレイヤーのワールド座標を一定以下にしないようにする
	if (currentPos.y >= -50000.0f)
	{
		currentPos.y -= dropAmount;
	}
	return currentPos;
}