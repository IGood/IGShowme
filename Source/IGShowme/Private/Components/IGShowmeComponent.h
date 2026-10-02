// Copyright Ian Good

#pragma once

#include "Components/ActorComponent.h"
#include "IGShowmeComponent.generated.h"

/**
 * Represents an actor component that can be used to demonstrate the lifecycle and behaviors of a component.
 *
 * The methods here just log messages before calling the base implementation so you can observe the order that methods
 * are executed.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UIGShowmeComponent : public UActorComponent
{
	GENERATED_BODY()

	UIGShowmeComponent();
	~UIGShowmeComponent();

	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;

	virtual void PostInitProperties() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	virtual void PostLoad() override;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	virtual void OnRegister() override;
	virtual void OnUnregister() override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void ReadyForReplication() override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void OnReplicationStartedForIris(const FOnReplicationStartedParams& Params) override;
	virtual void OnStopReplicationForIris(const FOnStopReplicationParams& Params) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void DestroyComponent(bool bPromoteChildren) override;
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UFUNCTION()
	void OnComponentActivated_Event(UActorComponent* Component, bool bReset);

	UFUNCTION()
	void OnComponentDeactivated_Event(UActorComponent* Component);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString TestMeString = TEXT("default");

private:
	// The name of this object goes away during destruction, so we cache it for our logging purposes.
	FName OriginalName;
	FName NameDuringDestruction;
	FString NetIcoDuringDestruction;
};
