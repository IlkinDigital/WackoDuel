#include "enginepch.h"
#include "Generated.h"

#include "WackoDuel\BulletScript.h"
#include "WackoDuel\ShooterController.h"

extern "C"
{
	static ScriptBuffer s_Scripts;

	SCRIPT_API const ScriptBuffer& GetScripts()
	{
		return s_Scripts;
	}

	SCRIPT_API void InitScripts()
	{
		Vast::NativeScriptComponent var0;
		var0.Bind<WD::BulletScript>();
		var0.Name = "WD::BulletScript";
		s_Scripts.emplace_back(var0);
		Vast::NativeScriptComponent var1;
		var1.Bind<WD::LeftShooterController>();
		var1.Name = "WD::LeftShooterController";
		s_Scripts.emplace_back(var1);
		Vast::NativeScriptComponent var2;
		var2.Bind<WD::RightShooterController>();
		var2.Name = "WD::RightShooterController";
		s_Scripts.emplace_back(var2);

	}

	SCRIPT_API void InitModule(Vast::Application* appPtr)
	{
		Vast::Application::Set(appPtr);
		Vast::Logger::Init();
		Vast::Application::Get().GetWindow().Init();
	}
}
