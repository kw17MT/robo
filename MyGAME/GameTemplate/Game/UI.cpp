#include "stdafx.h"
#include "UI.h"

bool UI::Start()
{
	m_reticle = NewGO<Reticle>(0);
	return true;
}

void UI::Update()
{
}