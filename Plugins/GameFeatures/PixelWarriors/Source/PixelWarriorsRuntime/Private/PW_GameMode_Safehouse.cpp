// Copyright Pixel Warriors Inc. 2025
#include "PW_GameMode_Safehouse.h"
#include "PW_GameState_Safehouse.h"  // Critical Include
#include "MoneyPickup.h"
#include "Kismet/GameplayStatics.h"






APW_GameMode_Safehouse::APW_GameMode_Safehouse()
{
    GameStateClass = APW_GameState_Safehouse::StaticClass();
}

void APW_GameMode_Safehouse::OnMatchStateSet()
{
    Super::OnMatchStateSet();  // No argument!

    // You can react here if needed (use MatchState variable)
    // e.g., if (MatchState == MatchState::InProgress) { ... }
}

void APW_GameMode_Safehouse::BeginPlay()
{
    Super::BeginPlay();

    if (APW_GameState_Safehouse* GS = GetGameState<APW_GameState_Safehouse>())
    {
        // Assume GS has FOnMatchStateChangedDelegate OnMatchStateChanged;
        GS->OnMatchStateChanged.AddUObject(this, &APW_GameMode_Safehouse::OnMatchStateChanged);
    }
}

void APW_GameMode_Safehouse::OnMatchStateChanged(FName NewState, FName OldState)
{
    if (NewState == MatchState::InProgress)
    {
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MoneySpawn"), MoneySpawnPoints);

        if (MoneyPickupClass && MoneySpawnPoints.Num() > 0)
        {
            GetWorld()->GetTimerManager().SetTimer(MoneySpawnTimerHandle, this, &APW_GameMode_Safehouse::SpawnMoneyBag, MoneyBagSpawnRate, true, 2.0f);
        }
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(MoneySpawnTimerHandle);
    }
}

void APW_GameMode_Safehouse::SpawnMoneyBag()
{
    if (!HasAuthority() || !MoneyPickupClass || MoneySpawnPoints.Num() == 0) return;

    const int32 RandomIndex = FMath::RandRange(0, MoneySpawnPoints.Num() - 1);
    AActor* SpawnPoint = MoneySpawnPoints[RandomIndex];

    if (!SpawnPoint) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    GetWorld()->SpawnActor<AMoneyPickup>(MoneyPickupClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation(), SpawnParams);
}