// Copyright Pixel Warriors Inc. 2025
#include "TeamSafe.h"                 // This class's own header
#include "PW_GameState_Safehouse.h"  // We need to talk to our custom GameState
#include "MoneyComponent.h"          // We need to get the player's wallet
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h" // We need the PlayerState to find the Team Interface
#include "GenericTeamAgentInterface.h" // The Lyra-native way to determine team affiliation
#include "Kismet/GameplayStatics.h" // Needed for GetGameState

ATeamSafe::ATeamSafe()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	TeamID = -1; // Default to an invalid team

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	RootComponent = OverlapComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// Bind our custom overlap function to the component's overlap event.
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ATeamSafe::OnOverlapBegin);
}

void ATeamSafe::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ALL interaction logic is server-authoritative.
	if (HasAuthority() && OtherActor)
	{
		// Make sure the overlapping actor is a character we can get a controller from.
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if (!PlayerPawn) return;

		AController* PlayerController = PlayerPawn->GetController();
		if(!PlayerController) return;

		// The controller is the authority for team affiliation in Lyra.
		const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PlayerController);

		if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId().GetId() == TeamID)
		{
			// --- Friendly Interaction: DEPOSIT ---
			UMoneyComponent* MoneyComp = OtherActor->FindComponentByClass<UMoneyComponent>();
			if (MoneyComp)
			{
				const int32 DepositedAmount = MoneyComp->ClearMoney();
				if (DepositedAmount > 0)
				{
					// If money was deposited, tell our custom GameState to update the master score.
					if(APW_GameState_Safehouse* GS = GetWorld()->GetGameState<APW_GameState_Safehouse>())
					{
						GS->AddMoneyToTeam(TeamID, DepositedAmount);
					}
				}
			}
		}
		else if (OtherTeamAgent) // This implies they are not on our team
		{
			// --- Hostile Interaction: RAID ---

			// 1. Get a reference to our custom GameState.
			if(APW_GameState_Safehouse* GS = GetWorld()->GetGameState<APW_GameState_Safehouse>())
			{
				// 2. Broadcast the raid event. This call is to a NetMulticast function, so all clients will execute it.
				// This is where you would trigger the on-screen UI timer for the raid.
				GS->Multicast_SafeRaided(TeamID);

				// TODO: We will add the 3-second raid timer logic here.
				// For now, this confirms we have the correct hooks to trigger the event.
			}
		}
	}
}	