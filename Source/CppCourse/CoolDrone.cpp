#include "CoolDrone.h"
#include "CoolControlPoint.h"
#include "CoolHealthComponent.h"
#include "TurretProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

ACoolDrone::ACoolDrone()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(Root);
	Sphere->SetSphereRadius(500.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACoolDrone::HandleBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACoolDrone::HandleEndOverlap);

	YawRoot = CreateDefaultSubobject<USceneComponent>("YawRoot");
	YawRoot->SetupAttachment(Root);

	PitchRoot = CreateDefaultSubobject<USceneComponent>("PitchRoot");
	PitchRoot->SetupAttachment(YawRoot);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>("MuzzlePoint");
	MuzzlePoint->SetupAttachment(PitchRoot);

	Health = CreateDefaultSubobject<UCoolHealthComponent>("Health");
	Health->OnDeath.AddDynamic(this, &ACoolDrone::HandleDeath);

	PrimaryActorTick.bCanEverTick = true;
}

void ACoolDrone::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = GetWorld()->GetTimeSeconds();
}


void ACoolDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CanSeeThePlayer())
	{
		HandleAggroState(DeltaTime);
	}
	else
	{
		if(bHasSeenPlayer)
		{
			HandleChaseState(DeltaTime);
		}
		else
		{
			HandleIdleState(DeltaTime);	
		}
	}
}

void ACoolDrone::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA<ACharacter>())
	{
		Target = OtherActor;
		UE_LOG(LogTemp, Display, TEXT("Target in range: %s"), *Target->GetName())
	}
}

void ACoolDrone::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == Target)
	{
		Target = nullptr;
		UE_LOG(LogTemp, Display, TEXT("Target lost"))
	}
}

void ACoolDrone::HandleAggroState(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Player spotted!"));
	bHasSeenPlayer = true;
	LastPlayerLocation = Target->GetActorLocation();
	
	FVector CurrentDirection = PitchRoot->GetForwardVector();
	FVector TargetDirection = Target->GetActorLocation() - PitchRoot->GetComponentLocation();
	TargetDirection.Normalize();

	CurrentDirection = UKismetMathLibrary::Vector_SlerpVectorToDirection(CurrentDirection, TargetDirection, 5.f * DeltaTime);

	FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(CurrentDirection);
	YawRoot->SetWorldRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
	PitchRoot->SetRelativeRotation(FRotator(TargetRotation.Pitch, 0.f, 0.f));
	
	float AimDotProduct = FVector::DotProduct(CurrentDirection, TargetDirection);
	if(AimDotProduct > 0.98f)
	{
		if(GetWorld()->TimeSince(LastFireTime) > 1.f / FireRate)
		{
			GetWorld()->SpawnActor<ATurretProjectile>(Projectile, MuzzlePoint->GetComponentLocation(), MuzzlePoint->GetComponentRotation());
			LastFireTime = GetWorld()->GetTimeSeconds();
		}
	}
	else
	{
		LastFireTime = GetWorld()->GetTimeSeconds();
	}
}

void ACoolDrone::HandleIdleState(float DeltaTime)
{
	if(ControlPoints.Num() == 0) return;
	UE_LOG(LogTemp, Display, TEXT("Idling"));

	if(!bHasSelectedControlPoint)
	{
		int32 RandomIndex = FMath::RandRange(0, ControlPoints.Num() - 1);

		while (RandomIndex == PreviousControlPointIndex)
		{
			RandomIndex = FMath::RandRange(0, ControlPoints.Num() - 1);
		}

		if(CanReachControlPoint(ControlPoints[RandomIndex]))
		{
			PreviousControlPointIndex = RandomIndex;
			RandomControlPoint = ControlPoints[RandomIndex];
			bHasSelectedControlPoint = true;
			// UE_LOG(LogTemp, Display, TEXT("Selected control point: %d"), RandomIndex);	
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("Cannot reach control point: %d"), RandomIndex);
		}
	}
	else
	{
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = RandomControlPoint->GetActorLocation();
        		
		FVector Direction = TargetLocation - CurrentLocation;
		Direction.Normalize();

		FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(Direction);
		
		FRotator CurrentYawRotation = YawRoot->GetComponentRotation();
		FRotator InterpolatedYawRotation = FMath::RInterpTo(CurrentYawRotation, FRotator(0.f, TargetRotation.Yaw, 0.f), DeltaTime, 5.f);
		YawRoot->SetWorldRotation(InterpolatedYawRotation);
		
		FRotator CurrentPitchRotation = PitchRoot->GetRelativeRotation();
		FRotator InterpolatedPitchRotation = FMath::RInterpTo(CurrentPitchRotation, FRotator(TargetRotation.Pitch, 0.f, 0.f), DeltaTime, 5.f);
		PitchRoot->SetRelativeRotation(InterpolatedPitchRotation);
		
		float Distance = FVector::Distance(CurrentLocation, TargetLocation);
        	
		if(Distance > 5.f)
		{
			CurrentLocation += Direction * Speed * DeltaTime;
			SetActorLocation(CurrentLocation);
		}
		else
		{
			bHasSelectedControlPoint = false;
			UE_LOG(LogTemp, Display, TEXT("Reached target!"));
		}
	}
}

void ACoolDrone::HandleChaseState(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Chasing player to last known position!"));
	
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = LastPlayerLocation;

	FVector Direction = TargetLocation - CurrentLocation;
	Direction.Normalize();
	
	CurrentLocation += Direction * Speed * DeltaTime;
	SetActorLocation(CurrentLocation);

	float Distance = FVector::Distance(CurrentLocation, TargetLocation);

	if(Distance < 5.f)
	{
		bHasSeenPlayer = false;
	}
}

bool ACoolDrone::CanSeeThePlayer() const
{
	if(Target)
	{
		FVector StartLocation = PitchRoot->GetComponentLocation();
		FVector EndLocation = Target->GetActorLocation();
	
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

		return !bHit || HitResult.GetActor() == Target;
	}
	return false;
}

bool ACoolDrone::CanReachControlPoint(ACoolControlPoint* ControlPoint) const
{
	FVector StartPoint = GetActorLocation();
	FVector EndPoint = ControlPoint->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionParams);

	return !bHit;
}

void ACoolDrone::HandleDeath()
{
	Destroy();
}
