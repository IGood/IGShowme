// Copyright Ian Good

#include "Modules/ModuleManager.h"
#include "IGShowmeInternal.h"

DEFINE_LOG_CATEGORY(LogIGShowme);

#define IGSHOWME_SUBJECTICO TEXT("🔌")

namespace IG::Showme
{
	class FModule final : public IModuleInterface
	{
		virtual void StartupModule() override
		{
			IGSHOWME_LOG_FUNCTION_CALL_SLIM;
		}

		virtual void PreUnloadCallback() override
		{
			IGSHOWME_LOG_FUNCTION_CALL_SLIM;
		}

		virtual void PostLoadCallback() override
		{
			IGSHOWME_LOG_FUNCTION_CALL_SLIM;
		}

		virtual void ShutdownModule() override
		{
			IGSHOWME_LOG_FUNCTION_CALL_SLIM;
		}
	};

} // namespace IG::Showme

IMPLEMENT_MODULE(IG::Showme::FModule, IGShowme);

#undef IGSHOWME_SUBJECTICO
