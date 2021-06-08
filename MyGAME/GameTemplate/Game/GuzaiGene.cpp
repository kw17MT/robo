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
	for (int i = 0; i < 99; i++) {
		if (m_guzaiFlag[i] == true) {
			DeleteGO(m_guzai[i]);
		}
	}
}

bool GuzaiGene::Start()
{
	return true;
}

void GuzaiGene::Update()
{
}