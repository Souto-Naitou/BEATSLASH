#include "BossRazerCollider.h"

void BossRazerCollider::OnCollisionEnter(Collider* other)
{
    // ヒットエフェクト等を追加する場合の拡張点
    (void)other;
}

void BossRazerCollider::OnCollisionStay(Collider* other)
{
    (void)other;
}

void BossRazerCollider::OnCollisionExit(Collider* other)
{
    (void)other;
}
