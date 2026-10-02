// Copyright Ian Good

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "IGShowmeWorldSubsystem.generated.h"

/**
 * Represents subsystem that can be used to demonstrate the lifecycle and behaviors of a world subsystem.
 *
 * The methods here just log messages before calling the base implementation so you can observe the order that methods
 * are executed.
 */
UCLASS(Blueprintable)
class UIGShowmeWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UIGShowmeWorldSubsystem();
	~UIGShowmeWorldSubsystem();

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void PostInitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void OnWorldComponentsUpdated(UWorld& World) override;

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	// The name of this object goes away during destruction, so we cache it for our logging purposes.
	FName OriginalName;
	FName NameDuringDestruction;
	FString NetIcoDuringDestruction;
};
