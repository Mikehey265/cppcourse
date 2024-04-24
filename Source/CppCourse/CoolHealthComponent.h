#pragma once
#include "Components/ActorComponent.h"
#include "CoolHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCoolHealthDeathSignature);

UCLASS(meta = (BlueprintSpawnableComponent))
class UCoolHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void TakeDamage(float Damage)
	{
		UE_LOG(LogTemp, Log, TEXT("Ouchie! I have %f / %f health"), CurrentHealth, MaximumHealth);
		CurrentHealth -= Damage;

		if (CurrentHealth <= 0.f)
		{
			CurrentHealth = 0.f;
			OnDeath.Broadcast();
		}
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "500.0"))
	float MaximumHealth = 100.f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = 0.f;

	UPROPERTY(BlueprintAssignable)
	FCoolHealthDeathSignature OnDeath;
};