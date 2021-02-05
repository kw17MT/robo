#include "stdafx.h"
#include "Light.h"
#include "math.h"

//ディレクションライトの向きを決める。
void Light::SetDirLig(float x, float y, float z)
{
	DirDirection.x = x;
	DirDirection.y = y;
	DirDirection.z = z;
}

//ディレクションライトの色を決める。
void Light::SetDirColor(float red, float green, float blue)
{
	DirColor.x = red;
	DirColor.y = green;
	DirColor.z = blue;
}

//ポイントライトの向きを決める。
void Light::SetPtPos(float x, float y, float z)
{
	ptPosition.x = x;
	ptPosition.y = y;
	ptPosition.z = z;
}

//ポイントライトの色を決める。
void Light::SetPtColor(float red, float green, float blue)
{
	ptColor.x = red;
	ptColor.x = green;
	ptColor.x = blue;
}

//スポットライトの方向を決める。
void Light::SetSpDir(float x, float y, float z)
{
	spDirection.x = x;
	spDirection.y = y;
	spDirection.z = z;
}

void Light::Normalize(Vector3 v)
{
	float nor = v.x * v.x + v.y * v.y + v.z * v.z;
	nor = sqrt(nor);
	if (nor < 0.0f) { nor *= -1.0f; }

	v.x = v.x / nor; v.y = v.y / nor; v.z = v.z / nor;
}