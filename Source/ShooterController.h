#pragma once

#include <Vast.h>

#include <Board2D/BoardFlipbook.h>
#include <Board2D/BoardStateMachine.h>

#include "BulletScript.h"

using namespace Vast;

REG_CLASS(WD::LeftShooterController)
REG_CLASS(WD::RightShooterController)

namespace WD {

	class LeftShooterController : public NativeScript
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
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);			
			
			m_LeftFB = CreateRef<Board2D::Flipbook>();
			m_LeftFB->SetTimeline(0.7f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);
			
			m_UpFB = CreateRef<Board2D::Flipbook>();
			m_UpFB->SetTimeline(0.7f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);
			
			m_DownFB = CreateRef<Board2D::Flipbook>();
			m_DownFB->SetTimeline(0.7f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);

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

			m_Animator = CreateScope<Board2D::StateMachine>();
			m_Animator->PushFlipbook(Idle, m_DownIdle);
			m_Animator->PushFlipbook(Walking, m_DownFB);

			m_Animator->ActivateState(Idle);

			m_Arena = GetEntityByName("1Arena")[0];
		}

		void OnUpdate(Timestep ts) override
		{
			m_LastShot += ts;

			auto& pos = GetComponent<TransformComponent>().Translation;
			if (Input::IsPressed(Key::W))
			{
				m_Animator->EditState(Idle, m_UpIdle);
				m_Animator->EditState(Walking, m_UpFB);
				m_Acc.y += m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else if (Input::IsPressed(Key::S))
			{
				m_Animator->EditState(Idle, m_DownIdle);
				m_Animator->EditState(Walking, m_DownFB);
				m_Acc.y -= m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else if (Input::IsPressed(Key::D))
			{
				m_Animator->EditState(Idle, m_RightIdle);
				m_Animator->EditState(Walking, m_RightFB);
				m_Acc.x += m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else if (Input::IsPressed(Key::A))
			{
				m_Animator->EditState(Idle, m_LeftIdle);
				m_Animator->EditState(Walking, m_LeftFB);
				m_Acc.x -= m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else
			{
				m_Acc = { 0.0f, 0.0f };
				m_Animator->ActivateState(Idle);
			}

			m_Acc *= std::pow(m_Friction, (float)ts);

			pos.x += m_Acc.x * ts;
			pos.y += m_Acc.y * ts;

			m_Animator->GetCurrentFlipbook()->Update(ts);

			GetComponent<RenderComponent>().Texture = m_Animator->GetCurrentFlipbook()->GetCurrentTexture();
		}

		void OnEvent(Event& event) override
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<KeyPressedEvent>(VAST_BIND_EVENT(OnKeyPressed));
		}

		bool OnKeyPressed(KeyPressedEvent& event)
		{
			if (event.GetKeyCode() == Key::LeftShift && event.GetRepeatCount() == 0 && m_LastShot >= 0.25f)
			{
				EmitBullet();
				m_LastShot = 0.0f;
			}

			return false;
		}

		void OnDamageRecieve(int damage)
		{
			m_Health -= damage;
			if (m_Health <= 0)
				DestroySelf();
			else
				GetComponent<TransformComponent>().Scale -= 0.05f;
		}

		const Vector2& GetAcceleration() const { return m_Acc; }
	private:
		void EmitBullet();
	private:
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

		float m_LastShot = 0.0f;

		int m_Health = 100;

		// Physics
		Vector2 m_Acc = { 0.0f, 0.0f };
		float m_Friction = 0.01f;
		float m_Speed = 8.0f;
	};

	class RightShooterController : public NativeScript
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
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_RightFB->PushKeyFrame(GetEntityByName("RightWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);

			m_LeftFB = CreateRef<Board2D::Flipbook>();
			m_LeftFB->SetTimeline(0.7f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_LeftFB->PushKeyFrame(GetEntityByName("LeftWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);

			m_UpFB = CreateRef<Board2D::Flipbook>();
			m_UpFB->SetTimeline(0.7f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_UpFB->PushKeyFrame(GetEntityByName("UpWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);

			m_DownFB = CreateRef<Board2D::Flipbook>();
			m_DownFB->SetTimeline(0.7f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk1")[0].GetComponent<RenderComponent>().Texture, 0.4f);
			m_DownFB->PushKeyFrame(GetEntityByName("DownWalk2")[0].GetComponent<RenderComponent>().Texture, 0.8f);

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

			m_Animator = CreateScope<Board2D::StateMachine>();
			m_Animator->PushFlipbook(Idle, m_DownIdle);
			m_Animator->PushFlipbook(Walking, m_DownFB);

			m_Animator->ActivateState(Idle);

			m_Arena = GetEntityByName("1Arena")[0];
		}

		void OnUpdate(Timestep ts) override
		{
			m_LastShot += ts;

			auto& pos = GetComponent<TransformComponent>().Translation;
			if (Input::IsPressed(Key::Up))
			{
				m_Animator->EditState(Idle, m_UpIdle);
				m_Animator->EditState(Walking, m_UpFB);
				m_Acc.y += m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else if (Input::IsPressed(Key::Down))
			{
				m_Animator->EditState(Idle, m_DownIdle);
				m_Animator->EditState(Walking, m_DownFB);
				m_Acc.y -= m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else if (Input::IsPressed(Key::Right))
			{
				m_Animator->EditState(Idle, m_RightIdle);
				m_Animator->EditState(Walking, m_RightFB);
				m_Acc.x += m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else if (Input::IsPressed(Key::Left))
			{
				m_Animator->EditState(Idle, m_LeftIdle);
				m_Animator->EditState(Walking, m_LeftFB);
				m_Acc.x -= m_Speed * ts;
				m_Animator->ActivateState(Walking);
			}
			else
			{
				m_Acc = { 0.0f, 0.0f };
				m_Animator->ActivateState(Idle);
			}

			m_Acc *= std::pow(m_Friction, (float)ts);

			pos.x += m_Acc.x * ts;
			pos.y += m_Acc.y * ts;

			m_Animator->GetCurrentFlipbook()->Update(ts);

			GetComponent<RenderComponent>().Texture = m_Animator->GetCurrentFlipbook()->GetCurrentTexture();
		}

		void OnEvent(Event& event) override
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<KeyPressedEvent>(VAST_BIND_EVENT(OnKeyPressed));
		}

		bool OnKeyPressed(KeyPressedEvent& event)
		{
			if (event.GetKeyCode() == Key::RightShift && m_LastShot >= 0.25f)
			{
				EmitBullet();
				m_LastShot = 0.0f;
			}

			return false;
		}

		void OnDamageRecieve(int damage)
		{
			m_Health -= damage;
			if (m_Health <= 0)
				DestroySelf();
			else
				GetComponent<TransformComponent>().Scale -= 0.05f;
		}

		const Vector2& GetAcceleration() const { return m_Acc; }
	private:
		void EmitBullet();
	private:
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

		float m_LastShot = 0.0f;

		int m_Health = 100;

		// Physics
		Vector2 m_Acc = { 0.0f, 0.0f };
		float m_Friction = 0.01f;
		float m_Speed = 8.0f;
	};

}