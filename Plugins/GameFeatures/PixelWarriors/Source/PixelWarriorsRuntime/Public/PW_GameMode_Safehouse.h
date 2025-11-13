// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "PW_GameMode_Safehouse.generated.h"

class AMoneyPickup;

// Define the delegate type
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMatchStateChangedDelegate, FName, NewState, FName, OldState);

UCLASS()
class PIXELWARRIORSRUNTIME_API APW_GameMode_Safehouse : public AModularGameMode
{
    GENERATED_BODY()

public:
    APW_GameMode_Safehouse();

    // Correct signature to match parent (no param)
    virtual void OnMatchStateSet() override;

protected:
    virtual void BeginPlay() override;

    // Called when GameState changes match state
    UFUNCTION()
    void OnMatchStateChanged(FName NewState, FName OldState);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Safehouse Rules")
    TSubclassOf<AMoneyPickup> MoneyPickupClass;

    UPROPERTY(EditDefaultsOnly, Category = "Safehouse Rules")
    float MoneyBagSpawnRate = 5.0f;

    UPROPERTY()
    TArray<AActor*> MoneySpawnPoints;

    FTimerHandle MoneySpawnTimerHandle;

    void SpawnMoneyBag();
};