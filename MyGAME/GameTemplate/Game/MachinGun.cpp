#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"
#include "Bullet.h"

namespace
{
	const Vector3 MACHINGUN_SCALE = { 0.2f,0.2f,0.2f };
}

MachinGun::~MachinGun()
{
	//現在生きている弾の削除
	for (int i = 0; i < m_bullets.size(); i++)
	{
		DeleteGO(m_bullets.back());
		m_bullets.pop_back();
	}
	m_bullets.clear();
	//モデルの削除
	DeleteGO(m_skinModelRender);
}

bool MachinGun::Start()
{
	//マシンガンのモデル生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	return true;
}

void MachinGun::Update()
{
	//to do ステートメントで管理する事
	if (g_pad[0]->IsPress(enButtonRB2))
	{
		//弾を生成
		m_bullets.push_back(NewGO<Bullet>(0));
		//弾の初期座標系を設定
		m_bullets.back()->SetTargetAndCurrentPos(m_targetPos, m_position);
	}
	//モデルの位置を更新する
	m_skinModelRender->SetPosition(m_position);
}