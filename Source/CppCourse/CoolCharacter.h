#pragma once
#include "GameFramework/Character.h"
#include "CoolCharacter.generated.h"

class UCoolHealthComponent;
class ATurretProjectile;

UCLASS()
class ACoolCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoolCharacter();

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateAimCamera();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateMovementCamera();

	UFUNCTION(BlueprintPure)
	FHitResult GetAimHitTarget();

	UFUNCTION(BlueprintCallable)
	void AddAmmo(int Amount)
	{
		Ammo += Amount;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Ammo = 10;

private:
	void HandleForwardInput(float Value);
	void HandleRightInput(float Value);

	void HandleLookHorizontal(float Value);
	void HandleLookVertical(float Value);

	void HandleJump();
	void HandleStopJump();

	void HandleStartAiming();
	void HandleStopAiming();

	void HandleShoot();

	void HandleOpenPauseMenu();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCoolHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurretProjectile> ProjectileClass;

	bool bIsAiming = false;
};
