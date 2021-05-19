/*!
 * @brief	エフェクト
 */

#include "stdafx.h"
#include "ExEngine.h"
#include "Effect.h"



Effect::Effect()
{
}
Effect::~Effect()
{
	
}

		
void Effect::Init(const char16_t* filePath)
{
	m_effect = EffectEngine::GetInstance()->LoadEffect(filePath);
	
}
void Effect::Play(const bool& isStop)
{
	//再生中のエフェクトを停止する。
	if (isStop == true) {
		EffectEngine::GetInstance()->Stop(m_handle);
	}
	else {

	}

	//新しく再生。
	m_handle = EffectEngine::GetInstance()->Play(m_effect);
}
void Effect::Update()
{
	EffectEngine::GetInstance()->UpdateEffectWorldMatrix(
		m_handle,
		m_position,
		m_rotation,
		m_scale
	);
}
