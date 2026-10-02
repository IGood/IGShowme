// Copyright Ian Good

#include "AssetRegistry/AssetData.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "IGShowmeInternal.h"
#include "Misc/AutomationTest.h"
#include "Tests/IGShowmeSpecimen.h"
#include "UObject/GCObjectScopeGuard.h"

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGShowmeObjectsSpec, "IGShowme.Objects", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

void PrintNamesAndSuch(const UObject* Obj)
{
	if (Obj == nullptr)
	{
		AddError(TEXT("Missing test subject."), 1);
		return;
	}

	UE_LOG(LogIGShowme, Log, TEXT("🧪: -------------------"));
	UE_LOG(LogIGShowme, Log, TEXT("🧪: %s..."), *TestParameterContext);

#define IGSHOWME_LOG_STRING(StringExpr) UE_LOG(LogIGShowme, Log, TEXT("🧪:  %-31hs = %s"), #StringExpr, *(StringExpr))
#define IGSHOWME_LOG_NAME(NameExpr) UE_LOG(LogIGShowme, Log, TEXT("🧪:  %-31hs = %s"), #NameExpr, *(NameExpr).ToString())

	// The comments below are the output for a transient `UIGShowmeSpecimen` instance named "testme" (2025-02-21, UE5.4)...
	{
		IGSHOWME_LOG_STRING(Obj->GetName());     // testme
		IGSHOWME_LOG_STRING(Obj->GetPathName()); // /Engine/Transient.testme
		IGSHOWME_LOG_STRING(Obj->GetFullName()); // IGShowmeSpecimen /Engine/Transient.testme
	}
	{
		UE_LOG(LogIGShowme, Log, TEXT("🧪: as a FAssetData..."))
		const FAssetData AssetData(Obj);
		IGSHOWME_LOG_NAME(AssetData.PackageName);             // /Engine/Transient
		IGSHOWME_LOG_NAME(AssetData.PackagePath);             // /Engine
		IGSHOWME_LOG_NAME(AssetData.AssetName);               // testme
		IGSHOWME_LOG_STRING(AssetData.GetObjectPathString()); // /Engine/Transient.testme
		IGSHOWME_LOG_STRING(AssetData.GetFullName());         // /Script/IGShowme.IGShowmeSpecimen /Engine/Transient.testme
		IGSHOWME_LOG_STRING(AssetData.GetExportTextName());   // /Script/IGShowme.IGShowmeSpecimen'/Engine/Transient.testme'
	}
	{
		UE_LOG(LogIGShowme, Log, TEXT("🧪: as a FSoftObjectPath..."))
		const FSoftObjectPath SoftPath(Obj);
		IGSHOWME_LOG_STRING(SoftPath.ToString());           // /Engine/Transient.testme
		IGSHOWME_LOG_STRING(SoftPath.GetAssetPathString()); // /Engine/Transient.testme
		IGSHOWME_LOG_STRING(SoftPath.GetSubPathString());   //
		IGSHOWME_LOG_STRING(SoftPath.GetLongPackageName()); // /Engine/Transient
		IGSHOWME_LOG_STRING(SoftPath.GetAssetName());       // testme
	}

#undef IGSHOWME_LOG_STRING
#undef IGSHOWME_LOG_NAME

	UE_LOG(LogIGShowme, Log, TEXT("🧪: -------------------"));
}

END_DEFINE_SPEC(FIGShowmeObjectsSpec)

