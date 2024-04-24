#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

// Reflection
// Information about classes, methods, properties etc.

class USphereComponent;
class ATurretProjectile;
class UCoolHealthComponent;

UCLASS()
class CPPCOURSE_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void HandleDeath();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* YawRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PitchRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* MuzzlePoint = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCoolHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float FireRate = 2.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurretProjectile> ProjectileClass;

	UPROPERTY(EditInstanceOnly)
	AActor* Target = nullptr;

	float LastFireTime = 0.f;
};
