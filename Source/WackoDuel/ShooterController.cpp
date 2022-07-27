#include "enginepch.h"
#include "ShooterController.h"

#include "BulletScript.h"

#include "Engine/DebugRenderer.h"

void WD::LeftShooterController::EmitBullet()
{
	if (m_Acc.x != 0.0f || m_Acc.y != 0.0f)
	{
		Entity bullet = CreateEntity("Bullet");

		auto& tc = bullet.GetComponent<TransformComponent>();
		tc.Translation = GetComponent<TransformComponent>().Translation;
		tc.Translation.z += 1.0f;
		tc.Scale = { 0.3f, 0.3f, 0.3f };
		bullet.AddComponent<RenderComponent>().Color = { 0.1f, 0.1f, 0.1f, 1.0f };
		bullet.AddComponent<NativeScriptComponent>().Bind<WD::BulletScript>("Shooter1", "Shooter2");
	}
}

void WD::RightShooterController::EmitBullet()
{
	if (m_Acc.x != 0.0f || m_Acc.y != 0.0f)
	{
		Entity bullet = CreateEntity("Bullet");

		auto& tc = bullet.GetComponent<TransformComponent>();
		tc.Translation = GetComponent<TransformComponent>().Translation;
		tc.Translation.z += 1.0f;
		tc.Scale = { 0.3f, 0.3f, 0.3f };
		bullet.AddComponent<RenderComponent>().Color = { 0.1f, 0.1f, 0.1f, 1.0f };
		bullet.AddComponent<NativeScriptComponent>().Bind<WD::BulletScript>("Shooter2", "Shooter1");
	}
}
