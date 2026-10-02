// Copyright Ian Good

#pragma once

#include "GameFramework/Actor.h"
#include "IGShowmeSparseDataActor.generated.h"

/**
 * This struct represents the sparse class data for `AIGShowmeSparseDataActor`.
 *
 * @see: https://dev.epicgames.com/documentation/en-us/unreal-engine/sparse-class-data-in-unreal-engine
 */
//~ In the editor, the properties here behave as is if they were properties of `AIGShowmeSparseDataActor`.
//~ Getters for this struct's properties are generated in `AIGShowmeSparseDataActor` by UHT so we end up with functions like this:
//~ const FName& AIGShowmeSparseDataActor::GetMyNameProperty() const;
USTRUCT(BlueprintType)
struct FIGShowmeMySparseClassData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Constants")
	float MyFloatProperty = 0.f;

	//~ `NoGetter` means that UHT will not create a getter function (we may implement it ourselves).
	UPROPERTY(EditDefaultsOnly, Category = "My Constants", meta = (NoGetter))
	int32 MyIntProperty = 0;

	//~ `GetByRef` means that Blueprint graphs access a const-ref instead of a copy & the generated `GetMyNameProperty`
	//~ function returns a const-ref.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Constants", meta = (GetByRef))
	FName MyNameProperty = "default";
};

/**
 * Represents an actor class that demonstrates Sparse Class Data.
 *
 * @see: https://dev.epicgames.com/documentation/en-us/unreal-engine/sparse-class-data-in-unreal-engine
 */
UCLASS(Blueprintable, SparseClassDataTypes = IGShowmeMySparseClassData)
class AIGShowmeSparseDataActor : public AActor
{
	GENERATED_BODY()

	AIGShowmeSparseDataActor();
	~AIGShowmeSparseDataActor();

#if WITH_EDITOR
	//~ Upgrade path: Transfers existing data into `FIGShowmeMySparseClassData`.
	virtual void MoveDataToSparseClassDataStruct() const override;
#endif // WITH_EDITOR

	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;

	virtual void PostInitProperties() override;

public:
	//~ `FIGShowmeMySparseClassData::MyIntProperty` is marked with `NoGetter`, so if we want custom logic, we implement it ourselves like this.
	UFUNCTION(BlueprintPure, Category = "My Constants")
	int32 GetMyIntProperty() const;

private:
	// These deprecated properties simulate properties that existed & were in use, but have been promoted to live in the sparse data.
#if WITH_EDITORONLY_DATA

	UPROPERTY()
	float MyFloatProperty_DEPRECATED;

	UPROPERTY()
	int32 MyIntProperty_DEPRECATED;

	UPROPERTY()
	FName MyNameProperty_DEPRECATED;

#endif // WITH_EDITORONLY_DATA

	// The name of this object goes away during destruction, so we cache it for our logging purposes.
	FName OriginalName;
	FName NameDuringDestruction;
	FString NetIcoDuringDestruction;
};
