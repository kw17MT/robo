#pragma once


struct Light
{
	//ê≥ãKâªÇ∑ÇÈÇ±Ç∆
	Vector3 DirDirection = { 0.0f,-1.0f,0.0f };
	float pad;
	Vector3 DirColor = { 0.5f,0.5f,0.5f };
};

class LightMaji
{
	Light light;

	Vector3 eyePos;
	float specPow = 0.5f;
	Vector3 ambinetLight = { 0.3f,0.3f,0.3f };
};