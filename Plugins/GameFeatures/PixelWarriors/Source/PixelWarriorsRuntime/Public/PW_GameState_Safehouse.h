// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "PW_Types.h"
#include "GameModes/LyraGameState.h"
#include "PW_GameState_Safehouse.generated.h"

UCLASS()
class PIXELWARRIORSRUNTIME_API APW_GameState_Safehouse : public ALyraGameState
{
	GENERATED_BODY()

public:
	APW_GameState_Safehouse();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSafehouseMatchStateChanged OnMatchStateChanged;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Money")
	void AddMoneyToTeam(int32 TeamID, int32 Amount);

	UPROPERTY(ReplicatedUsing=OnRep_TeamMoney, BlueprintReadOnly, Category="Money")
	int32 Team_0_Money;

	UPROPERTY(ReplicatedUsing=OnRep_TeamMoney, BlueprintReadOnly, Category="Money")
	int32 Team_1_Money;
    
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SafeRaided(int32 TeamID);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
 
  	UFUNCTION()
	void OnRep_TeamMoney(); // RepNotify for the UI

private:
	FName CachedMatchState;
};