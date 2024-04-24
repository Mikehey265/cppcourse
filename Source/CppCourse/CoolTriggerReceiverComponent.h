#pragma once
#include "Components/ActorComponent.h"
#include "CoolTriggerReceiverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerReceiverTriggeredSignature);

UCLASS(meta = (BlueprintSpawnableComponent))
class UCoolTriggerReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FTriggerReceiverTriggeredSignature OnTriggered;
};