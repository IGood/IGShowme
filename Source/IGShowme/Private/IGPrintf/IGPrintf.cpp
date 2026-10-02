// Copyright Ian Good

#include "IGPrintf/IGPrintf.h"
#include "Components/ActorComponent.h"
////#include "Containers/StringFwd.h"
#include "GameFramework/Actor.h"
#include "Misc/StringBuilder.h"

namespace IG::Printf
{
FString Format(const FTransform& Transform)
{
	TStringBuilder<100> StringBuilder;

	const FVector T = Transform.GetTranslation();
	const FQuat Q = Transform.GetRotation();
	const FVector S = Transform.GetScale3D();

	StringBuilder.AppendChar(TEXT('('));

	if (T.IsNearlyZero())
	{
		StringBuilder.Append(TEXT("T(0)"));
	}
	else
	{
		StringBuilder.Appendf(TEXT("X=%.2f, Y=%.2f, Z=%.2f"), T.X, T.Y, T.Z);
	}

	StringBuilder.AppendChar(TEXT('|'));

	if (Q.IsIdentity())
	{
		StringBuilder.Append(TEXT("R(0)"));
	}
	else
	{
		const FRotator R = Q.Rotator();
		StringBuilder.Appendf(TEXT("P=%.2f, Y=%.2f, R=%.2f"), R.Pitch, R.Yaw, R.Roll);
	}

	StringBuilder.AppendChar(TEXT('|'));

	if ((S - FVector::OneVector).IsNearlyZero())
	{
		StringBuilder.Append(TEXT("S(1)"));
	}
	else
	{
		StringBuilder.Appendf(TEXT("X=%.2f, Y=%.2f, Z=%.2f"), S.X, S.Y, S.Z);
	}

	StringBuilder.AppendChar(TEXT(')'));

	return StringBuilder.ToString();
}

FString Format(const UActorComponent* Component)
{
	static const FString NullComponentString = TEXT("NullComponent");
	return (Component != nullptr)
			 ? Format(Component->GetOwner()) + TEXT(':') + Component->GetName()
			 : NullComponentString;
}

FString Format(const AActor* Actor)
{
	static const FString NullActorString = TEXT("NullActor");
	return (Actor != nullptr) ? Actor->GetActorNameOrLabel() : NullActorString;
}

} // namespace IG::Printf
