#pragma once
#include "math.h"
#include "stdafx.h"

struct DirectionalLight
{
	Vector3 direction = { 0.0f,1.0f,0.0f };
	float pad0;
	Vector3 color = {10.0f, 10.0f, 10.0f };
};

struct Light
{
	DirectionalLight directionalLight;
	float pad1;
	Vector3 eyePos;
	float specPow = 0.5f;
	Vector3 ambientLight = { 0.6f, 0.6f, 0.6f };
};


