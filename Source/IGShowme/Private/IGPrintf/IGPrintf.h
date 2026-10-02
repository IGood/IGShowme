// Copyright Ian Good

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Math/MathFwd.h"
#include "UObject/Class.h"
#include "UObject/UObjectBaseUtility.h"

class AActor;
class UActorComponent;

/**
 * Macro to be used with logging for consistent formatting.
 *
 * Consumers can define their own `IG::Printf::Format` functions locally to support any type they want.
 */
#define IG_FMT(value) (*IG::Printf::Format(value))

/**
 * Macro for formatting UEnum values with options.
 *
 * @see `IG::Printf::EIGEnumFormats`
 * @example
 *	IG_FMT_ENUM(Slot, Qualified)
 *	IG_FMT_ENUM(NetDormancy, Display)
 */
#define IG_FMT_ENUM(value, fmt) (*IG::Printf::FormatEnum<IG::Printf::EIGEnumFormats::fmt>(value))

/**
 * Macro for formatting `FGuid` values with options.
 * `IG_FMT` can also be used with `FGuid`, which uses `DigitsWithHyphensInBraces`.
 *
 * @see `EGuidFormats`
 * @example
 *	IG_FMT_GUID(Id, Digits)
 *	IG_FMT_GUID(Id, Base36Encoded)
 */
#define IG_FMT_GUID(value, fmt) (*(value).ToString(EGuidFormats::fmt))

namespace IG::Printf
{
inline static constexpr const TCHAR* TrueString = TEXT("True");
inline static constexpr const TCHAR* FalseString = TEXT("False");
inline static const FString NoneString = TEXT("none");
inline static const FString NullString = TEXT("null");

/**
 * Returns "True" or "False".
 */
FORCEINLINE const TCHAR* const* Format(bool bValue)
{
	// Returns `TCHAR**` because we need this to work with `IG_FMT`.
	return &(bValue ? IG::Printf::TrueString : IG::Printf::FalseString);
}

/**
 * Returns a string like "{bd048ce3-358b-46c5-8cee-627c719418f8}".
 */
FORCEINLINE FString Format(const FGuid& Guid)
{
	return Guid.ToString(EGuidFormats::DigitsWithHyphensInBraces);
}

/**
 * Returns a string with translation, rotation, and scale values.
 * Something like these
 *  "(T(0)|R(0)|S(1))"
 *  "(X=12.34, Y=56.70, Z=0.00|P=1.00, Y=2.34, R=5.60|S(1))"
 */
FString Format(const FTransform& Transform);

/**
 * Returns the name of this object (with no path information) if non-null; otherwise "None".
 */
FORCEINLINE FString Format(const UObjectBaseUtility* Object)
{
	return GetNameSafe(Object);
}

/**
 * Returns the human-friendly display name of the object if possible; otherwise behaves like `GetNameSafe`.
 * Handles parameters whose type is `UClass*`, `TSubclassOf<T>`, and similar.
 */
FORCEINLINE FString Format(const UField* Field)
{
#if WITH_EDITORONLY_DATA
	return (Field != nullptr) ? Field->GetDisplayNameText().ToString() : IG::Printf::NoneString;
#else
	return (Field != nullptr) ? Field->GetName() : IG::Printf::NoneString;
#endif
}

/**
 * Returns the name of the component and its owner if non-null; otherwise "NullComponent".
 * Something like these
 *  "MyActor0:MyShapeComponent"
 *  "NullActor:StaticMeshComponent0"
 *  "NullComponent"
 */
FString Format(const UActorComponent* Component);

/**
 * Returns the name of this actor (with no path information) if non-null (behaves like `GetActorNameOrLabel`); otherwise "NullActor".
 */
FString Format(const AActor* Actor);

/** SFINAE helper to detect if a type has a `ToString` member function. */
template <typename T>
using TRequiresToString = std::enable_if_t<std::is_member_function_pointer_v<decltype(&T::ToString)>>;

/**
 * Template function for any type that has a `ToString` member function.
 */
template <typename T, typename = TRequiresToString<T>>
decltype(auto) Format(const T& Stringable)
{
	return Stringable.ToString();
}

/**
 * Template function for any type that has a `ToString` member function.
 * Accepts pointers and returns "null" if the pointer is null.
 */
template <typename T, typename = TRequiresToString<T>>
decltype(auto) Format(const T* Stringable)
{
	return (Stringable != nullptr) ? Stringable->ToString() : IG::Printf::NullString;
}

enum class EIGEnumFormats
{
	/**
	 * Value is qualified with the type name.
	 *
	 * For example: "ENetDormancy::DORM_Awake"
	 */
	Qualified,

	/**
	 * Value is not qualified with the type name.
	 *
	 * For example: "DORM_Awake"
	 */
	Unqualified,

	/**
	 * Value is a localized display string
	 *
	 * For example: "Awake"
	 */
	Display,
};

template <EIGEnumFormats EnumFormat, typename EnumType>
FString FormatEnum(const EnumType EnumeratorValue)
{
	if constexpr (EnumFormat == EIGEnumFormats::Qualified)
	{
		return UEnum::GetValueAsString(EnumeratorValue);
	}

	if constexpr (EnumFormat == EIGEnumFormats::Unqualified)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(EnumeratorValue));
	}

	if constexpr (EnumFormat == EIGEnumFormats::Display)
	{
		return UEnum::GetDisplayValueAsText(EnumeratorValue).ToString();
	}
}

inline const TCHAR* const* Format(EWorldType::Type Value)
{
	static const TCHAR* WorldTypeStr;
	WorldTypeStr = LexToString(Value);
	return &WorldTypeStr;
}

FORCEINLINE FString Format(EEndPlayReason::Type EndPlayReason)
{
	return FormatEnum<EIGEnumFormats::Unqualified>(EndPlayReason);
}

} // namespace IG::Printf

namespace UE::Net
{
enum class EStopReplicatingReason : uint32;
ENGINE_API const TCHAR* LexToString(EStopReplicatingReason Reason);
} // namespace UE::Net

namespace IG::Printf
{
inline const TCHAR* const* Format(UE::Net::EStopReplicatingReason Reason)
{
	static const TCHAR* ReasonStr;
	ReasonStr = UE::Net::LexToString(Reason);
	return &ReasonStr;
}

} // namespace IG::Printf
