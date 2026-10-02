// Copyright Ian Good

#include "Assets/IGShowmePrimaryDataAsset.h"
#include "Engine/AssetManager.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGShowmeAssetBundlesSpec, "IGShowme.AssetBundles", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

static FPrimaryAssetType ShowmePrimaryDataAssetType()
{
	return UIGShowmePrimaryDataAsset::StaticClass()->GetFName();
};

static FPrimaryAssetId GetRootAssetId()
{
	// This is one of the assets that comes with this plugin.
	return FPrimaryAssetId(ShowmePrimaryDataAssetType(), "DA_ShowmeRoot");
}

const UIGShowmePrimaryDataAsset* Root = nullptr;

/**
 * Make sure that the Asset Manager knows about our assets.
 */
static void AddPrimaryAssetType()
{
	UAssetManager& AssetManager = UAssetManager::Get();

	FPrimaryAssetTypeInfo TypeInfo;

	// If the type info already exists, then we're good to go.
	if (AssetManager.GetPrimaryAssetTypeInfo(ShowmePrimaryDataAssetType(), TypeInfo))
	{
		return;
	}

	// If the Asset Manager doesn't know about our type, then we need to add it by doing the things that normally
	// happen when the Asset Manager starts up & gets settings out of config files.
	TypeInfo = FPrimaryAssetTypeInfo(
		/*InPrimaryAssetType=*/ShowmePrimaryDataAssetType().GetName(),
		/*InAssetBaseClass=*/UIGShowmePrimaryDataAsset::StaticClass(),
		/*bInHasBlueprintClasses=*/false,
		/*bInIsEditorOnly=*/false,
		/*InDirectories=*/{{TEXT("/IGShowme")}},
		/*InSpecificAssets=*/{});

	bool bIsValid, bBaseClassWasLoaded;
	TypeInfo.FillRuntimeData(bIsValid, bBaseClassWasLoaded);
	AssetManager.ScanPathsForPrimaryAssets(TypeInfo.PrimaryAssetType, TypeInfo.AssetScanPaths, TypeInfo.AssetBaseClassLoaded, TypeInfo.bHasBlueprintClasses, TypeInfo.bIsEditorOnly);
	AssetManager.SetPrimaryAssetTypeRules(TypeInfo.PrimaryAssetType, TypeInfo.Rules);
}

/**
 * Load our "root" test asset via `LoadPrimaryAsset` with the specified bundles.
 */
bool LoadRoot(const TArray<FName>& LoadBundles)
{
	TSharedPtr<FStreamableHandle> Handle = UAssetManager::Get().LoadPrimaryAsset(GetRootAssetId(), LoadBundles);
	UTEST_VALID_EXPR(Handle);

	Handle->WaitUntilComplete();

	Root = CastChecked<UIGShowmePrimaryDataAsset>(Handle->GetLoadedAsset());

	return Root != nullptr;
}

END_DEFINE_SPEC(FIGShowmeAssetBundlesSpec)

void FIGShowmeAssetBundlesSpec::Define()
{
	BeforeEach([this] {
		AddPrimaryAssetType();

		// Do GC because we require that our assets aren't already loaded.
		CollectGarbage(RF_NoFlags);

		Root = UAssetManager::Get().GetPrimaryAssetObject<UIGShowmePrimaryDataAsset>(GetRootAssetId());
		TestFalse("Root is valid", IsValid(Root));

		Root = nullptr;
	});

	AfterEach([this] {
		UAssetManager& AssetManager = UAssetManager::Get();

		const int32 NumUnloaded = AssetManager.UnloadPrimaryAssetsWithType(ShowmePrimaryDataAssetType());
		TestTrue("Assets were unloaded", NumUnloaded > 0);

		// After "unloading", there shouldn't be any references to our assets, but we need to GC to actually clean up.
		CollectGarbage(RF_NoFlags);

		Root = AssetManager.GetPrimaryAssetObject<UIGShowmePrimaryDataAsset>(GetRootAssetId());
		TestFalse("Root is valid", IsValid(Root));

		Root = nullptr;
	});

	It("No Bundles", [this] {
		UTEST_TRUE_EXPR(LoadRoot({}));

		// Expect that all our soft references are invalid (the objects have not been loaded).
		TestInvalid("NotBundled", Root->NotBundled);
		TestInvalid("InBundleA", Root->InBundleA);
		for (int32 i = 0; const TSoftObjectPtr<UIGShowmePrimaryDataAsset>& ShowmePDA : Root->InBundleB)
		{
			TestInvalid(FString::Printf(TEXT("InBundleB[%d]"), i), ShowmePDA);
		}
		TestInvalid("InBundleAAndB", Root->InBundleAAndB);

		return true;
	});

	It("Bundle A", [this] {
		UTEST_TRUE_EXPR(LoadRoot({"ShowmeBundleA"}));

		// Expect only the things in `ShowmeBundleA` have been loaded (but not recursively).
		TestInvalid("NotBundled", Root->NotBundled);
		if (TestValid("InBundleA", Root->InBundleA))
		{
			// `LoadPrimaryAsset` does not recursively load dependencies.
			TestInvalid("recursive InBundleA", Root->InBundleA->InBundleA);
		}
		for (int32 i = 0; const TSoftObjectPtr<UIGShowmePrimaryDataAsset>& ShowmePDA : Root->InBundleB)
		{
			TestInvalid(FString::Printf(TEXT("InBundleB[%d]"), i), ShowmePDA);
		}
		TestValid("InBundleAAndB", Root->InBundleAAndB);

		// Now demonstrate that `PreloadPrimaryAssets` recursively loads dependencies.
		TSharedPtr<FStreamableHandle> Handle = UAssetManager::Get().PreloadPrimaryAssets({GetRootAssetId()}, {"ShowmeBundleA"}, /*bLoadRecursive=*/true);
		UTEST_VALID_EXPR(Handle);

		Handle->WaitUntilComplete();

		if (TestValid("InBundleA", Root->InBundleA))
		{
			TestValid("recursive InBundleA", Root->InBundleA->InBundleA);
		}

		return true;
	});

	It("Bundle B", [this] {
		UTEST_TRUE_EXPR(LoadRoot({"ShowmeBundleB"}));

		// Expect only the things in `ShowmeBundleB` have been loaded.
		TestInvalid("NotBundled", Root->NotBundled);
		TestInvalid("InBundleA", Root->InBundleA);
		for (int32 i = 0; const TSoftObjectPtr<UIGShowmePrimaryDataAsset>& ShowmePDA : Root->InBundleB)
		{
			TestValid(FString::Printf(TEXT("InBundleB[%d]"), i), ShowmePDA);
		}
		TestValid("InBundleAAndB", Root->InBundleAAndB);

		return true;
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
