#pragma once
#include "SoundSource.h"

class SoundManager
{
private:
	static const int m_soundNum = 18;

	static SoundManager* instance;
	CSoundSource* m_bgm = nullptr;
	CSoundSource* m_se[m_soundNum] = { nullptr };

	wchar_t m_seCollection[m_soundNum][256]
	{
		L"Assets/sound/basketball_buzzer1.wav",
		L"Assets/sound/blip01.wav",
		L"Assets/sound/button03b.wav",
		L"Assets/sound/correct_answer.wav",
		L"Assets/sound/cutting_a_onion_sppedy.wav",
		L"Assets/sound/dumping.wav",
		L"Assets/sound/falling2.wav",
		L"Assets/sound/machine_rotation1.wav",
		L"Assets/sound/machine_rotation2.wav",
		L"Assets/sound/mechanical.wav",
		L"Assets/sound/Money.wav",
		L"Assets/sound/PlayerAttack_00.wav",
		L"Assets/sound/poka01.wav",
		L"Assets/sound/poka02.wav",
		L"Assets/sound/putting_a_book2.wav",
		L"Assets/sound/select07.wav",
		L"Assets/sound/thi-n.wav",
		L"Assets/sound/Time.wav",
	};

public:
	SoundManager();
	~SoundManager();

	/**
	 * @brief インスタンスの取得
	 * @return 具材管理のインスタンス
	*/
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}

	/**
	 * @brief インスタンスの作成
	*/
	static void CreateInstance()
	{
		instance = new SoundManager;
	}

	/**
	 * @brief インスタンスの削除
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	void PlaySe(int seNumber) {
		m_se[seNumber]->Play(false);
	}

	void PlayBGM(bool playBGM) {
		if(playBGM)	m_bgm->Play(true);
		else m_bgm->Stop();
	}

	enum
	{
		enBuzzer = 0,
		enBoo,
		enChangeSprite,
		enCorrectAnswer,
		enCutting,
		enTrash,
		enFalling,
		enMachineRotation1,
		enMachineRotation2,
		enMechanical,
		enMoney,
		PlayerAttack_00,				//使ってる？
		enGrab,
		enPut,
		enPut2,
		enTarget,
		enThin,
		enTime,
	};
};

