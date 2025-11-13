// Copyright Pixel Warriors Inc. 2025
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoneyPickup.generated.h"

class USphereComponent;

UCLASS()
class PIXELWARRIORSRUNTIME_API AMoneyPickup : public AActor
{
	GENERATED_BODY()

public:
	AMoneyPickup();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* OverlapComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 MoneyValue;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};