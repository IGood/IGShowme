// Copyright Ian Good

#include "Subsystems/IGShowmeWorldSubsystem.h"
#include "Engine/World.h"
#include "IGShowmeInternal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IGShowmeWorldSubsystem)

#define IGSHOWME_SUBJECTICO TEXT("🌐")

UIGShowmeWorldSubsystem::UIGShowmeWorldSubsystem()
{
	IGSHOWME_LOG_CONSTRUCTOR;

	OriginalName = GetFName();
}

UIGShowmeWorldSubsystem::~UIGShowmeWorldSubsystem()
{
	IGSHOWME_LOG_DESTRUCTOR;
}

bool UIGShowmeWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
#define this Outer
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("outer=%s", *CastChecked<UWorld>(Outer)->GetDebugDisplayName());
#undef this

	return Super::ShouldCreateSubsystem(Outer);
}

void UIGShowmeWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::Initialize(Collection);
}

void UIGShowmeWorldSubsystem::Deinitialize()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	NameDuringDestruction = GetFName();
	NetIcoDuringDestruction = IG_NET_ICO(this);

	Super::Deinitialize();
}

void UIGShowmeWorldSubsystem::PostInitialize()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostInitialize();
}

void UIGShowmeWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("world=%s", *InWorld.GetDebugDisplayName());

	Super::OnWorldBeginPlay(InWorld);
}

void UIGShowmeWorldSubsystem::OnWorldComponentsUpdated(UWorld& World)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("world=%s", *World.GetDebugDisplayName());

	Super::OnWorldComponentsUpdated(World);
}

bool UIGShowmeWorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("world_type=%s", IG_FMT(WorldType));

	return Super::DoesSupportWorldType(WorldType);
}

#undef IGSHOWME_SUBJECTICO
