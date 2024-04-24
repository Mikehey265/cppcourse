#pragma once
#include "Engine/GameInstance.h"
#include "CoolGameInstance.generated.h"

class APlayerController;
class UCoolPauseMenu;

UCLASS(Abstract)
class UCoolGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OpenPauseMenu(APlayerController* Owner);

	UPROPERTY(EditDefaultsOnly, Category = "Classes|Widgets")
	TSubclassOf<UCoolPauseMenu> PauseMenuClass;
};
