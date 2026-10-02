// Copyright Ian Good

#include "Assets/IGShowmePrimaryDataAsset.h"
#include "IGShowmeInternal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IGShowmePrimaryDataAsset)

#define IGSHOWME_SUBJECTICO TEXT("🗒️")

UIGShowmePrimaryDataAsset::UIGShowmePrimaryDataAsset()
{
	IGSHOWME_LOG_CONSTRUCTOR;

	OriginalName = GetFName();
}

UIGShowmePrimaryDataAsset::~UIGShowmePrimaryDataAsset()
{
	IGSHOWME_LOG_DESTRUCTOR;
}

void UIGShowmePrimaryDataAsset::BeginDestroy()
{
	IGSHOWME_LOG_FUNCTION_CALL;

	// do custom stuff before calling Super

	NameDuringDestruction = GetFName();
	NetIcoDuringDestruction = IG_NET_ICO(this);

	Super::BeginDestroy();
}

void UIGShowmePrimaryDataAsset::FinishDestroy()
{
	IGSHOWME_LOG_FINISHDESTROY;

	// do custom stuff before calling Super

	Super::FinishDestroy();
}

#undef IGSHOWME_SUBJECTICO
