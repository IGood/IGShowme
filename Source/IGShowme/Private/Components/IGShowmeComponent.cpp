// Copyright Ian Good

#include "Components/IGShowmeComponent.h"
#include "GameFramework/Actor.h"
#include "IGShowmeInternal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IGShowmeComponent)

#define IGSHOWME_SUBJECTICO TEXT("📦")

UIGShowmeComponent::UIGShowmeComponent()
{
	IGSHOWME_LOG_CONSTRUCTOR;

	OriginalName = GetFName();

	bWantsInitializeComponent = true;

	OnComponentActivated.AddDynamic(this, &ThisClass::OnComponentActivated_Event);
	OnComponentDeactivated.AddDynamic(this, &ThisClass::OnComponentDeactivated_Event);
}

UIGShowmeComponent::~UIGShowmeComponent()
{
	IGSHOWME_LOG_DESTRUCTOR;
}

void UIGShowmeComponent::BeginDestroy()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	// do custom stuff before calling Super

	NameDuringDestruction = GetFName();
	NetIcoDuringDestruction = IG_NET_ICO(this);

	Super::BeginDestroy();
}

void UIGShowmeComponent::FinishDestroy()
{
	IGSHOWME_LOG_FINISHDESTROY;

	// do custom stuff before calling Super

	Super::FinishDestroy();
}

void UIGShowmeComponent::PostInitProperties()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostInitProperties();

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *TestMeString);
}

void UIGShowmeComponent::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostLoadSubobjects(OuterInstanceGraph);

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *TestMeString);
}

void UIGShowmeComponent::PostLoad()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostLoad();

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *TestMeString);
}

void UIGShowmeComponent::Activate(bool bReset)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("reset=%s", IG_FMT(bReset));

	Super::Activate(bReset);
}

void UIGShowmeComponent::Deactivate()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::Deactivate();
}

void UIGShowmeComponent::OnRegister()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::OnRegister();
}

void UIGShowmeComponent::OnUnregister()
{
	IGSHOWME_LOG_FUNCTION_CALL;
	UE_LOG(LogIGShowme, Log, TEXT("%s: %s %s { IsBeingDestroyed()=%s }"), IG_NET_ICO(this), IGSHOWME_SUBJECTICO, *GetName(), IG_FMT(IsBeingDestroyed()))

	Super::OnUnregister();
}

void UIGShowmeComponent::InitializeComponent()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::InitializeComponent();
}

void UIGShowmeComponent::UninitializeComponent()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::UninitializeComponent();
}

void UIGShowmeComponent::ReadyForReplication()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::ReadyForReplication();
}

void UIGShowmeComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	////IGSHOWME_LOG_FUNCTION_CALL; // noisy!

	Super::PreReplication(ChangedPropertyTracker);
}

void UIGShowmeComponent::OnReplicationStartedForIris(const FOnReplicationStartedParams& Params)
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::OnReplicationStartedForIris(Params);
}

void UIGShowmeComponent::OnStopReplicationForIris(const FOnStopReplicationParams& Params)
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::OnStopReplicationForIris(Params);
}

void UIGShowmeComponent::BeginPlay()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::BeginPlay();
}

void UIGShowmeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("reason=%s", IG_FMT(EndPlayReason));

	Super::EndPlay(EndPlayReason);
}

void UIGShowmeComponent::DestroyComponent(bool bPromoteChildren)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("promoteChildren=%s", IG_FMT(bPromoteChildren));

	Super::DestroyComponent(bPromoteChildren);
}

void UIGShowmeComponent::OnComponentCreated()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::OnComponentCreated();
}

void UIGShowmeComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("destroyingHierarchy=%s", IG_FMT(bDestroyingHierarchy));

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UIGShowmeComponent::OnComponentActivated_Event(UActorComponent* Component, bool bReset)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("reset=%s", IG_FMT(bReset));
}

void UIGShowmeComponent::OnComponentDeactivated_Event(UActorComponent* Component)
{
	IGSHOWME_LOG_FUNCTION_CALL;
}

#undef IGSHOWME_SUBJECTICO
