// Copyright Pixel Warriors Inc. 2025
#include "PW_GameState_Safehouse.h"
#include "Net/UnrealNetwork.h"

APW_GameState_Safehouse::APW_GameState_Safehouse() { /* Defaults */ }

void APW_GameState_Safehouse::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APW_GameState_Safehouse, Team_0_Money);
	DOREPLIFETIME(APW_GameState_Safehouse, Team_1_Money);
}

// THIS IS THE CORRECTED LOGIC. We do not need PreviousMatchState. GetMatchState() is the truth.
void APW_GameState_Safehouse::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	// The bell is rung with the one and only piece of information we need: the NEW state.
	OnMatchStateChanged.Broadcast(GetMatchState());
}

void APW_GameState_Safehouse::AddMoneyToTeam(int32 TeamID, int32 Amount)
{
	if (HasAuthority() && Amount > 0)
	{
		if (TeamID == 0) { Team_0_Money += Amount; }
		else if (TeamID == 1) { Team_1_Money += Amount; }
	}
}