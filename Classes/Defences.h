#include "BaseTurret.h"

namespace defences
{
	class MachineGun : public BaseTurret
	{
	public:
		MachineGun();
		MachineGun(float cellSize);
	};

	class Gun : public BaseTurret
	{
	public:
		Gun();
		Gun(float cellSize);

	private:
		float reload;
		float reloadBuffer;

		void attack(enemies::BaseEnemy* enemy, float delta) override;
	};

	class Artillery : public BaseTurret
	{
	public:
		Artillery();
		Artillery(float cellSize);

		void update(std::list< std::unique_ptr<enemies::BaseEnemy> >& enemiesList, float delta) override;
	private:
		float reload;
		float reloadBuffer;
		float damageRadius;

		void attack(std::list<enemies::BaseEnemy*> enemies, float delta);
		std::list<enemies::BaseEnemy*> getEnemiesInRadius(std::list< std::unique_ptr<enemies::BaseEnemy> >& enemiesList, Vec2 center, float radius);
	};
}