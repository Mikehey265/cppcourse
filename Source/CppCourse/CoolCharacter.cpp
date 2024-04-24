#include "CoolCharacter.h"
#include "CoolHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TurretProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "CoolGameInstance.h"

ACoolCharacter::ACoolCharacter()
{
	HealthComponent = CreateDefaultSubobject<UCoolHealthComponent>("Health");
}

void ACoolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &ACoolCharacter::HandleForwardInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACoolCharacter::HandleRightInput);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &ACoolCharacter::HandleLookHorizontal);
	PlayerInputComponent->BindAxis("LookVertical", this, &ACoolCharacter::HandleLookVertical);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACoolCharacter::HandleJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACoolCharacter::HandleStopJump);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACoolCharacter::HandleStartAiming);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACoolCharacter::HandleStopAiming);

	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ACoolCharacter::HandleShoot);

	PlayerInputComponent->BindAction("OpenPauseMenu", EInputEvent::IE_Pressed, this, &ACoolCharacter::HandleOpenPauseMenu);
}

FHitResult ACoolCharacter::GetAimHitTarget()
{
	FVector ViewOrigin;
	FRotator ViewRotation;
	GetController()->GetPlayerViewPoint(ViewOrigin, ViewRotation);

	FVector ViewForward = ViewRotation.Quaternion().GetForwardVector();

	FHitResult AimHit;
	GetWorld()->LineTraceSingleByChannel(
		AimHit,
		ViewOrigin, ViewOrigin + ViewForward * 10000.f,
		ECollisionChannel::ECC_Visibility
	);

	return AimHit;
}

void ACoolCharacter::HandleForwardInput(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;

	FVector Forward = ControlRotation.Quaternion().GetForwardVector();
	AddMovementInput(Forward, Value);
}

void ACoolCharacter::HandleRightInput(float Value)
{
	FVector Right = GetControlRotation().Quaternion().GetRightVector();
	AddMovementInput(Right, Value);
}

void ACoolCharacter::HandleLookHorizontal(float Value)
{
	AddControllerYawInput(Value);
}

void ACoolCharacter::HandleLookVertical(float Value)
{
	AddControllerPitchInput(Value);
}

void ACoolCharacter::HandleJump()
{
	Jump();
}

void ACoolCharacter::HandleStopJump()
{
	StopJumping();
}

void ACoolCharacter::HandleStartAiming()
{
	ActivateAimCamera();
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	bIsAiming = true;
}

void ACoolCharacter::HandleStopAiming()
{
	ActivateMovementCamera();
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsAiming = false;
}

void ACoolCharacter::HandleShoot()
{
	if (bIsAiming && ensure(ProjectileClass))
	{
		if (Ammo > 0)
		{
			FHitResult AimHit = GetAimHitTarget();
			FVector AimPoint = AimHit.bBlockingHit ? AimHit.Location : AimHit.TraceEnd;

			FVector ShootOrigin = GetActorLocation() + GetActorForwardVector() * 100.f;
			FVector ShootDirection = AimPoint - ShootOrigin;

			GetWorld()->SpawnActor<ATurretProjectile>(
				ProjectileClass,
				ShootOrigin,
				UKismetMathLibrary::MakeRotFromX(ShootDirection)
			);

			Ammo--;
		}
	}
}

void ACoolCharacter::HandleOpenPauseMenu()
{
	auto* GameInstance = GetWorld()->GetGameInstance<UCoolGameInstance>();
	GameInstance->OpenPauseMenu(GetController<APlayerController>());
}
