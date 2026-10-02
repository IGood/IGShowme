// Copyright Ian Good

#include "Actors/IGShowmeActor.h"
#include "Components/IGShowmeComponent.h"
#include "IGShowmeInternal.h"
#include "Net/Iris/ReplicationSystem/EngineReplicationBridge.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IGShowmeActor)

#define IGSHOWME_SUBJECTICO TEXT("🎭")

AIGShowmeActor::AIGShowmeActor()
{
	IGSHOWME_LOG_CONSTRUCTOR;

	OriginalName = GetFName();

	// Create a component in code so we can observe the difference between this one & ones that may be created by Blueprint.
	NativeShowmeComponent = CreateDefaultSubobject<UIGShowmeComponent>("NativeShowmeComponent");
	NativeShowmeComponent->TestMeString = TEXT("native-value");

	OnEndPlay.AddDynamic(this, &ThisClass::OnEndPlay_Event);
	OnDestroyed.AddDynamic(this, &ThisClass::OnDestroyed_Event);
}

AIGShowmeActor::~AIGShowmeActor()
{
	IGSHOWME_LOG_DESTRUCTOR;
}

void AIGShowmeActor::BeginDestroy()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	// do custom stuff before calling Super

	NameDuringDestruction = GetFName();
	NetIcoDuringDestruction = IG_NET_ICO(this);

	Super::BeginDestroy();
}

void AIGShowmeActor::FinishDestroy()
{
	IGSHOWME_LOG_FINISHDESTROY;

	// do custom stuff before calling Super

	Super::FinishDestroy();
}

void AIGShowmeActor::PostInitProperties()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostInitProperties();

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *TestMeString);
}

void AIGShowmeActor::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostLoadSubobjects(OuterInstanceGraph);

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *TestMeString);
}

void AIGShowmeActor::PostLoad()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostLoad();

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *TestMeString);
}

void AIGShowmeActor::RegisterAllComponents()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::RegisterAllComponents();
}

void AIGShowmeActor::PreRegisterAllComponents()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PreRegisterAllComponents();
}

void AIGShowmeActor::PostRegisterAllComponents()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostRegisterAllComponents();
}

void AIGShowmeActor::UnregisterAllComponents(bool bForReregister)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("forReregister=%s", IG_FMT(bForReregister));

	Super::UnregisterAllComponents(bForReregister);
}

void AIGShowmeActor::PostUnregisterAllComponents()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostUnregisterAllComponents();
}

void AIGShowmeActor::ReregisterAllComponents()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::ReregisterAllComponents();
}

void AIGShowmeActor::OnConstruction(const FTransform& Transform)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("transform=%s", IG_FMT(Transform));

	Super::OnConstruction(Transform);
}

void AIGShowmeActor::PostActorCreated()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostActorCreated();
}

void AIGShowmeActor::PreInitializeComponents()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PreInitializeComponents();
}

void AIGShowmeActor::PostInitializeComponents()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostInitializeComponents();
}

void AIGShowmeActor::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	////IGSHOWME_LOG_FUNCTION_CALL; // noisy!

	Super::PreReplication(ChangedPropertyTracker);
}

void AIGShowmeActor::OnReplicationStartedForIris(const FOnReplicationStartedParams& Params)
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::OnReplicationStartedForIris(Params);
}
void AIGShowmeActor::OnStopReplicationForIris(const FOnStopReplicationParams& Params)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("reason=%s", IG_FMT(Params.StopReplicatingParams.StopReplicatingReason));

	Super::OnStopReplicationForIris(Params);
}

void AIGShowmeActor::BeginPlay()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::BeginPlay();
}

void AIGShowmeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("reason=%s", IG_FMT(EndPlayReason));

	Super::EndPlay(EndPlayReason);
}

void AIGShowmeActor::OnEndPlay_Event(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	IGSHOWME_LOG_FUNCTION_CALL_ARGS("reason=%s", IG_FMT(EndPlayReason));
}

void AIGShowmeActor::OnDestroyed_Event(AActor* DestroyedActor)
{
	IGSHOWME_LOG_FUNCTION_CALL;
}

#undef IGSHOWME_SUBJECTICO
