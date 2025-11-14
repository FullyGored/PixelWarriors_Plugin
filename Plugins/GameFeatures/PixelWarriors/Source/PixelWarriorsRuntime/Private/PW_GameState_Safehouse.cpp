// Copyright Pixel Warriors Inc. 2025
#include "PW_GameState_Safehouse.h"
#include "Net/UnrealNetwork.h"

APW_GameState_Safehouse::APW_GameState_Safehouse() 
{ 
	Team_0_Money = 0; 
	Team_1_Money = 0; 
}

void APW_GameState_Safehouse::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APW_GameState_Safehouse, Team_0_Money);
	DOREPLIFETIME(APW_GameState_Safehouse, Team_1_Money);
}

void APW_GameState_Safehouse::BeginPlay()
{
	Super::BeginPlay();
	CachedMatchState = NAME_None;
}

void APW_GameState_Safehouse::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    
	// Check if match state changed
	if (GameModeClass) // Only on server or if replicated
	{
		// You'll need to find the appropriate way to get match state in Lyra
		// This is just a template
	}
}

void APW_GameState_Safehouse::AddMoneyToTeam_Implementation(int32 TeamID, int32 Amount)
{
	if (HasAuthority() && Amount > 0)
	{
		if (TeamID == 0) { Team_0_Money += Amount; }
		else if (TeamID == 1) { Team_1_Money += Amount; }
	}
}

void APW_GameState_Safehouse::OnRep_TeamMoney()
{
	// Called when money replicates to clients
	// Add any UI update logic here
}

void APW_GameState_Safehouse::Multicast_SafeRaided_Implementation(int32 TeamID)
{
	UE_LOG(LogTemp, Warning, TEXT("Team %d's safe has been raided!"), TeamID);
}