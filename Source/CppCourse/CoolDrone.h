#pragma once

#include "CoolDrone.generated.h"

class ATurretProjectile;
class ACoolControlPoint;
class UCoolHealthComponent;
class USphereComponent;

UCLASS()
class ACoolDrone : public AActor
{
	GENERATED_BODY()

public:
	ACoolDrone();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
private:
	UFUNCTION()
	void HandleAggroState(float DeltaTime);

	UFUNCTION()
	void HandleIdleState(float DeltaTime);

	UFUNCTION()
	void HandleChaseState(float DeltaTime);

	UFUNCTION()
	bool CanSeeThePlayer() const;
	UFUNCTION()
	bool CanReachControlPoint(ACoolControlPoint* ControlPoint) const;

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
	UCoolHealthComponent* Health = nullptr;

	UPROPERTY(EditInstanceOnly)
	AActor* Target = nullptr;
	
	UPROPERTY(EditInstanceOnly)
	TArray<ACoolControlPoint*> ControlPoints;

	UPROPERTY(EditInstanceOnly)
	float Speed = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float FireRate = 2.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurretProjectile> Projectile;
	
	bool bHasSelectedControlPoint = false;
	ACoolControlPoint* RandomControlPoint = nullptr;
	int32 PreviousControlPointIndex = -1;
	float LastFireTime = 0.f;

	FVector LastPlayerLocation;
	bool bHasSeenPlayer = false;
};
