#pragma once

#include <Vast.h>

using namespace Vast;

REG_CLASS(WD::BulletScript)

namespace WD {

	class BulletScript : public ScriptableEntity
	{
	public:
		BulletScript() = default;
		BulletScript(const String& master, const String& enemy)
			: m_MasterName(master), m_EnemyName(enemy) {}

		void SetMasterName(const String& name) { m_MasterName = name; }
		void SetEnemyName(const String& name) { m_EnemyName = name; }

		void OnCreate() override;
		void OnUpdate(Timestep ts) override;
		void OnDestroy() override
		{
			VAST_TRACE("Bullet destroyed");
		}
	private:
		String m_MasterName;
		String m_EnemyName;
		float m_Speed = 3.0f;
		Vector2 m_Direction = { 1.0f, 1.0f };
	};
}