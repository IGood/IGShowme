// Copyright Ian Good

#include "IGNetIco.h"
#include "Engine/World.h"

namespace IG
{
const TCHAR* GetNetModeIcons(const UObject* WorldContextObject)
{
	const UWorld* World = (WorldContextObject != nullptr) ? WorldContextObject->GetWorld() : nullptr;
	if (World != nullptr)
	{
		switch (World->GetNetMode())
		{
			// server with no local players
			case NM_DedicatedServer:
				return TEXT("🎅");

			// client connected to a remote server
			case NM_Client:
				return TEXT("👶");

			// other
			default:
				return TEXT("🎅👶");
		}
	}

	return TEXT("🤔");
}

} // namespace IG
