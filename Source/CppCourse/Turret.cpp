#include "Turret.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretProjectile.h"
#include "CoolHealthComponent.h"

ATurret::ATurret()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(Root);
	Sphere->SetSphereRadius(2000.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATurret::HandleBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ATurret::HandleEndOverlap);

	YawRoot = CreateDefaultSubobject<USceneComponent>("YawRoot");
	YawRoot->SetupAttachment(Root);
	
	PitchRoot = CreateDefaultSubobject<USceneComponent>("PitchRoot");
	PitchRoot->SetupAttachment(YawRoot);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>("MuzzlePoint");
	MuzzlePoint->SetupAttachment(PitchRoot);

	HealthComponent = CreateDefaultSubobject<UCoolHealthComponent>("Health");
	HealthComponent->OnDeath.AddDynamic(this, &ATurret::HandleDeath);

	PrimaryActorTick.bCanEverTick = true;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = GetWorld()->GetTimeSeconds();
}

void ATurret::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ACharacter>())
	{
		Target = OtherActor;
	}
}

void ATurret::HandleEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == Target)
	{
		Target = nullptr;
	}
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		FVector CurrentDirection = PitchRoot->GetForwardVector();
		FVector TargetDirection = Target->GetActorLocation() - PitchRoot->GetComponentLocation();
		TargetDirection.Normalize();

		CurrentDirection = UKismetMathLibrary::Vector_SlerpVectorToDirection(CurrentDirection, TargetDirection, 5.f * DeltaTime);

		FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(CurrentDirection);
		YawRoot->SetWorldRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
		PitchRoot->SetRelativeRotation(FRotator(TargetRotation.Pitch, 0.f, 0.f));

		float AimDotProduct = FVector::DotProduct(CurrentDirection, TargetDirection);
		if (AimDotProduct > 0.98f)
		{
			if (GetWorld()->TimeSince(LastFireTime) > 1.f / FireRate)
			{
				GetWorld()->SpawnActor<ATurretProjectile>(
					ProjectileClass,
					MuzzlePoint->GetComponentLocation(),
					MuzzlePoint->GetComponentRotation()
				);
				LastFireTime = GetWorld()->GetTimeSeconds();
			}
		}
		else
		{
			LastFireTime = GetWorld()->GetTimeSeconds();
		}
	}
}

void ATurret::HandleDeath()
{
	Destroy();
}
