#pragma once
#include "BaseEnemy.h"

namespace enemies
{
	class NormalEnemy : public BaseEnemy
	{
	public:
		NormalEnemy();
	};

	class ThickEnemy : public BaseEnemy
	{
	public:
		ThickEnemy();
	};

	class FastEnemy : public BaseEnemy
	{
	public:
		FastEnemy();
	};
}