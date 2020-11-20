#include "Enemy.h"

void Enemy::Attack(SObject* pPlayer)
{
	if (m_pProjectile)
	{
		SProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };
		SPoint vDir = pPlayer->m_ptPos - m_ptPos;
		info.m_vDir = SPoint(0.0f, 1.0f);
		m_ProjectileList.push_back(info);
	}
}

void Enemy::Damage()
{

}
void Enemy::Dead()
{

}