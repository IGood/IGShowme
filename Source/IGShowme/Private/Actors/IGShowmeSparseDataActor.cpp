// Copyright Ian Good

#include "Actors/IGShowmeSparseDataActor.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "IGShowmeInternal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IGShowmeSparseDataActor)

#define IGSHOWME_SUBJECTICO TEXT("🎭")

AIGShowmeSparseDataActor::AIGShowmeSparseDataActor()
{
	IGSHOWME_LOG_CONSTRUCTOR;

	OriginalName = GetFName();

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *GetMyNameProperty().ToString());
}

AIGShowmeSparseDataActor::~AIGShowmeSparseDataActor()
{
	IGSHOWME_LOG_DESTRUCTOR;
}

#if WITH_EDITOR

void AIGShowmeSparseDataActor::MoveDataToSparseClassDataStruct() const
{
	IGSHOWME_LOG_FUNCTION_CALL;

	// Don't overwrite the sparse data if it has already been saved.
	UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass == nullptr || BPClass->bIsSparseClassDataSerializable)
	{
		return;
	}

	Super::MoveDataToSparseClassDataStruct();

#if WITH_EDITORONLY_DATA

	// Unreal Header Tool (UHT) generates `GetMutableIGShowmeMySparseClassData`.
	FIGShowmeMySparseClassData* SparseClassData = GetMutableIGShowmeMySparseClassData();

	// Migrate our old properties to the sparse data.
	SparseClassData->MyFloatProperty = MyFloatProperty_DEPRECATED;
	SparseClassData->MyIntProperty = MyIntProperty_DEPRECATED;
	SparseClassData->MyNameProperty = MyNameProperty_DEPRECATED;

#endif // WITH_EDITORONLY_DATA
}

#endif // WITH_EDITOR

void AIGShowmeSparseDataActor::BeginDestroy()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	// do custom stuff before calling Super

	NameDuringDestruction = GetFName();
	NetIcoDuringDestruction = IG_NET_ICO(this);

	Super::BeginDestroy();
}

void AIGShowmeSparseDataActor::FinishDestroy()
{
	IGSHOWME_LOG_FINISHDESTROY;

	// do custom stuff before calling Super

	Super::FinishDestroy();
}

void AIGShowmeSparseDataActor::PostInitProperties()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	Super::PostInitProperties();

	// Log a property to demonstrate at which point values are changed from their defaults to what's set in the editor.
	IGSHOWME_LOG_MEMBER("%s", *GetMyNameProperty().ToString());
}

int32 AIGShowmeSparseDataActor::GetMyIntProperty() const
{
	IGSHOWME_LOG_FUNCTION_CALL;

	// This is how UHT implements these getters (see `IGShowmeSparseDataActor.generated.h`).
	int32 MyIntProperty = GetIGShowmeMySparseClassData(EGetSparseClassDataMethod::ArchetypeIfNull)->MyIntProperty;
	// From here, we could implement custom logic before returning the final value.

	return MyIntProperty;
}

#undef IGSHOWME_SUBJECTICO
