// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "PW_Types.h" // Include our single source of truth
#include "GameModes/LyraGameState.h"
#include "PW_GameState_Safehouse.generated.h"

UCLASS()
class PIXELWARRIORSRUNTIME_API APW_GameState_Safehouse : public ALyraGameState
{
	GENERATED_BODY()

public:
	APW_GameState_Safehouse();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// This is the Town Crier's bell. It is the one and only public signal.
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSafehouseMatchStateChanged OnMatchStateChanged;

	// Money functions remain correct
	void AddMoneyToTeam(int32 TeamID, int32 Amount);
	UPROPERTY(Replicated) int32 Team_0_Money;
	UPROPERTY(Replicated) int32 Team_1_Money;
	
protected:
	// This is the core engine function we override to know WHEN to ring the bell.
	virtual void OnMatchStateSet();
};