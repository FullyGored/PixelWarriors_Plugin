// Copyright Pixel Warriors Inc. 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoneyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewMoney);

/**
 * A replicated component for handling player money.
 * Each player can carry up to a fixed limit of money.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PIXELWARRIORSRUNTIME_API UMoneyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMoneyComponent();

	/** Current amount of money carried (replicated). */
	UPROPERTY(ReplicatedUsing=OnRep_Money, BlueprintReadOnly, Category="Money")
	int32 Money;

	/** Called when the money value changes (on clients). */
	UPROPERTY(BlueprintAssignable, Category="Money")
	FOnMoneyChanged OnMoneyChanged;

	/** Modify the carried money by Delta (can be positive or negative). Server-only. */
	UFUNCTION(BlueprintCallable, Category="Money")
	int32 ModifyMoney(int32 Delta);

	/** Clear all carried money, returning the cleared amount. Server-only. */
	UFUNCTION(BlueprintCallable, Category="Money")
	int32 ClearMoney();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called when Money is updated (client-side). */
	UFUNCTION()
	void OnRep_Money();
};
