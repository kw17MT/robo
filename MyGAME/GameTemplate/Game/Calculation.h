#pragma once


Vector3 CalcScreenPos(Vector3& screenPos, Vector3 pos)
{
	//�Ή�����G�̃|�W�V��������X�N���[�����W�����߂�
	g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, pos);
	Vector3 calculatedPos = Vector3::Zero;
	calculatedPos.x = -screenPos.x;
	calculatedPos.y = screenPos.y;
	//�G�̈ʒu�ƃJ�����̑O�����̓��ςɂ���ăA�C�R�����f�������߂�
	//�G����J�����ւ̃x�N�g���쐬
	Vector3 targetToCameraVec = g_camera3D->GetPosition() - pos;
	//���K��
	targetToCameraVec.Normalize();
	//�G�̈ʒu�ƃJ�����̑O�����̓���
	float dot = g_camera3D->GetForward().Dot(targetToCameraVec);
	//�G���J�����̑O�����ɂ���Ȃ�Ήf��
	if (dot < 0.0f)
	{
		calculatedPos.z = 0.0f;
	}
	//��둤�ɂ���
	else
	{
		calculatedPos.z = -1.0f;
	}

	return calculatedPos;
}

