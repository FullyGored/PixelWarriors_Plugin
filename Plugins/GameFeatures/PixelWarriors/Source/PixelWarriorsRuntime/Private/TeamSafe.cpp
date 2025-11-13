// Copyright Pixel Warriors Inc. 2025
// TeamSafe.cpp
#include "TeamSafe.h" // Its own header first
#include "MoneyComponent.h"
#include "PW_GameState_Safehouse.h" // The simple, correct path
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "GenericTeamAgentInterface.h"

ATeamSafe::ATeamSafe()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);

	// Root = overlap box
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	RootComponent = OverlapComp;
	OverlapComp->SetCollisionProfileName(TEXT("OverlapAll"));
	OverlapComp->SetGenerateOverlapEvents(true);

	// Visual mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// Bind overlap
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ATeamSafe::OnOverlapBegin);
}

/* -------------------------------------------------------------------------
   Overlap
   ------------------------------------------------------------------------- */
void ATeamSafe::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                               AActor* OtherActor,
                               UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex,
                               bool bFromSweep,
                               const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor) return;

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) return;

	// ----- TEAM CHECK -----
	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Pawn->GetController());
	if (!TeamAgent) return;

	const uint8 OtherTeamID = TeamAgent->GetGenericTeamId().GetId();

	// ----- DEPOSIT (own team) -----
	if (OtherTeamID == TeamID)
	{
		if (UMoneyComponent* MoneyComp = OtherActor->FindComponentByClass<UMoneyComponent>())
		{
			const int32 Deposited = MoneyComp->ClearMoney();
			if (Deposited > 0)
			{
				if (APW_GameState_Safehouse* GS = GetWorld()->GetGameState<APW_GameState_Safehouse>())
				{
					GS->AddMoneyToTeam(TeamID, Deposited);
				}
			}
		}
		return;
	}

	// ----- RAID (enemy team) -----
	if (!bIsBeingRaided)
	{
		StartRaid(OtherActor);
	}
}

/* -------------------------------------------------------------------------
   Raid – start
   ------------------------------------------------------------------------- */
void ATeamSafe::StartRaid(AActor* Raider)
{
	bIsBeingRaided = true;

	if (APW_GameState_Safehouse* GS = GetWorld()->GetGameState<APW_GameState_Safehouse>())
	{
		GS->Multicast_SafeRaided(TeamID);
	}

	// 3-second “stealing” delay
	GetWorldTimerManager().SetTimer(
		RaidTimerHandle,
		FTimerDelegate::CreateUObject(this, &ATeamSafe::CompleteRaid, Raider),
		3.0f,
		false);
}

/* -------------------------------------------------------------------------
   Raid – finish (the line that crashed)
   ------------------------------------------------------------------------- */
void ATeamSafe::CompleteRaid(AActor* Raider)
{
	bIsBeingRaided = false;

	APW_GameState_Safehouse* GS = GetWorld()->GetGameState<APW_GameState_Safehouse>();
	if (!GS) return;

	// ----- Pick the correct team money variable -----
	int32& TargetMoney = (TeamID == 0) ? GS->Team_0_Money : GS->Team_1_Money;

	if (TargetMoney <= 0) return;

	const int32 Stolen = FMath::Min(1000, TargetMoney);
	TargetMoney -= Stolen;

	if (UMoneyComponent* MoneyComp = Raider->FindComponentByClass<UMoneyComponent>())
	{
		MoneyComp->ModifyMoney(Stolen);
	}
}

/* -------------------------------------------------------------------------
   OPTIONAL: Spawn a 200 $ drop in the middle of the map
   ------------------------------------------------------------------------- */
void SpawnMoneyDrop(UWorld* World, const FVector& Location)
{
	if (!World) return;

	// You need a simple AActor subclass that has a UMoneyComponent
	// (create one in BP or C++ – here we assume it’s called AMoneyDrop)
	TSubclassOf<AActor> DropClass = /* load your class */ nullptr;
	if (!DropClass) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Drop = World->SpawnActor<AActor>(DropClass, Location, FRotator::ZeroRotator, Params);
	if (UMoneyComponent* MC = Drop->FindComponentByClass<UMoneyComponent>())
	{
		MC->ModifyMoney(200);
	}
}

/* Call from GameMode BeginPlay, for example:
   FVector Center = GetWorld()->GetMapBounds().GetCenter();
   SpawnMoneyDrop(GetWorld(), Center);
*/