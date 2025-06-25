
#include "WeaponAxe.h"
#include "../../Application.h"

WeaponAxe::WeaponAxe(void)
{
}

WeaponAxe::~WeaponAxe(void)
{
}

void WeaponAxe::SetParam(void)
{
    speed_ = AXE_SPEED;
    collisionRadius_ = AXE_COLLISION_RADIUS;
    scales_ = AXE_SCALE;
    gravity_ = AXE_GRAVITY;
    jumpPow_ = AXE_JUMP_POW;
}

void WeaponAxe::Update()
{
    if (!isAlive_) return;

    // �����^��
    UpdateParabolaAngle(pos_, velocity_, gravity_, angles_);

    // �n�ʂɓ��������Ƃ��ɐ�����Ԃ�����

}

void WeaponAxe::Draw(void)
{
    if (isAlive_ && modelId_ != -1) {
        MV1SetPosition(modelId_, pos_);
        MV1SetRotationXYZ(modelId_, angles_);
        MV1DrawModel(modelId_);
    }
}

void WeaponAxe::Release(void)
{
    if (modelId_ != -1) {
        MV1DeleteModel(modelId_);
        modelId_ = -1;
    }
}

void WeaponAxe::Use(VECTOR pos, VECTOR dir)
{
    isAlive_ = true;
    pos_ = VAdd(pos, AXE_RELATIVE_POS);
    velocity_ = VScale(dir, speed_);
    velocity_.y = jumpPow_;
    angles_.y = atan2f(dir.x, dir.z);
    angles_.x = 0;
    angles_.z = 0;
    Load(); // ���f���������[�h�Ȃ烍�[�h
    MV1SetPosition(modelId_, pos_);
    MV1SetRotationXYZ(modelId_, angles_);
}

void WeaponAxe::Load(void)
{
    if (modelId_ == -1)
    {
        // �p�X���v���W�F�N�g�ɍ��킹�ďC��
        modelId_ = MV1LoadModel("Data/Model/Weapon/Axe_Wood.mv1");
        MV1SetScale(modelId_, scales_);
    }
}

void WeaponAxe::UpdateParabolaAngle(VECTOR& pos, VECTOR& vel, float gravity, VECTOR& angles)
{
    //���˕��^��
    pos.x += vel.x;
    pos.y += vel.y;
    pos.z += vel.z;
    vel.y -= gravity;

    //�i�s�����ɐn��������
    angles.y = atan2f(vel.x, vel.z);
}