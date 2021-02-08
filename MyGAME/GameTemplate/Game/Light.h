#pragma once

struct Light
{
	Vector3 DirDirection;
	float pad;
	Vector3 DirColor;
	float pad1;
	Vector3 eyePos;
	float pad2;

	//Point
	Vector3 ptPosition;
	float pad3;
	Vector3 ptColor;
	float ptRange;

	//Spot
	Vector3 spDirection;
	float spAngle;

	/*void SetDirLig(float x, float y, float z);
	void SetDirColor(float red, float green, float blue);

	void SetPtPos(float x, float y, float z);
	void SetPtColor(float red, float green, float blue);

	void SetSpDir(float x, float y, float z);*/

	void Update();
};

