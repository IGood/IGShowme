// Copyright Ian Good

#pragma once

#include "GameFramework/Actor.h"
#include "IGShowmeActor.generated.h"

/**
 * Represents an actor that can be used to demonstrate the lifecycle and behaviors of an actor.
 *
 * The methods here just log messages before calling the base implementation so you can observe the order that methods
 * are executed.
 */
UCLASS(Blueprintable)
class AIGShowmeActor : public AActor
{
	GENERATED_BODY()

	AIGShowmeActor();
	~AIGShowmeActor();

	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;

	virtual void PostInitProperties() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void PostLoad() override;

	virtual void RegisterAllComponents() override;
	virtual void PreRegisterAllComponents() override;
	virtual void PostRegisterAllComponents() override;

	virtual void UnregisterAllComponents(bool bForReregister) override;
	virtual void PostUnregisterAllComponents() override;
	virtual void ReregisterAllComponents() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostActorCreated() override;

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void OnReplicationStartedForIris(const FOnReplicationStartedParams& Params) override;
	virtual void OnStopReplicationForIris(const FOnStopReplicationParams& Params) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnEndPlay_Event(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	UFUNCTION()
	void OnDestroyed_Event(AActor* DestroyedActor);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString TestMeString = TEXT("default");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UIGShowmeComponent> NativeShowmeComponent;

private:
	// The name of this object goes away during destruction, so we cache it for our logging purposes.
	FName OriginalName;
	FName NameDuringDestruction;
	FString NetIcoDuringDestruction;
};