void FIGShowmeObjectsSpec::Define()
{
	/**
	 * These tests demonstrate the return values of various name and path methods of an object.
	 */
	Describe("Object names and such", [this] {
		It("Static Mesh Asset", [this] {
			const UObject* Obj = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
			PrintNamesAndSuch(Obj);
		});

		It("BP Actor Class", [this] {
			const UClass* Class = LoadClass<AActor>(nullptr, TEXT("/IGShowme/BP_ShowmeActor.BP_ShowmeActor_C"));
			PrintNamesAndSuch(Class);
		});

		It("BP Actor Class CDO", [this] {
			const UClass* Class = LoadClass<AActor>(nullptr, TEXT("/IGShowme/BP_ShowmeActor.BP_ShowmeActor_C"));
			PrintNamesAndSuch(Class != nullptr ? Class->GetDefaultObject() : nullptr);
		});

		It("Transient Object", [this] {
			PrintNamesAndSuch(NewObject<UIGShowmeSpecimen>(GetTransientPackage(), "testme"));
		});

		It("Static Class", [this] {
			PrintNamesAndSuch(UIGShowmeSpecimen::StaticClass());
		});

		It("Static Class CDO", [this] {
			PrintNamesAndSuch(GetDefault<UIGShowmeSpecimen>());
		});
	});

	Describe("Garbage Collection", [this] {
		/**
		 * This test demonstrates what happens when an object is explicitly "marked as garbage".
		 * - it is no longer "valid"
		 * - after GC happens, any UProperties that point to that object have been nulled-out
		 */
		It("Invalidates UProperties", [this] {
			UIGShowmeSpecimen* Specimen = NewObject<UIGShowmeSpecimen>(GetTransientPackage(), "Rooted_Thing");
			const FGCObjectScopeGuard Guard(Specimen);

			Specimen->TestMeObject = NewObject<UIGShowmeSpecimen>(GetTransientPackage(), "Tempy_McTempface");
			Specimen->TestMeObjectArray.Emplace(Specimen->TestMeObject);

			// A new UObject is always valid. We know this.
			TestTrue("UObject is valid", IsValid(Specimen->TestMeObject));

			Specimen->TestMeObject->MarkAsGarbage();

			// Marking as garbage does *not* affect UProperty state - the pointer is still valid.
			UTEST_NOT_NULL("UObject", Specimen->TestMeObject.Get());

			// Marking as garbage *does* invalidate the UObject itself.
			TestFalse("UObject is valid", IsValid(Specimen->TestMeObject));

			GEngine->PerformGarbageCollectionAndCleanupActors();

			// After GC, a UProperty that pointed to the UObject has actually been reset to null!
			TestNull("UObject", Specimen->TestMeObject);

			// The element hasn't been removed from the array, but it is now null.
			TestNull("UObject", Specimen->TestMeObjectArray[0]);

			return true;
		});
	});

	Describe("New Objects", [this] {
		/**
		 * This test demonstrates that creating a "new object" with the same outer, type, and name will just reset the
		 * existing object.
		 * This is actually nice because it allows the engine to do special things like overwrite assets for reimports
		 * and whatnot.
		 */
		It("Aren't always new", [this] {
			// Create a new object & change its default state.
			UIGShowmeSpecimen* SpecimenA = NewObject<UIGShowmeSpecimen>(GetTransientPackage(), "testme");
			TestEqual("TestMeString property", SpecimenA->TestMeString, UIGShowmeSpecimen::MyDefaultStringValue);
			SpecimenA->TestMeString = TEXT("ohhi");
			TestEqual("TestMeString property", SpecimenA->TestMeString, TEXT("ohhi"));

			// Create a new object in the same package & observe that it's actually the same thing in memory, but its
			// state has been reset.
			UIGShowmeSpecimen* SpecimenB = NewObject<UIGShowmeSpecimen>(GetTransientPackage(), "testme");
			TestSamePtr("Object pointers", SpecimenA, SpecimenB);
			TestEqual("TestMeString property", SpecimenB->TestMeString, UIGShowmeSpecimen::MyDefaultStringValue);
		});
	});

	Describe("TObjectPtr", [this] {
		/**
		 * This test demonstrates the functionality of `TObjectPtr` and how it behaves in null & non-null states.
		 */
		It("Instance methods", [this] {
			const FName ExpectedName = "testme";

			TObjectPtr<UIGShowmeSpecimen> Specimen;

			TestNull("pointer", Specimen.Get());
			TestEqual("object name", Specimen.GetFName(), FName());
			TestNull("object class", Specimen.GetClass());
			TestFalse("run time type check", Specimen.IsA<UIGShowmeSpecimen>());

			Specimen = NewObject<UIGShowmeSpecimen>(GetTransientPackage(), ExpectedName);

			TestNotNull("pointer", Specimen.Get());
			TestEqual("object name", Specimen.GetFName(), ExpectedName);
			TestSamePtr("object class", Specimen.GetClass(), UIGShowmeSpecimen::StaticClass());
			TestTrue("run time type check", Specimen.IsA<UIGShowmeSpecimen>());
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
