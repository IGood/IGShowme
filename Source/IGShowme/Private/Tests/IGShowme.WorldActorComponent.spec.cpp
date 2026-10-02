// Copyright Ian Good

#include "Actors/IGShowmeActor.h"
#include "Components/IGShowmeComponent.h"
#include "Engine/Engine.h"
#include "IGShowmeInternal.h"
#include "Misc/AutomationTest.h"
#include "Tests/IGShowmeSpecimen.h"
#include "UObject/GCObjectScopeGuard.h"
#include "UObject/Package.h"

#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FIGShowmeWorldActorComponentSpec, "IGShowme.WorldActorComponent", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

struct FScopedTestWorld
{
	explicit FScopedTestWorld(bool bDoInitAndBegin = true)
	{
		const FName UniqueWorldName = MakeUniqueObjectName(GetTransientPackage(), UWorld::StaticClass(), TEXT("ShowmeTestWorld"));
		World = NewObject<UWorld>(GetTransientPackage(), UniqueWorldName);
		World->WorldType = EWorldType::Game;

		FWorldContext& WorldContext = GEngine->CreateNewWorldContext(World->WorldType);
		WorldContext.SetCurrentWorld(World);

		////World->CreatePhysicsScene(nullptr);

		World->InitializeNewWorld(FWorldInitializationValues()
				.InitializeScenes(false)
				.AllowAudioPlayback(false)
				.RequiresHitProxies(false)
				.CreatePhysicsScene(false)
				.CreateNavigation(false)
				.CreateAISystem(false)
				.ShouldSimulatePhysics(false)
				.EnableTraceCollision(false)
				.ForceUseMovementComponentInNonGameWorld(false)
				.SetTransactional(false)
				.CreateFXSystem(false)
				.CreateWorldPartition(false)
				.EnableWorldPartitionStreaming(false)
				.AllowLumenPrimitiveTrackingInPreviewWorld(false));

		if (bDoInitAndBegin)
		{
			World->InitializeActorsForPlay(FURL());
			World->BeginPlay();
		}
	}

	~FScopedTestWorld()
	{
		GEngine->DestroyWorldContext(World);
		World->DestroyWorld(/*bInformEngineOfWorld=*/false);
	}

	UWorld* operator->() const { return World; }

private:
	UWorld* World = nullptr;
};

// Add helper methods here.

END_DEFINE_SPEC(FIGShowmeWorldActorComponentSpec)

