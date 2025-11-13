// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "TeamSafe.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UMoneyComponent;
class APW_GameState_Safehouse;

UCLASS()
class PIXELWARRIORSRUNTIME_API ATeamSafe : public AActor
{
	GENERATED_BODY()

public:
	ATeamSafe();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> OverlapComp;

	// Set per-instance in the level
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Team")
	uint8 TeamID = 255;               // 255 = invalid

	/** Overlap */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);

	/** Raid handling */
	void StartRaid(AActor* Raider);
	void CompleteRaid(AActor* Raider);

	FTimerHandle RaidTimerHandle;
	bool bIsBeingRaided = false;
};