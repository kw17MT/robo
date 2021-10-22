#include "stdafx.h"
#include "UI.h"

UI::~UI()
{
	DeleteGO(m_reticle);
}

bool UI::Start()
{
	m_reticle = NewGO<Reticle>(0);
	return true;
}

void UI::Update()
{
}