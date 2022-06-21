#pragma once

#include <Vast.h>

#include <Board2D/BoardFlipbook.h>
#include <Board2D/BoardStateMachine.h>

using namespace Vast;

REG_CLASS(WD::LeftShooterController)
REG_CLASS(WD::RightShooterController)

namespace WD {

	class LeftShooterController : public ScriptableEntity
	{
	public:
		enum ShooterState : uint16
		{ 
			Idle, Walking
		};

		void OnCreate() override
		{
			m_RightFB = CreateRef<Board2D::Flipbook>();
			m_RightFB->SetTimeline(0.7f);
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);			
			
			m_LeftFB = CreateRef<Board2D::Flipbook>();
			m_LeftFB->SetTimeline(0.7f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);
			
			m_UpFB = CreateRef<Board2D::Flipbook>();
			m_UpFB->SetTimeline(0.7f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);
			
			m_DownFB = CreateRef<Board2D::Flipbook>();
			m_DownFB->SetTimeline(0.7f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_RightIdle = CreateRef<Board2D::Flipbook>();
			m_RightIdle->SetTimeline(1.0f);
			m_RightIdle->PushKeyFrame(GetEntityByName("RightIdle")[0].GetComponent<RenderComponent>().Texture, 0.5f);

			m_LeftIdle = CreateRef<Board2D::Flipbook>();
			m_LeftIdle->SetTimeline(0.7f);
			m_LeftIdle->PushKeyFrame(GetEntityByName("LeftIdle")[0].GetComponent<RenderComponent>().Texture, 0.45f);

			m_UpIdle = CreateRef<Board2D::Flipbook>();
			m_UpIdle->SetTimeline(0.7f);
			m_UpIdle->PushKeyFrame(GetEntityByName("UpIdle")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_DownIdle = CreateRef<Board2D::Flipbook>();
			m_DownIdle->SetTimeline(0.7f);
			m_DownIdle->PushKeyFrame(GetEntityByName("DownIdle")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_Animator = CreateScope<Board2D::StateMachine>(m_DownIdle);
			m_Animator->PushFlipbook(Idle, m_DownIdle);
			m_Animator->PushFlipbook(Walking, m_DownFB);

			m_Animator->ActivateState(Idle);

			m_Arena = GetEntityByName("1Arena")[0];

			CreateEntity("Bullet");
		}

		void OnUpdate(Timestep ts) override
		{
			auto& pos = GetComponent<TransformComponent>().Translation;
			if (Input::IsPressed(Key::W))
			{
				m_Animator->EditState(Idle, m_UpIdle);
				m_Animator->EditState(Walking, m_UpFB);
				m_Acc.y += m_Speed * ts;
			}
			else if (Input::IsPressed(Key::S))
			{
				m_Animator->EditState(Idle, m_DownIdle);
				m_Animator->EditState(Walking, m_DownFB);
				m_Acc.y -= m_Speed * ts;
			}
			else if (Input::IsPressed(Key::D))
			{
				m_Animator->EditState(Idle, m_RightIdle);
				m_Animator->EditState(Walking, m_RightFB);
				m_Acc.x += m_Speed * ts;
			}
			else if (Input::IsPressed(Key::A))
			{
				m_Animator->EditState(Idle, m_LeftIdle);
				m_Animator->EditState(Walking, m_LeftFB);
				m_Acc.x -= m_Speed * ts;
			}
			else
				m_Acc = { 0.0f, 0.0f };

			m_Acc *= std::pow(m_Friction, (float)ts);

			if (m_Acc.x >= 1.0f || m_Acc.y >= 1.0f || m_Acc.x <= -1.0f || m_Acc.y <= -1.0f)
				m_Animator->ActivateState(Walking);
			else
				m_Animator->ActivateState(Idle);

			pos.x += m_Acc.x * ts;
			pos.y += m_Acc.y * ts;

			m_Animator->GetCurrentFlipbook()->Update(ts);

			GetComponent<RenderComponent>().Texture = m_Animator->GetCurrentFlipbook()->GetCurrentTexture();
		}
	private:
		void EmitBullet()
		{
			Entity bullet = CreateEntity("Bullet");
			bullet.AddOrReplaceComponent<TransformComponent>().Translation = GetComponent<TransformComponent>().Translation;
		}
	private:
		Ref<Board2D::Flipbook> m_Active;
		Scope<Board2D::StateMachine> m_Animator;
		Ref<Board2D::Flipbook> m_RightFB;
		Ref<Board2D::Flipbook> m_LeftFB;
		Ref<Board2D::Flipbook> m_UpFB;
		Ref<Board2D::Flipbook> m_DownFB;

		Ref<Board2D::Flipbook> m_RightIdle;
		Ref<Board2D::Flipbook> m_LeftIdle;
		Ref<Board2D::Flipbook> m_UpIdle;
		Ref<Board2D::Flipbook> m_DownIdle;

		Entity m_Arena;

		// Physics
		Vector2 m_Acc = { 0.0f, 0.0f };
		float m_Friction = 0.01f;
		float m_Speed = 8.0f;
	};

	class RightShooterController : public ScriptableEntity
	{
	public:
		enum ShooterState : uint16
		{
			Idle, Walking
		};

		void OnCreate() override
		{
			m_RightFB = CreateRef<Board2D::Flipbook>();
			m_RightFB->SetTimeline(0.7f);
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_LeftFB = CreateRef<Board2D::Flipbook>();
			m_LeftFB->SetTimeline(0.7f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_UpFB = CreateRef<Board2D::Flipbook>();
			m_UpFB->SetTimeline(0.7f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_DownFB = CreateRef<Board2D::Flipbook>();
			m_DownFB->SetTimeline(0.7f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk1")[0].GetComponent<RenderComponent>().Texture, 0.45f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk2")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_RightIdle = CreateRef<Board2D::Flipbook>();
			m_RightIdle->SetTimeline(1.0f);
			m_RightIdle->PushKeyFrame(GetEntityByName("RightIdle")[0].GetComponent<RenderComponent>().Texture, 0.5f);

			m_LeftIdle = CreateRef<Board2D::Flipbook>();
			m_LeftIdle->SetTimeline(0.7f);
			m_LeftIdle->PushKeyFrame(GetEntityByName("LeftIdle")[0].GetComponent<RenderComponent>().Texture, 0.45f);

			m_UpIdle = CreateRef<Board2D::Flipbook>();
			m_UpIdle->SetTimeline(0.7f);
			m_UpIdle->PushKeyFrame(GetEntityByName("UpIdle")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_DownIdle = CreateRef<Board2D::Flipbook>();
			m_DownIdle->SetTimeline(0.7f);
			m_DownIdle->PushKeyFrame(GetEntityByName("DownIdle")[0].GetComponent<RenderComponent>().Texture, 0.9f);

			m_Animator = CreateScope<Board2D::StateMachine>(m_DownIdle);
			m_Animator->PushFlipbook(Idle, m_DownIdle);
			m_Animator->PushFlipbook(Walking, m_DownFB);

			m_Animator->ActivateState(Idle);

			m_Arena = GetEntityByName("1Arena")[0];
		}

		void OnUpdate(Timestep ts) override
		{
			auto& pos = GetComponent<TransformComponent>().Translation;
			if (Input::IsPressed(Key::Up))
			{
				m_Animator->EditState(Idle, m_UpIdle);
				m_Animator->EditState(Walking, m_UpFB);
				pos.y += m_Speed * ts;
			}
			else if (Input::IsPressed(Key::Down))
			{
				m_Animator->EditState(Idle, m_DownIdle);
				m_Animator->EditState(Walking, m_DownFB);
				pos.y -= m_Speed * ts;
			}
			else if (Input::IsPressed(Key::Right))
			{
				m_Animator->EditState(Idle, m_RightIdle);
				m_Animator->EditState(Walking, m_RightFB);
				pos.x += m_Speed * ts;
			}
			else if (Input::IsPressed(Key::Left))
			{
				m_Animator->EditState(Idle, m_LeftIdle);
				m_Animator->EditState(Walking, m_LeftFB);
				pos.x -= m_Speed * ts;
			}
			

			if (m_LastTranslation != pos)
				m_Animator->ActivateState(Walking);
			else
				m_Animator->ActivateState(Idle);


			m_Animator->GetCurrentFlipbook()->Update(ts);

			GetComponent<RenderComponent>().Texture = m_Animator->GetCurrentFlipbook()->GetCurrentTexture();
			m_LastTranslation = pos;
		}
	private:
		void EmitBullet()
		{
			Entity bullet = CreateEntity("Bullet");

			bullet.GetComponent<TransformComponent>().Translation = GetComponent<TransformComponent>().Translation;
		}
	private:
		Vector3 m_LastTranslation;
		Ref<Board2D::Flipbook> m_Active;
		Scope<Board2D::StateMachine> m_Animator;
		Ref<Board2D::Flipbook> m_RightFB;
		Ref<Board2D::Flipbook> m_LeftFB;
		Ref<Board2D::Flipbook> m_UpFB;
		Ref<Board2D::Flipbook> m_DownFB;

		Ref<Board2D::Flipbook> m_RightIdle;
		Ref<Board2D::Flipbook> m_LeftIdle;
		Ref<Board2D::Flipbook> m_UpIdle;
		Ref<Board2D::Flipbook> m_DownIdle;

		Entity m_Arena;

		float m_Speed = 1.2f;
	};

}