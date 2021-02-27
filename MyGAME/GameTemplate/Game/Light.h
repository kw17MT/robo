#pragma once
#include "math.h"

struct Light
{
	Vector3 DirDirection = { -1.0f,1.0f,0.0f };
	float pad;
	Vector3 DirColor = { 1.0f,1.0f,1.0f };
	float pad1;
	Vector3 eyePos;
	float pad2;

	//Point
	Vector3 ptPosition = { 0.0f, 60.0f,0.0f };
	float pad3;
	Vector3 ptColor = { 0.0f, 100.0f,100.0f };
	float ptRange = 300.0f;

	//Spot
	Vector3 spDirection = { 1.0f,-1.0f,0.0f };
	float spAngle = Math::DegToRad(30.0f);
};

