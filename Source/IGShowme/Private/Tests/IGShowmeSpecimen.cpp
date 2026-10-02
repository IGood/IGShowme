// Copyright Ian Good

#include "Tests/IGShowmeSpecimen.h"
#include "IGShowmeInternal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IGShowmeSpecimen)

#define IGSHOWME_SUBJECTICO TEXT("🦠")

UIGShowmeSpecimen::UIGShowmeSpecimen()
{
	IGSHOWME_LOG_CONSTRUCTOR;

	OriginalName = GetFName();
}

UIGShowmeSpecimen::~UIGShowmeSpecimen()
{
	IGSHOWME_LOG_DESTRUCTOR;
}

void UIGShowmeSpecimen::BeginDestroy()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	// do custom stuff before calling Super

	NameDuringDestruction = GetFName();
	NetIcoDuringDestruction = IG_NET_ICO(this);

	Super::BeginDestroy();
}

void UIGShowmeSpecimen::FinishDestroy()
{
	IGSHOWME_LOG_FINISHDESTROY;

	// do custom stuff before calling Super

	Super::FinishDestroy();
}

#undef IGSHOWME_SUBJECTICO
