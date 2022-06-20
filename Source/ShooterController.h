#pragma once

#include <Vast.h>

using namespace Vast;

REG_CLASS(WD::LeftShooterController)
REG_CLASS(WD::RightShooterController)

namespace WD {

	class LeftShooterController : public ScriptableEntity
	{
	public:
		void OnCreate() override
		{
			m_Arena = GetEntityByName("Arena")[0];
		}

		void OnUpdate(Timestep ts) override
		{
			auto& pos = GetComponent<TransformComponent>().Translation;
			if (Input::IsPressed(Key::W))
				m_Acc.y += m_Speed * ts;
			if (Input::IsPressed(Key::S))
				m_Acc.y -= m_Speed * ts;
			if (Input::IsPressed(Key::D))
				m_Acc.x += m_Speed * ts;
			if (Input::IsPressed(Key::A))
				m_Acc.x -= m_Speed * ts;

			m_Acc *= std::pow(m_Friction, (float)ts);

			pos.x += m_Acc.x * ts;
			pos.y += m_Acc.y * ts;
		}
	private:
		void EmitBullet()
		{
			Entity bullet = CreateEntity("Bullet");

			bullet.GetComponent<TransformComponent>().Translation = GetComponent<TransformComponent>().Translation;
		}
	private:
		Entity m_Arena;
		Vector2 m_Acc = { 0.0f, 0.0f };
		float m_Friction = 0.05f;
		float m_Speed = 8.0f;
	};

	class RightShooterController : public ScriptableEntity
	{
	public:
		void OnUpdate(Timestep ts) override
		{
			auto& pos = GetComponent<TransformComponent>().Translation;
			if (Input::IsPressed(Key::Up))
				pos.y += m_Speed * ts;
			if (Input::IsPressed(Key::Down))
				pos.y -= m_Speed * ts;
			if (Input::IsPressed(Key::Right))
				pos.x += m_Speed * ts;
			if (Input::IsPressed(Key::Left))
				pos.x -= m_Speed * ts;
		}
	private:
		float m_Speed = 2.0f;
	};

}