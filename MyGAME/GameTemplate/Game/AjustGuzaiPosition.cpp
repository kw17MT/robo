#include "stdafx.h"
#include "AjustGuzaiPosition.h"
#include "SoundSource.h"

namespace {
	const int DRAW_PRIORITY_HIGH = 0;
	const float SE_VOLUME = 1.0f;
	const float AJUST_SPEED_TO_FOLLOW_PLAYER = 90.0f;
}

Vector3 AjustGuzaiPosition::CalcAjustedPosition(Vector3 normalizedSpeed, Vector3 playerPosition)
{
	Vector3 ajustedPosition = playerPosition;
	ajustedPosition += normalizedSpeed * AJUST_SPEED_TO_FOLLOW_PLAYER;

	return ajustedPosition;
}