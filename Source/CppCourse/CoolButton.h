#pragma once
#include "GameFramework/Actor.h"
#include "CoolButton.generated.h"

class USceneComponent;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FButtonTargetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	AActor* Target = nullptr;

	UPROPERTY(EditAnywhere)
	float Delay = 0.f;

	float TimeToTrigger = 0.f;
};

UCLASS()
class ACoolButton : public AActor
{
	GENERATED_BODY()

public:
	ACoolButton();

	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnButtonPressed();

private:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Trigger = nullptr;

	UPROPERTY(EditInstanceOnly)
	TArray<FButtonTargetData> Targets;

	bool bHasBeenPressed = false;
};