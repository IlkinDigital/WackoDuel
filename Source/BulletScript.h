#pragma once

#include <Vast.h>

using namespace Vast;

REG_CLASS(WD::BulletScript)

namespace WD {

	class BulletScript : public ScriptableEntity
	{
	public:
		void OnCreate() override;
		void OnUpdate(Timestep ts) override;
	private:
		float m_Speed = 3.0f;
		Vector2 m_Direction = { 1.0f, 1.0f };
	};
}