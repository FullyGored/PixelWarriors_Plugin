// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "GameModes/LyraGameMode.h" // The correct parent
#include "UObject/ObjectPtr.h"
#include "PW_GameMode_Safehouse.generated.h"

class AMoneyPickup;

UCLASS()
class PIXELWARRIORSRUNTIME_API APW_GameMode_Safehouse : public ALyraGameMode
{
	GENERATED_BODY()	

public:
	APW_GameMode_Safehouse();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSafehouseMatchStateChanged(FName NewState); // ONE PARAMETER

private:
	// Spawning properties...
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AMoneyPickup> MoneyPickupClass;
	UPROPERTY(EditDefaultsOnly) float MoneyBagSpawnRate = 5.0f;
	TArray<AActor*> MoneySpawnPoints;
	FTimerHandle MoneySpawnTimerHandle;
	void SpawnMoneyBag();
};