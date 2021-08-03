#include "stdafx.h"
#include "GuzaiScale.h"

namespace
{
	const Vector3 EGG_SCALE = { 0.7f,1.0f,0.7f };
	const Vector3 MAX_COOKED_EGG_SCALE = { 1.0,1.3f,1.0f };
	const Vector3 MAX_TARGETED_SCALE = { 1.3f,1.3f,1.3f };
	const Vector3 MIN_TARGETED_SCALE = Vector3::One;
	const Vector3 TARGETED_SCALE_AMOUNT = { 0.05f,0.05f,0.05f };
}

Vector3 GuzaiScale::ChangeScaleDependOnTargetedState(bool isTargeted, int typeNumber, Vector3 nowScale)
{
	//プレイヤーにターゲットされていたら拡大表示
	Vector3 finalScale = nowScale;

	if (isTargeted == true) {
		if (typeNumber == 1) {
			finalScale += TARGETED_SCALE_AMOUNT;
			if (finalScale.x >= MAX_COOKED_EGG_SCALE.x) {
				finalScale = MAX_COOKED_EGG_SCALE;
			}
			return finalScale;
		}
		else {
			finalScale += TARGETED_SCALE_AMOUNT;
			if (finalScale.x >= MAX_TARGETED_SCALE.x) {
				finalScale = MAX_TARGETED_SCALE;
			}
			return finalScale;
		}
	}
	//されていなければ普通のサイズに
	else {
		if (typeNumber == 1) {
			finalScale -= TARGETED_SCALE_AMOUNT;
			if (finalScale.x <= EGG_SCALE.x) {
				finalScale = EGG_SCALE;
			}
			return finalScale;
		}
		else {
			finalScale -= TARGETED_SCALE_AMOUNT;
			if (finalScale.x <= MIN_TARGETED_SCALE.x) {
				finalScale = MIN_TARGETED_SCALE;
			}
			return finalScale;
		}
	}
}