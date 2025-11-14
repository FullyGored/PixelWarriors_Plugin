// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "GameModes/LyraGameState.h"
#include "PW_Types.h"
#include "PW_GameState_Safehouse.generated.h"

UCLASS()
class PIXELWARRIORS_API APW_GameState_Safehouse : public ALyraGameState
{
	GENERATED_BODY()

public:

	APW_GameState_Safehouse(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Delegate: Notifies when match state changes */
	UPROPERTY(BlueprintAssignable, Category = "Match")
	FOnSafehouseMatchStateChanged OnMatchStateChanged;

	/** Add money to a specific team */
	UFUNCTION(BlueprintCallable, Category = "Money")
	void AddMoneyToTeam(int32 TeamID, int32 Amount);

	/** Replicated team money counters */
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Money")
	int32 Team_0_Money;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Money")
	int32 Team_1_Money;

	/** Multicast event when a safehouse is fully raided */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SafeRaided(int32 TeamID);

protected:

	/** Proper Lyra override */
	virtual void OnMatchStateSet(FName NewState);

	/** Required for replicated variables */
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};

