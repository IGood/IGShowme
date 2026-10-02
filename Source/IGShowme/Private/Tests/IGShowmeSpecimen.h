// Copyright Ian Good

#pragma once

#include "Misc/EnumRange.h"
#include "UObject/Object.h"
#include "IGShowmeSpecimen.generated.h"

/**
 * Represents a UEnum that is used in unit tests to demonstrate behaviors of certain Unreal Engine features.
 */
UENUM()
enum class EIGShowmeVeggies
{
	Carrot,
	Broccoli,
	Zucchini,
	Spinach,
	Potato,
	Tomato,
	Onion,
	Lettuce,
	Cucumber,
	Peas,
	Corn,
	Eggplant,

	Count
};

ENUM_RANGE_BY_COUNT(EIGShowmeVeggies, EIGShowmeVeggies::Count);

inline constexpr size_t AsIndex(EIGShowmeVeggies Value)
{
	return static_cast<size_t>(Value);
}

/**
 * Represents an object that is used in unit tests to demonstrate behaviors of certain Unreal Engine features.
 */
UCLASS()
class UIGShowmeSpecimen : public UObject
{
	GENERATED_BODY()

	UIGShowmeSpecimen();
	~UIGShowmeSpecimen();

	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;

public:
	inline static const FString MyDefaultStringValue = TEXT("hi im new");

	UPROPERTY()
	FString TestMeString = MyDefaultStringValue;

	UPROPERTY()
	TObjectPtr<UObject> TestMeObject;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> TestMeObjectArray;

private:
	// The name of this object goes away during destruction, so we cache it for our logging purposes.
	FName OriginalName;
	FName NameDuringDestruction;
	FString NetIcoDuringDestruction;
};
