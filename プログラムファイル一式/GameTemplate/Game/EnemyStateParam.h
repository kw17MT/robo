#pragma once

namespace
{
	namespace dash
	{
		const float MAX_TIME = 5.0f;						//ダッシュの最大継続時間
	}
	namespace around
	{
		const float MAX_TIME = 6.0f;						//旋回の最大継続時間
	}
	namespace behind
	{
		const float DOT_RECOGNIZE_PLAYER = -0.85f;			//プレイヤーが見ていると感じる内積
	}
	namespace approach
	{
		const float LIMIT_APPROACH = 2000.0f;				//プレイヤーに近づける距離
		const float LIMIT_RECOGNIZE = 7000.0f;				//プレイヤーを認識できる距離
	}	
	namespace stay
	{
		const float MAX_TIME = 10.0f;						//待機の最大継続時間
	}
	namespace stateNo
	{
		const int DASH = 0;									//ダッシュの番号
		const int AROUND = 1;								//旋回の番号
		const int BACK = 2;									//後退の番号
		const int BEHIND = 3;								//背後をとる状態の番号
		const int APPROACH = 4;								//接近状態の番号
		//const int STAY = 5;
	}
}

struct EnemyStateParam
{
	EnemyStateParam()
	{
		timer = 0.0f;										
		distanceBetweenEnemyToPlayer = 0.0f;				
		toTargetDirection = Vector3::Zero;					
		getBehind = false;									
	}

	float timer;											//経過時間
	float distanceBetweenEnemyToPlayer;						//敵とプレイヤーの距離
	Vector3 toTargetDirection;								//ターゲットへの向き
	bool getBehind;											//背後を継続して取りに行くか
};