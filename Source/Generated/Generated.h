
#include "Vast/Scene/Components/NativeScriptComponent.h"
#include "Vast/Scene/Entity.h"
#include "Vast/ApplicationCore/Application.h"

#define SCRIPT_API __declspec(dllexport)

extern "C"
{
	using ScriptBuffer = Vast::DArray<Vast::NativeScriptComponent>;

	SCRIPT_API const ScriptBuffer& GetScripts();
	SCRIPT_API void InitScripts();
	SCRIPT_API void InitModule(Vast::Application* appPtr);
}
