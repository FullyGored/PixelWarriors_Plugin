// PW_GameState_Safehouse.h
// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "PW_GameState_Safehouse.generated.h"

// Delegate type (add if not already in another header)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMatchStateChangedDelegate, FName, NewState, FName, OldState);

UCLASS()
class PIXELWARRIORSRUNTIME_API APW_GameState_Safehouse : public AModularGameState
{
	GENERATED_BODY()

public:
	APW_GameState_Safehouse();

	// Override to broadcast delegate
	virtual void OnMatchStateSet();

	// Delegate for match state changes
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMatchStateChangedDelegate OnMatchStateChanged;

	// Your existing members
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Team_0_Money = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Team_1_Money = 0;

	UFUNCTION(Server, Reliable)
	void AddMoneyToTeam(uint8 TeamID, int32 Amount);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SafeRaided(uint8 TeamID);
};