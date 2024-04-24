#include "CoolButton.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "CoolTriggerReceiverComponent.h"

ACoolButton::ACoolButton()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(Root);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACoolButton::HandleBeginOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void ACoolButton::Tick(float DeltaTime)
{
	if (bHasBeenPressed)
	{
		for (FButtonTargetData& TargetData : Targets)
		{
			if (TargetData.TimeToTrigger < 0.f)
				continue;

			TargetData.TimeToTrigger -= DeltaTime;
			if (TargetData.TimeToTrigger < 0.f)
			{
				AActor* Target = TargetData.Target;
				if (ensureMsgf(Target, TEXT("You put a nullptr in the targets array. Please dont.")))
				{
					auto* Receiver = Target->FindComponentByClass<UCoolTriggerReceiverComponent>();
					if (ensureMsgf(Receiver, TEXT("You put a target that doesn't have a receiver. Thats not right!")))
					{
						Receiver->OnTriggered.Broadcast();
					}
				}
			}
		}
	}
}

void ACoolButton::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHasBeenPressed)
	{
		UE_LOG(LogTemp, Log, TEXT("I was pressed! Hurray!"));
		bHasBeenPressed = true;

		OnButtonPressed();

		// foreach(AActor Target in Targets)
		for(FButtonTargetData& TargetData : Targets)
		{
			TargetData.TimeToTrigger = TargetData.Delay;
		}
	}
}
