#include "stdafx.h"
#include "GuzaiGene.h"
#include "Guzai.h"
#include "Buff.h"
#include "DeBuff.h"
#include "SkinModelRender.h"
#include "DishGene.h"

#include <ctime>        // time
#include <cstdlib>      // srand,rand
#include <random>		//—”¶¬—p

GuzaiGene::~GuzaiGene()
{
	//DeleteGO(m_skinModelRender);

	////‹ïŞ‚ğíœ
	//DeleteGO(m_skinModelRender);
	/*for (int i = 0; i < guzaiNum; i++) {
		if (m_guzai[i] != nullptr) {
			DeleteGO(m_guzai[i]);
		}
	}*/
	for (int i = 0; i < 99; i++) {
		if (m_guzaiFlag[i] == true) {
			DeleteGO(m_guzai[i]);
		}
	}
}

bool GuzaiGene::Start()
{
	/*m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gu/egg.tkm", nullptr, enModelUpAxisY, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);*/

	//m_dishGene = FindGO<DishGene>("dishGene");

	return true;
}

void GuzaiGene::Update()
{
}