#include "stdafx.h"
#include "SoundManager.h"
#include "SoundSource.h"

SoundManager::SoundManager()
{
	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/BGM/BGM1.wav", false);

	for (int i = 0; i < m_soundNum; i++) {
		m_se[i] = NewGO<CSoundSource>(0);
		m_se[i]->Init(m_seCollection[i], false);
	}
}

SoundManager::~SoundManager()
{
	DeleteGO(m_bgm);
	
	for (int i = 0; i < m_soundNum; i++) {
		DeleteGO(m_se[i]);
	}
}