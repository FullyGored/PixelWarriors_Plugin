// Copyright Pixel Warriors Inc. 2025

#include "MoneyComponent.h"
#include "Net/UnrealNetwork.h"

UMoneyComponent::UMoneyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	Money = 0;
}

void UMoneyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMoneyComponent, Money);
}

void UMoneyComponent::OnRep_Money()
{
	OnMoneyChanged.Broadcast(Money);
}

int32 UMoneyComponent::ModifyMoney(int32 Delta)
{
	// Ensure this only runs on the server.
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return 0;
	}

	const int32 OldMoney = Money;

	// Clamp between 0 and the max carry limit.
	Money = FMath::Clamp(Money + Delta, 0, 1000);

	if (Money != OldMoney)
	{
		OnRep_Money();
	}

	return Money - OldMoney;
}

int32 UMoneyComponent::ClearMoney()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return 0;
	}

	const int32 ClearedAmount = Money;
	Money = 0;

	OnRep_Money();
	return ClearedAmount;
}
