#include "TurretProjectile.h"
#include "Components/SphereComponent.h"
#include "CoolHealthComponent.h"

ATurretProjectile::ATurretProjectile()
{
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = Sphere;

	PrimaryActorTick.bCanEverTick = true;
}

void ATurretProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult SweepHit;
	AddActorLocalOffset(FVector::ForwardVector * Speed * DeltaTime, true, &SweepHit);

	if (SweepHit.bBlockingHit)
	{
		AActor* HitActor = SweepHit.GetActor();
		if (HitActor)
		{
			UCoolHealthComponent* HealthComponent = HitActor->FindComponentByClass<UCoolHealthComponent>();
			if (HealthComponent)
			{
				HealthComponent->TakeDamage(20.f);
			}
		}

		if (SweepHit.GetComponent() && SweepHit.GetComponent()->IsSimulatingPhysics())
		{
			SweepHit.GetComponent()->AddImpulseAtLocation(
				GetActorForwardVector() * Speed * 10.f,
				SweepHit.ImpactPoint
			);
		}

		Destroy();
	}
}