// Copyright Pixel Warriors Inc. 2025
#include "PW_GameMode_Safehouse.h"    // This class's own header
#include "PW_GameState_Safehouse.h"  // Our custom GameState, where the delegate lives
#include "MoneyPickup.h"             // The actor class we intend to spawn
#include "Kismet/GameplayStatics.h"  // For engine utilities like GetAllActorsWithTag
#include "EngineUtils.h"             // For additional engine utilities
#include "TimerManager.h"            // For FTimerHandle and GetWorld()->GetTimerManager()

// In the constructor, we establish the critical link to our custom GameState class.
APW_GameMode_Safehouse::APW_GameMode_Safehouse()
{
	// Tell the engine that this GameMode MUST use our APW_GameState_Safehouse.
	GameStateClass = APW_GameState_Safehouse::StaticClass(); 
}

// BeginPlay is where our server-only GameMode subscribes to events.
void APW_GameMode_Safehouse::BeginPlay()
{
	// Always call the parent's function first to ensure core engine systems initialize.
	Super::BeginPlay();

	// Get our specific GameState and bind our listener function to its delegate.
	// This is the clean, correct, event-driven method.
	if (APW_GameState_Safehouse* GS = GetGameState<APW_GameState_Safehouse>())
	{
		// Subscribe our local function to the GameState's "Town Crier" broadcast.
		GS->OnMatchStateChanged.AddDynamic(this, &APW_GameMode_Safehouse::OnSafehouseMatchStateChanged);
	}
}

// This function is our "ear." It now gets called automatically when the GameState's match state changes.
void APW_GameMode_Safehouse::OnSafehouseMatchStateChanged(FName NewState)
{
	// When the match officially begins...
	if (NewState == MatchState::InProgress)
	{
		// ...we, the server, find all the potential money spawn points in the level.
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MoneySpawn"), MoneySpawnPoints);
        
		if (MoneyPickupClass && MoneySpawnPoints.Num() > 0)
		{
			// Start our looping "factory" timer to spawn money bags periodically.
			GetWorld()->GetTimerManager().SetTimer(MoneySpawnTimerHandle, this, &APW_GameMode_Safehouse::SpawnMoneyBag, MoneyBagSpawnRate, true, 2.0f);
		}
	}
	else
	{
		// If the match ends, we must clean up and stop our factory timer.
		GetWorld()->GetTimerManager().ClearTimer(MoneySpawnTimerHandle);
	}
}

// This is the server-only function that performs the spawn action for a single money bag.
void APW_GameMode_Safehouse::SpawnMoneyBag()
{
	// This is our triple safety check. Must be server, have a class to spawn, and have a place to spawn it.
	if (!HasAuthority() || !MoneyPickupClass || MoneySpawnPoints.Num() == 0)
	{
		return;
	}

	// Pick a random location from our pre-collected list of spawn points.
	const int32 RandomIndex = FMath::RandRange(0, MoneySpawnPoints.Num() - 1);
	AActor* SpawnPoint = MoneySpawnPoints[RandomIndex];
    
	if (!SpawnPoint)
	{
		return; // Another safety check in case the actor in the array was destroyed.
	}

	// Prepare spawn parameters. This method ensures the actor will spawn even if the spot is slightly occupied.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
	// Execute the spawn command. This creates a new AMoneyPickup in the world.
	GetWorld()->SpawnActor<AMoneyPickup>(MoneyPickupClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(), SpawnParams);
}