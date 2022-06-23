#include "enginepch.h"
#include "BulletScript.h"

#include "ShooterController.h"

void WD::BulletScript::OnCreate()
{
	auto& nsc = GetEntityByName("Shooter1")[0].GetComponent<NativeScriptComponent>();
	m_Direction = ((LeftShooterController*)nsc.Instance)->GetAcceleration();

	float length = std::sqrtf(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y);

	m_Direction /= length;
	
	GetComponent<RenderComponent>().Texture = GetEntityByName("Rock")[0].GetComponent<RenderComponent>().Texture;
}

void WD::BulletScript::OnUpdate(Timestep ts)
{
	auto& pos = GetComponent<TransformComponent>().Translation;

	pos.x += m_Direction.x * m_Speed * ts;
	pos.y += m_Direction.y * m_Speed * ts;

	GetComponent<TransformComponent>().Rotation.z += glm::radians(m_Speed * 100) * ts;
}
