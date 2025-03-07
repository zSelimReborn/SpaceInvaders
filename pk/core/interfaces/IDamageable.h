#pragma once

class IDamageable
{
public:
	virtual bool TakeDamage(float InDamage) = 0;
	virtual ~IDamageable() = default;
};