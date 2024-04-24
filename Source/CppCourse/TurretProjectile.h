#pragma once
#include "GameFramework/Actor.h"
#include "TurretProjectile.generated.h"

class USphereComponent;

UCLASS()
class ATurretProjectile : public AActor
{
	GENERATED_BODY()

public:
	ATurretProjectile();

	void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float Speed = 2000.f;
};