void FIGShowmeWorldActorComponentSpec::Define()
{
	Describe("Events", [this] {
		/**
		 * This test demonstrates the order in which World events are invoked.
		 * NOTE: Some of these events don't fire during this test because of the minimal setup performed (no game
		 * instance, no game mode) & because we're not doing some of the operations that the engine is responsible for
		 * (e.g. world tear down).
		 */
		It("World Events", [this] {

		/** Adds a lambda that logs the name of the event. The lambda is removed on scope exit. */
#define IG_ADD_TEMP_LOGGING(EventName)                                                              \
	FDelegateHandle EventName##Handle = FWorldDelegates::EventName.AddLambda([](auto&&... Unused) { \
		UE_LOG(LogIGShowme, Log, TEXT("🧪: 🌎 FWorldDelegates::%hs"), #EventName);                  \
	});                                                                                             \
	ON_SCOPE_EXIT                                                                                   \
	{                                                                                               \
		FWorldDelegates::EventName.Remove(EventName##Handle);                                       \
	}
			// Add callbacks for each of these `FWorldDelegates` events.
			IG_ADD_TEMP_LOGGING(OnPostWorldCreation);
			IG_ADD_TEMP_LOGGING(OnPreWorldInitialization);
			IG_ADD_TEMP_LOGGING(OnPostWorldInitialization);
			IG_ADD_TEMP_LOGGING(OnWorldCleanup);
			IG_ADD_TEMP_LOGGING(OnPostWorldCleanup);
			IG_ADD_TEMP_LOGGING(OnPreWorldFinishDestroy);
			IG_ADD_TEMP_LOGGING(OnWorldInitializedActors);
			IG_ADD_TEMP_LOGGING(OnWorldBeginTearDown);

#undef IG_ADD_TEMP_LOGGING

			FScopedTestWorld World(/*bDoInitAndBegin=*/false);

			/** Adds a lambda that logs the name of the event. */
#define IG_ADD_LOGGING(EventName)                                                 \
	World->EventName.AddLambda([](auto&&... Unused) {                             \
		UE_LOG(LogIGShowme, Log, TEXT("🧪: 🌎 UWorld::%hs_Event()"), #EventName); \
	})
			// Add callbacks for each of these `UWorld` events.
			IG_ADD_LOGGING(OnActorsInitialized);
			IG_ADD_LOGGING(OnWorldBeginPlay);
			IG_ADD_LOGGING(OnWorldMatchStarting);

#undef IG_ADD_LOGGING

			World->InitializeActorsForPlay(FURL());
			World->BeginPlay();
		});

		/**
		 * This test demonstrates the order in which actor spawn events are invoked.
		 */
		It("Spawning Actors", [this] {
			FScopedTestWorld World;

			FDelegateHandle OnActorPreSpawnInitializationHandle = World->AddOnActorPreSpawnInitialization(FOnActorSpawned::FDelegate::CreateLambda(
				[](AActor* Actor) {
					UE_LOG(LogIGShowme, Log, TEXT("🧪: 🌎 UWorld::OnActorPreSpawnInitialization_Event(actor=%s)"), *Actor->GetName());
				}));

			FDelegateHandle OnActorSpawnedHandle = World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateLambda(
				[](AActor* Actor) {
					UE_LOG(LogIGShowme, Log, TEXT("🧪: 🌎 UWorld::OnActorSpawned_Event(actor=%s)"), *Actor->GetName());
				}));

			ON_SCOPE_EXIT
			{
				World->RemoveOnActorPreSpawnInitialization(OnActorPreSpawnInitializationHandle);
				World->RemoveOnActorSpawnedHandler(OnActorSpawnedHandle);
			};

			FActorSpawnParameters SpawnParams;
			SpawnParams.Name = "TestMeActor";
			SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
			SpawnParams.CustomPreSpawnInitialization = [](AActor* Actor) {
				// Log to demonstrate at which point in the actor's lifecycle `CustomPreSpawnInitialization` is called.
				UE_LOG(LogIGShowme, Log, TEXT("🧪: 🌎 FActorSpawnParameters::CustomPreSpawnInitialization_Func(actor=%s)"), *Actor->GetName());
			};

			World->SpawnActor<AIGShowmeActor>(SpawnParams);
		});
	});

	Describe("Components", [this] {
		/**
		 * This test demonstrates the behavior of `DestroyComponent`.
		 */
		It("Are garbage when destroyed", [this] {
			FScopedTestWorld World;

			AIGShowmeActor* const ShowmeActor = World->SpawnActor<AIGShowmeActor>();

			UActorComponent* const ShowmeComponent = NewObject<UIGShowmeComponent>(ShowmeActor);
			ShowmeComponent->RegisterComponent();

			// Sanity check - the component has been added to the actor upon creation.
			TInlineComponentArray<UIGShowmeComponent*> ShowmeComponents(ShowmeActor);
			TestTrue("component lookup", ShowmeComponents.Contains(ShowmeComponent));

			UIGShowmeSpecimen* const Specimen = NewObject<UIGShowmeSpecimen>(GetTransientPackage(), "Rooted_Thing");
			const FGCObjectScopeGuard Guard(Specimen);

			Specimen->TestMeObject = ShowmeComponent;

			ShowmeComponent->DestroyComponent();

			// Pointers are unchanged.
			TestSamePtr<UObject>("component pointers", ShowmeComponent, Specimen->TestMeObject);

			// Destruction removes the component.
			ShowmeComponents = TInlineComponentArray<UIGShowmeComponent*>(ShowmeActor);
			TestFalse("component lookup", ShowmeComponents.Contains(ShowmeComponent));

			// Destruction invalidates the component.
			TestFalse("component is valid", IsValid(ShowmeComponent));

			GEngine->PerformGarbageCollectionAndCleanupActors();

			// After GC, a UProperty that pointed to the component has actually been reset to null!
			TestNull("component", Specimen->TestMeObject);
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
