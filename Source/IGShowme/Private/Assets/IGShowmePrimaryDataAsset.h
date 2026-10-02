// Copyright Ian Good

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IGShowmePrimaryDataAsset.generated.h"

/**
 * Represents a Primary Data Asset for use with tests that demonstrate how asset bundles work.
 */
UCLASS()
class UIGShowmePrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UIGShowmePrimaryDataAsset();
	~UIGShowmePrimaryDataAsset();

	virtual void BeginDestroy() override;
	virtual void FinishDestroy() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Soft Refs")
	TSoftObjectPtr<UIGShowmePrimaryDataAsset> NotBundled;

	UPROPERTY(EditDefaultsOnly, Category = "Soft Refs", meta = (AssetBundles = "ShowmeBundleA"))
	TSoftObjectPtr<UIGShowmePrimaryDataAsset> InBundleA;

	UPROPERTY(EditDefaultsOnly, Category = "Soft Refs", meta = (AssetBundles = "ShowmeBundleB"))
	TArray<TSoftObjectPtr<UIGShowmePrimaryDataAsset>> InBundleB;

	UPROPERTY(EditDefaultsOnly, Category = "Soft Refs", meta = (AssetBundles = "ShowmeBundleA,ShowmeBundleB"))
	TSoftObjectPtr<UIGShowmePrimaryDataAsset> InBundleAAndB;

private:
	// The name of this object goes away during destruction, so we cache it for our logging purposes.
	FName OriginalName;
	FName NameDuringDestruction;
	FString NetIcoDuringDestruction;
};
