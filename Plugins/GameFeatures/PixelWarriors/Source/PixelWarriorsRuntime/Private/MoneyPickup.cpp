// Copyright Pixel Warriors Inc. 2025
#include "MoneyPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MoneyComponent.h" // We need to know about our Money Component

AMoneyPickup::AMoneyPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // This actor needs to exist on all clients
	MoneyValue = 100;

	OverlapComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	RootComponent = OverlapComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // The mesh is for looks only

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AMoneyPickup::OnOverlapBegin);
}

void AMoneyPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Run this logic ONLY on the server
	if (HasAuthority())
	{
		// Try to find a MoneyComponent on the actor that overlapped us
		if (OtherActor)
		{
			UMoneyComponent* MoneyComp = OtherActor->FindComponentByClass<UMoneyComponent>();
			if (MoneyComp)
			{
				MoneyComp->ModifyMoney(MoneyValue);
				Destroy(); // Destroy the pickup after it's been collected
			}
		}
	}
}