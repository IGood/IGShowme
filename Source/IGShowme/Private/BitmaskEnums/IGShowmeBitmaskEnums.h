// Copyright Ian Good

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/EnumClassFlags.h"
#include "Misc/EnumRange.h"
#include "IGShowmeBitmaskEnums.generated.h"

/**
 * Represents a bitflags enum type that can be used in Blueprints as a strong type.
 *
 * This type is allowed to use `BlueprintType` because it's declared as `enum` and not `enum class`.
 * If it declared as `enum class`, then UHT produces an error.
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EIGShowmeCardinalFlags : int32
{
	None = 0,
	North = 1 << 0,
	East = 1 << 1,
	South = 1 << 2,
	West = 1 << 3,

	NE = North | East,
};

ENUM_CLASS_FLAGS(EIGShowmeCardinalFlags);

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EIGShowmeCardinalFlagsBad : int32
{
	None = 0 UMETA(Hidden),
	North = 1 << 0,
	East = 1 << 1,
	South = 1 << 2,
	West = 1 << 3,

	NE = North | East,
};

ENUM_CLASS_FLAGS(EIGShowmeCardinalFlagsBad);

UENUM(meta = (Bitflags))
enum class EIGShowmeCardinalFlagsDumb : uint8
{
	North,
	East,
	South,
	West,
};

USTRUCT(BlueprintType)
struct FIGShowmeCardinal
{
	GENERATED_BODY()

	//~ Prefer `CardinalFlags` instead of `CardinalFlags_Internal` in C++ code.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlags"))
	int32 CardinalFlags_Internal = 0;
	EIGShowmeCardinalFlags& CardinalFlags() { return reinterpret_cast<EIGShowmeCardinalFlags&>(CardinalFlags_Internal); }
	EIGShowmeCardinalFlags CardinalFlags() const { return static_cast<EIGShowmeCardinalFlags>(CardinalFlags_Internal); }

	//~ can edit single values in details panel; no pin on node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EIGShowmeCardinalFlagsBad CardinalFlagsBadA = {};

	//~ can edit single values in details panel; no pin on node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlagsBad"))
	EIGShowmeCardinalFlagsBad CardinalFlagsBadB = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlagsDumb"))
	int32 CardinalFlagsDumb = 0;
};

UCLASS()
class UIGShowmeBitmaskEnumsBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Autocast functions must be public (see `FAutocastFunctionMap::IsAutocastFunction`).
public:
	UFUNCTION(BlueprintPure, Category = "IGShowme|Cardinal")
	static EIGShowmeCardinalFlags GetCardinalFlags(const FIGShowmeCardinal& Cardinal)
	{
		return static_cast<EIGShowmeCardinalFlags>(Cardinal.CardinalFlags_Internal);
	}

	UFUNCTION(BlueprintCallable, Category = "IGShowme|Cardinal")
	static EIGShowmeCardinalFlags TestCardinalFlagsParameters(

		// masking works
		UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlags"))
			int32 CardinalFlagsBMI,

		// masking is ignored - singles only
		EIGShowmeCardinalFlags CardinalFlagsA,

		// masking is ignored - singles only
		UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlags"))
			EIGShowmeCardinalFlags CardinalFlagsB,

		// masking works, but no names
		UPARAM(meta = (Bitmask))
			EIGShowmeCardinalFlags CardinalFlagsC,

		// masking is ignored - singles only
		UPARAM(meta = (BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlags"))
			EIGShowmeCardinalFlags CardinalFlagsD,

		// unsupported in BP (enum bigger than byte)
		EIGShowmeCardinalFlagsBad CardinalFlagsBadA,

		// unsupported in BP (enum bigger than byte)
		UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlagsBad"))
			EIGShowmeCardinalFlagsBad CardinalFlagsBadB,

		UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlagsDumb"))
			int32 CardinalFlagsDumb)
	{
		return static_cast<EIGShowmeCardinalFlags>(CardinalFlagsBMI);
	}

	UFUNCTION(BlueprintPure, Category = "Utilities", meta = (Keywords = "construct build", NativeMakeFunc, BlueprintThreadSafe))
	static EIGShowmeCardinalFlags MakeCardinalFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlags")) int32 CardinalFlags)
	{
		return static_cast<EIGShowmeCardinalFlags>(CardinalFlags);
	}

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Cardinal Flags", CompactNodeTitle = "->", BlueprintThreadSafe, BlueprintAutocast), Category = "Utilities")
	static EIGShowmeCardinalFlags Conv_IntToCardinalFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/IGShowme.EIGShowmeCardinalFlags")) int32 CardinalFlags)
	{
		return static_cast<EIGShowmeCardinalFlags>(CardinalFlags);
	}

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Integer", CompactNodeTitle = "->", BlueprintThreadSafe, BlueprintAutocast), Category = "Utilities")
	static int32 Conv_CardinalFlagsToInt(EIGShowmeCardinalFlags CardinalFlags)
	{
		return static_cast<int32>(CardinalFlags);
	}
};
